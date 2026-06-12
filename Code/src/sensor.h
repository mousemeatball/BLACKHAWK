/*
 * sensor.h - IMU Sensor Module
 * 
 * TODO: Replace with actual IMU driver (e.g., MPU9250, BNO055, ICM20948)
 */

#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

struct SensorData {
    float accel_x, accel_y, accel_z;      // m/s²
    float gyro_x, gyro_y, gyro_z;         // deg/s
    float accel_bias_x, accel_bias_y, accel_bias_z;
    float gyro_bias_x, gyro_bias_y, gyro_bias_z;
    uint32_t last_update_ms;
    bool valid;
};

// Initialize IMU sensor
bool sensor_init();

// Read sensor data
bool sensor_read();

// Get current sensor data
SensorData* sensor_get_data();

// Collect a sample for bias calibration
void sensor_collect_bias_sample();

// Compute bias from collected samples
void sensor_compute_bias();

// Check if calibration is complete
bool sensor_is_calibrated();

// Reset calibration
void sensor_reset_calibration();

#endif // SENSOR_H
