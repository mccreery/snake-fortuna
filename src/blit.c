#include <avr/pgmspace.h>
#include <stdbool.h>

#include "blit.h"
#include "hw/ili934x.h"
#include "color.h"

__attribute__((section (".noinit")))
uint16_t default_palette[16];
uint16_t * palette = default_palette;
uint8_t transparent_index = 16;

// true when the last command was a MEMORY_WRITE
static bool writing;

static void write_palette_pixel(uint8_t i) {
    if(i != transparent_index) {
        if(!writing) {
            write_cmd(WRITE_MEMORY_CONTINUE);
            writing = true;
        }
        // Write pixel
        write_data16(palette[i]);
    } else {
        if(writing) {
            write_cmd(READ_MEMORY_CONTINUE);
            writing = false;

            // Skip dummy byte
            (void)DATA_ADDR;
        }

        // Skip pixel
        (void)DATA_ADDR;
        (void)DATA_ADDR;
        (void)DATA_ADDR;
    }
}

void gen_palette_mono(uint16_t color) {
    palette[0] = 0;
    palette[15] = color;

    for(uint8_t i = 1; i < 15; i++) {
        palette[i] = brightness_4(color, i);
    }
}

void start_blitting(void) {
    write_cmd(MEMORY_WRITE);
    writing = true;
}

void blit_1_palette(const uint8_t * pgm_data, size_t px_offset, size_t px_size) {
    pgm_data += px_offset / 8;
    const uint8_t * pgm_data_end = pgm_data + px_size / 8;

    for(; pgm_data < pgm_data_end; pgm_data++) {
        uint8_t x = pgm_read_byte(pgm_data);

        for(int8_t i = 7; i >= 0; i--) {
            uint8_t j = (x >> i) & 1 ? 15 : 0;
            write_palette_pixel(j);
        }
    }
}

void blit_2_palette(const uint8_t * pgm_data, size_t px_offset, size_t px_size) {
    pgm_data += px_offset / 4;
    const uint8_t * pgm_data_end = pgm_data + px_size / 4;

    for(; pgm_data < pgm_data_end; pgm_data++) {
        uint8_t x = pgm_read_byte(pgm_data);

        for(int8_t i = 6; i >= 0; i -= 2) {
            uint8_t j = (x >> i & 3) * 5;
            write_palette_pixel(j);
        }
    }
}

void blit_4_palette(const uint8_t * pgm_data, size_t px_offset, size_t px_size) {
    pgm_data += px_offset / 2;
    const uint8_t * pgm_data_end = pgm_data + px_size / 2;

    for(; pgm_data < pgm_data_end; pgm_data++) {
        uint8_t x = pgm_read_byte(pgm_data);

        write_data16(palette[x >> 4]);
        write_data16(palette[x & 0x0f]);
    }
}

void blit_565le_rgb(const uint8_t * pgm_data, size_t px_offset, size_t px_size) {
    pgm_data += px_offset * 2;
    const uint8_t * pgm_data_end = pgm_data + px_size * 2;

    for(; pgm_data < pgm_data_end; pgm_data += 2) {
        // Swap endianness
        write_data16(pgm_read_byte(pgm_data + 1));
        write_data16(pgm_read_byte(pgm_data));
    }
}

void blit_clear(const uint8_t * pgm_data, size_t px_offset, size_t px_size) {
    for(size_t i = 0; i < px_size; i++) {
        write_data16(palette[0]);
    }
}
