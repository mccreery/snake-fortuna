#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PRESCALE_0 1
#define PRESCALE_8 2
#define PRESCALE_64 3
#define PRESCALE_256 4
#define PRESCALE_1024 5

/*
 * Set the timer prescaler with one of PRESCALE_0..PRESCALE_1024
 */
void prescale_timer0(uint8_t prescaler);

/*
 * Enables the Clear Timer on Cmpare match (CTC) mode for the timer
 */
void set_timer0_compare(uint8_t max);

#endif
