# TVC PCB Firmware

Dual-axis thrust vector control firmware for Teensy 4.1 with PID stabilization.

## Overview

This firmware provides complete board-level control for a custom rocket thrust vector control (TVC) PCB. It includes:

- IMU sensor reading and calibration
- Attitude estimation using complementary filter
- Dual-axis PID control (pitch/X and yaw/Y)
- Actuator PWM output
- Safety state machine with arming/disarming
- LED and buzzer status indication
- Serial debug interface for tuning

## Hardware

- **Microcontroller**: Teensy 4.1
- **IMU**: Placeholder (TODO: replace with actual sensor driver)
- **Actuators**: 2x PWM servos or ESCs
- **Status**: LED + buzzer
- **Safety**: Arm switch input

## Building

### Requirements

- PlatformIO
- Teensy 4.1 board

### Build Commands

```bash
# Install PlatformIO (if not already installed)
pip install platformio

# Build the firmware
pio run

# Upload to Teensy
pio run --target upload

# Monitor serial output
pio device monitor
```

## File Structure

```
Code/
├── src/
│   ├── main.cpp              # Entry point
│   ├── config.h              # Configuration constants
│   ├── pins.h                # Pin definitions
│   ├── sensor.h/cpp          # IMU sensor module
│   ├── estimator.h/cpp       # Attitude estimation
│   ├── pid.h/cpp             # PID controller
│   ├── actuator.h/cpp        # Actuator driver
│   ├── safety.h/cpp          # State machine
│   ├── status.h/cpp          # LED/buzzer status
│   └── debug.h/cpp           # Serial debug interface
├── platformio.ini            # Build configuration
└── README.md                 # This file
```

## Configuration

### Pin Assignments

Edit `src/pins.h` to match your PCB schematic:

```cpp
#define PIN_IMU_SDA              18
#define PIN_IMU_SCL              19
#define PIN_ACTUATOR_X           6
#define PIN_ACTUATOR_Y           9
#define PIN_LED_STATUS           13
#define PIN_ARM_SWITCH           3
// ... etc
```

### PID Gains

Edit `src/config.h` to set PID gains:

```cpp
#define PID_X_KP                 0.0f
#define PID_X_KI                 0.0f
#define PID_X_KD                 0.0f
#define PID_X_KD_RATE            0.0f
// ... same for Y-axis
```

### Timing

Control loop runs at 500 Hz by default. Edit in `src/config.h`:

```cpp
#define CONTROL_LOOP_FREQ_HZ     500
```

## Serial Commands

Connect via USB serial at 115200 baud:

- `a` - Arm system
- `d` - Disarm system
- `e` - Enable active control
- `x` - Disable active control
- `c` - Force recalibration
- `s` - Status query
- `t` - Toggle debug output

## State Machine

The firmware uses a 6-state machine:

1. **BOOT** - Hardware initialization
2. **SENSOR_CALIBRATION** - Collecting sensor bias data
3. **IDLE** - System ready, awaiting arm
4. **ARMED** - Armed but not actively controlling
5. **ACTIVE_CONTROL** - PID loops running
6. **FAULT** - Error condition, requires reset

## Tuning Procedure

### Initial Setup

1. Start with all PID gains at zero
2. Verify actuators move to neutral on boot
3. Check sensor data via serial output

### Tuning P Gain

1. Set P to small value (e.g., 0.1)
2. Arm and enable control
3. Apply small disturbance
4. Increase P until response is snappy but not oscillating

### Tuning D Rate

1. Add small Kd_rate (e.g., 0.01)
2. Increase until oscillations are damped

### Tuning I Gain

1. Only add if steady-state error persists
2. Start with very small Ki (e.g., 0.001)
3. Increase until error is eliminated

## TODO: Hardware-Specific Replacements

Before flight, replace these placeholder sections with actual hardware details:

### IMU Driver

Replace `sensor.cpp` with actual IMU driver:
- MPU9250, BNO055, ICM20948, or other sensor
- I2C or SPI communication
- Register initialization
- Data scaling

### PWM Output

Replace `actuator.cpp` PWM method:
- Servo library for standard servos
- Hardware PWM for ESCs
- Custom timer for motor drivers

### Pin Assignments

Update `pins.h` with actual PCB pin numbers from schematic.

### Sensor Orientation

Add rotation matrix in `estimator.cpp` if IMU is not aligned with rocket axes.

## Safety Notes

- Always test with actuators disconnected first
- Verify neutral position before connecting actuators
- Use test stand for initial tuning
- Never arm system with nozzle pointed toward people or equipment
- Always have emergency stop ready

## License

Copyright (c) 2026. All rights reserved.
