/*
 * pins.h - TVC PCB Firmware Pin Definitions
 * 
 * TODO: Replace all placeholder pin assignments with actual schematic-confirmed details
 */

#ifndef PINS_H
#define PINS_H

// ============================================================================
// IMU INTERFACE (assumed I2C)
// TODO: Verify with schematic
// ============================================================================

#define PIN_IMU_SDA              18
#define PIN_IMU_SCL              19
#define PIN_IMU_INT              2   // Data-ready interrupt (optional)

// ============================================================================
// ACTUATOR OUTPUTS (PWM)
// TODO: Verify with schematic
// ============================================================================

#define PIN_ACTUATOR_X           6
#define PIN_ACTUATOR_Y           9

// ============================================================================
// STATUS OUTPUTS
// TODO: Verify with schematic
// ============================================================================

#define PIN_LED_STATUS           13
#define PIN_LED_ARMED            14
#define PIN_BUZZER               15

// ============================================================================
// SAFETY INPUTS
// TODO: Verify with schematic
// ============================================================================

#define PIN_ARM_SWITCH           3
#define PIN_CONTINUITY           4

// ============================================================================
// DEBUG INTERFACE
// ============================================================================

#define DEBUG_SERIAL             Serial
#define DEBUG_BAUD               115200

#endif // PINS_H
