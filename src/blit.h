#ifndef BLIT_H_
#define BLIT_H_

#include <stddef.h>
#include <stdint.h>

extern uint16_t default_palette[16];

/**
 * Current palette.
 * The 1-bit blitter uses 0 and 15,
 * the 2-bit blitter uses 0, 5, 10, 15,
 * the 4-bit blitter uses all 16 colors.
 */
extern uint16_t * palette;

/**
 * Generates a monochrome palette in the current address.
 * @param color The brightest color in the palette.
 * @param step The distance between palette entries to modify.
 */
void gen_palette_mono(uint16_t color, uint8_t step);

/**
 * Blitter type. Blitter functions should only be passed multiples of 8,
 * as some may discard any remainder.
 */
typedef void blitter_t(const uint8_t * pgm_data, size_t px_offset, size_t px_size);

blitter_t blit_1_palette,
        blit_2_palette,
        blit_4_palette,
        blit_565le_rgb,
        blit_clear;

#endif
