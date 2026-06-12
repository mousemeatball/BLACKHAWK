/*
 * estimator.h - Attitude Estimation Module
 * 
 * Complementary filter for attitude angle and rate estimation
 */

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <Arduino.h>

struct AttitudeData {
    float pitch_angle;        // degrees
    float yaw_angle;          // degrees
    float pitch_rate;         // deg/s
    float yaw_rate;           // deg/s
};

// Initialize estimator
void estimator_init();

// Update attitude estimate with new sensor data
void estimator_update();

// Get current attitude data
AttitudeData* estimator_get_attitude();

// Reset estimator to zero
void estimator_reset();

#endif // ESTIMATOR_H
