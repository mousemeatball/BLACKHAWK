/*
 * actuator.cpp - Actuator Driver Module Implementation
 * 
 * PWM output to servo/ESC for X and Y axis control
 * TODO: Replace with actual PWM method based on actuator type
 */

#include "actuator.h"
#include "config.h"
#include "pins.h"

static ActuatorCommand actuators = {0};

void actuator_init() {
    // TODO: Verify PWM pin assignments match schematic
    pinMode(PIN_ACTUATOR_X, OUTPUT);
    pinMode(PIN_ACTUATOR_Y, OUTPUT);
    
    // Set PWM frequency
    analogWriteFrequency(PIN_ACTUATOR_X, PWM_FREQUENCY_HZ);
    analogWriteFrequency(PIN_ACTUATOR_Y, PWM_FREQUENCY_HZ);
    
    // Force to neutral on init
    actuator_set_neutral();
}

void actuator_set_neutral() {
    actuators.x_output = 0.0f;
    actuators.y_output = 0.0f;
    actuators.x_pwm_us = PWM_NEUTRAL_US;
    actuators.y_pwm_us = PWM_NEUTRAL_US;
    
    // Write neutral PWM
    analogWrite(PIN_ACTUATOR_X, 128); // Center for 8-bit PWM
    analogWrite(PIN_ACTUATOR_Y, 128);
    
    // TODO: If using servo library, use:
    // servo_x.writeMicroseconds(PWM_NEUTRAL_US);
    // servo_y.writeMicroseconds(PWM_NEUTRAL_US);
}

void actuator_write(float x_cmd, float y_cmd) {
    // Clamp commands
    if (x_cmd > 1.0f) x_cmd = 1.0f;
    if (x_cmd < -1.0f) x_cmd = -1.0f;
    if (y_cmd > 1.0f) y_cmd = 1.0f;
    if (y_cmd < -1.0f) y_cmd = -1.0f;
    
    actuators.x_output = x_cmd;
    actuators.y_output = y_cmd;
    
    // Convert to PWM microseconds
    actuators.x_pwm_us = PWM_NEUTRAL_US + (int16_t)(x_cmd * PWM_OUTPUT_RANGE_US);
    actuators.y_pwm_us = PWM_NEUTRAL_US + (int16_t)(y_cmd * PWM_OUTPUT_RANGE_US);
    
    // Clamp PWM
    if (actuators.x_pwm_us < PWM_MIN_US) actuators.x_pwm_us = PWM_MIN_US;
    if (actuators.x_pwm_us > PWM_MAX_US) actuators.x_pwm_us = PWM_MAX_US;
    if (actuators.y_pwm_us < PWM_MIN_US) actuators.y_pwm_us = PWM_MIN_US;
    if (actuators.y_pwm_us > PWM_MAX_US) actuators.y_pwm_us = PWM_MAX_US;
    
    // Write PWM
    // TODO: If using servo library:
    // servo_x.writeMicroseconds(actuators.x_pwm_us);
    // servo_y.writeMicroseconds(actuators.y_pwm_us);
    
    // Placeholder: analogWrite for development
    uint8_t x_duty = map(actuators.x_pwm_us, PWM_MIN_US, PWM_MAX_US, 0, 255);
    uint8_t y_duty = map(actuators.y_pwm_us, PWM_MIN_US, PWM_MAX_US, 0, 255);
    analogWrite(PIN_ACTUATOR_X, x_duty);
    analogWrite(PIN_ACTUATOR_Y, y_duty);
}

ActuatorCommand* actuator_get_command() {
    return &actuators;
}

void actuator_set_pwm(uint16_t x_pwm_us, uint16_t y_pwm_us) {
    // Clamp PWM
    if (x_pwm_us < PWM_MIN_US) x_pwm_us = PWM_MIN_US;
    if (x_pwm_us > PWM_MAX_US) x_pwm_us = PWM_MAX_US;
    if (y_pwm_us < PWM_MIN_US) y_pwm_us = PWM_MIN_US;
    if (y_pwm_us > PWM_MAX_US) y_pwm_us = PWM_MAX_US;
    
    actuators.x_pwm_us = x_pwm_us;
    actuators.y_pwm_us = y_pwm_us;
    
    // Convert to normalized output
    actuators.x_output = (float)(x_pwm_us - PWM_NEUTRAL_US) / PWM_OUTPUT_RANGE_US;
    actuators.y_output = (float)(y_pwm_us - PWM_NEUTRAL_US) / PWM_OUTPUT_RANGE_US;
    
    // Write PWM
    uint8_t x_duty = map(x_pwm_us, PWM_MIN_US, PWM_MAX_US, 0, 255);
    uint8_t y_duty = map(y_pwm_us, PWM_MIN_US, PWM_MAX_US, 0, 255);
    analogWrite(PIN_ACTUATOR_X, x_duty);
    analogWrite(PIN_ACTUATOR_Y, y_duty);
}
