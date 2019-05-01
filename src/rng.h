#ifndef RNG_H_
#define RNG_H_

#include <stdint.h>

extern uint8_t rng8_seed;
#define RNG8_POLYNOMIAL (0xB8)

uint8_t rng8(void);

#endif
