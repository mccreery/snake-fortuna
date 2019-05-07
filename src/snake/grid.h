/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef GRID_H_
#define GRID_H_

#include "geom.h"

uint16_t read_cell(const point_t pos);
void write_cell(const point_t pos, const uint16_t color);
void clear_grid(void);

/**
 * Fits the grid inside the bounding box using the grid_size.
 */
void fit_grid(rectangle_t bounds);

extern point_t grid_position;
extern uint8_t cell_size;
extern uint8_t grid_size;

#endif
