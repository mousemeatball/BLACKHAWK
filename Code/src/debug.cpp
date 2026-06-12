/*
 * debug.cpp - Debug Interface Module Implementation
 * 
 * Serial debug output for telemetry and command parsing
 */

#include "debug.h"
#include "safety.h"
#include "estimator.h"
#include "actuator.h"
#include "sensor.h"
#include "pins.h"

static bool debug_enabled = true;

void debug_init() {
    DEBUG_SERIAL.begin(DEBUG_BAUD);
}

void debug_output() {
    if (!debug_enabled) {
        return;
    }
    
    SystemState state = safety_get_state();
    AttitudeData* att = estimator_get_attitude();
    ActuatorCommand* act = actuator_get_command();
    SensorData* sensor = sensor_get_data();
    
    DEBUG_SERIAL.print("STATE:");
    DEBUG_SERIAL.print(state);
    DEBUG_SERIAL.print(",");
    
    DEBUG_SERIAL.print("PITCH:");
    DEBUG_SERIAL.print(att->pitch_angle, 2);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print("YAW:");
    DEBUG_SERIAL.print(att->yaw_angle, 2);
    DEBUG_SERIAL.print(",");
    
    DEBUG_SERIAL.print("RATE_P:");
    DEBUG_SERIAL.print(att->pitch_rate, 2);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print("RATE_Y:");
    DEBUG_SERIAL.print(att->yaw_rate, 2);
    DEBUG_SERIAL.print(",");
    
    DEBUG_SERIAL.print("ACT_X:");
    DEBUG_SERIAL.print(act->x_output, 3);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print("ACT_Y:");
    DEBUG_SERIAL.print(act->y_output, 3);
    DEBUG_SERIAL.print(",");
    
    DEBUG_SERIAL.print("PWM_X:");
    DEBUG_SERIAL.print(act->x_pwm_us);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print("PWM_Y:");
    DEBUG_SERIAL.print(act->y_pwm_us);
    
    DEBUG_SERIAL.println();
}

void debug_process_commands() {
    if (DEBUG_SERIAL.available()) {
        char cmd = DEBUG_SERIAL.read();
        
        switch (cmd) {
            case 'a': // Arm command
                safety_set_serial_arm(true);
                DEBUG_SERIAL.println("ARM_CMD_RECEIVED");
                break;
                
            case 'd': // Disarm command
                safety_set_serial_arm(false);
                safety_set_enable(false);
                DEBUG_SERIAL.println("DISARM_CMD_RECEIVED");
                break;
                
            case 'e': // Enable control
                safety_set_enable(true);
                DEBUG_SERIAL.println("ENABLE_CMD_RECEIVED");
                break;
                
            case 'x': // Disable control
                safety_set_enable(false);
                DEBUG_SERIAL.println("DISABLE_CMD_RECEIVED");
                break;
                
            case 'r': // Reset from fault
                if (safety_get_state() == STATE_FAULT) {
                    // Force reset by setting state to BOOT
                    // This is a hack - in production, use proper reset
                    DEBUG_SERIAL.println("RESET_CMD_RECEIVED - REBOOT REQUIRED");
                }
                break;
                
            case 'c': // Force calibration
                if (safety_get_state() == STATE_IDLE) {
                    sensor_reset_calibration();
                    safety_set_calibration_complete(false);
                    DEBUG_SERIAL.println("CALIBRATE_CMD_RECEIVED");
                }
                break;
                
            case 's': // Status query
                DEBUG_SERIAL.print("STATE:");
                DEBUG_SERIAL.println(safety_get_state());
                DEBUG_SERIAL.print("ARM_SW:");
                DEBUG_SERIAL.println(safety_get_arm_switch());
                DEBUG_SERIAL.print("SERIAL_ARM:");
                DEBUG_SERIAL.println(safety_get_serial_arm());
                DEBUG_SERIAL.print("ENABLE:");
                DEBUG_SERIAL.println(safety_get_enable());
                DEBUG_SERIAL.print("CALIB:");
                DEBUG_SERIAL.println(safety_calibration_complete());
                break;
                
            case 't': // Toggle debug output
                debug_enabled = !debug_enabled;
                DEBUG_SERIAL.print("DEBUG:");
                DEBUG_SERIAL.println(debug_enabled ? "ON" : "OFF");
                break;
        }
    }
}

void debug_set_enabled(bool enabled) {
    debug_enabled = enabled;
}

bool debug_is_enabled() {
    return debug_enabled;
}
