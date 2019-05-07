/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>
#include <stdint.h>

// holds the current button state, with pressed buttons being 0 bits
// mask each button using e.g. _BV(PNORTH)
extern uint8_t button_state;
// holds the buttons which changed as a result of the previous call
// to update_buttons
extern uint8_t button_changed;

#define BUTTONS_PRESSED (~button_state & button_changed)
#define BUTTONS_RELEASED (button_state & button_changed)

#define PORTCOMPASS PORTC
#define DDRCOMPASS  DDRC
#define PINCOMPASS  PINC

#define PNORTH       PC2
#define PEAST        PC3
#define PSOUTH       PC4
#define PWEST        PC5
#define COMPASS_MASK (_BV(PNORTH) | _BV(PEAST) | _BV(PSOUTH) | _BV(PWEST))

#define PORTCENTER PORTE
#define DDRCENTER  DDRE
#define PINCENTER  PINE

#define PCENTER     PE7
#define CENTER_MASK _BV(PCENTER)

// Grabs the current value of the button inputs
#define BUTTONS_RAW ((PINCOMPASS & COMPASS_MASK) | (PINCENTER & CENTER_MASK))

void init_buttons(void);
void update_buttons(void);

#endif
