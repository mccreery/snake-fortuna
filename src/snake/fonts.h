#ifndef SNAKE_FONTS_H_
#define SNAKE_FONTS_H_

#include "blob.h"
#include "font/font.h"

USEBLOB(segment24x32_2);
USEBLOB(sqr8x8_2);

extern uint16_t white_palette[16];

static inline void init_font_seg(void) {
    pgm_font_data = BLOB(segment24x32_2);
    font_data_first_char = '0';
    font_blitter = blit_2_palette;

    font_size = (point_t){24, 32};
    font_pitch = (point_t){24, 36};
}

static inline void init_font_sqr(void) {
    pgm_font_data = BLOB(sqr8x8_2);
    font_data_first_char = ' ';
    font_blitter = blit_2_palette;

    font_size = (point_t){8, 8};
    font_pitch = (point_t){8, 16};
}

#endif
