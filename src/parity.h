/**
 * Defines a static inline parity function for common reuse
 */
#ifndef PARITY_H_
#define PARITY_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Calculates the parity of a block of memory.
 */
static inline uint8_t parity(void * data, size_t nbytes) {
    uint8_t parity = 0;

    for(size_t i = 0; i < nbytes; i++) {
        parity ^= ((uint8_t *)data)[i];
    }
    return parity;
}

#endif
