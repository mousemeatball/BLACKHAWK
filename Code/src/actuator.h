/*
 * actuator.h - Actuator Driver Module
 * 
 * PWM output to servo/ESC for X and Y axis control
 * TODO: Replace with actual PWM method based on actuator type
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>

struct ActuatorCommand {
    float x_output;           // Normalized -1.0 to +1.0
    float y_output;           // Normalized -1.0 to +1.0
    uint16_t x_pwm_us;
    uint16_t y_pwm_us;
};

// Initialize actuator outputs
void actuator_init();

// Set actuators to neutral position
void actuator_set_neutral();

// Write actuator commands
void actuator_write(float x_cmd, float y_cmd);

// Get current actuator command
ActuatorCommand* actuator_get_command();

// Set actuator to specific PWM values (for testing)
void actuator_set_pwm(uint16_t x_pwm_us, uint16_t y_pwm_us);

#endif // ACTUATOR_H
