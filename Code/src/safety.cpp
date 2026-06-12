/*
 * safety.cpp - Safety and State Machine Module Implementation
 * 
 * State machine for system safety, arming, and fault handling
 */

#include "safety.h"
#include "sensor.h"
#include "config.h"
#include "pins.h"

static SystemState current_state = STATE_BOOT;
static SystemState previous_state = STATE_BOOT;
static uint32_t state_entry_time_ms = 0;

static bool arm_switch_closed = false;
static bool serial_arm_received = false;
static bool enable_signal_received = false;
static bool calibration_complete = false;
static uint32_t last_valid_sensor_ms = 0;

void safety_init() {
    pinMode(PIN_ARM_SWITCH, INPUT_PULLUP);
    pinMode(PIN_CONTINUITY, INPUT_PULLUP);
    
    current_state = STATE_BOOT;
    previous_state = STATE_BOOT;
    state_entry_time_ms = millis();
    
    arm_switch_closed = false;
    serial_arm_received = false;
    enable_signal_received = false;
    calibration_complete = false;
    last_valid_sensor_ms = millis();
}

void safety_update_inputs() {
    arm_switch_closed = (digitalRead(PIN_ARM_SWITCH) == LOW);
    // TODO: Add continuity check if implemented
}

void safety_update_state_machine() {
    SystemState next_state = current_state;
    uint32_t now = millis();
    
    switch (current_state) {
        case STATE_BOOT:
            // Transition after successful init
            if (now - state_entry_time_ms > BOOT_TIMEOUT_MS) {
                next_state = STATE_SENSOR_CALIBRATION;
            }
            break;
            
        case STATE_SENSOR_CALIBRATION:
            if (calibration_complete) {
                next_state = STATE_IDLE;
            } else if (now - state_entry_time_ms > CALIBRATION_TIMEOUT_MS) {
                next_state = STATE_FAULT;
            }
            break;
            
        case STATE_IDLE:
            if (arm_switch_closed && serial_arm_received && safety_sensors_valid()) {
                next_state = STATE_ARMED;
            } else if (!safety_sensors_valid()) {
                next_state = STATE_FAULT;
            }
            break;
            
        case STATE_ARMED:
            if (!arm_switch_closed || !serial_arm_received) {
                next_state = STATE_IDLE;
            } else if (enable_signal_received && safety_sensors_valid()) {
                next_state = STATE_ACTIVE_CONTROL;
            } else if (!safety_sensors_valid()) {
                next_state = STATE_FAULT;
            }
            break;
            
        case STATE_ACTIVE_CONTROL:
            if (!arm_switch_closed || !serial_arm_received || !enable_signal_received) {
                next_state = STATE_ARMED;
            } else if (!safety_sensors_valid()) {
                next_state = STATE_FAULT;
            }
            break;
            
        case STATE_FAULT:
            // Requires power cycle or serial reset to exit
            break;
    }
    
    // State transition handling
    if (next_state != current_state) {
        previous_state = current_state;
        current_state = next_state;
        state_entry_time_ms = now;
    }
}

SystemState safety_get_state() {
    return current_state;
}

SystemState safety_get_previous_state() {
    return previous_state;
}

uint32_t safety_get_state_entry_time() {
    return state_entry_time_ms;
}

bool safety_sensors_valid() {
    SensorData* sensor = sensor_get_data();
    uint32_t now = millis();
    return (sensor->valid && (now - sensor->last_update_ms) < SENSOR_TIMEOUT_MS);
}

bool safety_get_arm_switch() {
    return arm_switch_closed;
}

void safety_set_serial_arm(bool armed) {
    serial_arm_received = armed;
}

bool safety_get_serial_arm() {
    return serial_arm_received;
}

void safety_set_enable(bool enabled) {
    enable_signal_received = enabled;
}

bool safety_get_enable() {
    return enable_signal_received;
}

bool safety_calibration_complete() {
    return calibration_complete;
}

void safety_set_calibration_complete(bool complete) {
    calibration_complete = complete;
}

void safety_force_fault() {
    previous_state = current_state;
    current_state = STATE_FAULT;
    state_entry_time_ms = millis();
}
