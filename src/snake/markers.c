/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include <avr/pgmspace.h>

#include "contexts.h"
#include "markers.h"
#include "turns.h"

#define START_X (_BV(GRID_SIZE_LOG2 - 1))
#define START_Y START_X
#define START_MARKER (marker_t){{START_X, START_Y}, {0, 1}}

// Simple square
static const marker_t demo_moves[] PROGMEM = {
    {{1, 6}, {1, 0}},
    {{4, 6}, {0, -1}},
    {{4, 29}, {1, 0}},
    {{18, 29}, {0, -1}},
    {{18, 19}, {1, 0}},
    {{24, 19}, {0, -1}},
    {{24, 14}, {-1, 0}},
    {{18, 14}, {0, -1}},
    {{18, 11}, {1, 0}},
    {{23, 11}, {0, 1}},
    {{23, 13}, {1, 0}},
    {{5, 13}, {0, 1}},
    {{5, 16}, {-1, 0}},
    {{28, 16}, {0, 1}},
    {{28, 19}, {1, 0}},
    {{2, 19}, {0, -1}},
    {{2, 18}, {1, 0}},
    {{7, 18}, {0, -1}},
    {{7, 12}, {1, 0}},
    {{13, 12}, {0, -1}},
    {{13, 3}, {1, 0}},
    {{29, 3}, {0, -1}},
    {{29, 30}, {-1, 0}},
    {{25, 30}, {0, -1}},
    {{25, 27}, {1, 0}},
    {{1, 27}, {0, 1}}
};
static marker_t demo_move;
static uint8_t demo_index;

marker_t tail, head;
point_t next_turn;

#include "grid.h"

static void read_demo_move(void) {
    for(uint8_t i = 0; i < sizeof(demo_move); i++) {
        ((uint8_t *)&demo_move)[i] = pgm_read_byte(&((uint8_t *)&demo_moves[demo_index])[i]);
    }
}

void setup_markers(void) {
    clear_turns();
    tail = head = START_MARKER;
    next_turn = POINT_ZERO;

    if(demo) {
        demo_index = 0;
        read_demo_move();
    }

    // Add a single cell to the snake
    move_head();
}

static void apply_direction(marker_t * marker) {
    if(marker->direction.x > 0) {
        ++marker->position.x;
        if(marker->position.x == GRID_SIZE) { marker->position.x = 0; }
    } else if(marker->direction.x < 0) {
        --marker->position.x;
        if(marker->position.x == -1) { marker->position.x = GRID_SIZE - 1; }
    }

    if(marker->direction.y > 0) {
        ++marker->position.y;
        if(marker->position.y == GRID_SIZE) { marker->position.y = 0; }
    } else if(marker->direction.y < 0) {
        --marker->position.y;
        if(marker->position.y == -1) { marker->position.y = GRID_SIZE - 1; }
    }
}

void move_head(void) {
    if(demo) {
        if(point_eq(demo_move.position, head.position)
                && enqueue_turn(demo_move)) {
            head.direction = demo_move.direction;

            ++demo_index;
            if(demo_index == sizeof(demo_moves) / sizeof(demo_moves[0])) {
                demo_index = 0;
            }
            read_demo_move();
        }
    } else if(!point_eq(next_turn, POINT_ZERO)) {
        // Drop turns if we've reached the limit
        if(enqueue_turn((marker_t){head.position, next_turn})) {
            head.direction = next_turn;
        }
        next_turn = POINT_ZERO;
    }
    apply_direction(&head);
}

void move_tail(void) {
    apply_direction(&tail);

    if(has_turn()) {
        marker_t turn = peek_turn();

        if(point_eq(tail.position, turn.position)) {
            dequeue_turn();
            tail.direction = turn.direction;
        }
    }
}
