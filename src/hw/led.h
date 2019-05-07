/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <stdbool.h>

/*
 * Initializes the LED's output pin.
 */
static inline void init_led(void) {
    // Enable LED pin
    DDRB |= _BV(DDB7); // Output
    PORTB &= ~_BV(PORTB7); // Init LED to off
}

/*
 * Enables or disables the LED.
 */
static inline void set_led(bool lit) {
    if(lit) {
        PORTB |= _BV(PORTB7);
    } else {
        PORTB &= ~_BV(PORTB7);
    }
}

/*
 * Toggles the LED.
 */
static inline void toggle_led(void) {
    // Input pin doubles as a toggle output
    PINB |= _BV(PINB7);
}

#endif
