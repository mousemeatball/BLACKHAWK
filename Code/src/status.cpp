/*
 * status.cpp - LED and Buzzer Status Module Implementation
 * 
 * Status indication via LED patterns and buzzer
 */

#include "status.h"
#include "safety.h"
#include "pins.h"

static uint32_t blink_counter = 0;

void status_init() {
    pinMode(PIN_LED_STATUS, OUTPUT);
    pinMode(PIN_LED_ARMED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    
    digitalWrite(PIN_LED_STATUS, LOW);
    digitalWrite(PIN_LED_ARMED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
}

void status_update() {
    blink_counter++;
    SystemState state = safety_get_state();
    
    switch (state) {
        case STATE_BOOT:
            // LED off
            digitalWrite(PIN_LED_STATUS, LOW);
            digitalWrite(PIN_LED_ARMED, LOW);
            digitalWrite(PIN_BUZZER, LOW);
            break;
            
        case STATE_SENSOR_CALIBRATION:
            // Slow blink (1 Hz = 10 ticks at 100Hz)
            digitalWrite(PIN_LED_STATUS, (blink_counter % 10 < 5) ? HIGH : LOW);
            digitalWrite(PIN_LED_ARMED, LOW);
            digitalWrite(PIN_BUZZER, LOW);
            break;
            
        case STATE_IDLE:
            // Steady on
            digitalWrite(PIN_LED_STATUS, HIGH);
            digitalWrite(PIN_LED_ARMED, LOW);
            digitalWrite(PIN_BUZZER, LOW);
            break;
            
        case STATE_ARMED:
            // Fast blink (4 Hz = 25 ticks at 100Hz)
            digitalWrite(PIN_LED_STATUS, (blink_counter % 25 < 12) ? HIGH : LOW);
            digitalWrite(PIN_LED_ARMED, (blink_counter % 25 < 12) ? HIGH : LOW);
            digitalWrite(PIN_BUZZER, LOW);
            break;
            
        case STATE_ACTIVE_CONTROL:
            // Solid on
            digitalWrite(PIN_LED_STATUS, HIGH);
            digitalWrite(PIN_LED_ARMED, HIGH);
            digitalWrite(PIN_BUZZER, LOW);
            break;
            
        case STATE_FAULT:
            // Rapid blink (8 Hz = 12.5 ticks at 100Hz)
            digitalWrite(PIN_LED_STATUS, (blink_counter % 12 < 6) ? HIGH : LOW);
            digitalWrite(PIN_LED_ARMED, (blink_counter % 12 < 6) ? HIGH : LOW);
            digitalWrite(PIN_BUZZER, (blink_counter % 12 < 6) ? HIGH : LOW);
            break;
    }
}

void status_set_led(bool on) {
    digitalWrite(PIN_LED_STATUS, on ? HIGH : LOW);
}

void status_set_buzzer(bool on) {
    digitalWrite(PIN_BUZZER, on ? HIGH : LOW);
}
