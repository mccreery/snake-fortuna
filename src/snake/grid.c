/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include "grid.h"
#include "hw/display.h"
#include "hw/ili934x.h"

#define GRID_BORDER 0x1082

uint8_t grid_size;
uint8_t cell_size;
point_t grid_position;

static int16_t min16(int16_t a, int16_t b) {
    return a < b ? a : b;
}

void fit_grid(rectangle_t bounds) {
    cell_size = min16(bounds.size.x, bounds.size.y) / grid_size;
    int16_t grid_real_size = cell_size * grid_size;

    grid_position.x = bounds.pos.x + (bounds.size.x - grid_real_size) / 2;
    grid_position.y = bounds.pos.y + (bounds.size.y - grid_real_size) / 2;
}

static inline rectangle_t get_cell_rect(const point_t pos) {
    return (rectangle_t){
            {pos.x * cell_size + grid_position.x,
                    pos.y * cell_size + grid_position.y},
            {cell_size - 1, cell_size - 1}};
}

static uint16_t read_color(void) {
    write_cmd(MEMORY_READ);
    (void)DATA_ADDR;

    uint16_t color = 0;
    color |= DATA_ADDR >> 3 << 11;
    color |= DATA_ADDR >> 2 << 5;
    color |= DATA_ADDR >> 3;
    return color;
}

uint16_t read_cell(const point_t pos) {
    set_rect(get_cell_rect(pos));
    return read_color();
}

void write_cell(const point_t pos, uint16_t color) {
    fill_rect(get_cell_rect(pos), color);
}

static inline void repeat_color(uint16_t color, uint8_t n) {
    for(uint8_t i = 0; i < n; i++) {
        write_data16(color);
    }
}

static inline void draw_normal_row(void) {
    for(uint8_t x = 0; x < grid_size; x++) {
        repeat_color(0, cell_size - 1);
        write_data16(GRID_BORDER);
    }
}

void clear_grid(void) {
    int16_t size = grid_size * cell_size;
    set_rect((rectangle_t){grid_position, {size, size}});
    write_cmd(MEMORY_WRITE);

    for(uint8_t y = 0; y < grid_size; y++) {
        for(uint8_t z = 1; z < cell_size; z++) {
            draw_normal_row();
        }
        repeat_color(GRID_BORDER, size);
    }
}
