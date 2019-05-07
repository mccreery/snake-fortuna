/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include "rng.h"

uint8_t rng8_seed = 3;

uint8_t rng8(void) {
    return rng8_seed = (rng8_seed >> 1) ^
            (-(rng8_seed & 1) & RNG8_POLYNOMIAL);
}
