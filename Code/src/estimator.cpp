/*
 * estimator.cpp - Attitude Estimation Module Implementation
 * 
 * Complementary filter for attitude angle and rate estimation
 */

#include "estimator.h"
#include "sensor.h"
#include "config.h"
#include <math.h>

static AttitudeData attitude = {0};
static float pitch_gyro_filtered = 0.0f;
static float yaw_gyro_filtered = 0.0f;
static float accel_x_filtered = 0.0f;
static float accel_y_filtered = 0.0f;
static float accel_z_filtered = 0.0f;

static float low_pass_filter(float input, float prev_output, float tau_ms, float dt_ms) {
    float alpha = dt_ms / (tau_ms + dt_ms);
    return alpha * input + (1.0f - alpha) * prev_output;
}

void estimator_init() {
    attitude.pitch_angle = 0.0f;
    attitude.yaw_angle = 0.0f;
    attitude.pitch_rate = 0.0f;
    attitude.yaw_rate = 0.0f;
    
    pitch_gyro_filtered = 0.0f;
    yaw_gyro_filtered = 0.0f;
    accel_x_filtered = 0.0f;
    accel_y_filtered = 0.0f;
    accel_z_filtered = 0.0f;
}

void estimator_update() {
    SensorData* sensor = sensor_get_data();
    
    if (!sensor->valid) {
        return;
    }
    
    // Filter gyro rates
    pitch_gyro_filtered = low_pass_filter(sensor->gyro_y, pitch_gyro_filtered, 
                                          GYRO_FILTER_TAU_MS, CONTROL_LOOP_DT_MS);
    yaw_gyro_filtered = low_pass_filter(sensor->gyro_x, yaw_gyro_filtered, 
                                        GYRO_FILTER_TAU_MS, CONTROL_LOOP_DT_MS);
    
    // Filter accelerometer
    accel_x_filtered = low_pass_filter(sensor->accel_x, accel_x_filtered,
                                       ACCEL_FILTER_TAU_MS, CONTROL_LOOP_DT_MS);
    accel_y_filtered = low_pass_filter(sensor->accel_y, accel_y_filtered,
                                       ACCEL_FILTER_TAU_MS, CONTROL_LOOP_DT_MS);
    accel_z_filtered = low_pass_filter(sensor->accel_z, accel_z_filtered,
                                       ACCEL_FILTER_TAU_MS, CONTROL_LOOP_DT_MS);
    
    // Compute accelerometer-based angles (tilt)
    float accel_pitch = atan2f(accel_y_filtered, accel_z_filtered) * 180.0f / M_PI;
    float accel_yaw = atan2f(-accel_x_filtered, accel_z_filtered) * 180.0f / M_PI;
    
    // Complementary filter for attitude
    attitude.pitch_angle = COMPLEMENTARY_FILTER_GYRO_WEIGHT * 
                          (attitude.pitch_angle + pitch_gyro_filtered * CONTROL_LOOP_DT_SEC) +
                          (1.0f - COMPLEMENTARY_FILTER_GYRO_WEIGHT) * accel_pitch;
    attitude.yaw_angle = COMPLEMENTARY_FILTER_GYRO_WEIGHT * 
                        (attitude.yaw_angle + yaw_gyro_filtered * CONTROL_LOOP_DT_SEC) +
                        (1.0f - COMPLEMENTARY_FILTER_GYRO_WEIGHT) * accel_yaw;
    
    // Store filtered rates
    attitude.pitch_rate = pitch_gyro_filtered;
    attitude.yaw_rate = yaw_gyro_filtered;
}

AttitudeData* estimator_get_attitude() {
    return &attitude;
}

void estimator_reset() {
    attitude.pitch_angle = 0.0f;
    attitude.yaw_angle = 0.0f;
    attitude.pitch_rate = 0.0f;
    attitude.yaw_rate = 0.0f;
    
    pitch_gyro_filtered = 0.0f;
    yaw_gyro_filtered = 0.0f;
    accel_x_filtered = 0.0f;
    accel_y_filtered = 0.0f;
    accel_z_filtered = 0.0f;
}
