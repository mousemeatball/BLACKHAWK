/*
 * pid.h - PID Controller Module
 * 
 * Dual-axis PID controller with anti-windup, derivative filtering, and rate damping
 */

#ifndef PID_H
#define PID_H

#include <Arduino.h>

struct PIDState {
    float kp, ki, kd, kd_rate;
    float integral;
    float integral_limit;
    float output_limit;
    float error_prev;
    float derivative_filtered;
    float deadband;
};

// Initialize a PID controller
void pid_init(PIDState* pid, float kp, float ki, float kd, float kd_rate,
              float integral_limit, float output_limit, float deadband);

// Reset PID controller state
void pid_reset(PIDState* pid);

// Update PID controller and compute output
float pid_update(PIDState* pid, float setpoint, float measured, float rate, 
                 float dt_sec, bool enable_integration);

// Set PID gains
void pid_set_gains(PIDState* pid, float kp, float ki, float kd, float kd_rate);

// Get current PID output (for debugging)
float pid_get_output(const PIDState* pid);

// Get current integral term (for debugging)
float pid_get_integral(const PIDState* pid);

#endif // PID_H
