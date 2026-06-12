/*
 * debug.h - Debug Interface Module
 * 
 * Serial debug output for telemetry and command parsing
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// Initialize debug interface
void debug_init();

// Output telemetry data
void debug_output();

// Process incoming serial commands
void debug_process_commands();

// Enable/disable debug output
void debug_set_enabled(bool enabled);

// Check if debug is enabled
bool debug_is_enabled();

#endif // DEBUG_H
