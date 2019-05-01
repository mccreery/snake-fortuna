#ifndef HUE_H_
#define HUE_H_

#include <stdint.h>

/**
 * Converts a hue on a circle mapped from 0..359 to 0..191,
 * to a rgb565 color.
 */
uint16_t hue192to565(uint8_t hue);

#endif
