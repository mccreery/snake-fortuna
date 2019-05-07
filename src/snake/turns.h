/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef TURNS_H_
#define TURNS_H_

#include "geom.h"

/**
 * A structure containing a position on the grid and a direction.
 */
typedef struct {
    point_t position;
    point_t direction;
} marker_t;

/**
 * @return true if the turn was enqueued. Fails if the queue is full.
 */
bool enqueue_turn(marker_t turn);

/**
 * @return true if there are any turns in the turn queue.
 */
bool has_turn(void);

/**
 * @return The least recently enqueued turn on the queue.
 */
marker_t peek_turn(void);

/**
 * Removes the least recently enqueued turn on the queue and returns it.
 */
marker_t dequeue_turn(void);

/**
 * Dequeues all turns.
 */
void clear_turns(void);

#endif
