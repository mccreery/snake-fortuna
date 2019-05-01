#ifndef MARKERS_H_
#define MARKERS_H_

#include <avr/io.h>
#include "turns.h"

#define GRID_SIZE_LOG2 5
#define GRID_SIZE _BV(GRID_SIZE_LOG2)
#define GRID_MASK (_BV(GRID_SIZE_LOG2) - 1)

extern marker_t tail, head;
extern point_t next_turn;

/**
 * Resets the markers to their game start positions.
 */
void setup_markers(void);

/**
 * Adds one cell to the head of the snake.
 */
void move_head(void);

/**
 * Removes one cell from the tail of the snake.
 */
void move_tail(void);

#endif
