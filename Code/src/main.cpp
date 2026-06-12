/*
 * main.cpp - TVC PCB Firmware Main Entry Point
 * 
 * Dual-axis thrust vector control with PID stabilization
 * Teensy 4.1
 */

#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "sensor.h"
#include "estimator.h"
#include "pid.h"
#include "actuator.h"
#include "safety.h"
#include "status.h"
#include "debug.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

static PIDState pid_x, pid_y;
static uint32_t last_control_loop_ms = 0;
static uint32_t last_status_update_ms = 0;
static uint32_t last_debug_output_ms = 0;

// ============================================================================
// CONTROL LOOP
// ============================================================================

void control_loop() {
    uint32_t now = millis();
    
    // Fixed-rate timing
    if (now - last_control_loop_ms >= CONTROL_LOOP_DT_MS) {
        last_control_loop_ms = now;
        
        // Read sensors
        sensor_read();
        
        // Collect calibration samples if in calibration state
        if (safety_get_state() == STATE_SENSOR_CALIBRATION) {
            SensorData* sensor = sensor_get_data();
            if (sensor->valid) {
                sensor_collect_bias_sample();
                
                // Check if calibration complete
                static uint32_t calibration_samples = 0;
                calibration_samples++;
                if (calibration_samples >= CALIBRATION_SAMPLES) {
                    sensor_compute_bias();
                    safety_set_calibration_complete(true);
                    calibration_samples = 0;
                }
            }
        }
        
        // Update attitude estimate
        estimator_update();
        
        // Run PID control if in active control state
        if (safety_get_state() == STATE_ACTIVE_CONTROL) {
            SensorData* sensor = sensor_get_data();
            AttitudeData* att = estimator_get_attitude();
            
            if (sensor->valid) {
                float x_output = pid_update(&pid_x, 0.0f, att->pitch_angle, 
                                            att->pitch_rate, CONTROL_LOOP_DT_SEC, true);
                float y_output = pid_update(&pid_y, 0.0f, att->yaw_angle, 
                                            att->yaw_rate, CONTROL_LOOP_DT_SEC, true);
                actuator_write(x_output, y_output);
            } else {
                // Force neutral if sensor invalid
                actuator_set_neutral();
            }
        } else {
            // Force neutral in all other states
            actuator_set_neutral();
        }
        
        // Update state machine
        safety_update_inputs();
        safety_update_state_machine();
        
        // Handle state transitions
        SystemState current_state = safety_get_state();
        static SystemState previous_state = STATE_BOOT;
        
        if (current_state != previous_state) {
            previous_state = current_state;
            
            // Entry actions
            switch (current_state) {
                case STATE_BOOT:
                    // No action
                    break;
                    
                case STATE_SENSOR_CALIBRATION:
                    sensor_reset_calibration();
                    safety_set_calibration_complete(false);
                    break;
                    
                case STATE_IDLE:
                    actuator_set_neutral();
                    pid_reset(&pid_x);
                    pid_reset(&pid_y);
                    break;
                    
                case STATE_ARMED:
                    actuator_set_neutral();
                    pid_reset(&pid_x);
                    pid_reset(&pid_y);
                    break;
                    
                case STATE_ACTIVE_CONTROL:
                    // PID loops will start running
                    break;
                    
                case STATE_FAULT:
                    actuator_set_neutral();
                    pid_reset(&pid_x);
                    pid_reset(&pid_y);
                    break;
            }
        }
    }
    
    // Status LED update (lower frequency)
    if (now - last_status_update_ms >= (1000 / STATUS_UPDATE_FREQ_HZ)) {
        last_status_update_ms = now;
        status_update();
    }
    
    // Debug output (lower frequency, non-blocking)
    if (now - last_debug_output_ms >= (1000 / DEBUG_OUTPUT_FREQ_HZ)) {
        last_debug_output_ms = now;
        debug_output();
    }
    
    // Process serial commands (every loop iteration)
    debug_process_commands();
}

// ============================================================================
// ARDUINO SETUP AND LOOP
// ============================================================================

void setup() {
    // Initialize serial for debug
    DEBUG_SERIAL.begin(DEBUG_BAUD);
    while (!DEBUG_SERIAL && millis() < 3000); // Wait for serial
    
    DEBUG_SERIAL.println("TVC PCB Firmware Booting...");
    DEBUG_SERIAL.println("Version: 1.0.0");
    DEBUG_SERIAL.println("Target: Teensy 4.1");
    
    // Initialize modules
    sensor_init();
    estimator_init();
    actuator_init();
    status_init();
    safety_init();
    debug_init();
    
    // Initialize PID controllers
    pid_init(&pid_x, PID_X_KP, PID_X_KI, PID_X_KD, PID_X_KD_RATE,
             PID_X_INTEGRAL_LIMIT, PID_X_OUTPUT_LIMIT, DEADBAND_DEG);
    pid_init(&pid_y, PID_Y_KP, PID_Y_KI, PID_Y_KD, PID_Y_KD_RATE,
             PID_Y_INTEGRAL_LIMIT, PID_Y_OUTPUT_LIMIT, DEADBAND_DEG);
    
    // Force actuators to neutral
    actuator_set_neutral();
    
    DEBUG_SERIAL.println("Initialization complete.");
    DEBUG_SERIAL.println("System ready. Send 'a' to arm, 'e' to enable control.");
    DEBUG_SERIAL.println("Send 's' for status, 'h' for help.");
}

void loop() {
    control_loop();
}
