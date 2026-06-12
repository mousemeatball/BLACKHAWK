/*
 * config.h - TVC PCB Firmware Configuration Constants
 * 
 * All tunable parameters, timing constants, and limits
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// TIMING CONFIGURATION
// ============================================================================

#define CONTROL_LOOP_FREQ_HZ     500    // Control loop frequency
#define CONTROL_LOOP_DT_MS       (1000 / CONTROL_LOOP_FREQ_HZ)
#define CONTROL_LOOP_DT_SEC      (1.0f / CONTROL_LOOP_FREQ_HZ)
#define STATUS_UPDATE_FREQ_HZ    100
#define DEBUG_OUTPUT_FREQ_HZ     50

// ============================================================================
// SENSOR CALIBRATION
// ============================================================================

#define CALIBRATION_SAMPLES      1000   // Number of samples for bias calibration
#define CALIBRATION_DURATION_MS  2000   // 2 seconds at 500 Hz

// ============================================================================
// PWM OUTPUT CONFIGURATION
// ============================================================================

#define PWM_FREQUENCY_HZ         50     // Standard servo frequency
#define PWM_NEUTRAL_US           1500
#define PWM_MIN_US               1000
#define PWM_MAX_US               2000
#define PWM_OUTPUT_RANGE_US      500    // ±500µs from neutral

// ============================================================================
// SENSOR LIMITS
// ============================================================================

#define GYRO_RATE_LIMIT_DPS      2000   // Degrees per second
#define ACCEL_LIMIT_G            16     // G
#define SENSOR_TIMEOUT_MS        10     // Max time between valid reads

// ============================================================================
// PID DEFAULTS - X-AXIS (PITCH)
// ============================================================================

#define PID_X_KP                 0.0f
#define PID_X_KI                 0.0f
#define PID_X_KD                 0.0f
#define PID_X_KD_RATE            0.0f
#define PID_X_INTEGRAL_LIMIT     10.0f
#define PID_X_OUTPUT_LIMIT       1.0f   // Normalized output (±1.0)

// ============================================================================
// PID DEFAULTS - Y-AXIS (YAW)
// ============================================================================

#define PID_Y_KP                 0.0f
#define PID_Y_KI                 0.0f
#define PID_Y_KD                 0.0f
#define PID_Y_KD_RATE            0.0f
#define PID_Y_INTEGRAL_LIMIT     10.0f
#define PID_Y_OUTPUT_LIMIT       1.0f

// ============================================================================
// FILTERING CONFIGURATION
// ============================================================================

#define GYRO_FILTER_TAU_MS       10     // Low-pass filter time constant
#define ACCEL_FILTER_TAU_MS      50
#define DERIVATIVE_FILTER_TAU_MS 20
#define COMPLEMENTARY_FILTER_GYRO_WEIGHT 0.98f

// ============================================================================
// DEADBAND CONFIGURATION
// ============================================================================

#define DEADBAND_DEG             0.5f   // Deadband around zero error

// ============================================================================
// STATE MACHINE TIMEOUTS
// ============================================================================

#define BOOT_TIMEOUT_MS          100
#define CALIBRATION_TIMEOUT_MS   5000
#define WATCHDOG_TIMEOUT_MS      100

#endif // CONFIG_H
