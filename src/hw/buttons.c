#include "buttons.h"

uint8_t button_state;
uint8_t button_changed;

void init_buttons(void) {
    // detect 0.5x A presses
    button_state = BUTTONS_RAW;

    // enable compass input pins
    DDRCOMPASS &= ~COMPASS_MASK;
    PORTCOMPASS |= COMPASS_MASK;

    // enable center input pins
    DDRCENTER &= ~CENTER_MASK;
    PORTCENTER |= CENTER_MASK;
}

void update_buttons(void) {
    // bitplanes: plane0 = low bit of counters, plane1 = high bit of counters
    static uint8_t plane0, plane1;

    // read inputs
    uint8_t changed = button_state ^ BUTTONS_RAW;

    // increment counters
    plane1 = (plane1 ^ plane0) & changed;
    plane0 = ~plane0 & changed;

    // counters which reach 3 need to toggle
    button_changed = plane0 & plane1;
    button_state ^= button_changed;
}
