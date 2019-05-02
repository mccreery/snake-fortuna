#include <avr/pgmspace.h>
#include <string.h>

#include "font.h"
#include "hw/display.h"
#include "hw/ili934x.h"
#include "color.h"

const uint8_t * pgm_font_data;
char font_data_first_char;
blitter_t * font_blitter;

point_t font_size;
point_t font_pitch;

size_t line_length(const char * str) {
    size_t n;
    for(n = 0; str[n] && str[n] != '\n'; n++);

    return n;
}

int16_t line_width(const char * str) {
    return line_length(str) * font_size.x;
}

void draw_char(int16_t x, int16_t y, char c) {
    set_rect((rectangle_t){{x, y}, font_size});
    start_blitting();

    size_t px_size = font_size.x * font_size.y;
    size_t px_offset = (c - font_data_first_char) * px_size;
    font_blitter(pgm_font_data, px_offset, px_size);
}

void draw_substring(int16_t x, int16_t y, const char * str, size_t start, size_t end, text_align_t align) {
    point_t c_pos = {x, y - font_pitch.y};

    for(size_t i = start; i < end && str[i]; i++) {
        // New line
        if(i == start || str[i-1] == '\n') {
            c_pos.x = x;
            c_pos.y += font_pitch.y;

            if(align) {
                c_pos.x -= line_width(str + i) * align / 2;
            }
        }

        // Normal char
        if(str[i] != '\n') {
            draw_char(c_pos.x, c_pos.y, str[i]);
            c_pos.x += font_pitch.x;
        }
    }
}
