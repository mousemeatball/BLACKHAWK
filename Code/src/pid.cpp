/*
 * pid.cpp - PID Controller Module Implementation
 * 
 * Dual-axis PID controller with anti-windup, derivative filtering, and rate damping
 */

#include "pid.h"
#include "config.h"
#include <math.h>

static float low_pass_filter(float input, float prev_output, float tau_ms, float dt_ms) {
    float alpha = dt_ms / (tau_ms + dt_ms);
    return alpha * input + (1.0f - alpha) * prev_output;
}

void pid_init(PIDState* pid, float kp, float ki, float kd, float kd_rate,
              float integral_limit, float output_limit, float deadband) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->kd_rate = kd_rate;
    pid->integral = 0.0f;
    pid->integral_limit = integral_limit;
    pid->output_limit = output_limit;
    pid->error_prev = 0.0f;
    pid->derivative_filtered = 0.0f;
    pid->deadband = deadband;
}

void pid_reset(PIDState* pid) {
    pid->integral = 0.0f;
    pid->error_prev = 0.0f;
    pid->derivative_filtered = 0.0f;
}

float pid_update(PIDState* pid, float setpoint, float measured, float rate, 
                 float dt_sec, bool enable_integration) {
    float error = setpoint - measured;
    
    // Apply deadband
    if (fabsf(error) < pid->deadband) {
        error = 0.0f;
    }
    
    // Integral term with anti-windup
    if (enable_integration) {
        pid->integral += error * dt_sec;
        // Clamp integral
        if (pid->integral > pid->integral_limit) {
            pid->integral = pid->integral_limit;
        } else if (pid->integral < -pid->integral_limit) {
            pid->integral = -pid->integral_limit;
        }
    }
    
    // Derivative term (on error, filtered)
    float derivative = (error - pid->error_prev) / dt_sec;
    pid->derivative_filtered = low_pass_filter(derivative, pid->derivative_filtered,
                                               DERIVATIVE_FILTER_TAU_MS, dt_sec * 1000.0f);
    pid->error_prev = error;
    
    // Compute output
    float output = pid->kp * error + 
                  pid->ki * pid->integral + 
                  pid->kd * pid->derivative_filtered +
                  pid->kd_rate * (-rate); // Rate damping term
    
    // Clamp output
    if (output > pid->output_limit) {
        output = pid->output_limit;
        // Anti-windup: reduce integral when saturated
        if (pid->ki > 0.0f && enable_integration) {
            pid->integral -= (output - pid->output_limit) / pid->ki * dt_sec;
        }
    } else if (output < -pid->output_limit) {
        output = -pid->output_limit;
        if (pid->ki > 0.0f && enable_integration) {
            pid->integral -= (output + pid->output_limit) / pid->ki * dt_sec;
        }
    }
    
    return output;
}

void pid_set_gains(PIDState* pid, float kp, float ki, float kd, float kd_rate) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->kd_rate = kd_rate;
}

float pid_get_output(const PIDState* pid) {
    return 0.0f; // Output is computed in update, not stored
}

float pid_get_integral(const PIDState* pid) {
    return pid->integral;
}
