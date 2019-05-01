#ifndef CLOCK_H_
#define CLOCK_H_

#include <avr/io.h>
#include <stdint.h>

static inline void set_prescaler_log2(uint8_t x) {
    CLKPR = _BV(CLKPCE);
    CLKPR = x;
}

#endif
