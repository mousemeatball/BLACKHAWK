/*
 * safety.h - Safety and State Machine Module
 * 
 * State machine for system safety, arming, and fault handling
 */

#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>

enum SystemState {
    STATE_BOOT,
    STATE_SENSOR_CALIBRATION,
    STATE_IDLE,
    STATE_ARMED,
    STATE_ACTIVE_CONTROL,
    STATE_FAULT
};

// Initialize safety module
void safety_init();

// Update safety inputs (arm switch, continuity, etc.)
void safety_update_inputs();

// Update state machine
void safety_update_state_machine();

// Get current system state
SystemState safety_get_state();

// Get previous system state
SystemState safety_get_previous_state();

// Get state entry time
uint32_t safety_get_state_entry_time();

// Check if sensors are valid
bool safety_sensors_valid();

// Get arm switch state
bool safety_get_arm_switch();

// Set serial arm command
void safety_set_serial_arm(bool armed);

// Get serial arm state
bool safety_get_serial_arm();

// Set enable signal
void safety_set_enable(bool enabled);

// Get enable signal state
bool safety_get_enable();

// Check if calibration is complete
bool safety_calibration_complete();

// Set calibration complete
void safety_set_calibration_complete(bool complete);

// Force transition to fault state
void safety_force_fault();

#endif // SAFETY_H
