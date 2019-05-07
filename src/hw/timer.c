/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include <avr/io.h>

#include "timer.h"

void prescale_timer0(uint8_t prescaler) {
    TCCR0B = (TCCR0B & 0xF8) | (prescaler & 0x7);
}

void set_timer0_compare(uint8_t max) {
    TCCR0A = (TCCR0A & 0xFC) | _BV(WGM01);
    TCCR0A &= ~_BV(WGM02);

    OCR0A = max;
}
