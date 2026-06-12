/*
 * status.h - LED and Buzzer Status Module
 * 
 * Status indication via LED patterns and buzzer
 */

#ifndef STATUS_H
#define STATUS_H

#include <Arduino.h>

// Initialize status outputs
void status_init();

// Update status outputs based on current system state
void status_update();

// Set status LED directly (for testing)
void status_set_led(bool on);

// Set buzzer directly (for testing)
void status_set_buzzer(bool on);

#endif // STATUS_H
