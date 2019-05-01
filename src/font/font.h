#ifndef FONT_H_
#define FONT_H_

#include <stdint.h>
#include <stddef.h>

#include "blit.h"
#include "geom.h"

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} text_align_t;

extern const uint8_t * pgm_font_data;
extern char font_data_first_char;
extern blitter_t * font_blitter;

extern point_t font_size;
extern point_t font_pitch;

size_t line_length(const char * str);
int16_t line_width(const char * str);

void draw_char(int16_t x, int16_t y, char c);
void draw_substring(int16_t x, int16_t y, const char * str, size_t start, size_t end, text_align_t align);

static inline void draw_string(int16_t x, int16_t y, const char * str, text_align_t align) {
    draw_substring(x, y, str, 0, -1, align);
}

#endif
