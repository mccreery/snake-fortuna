#include <avr/pgmspace.h>

#include "contexts.h"
#include "markers.h"
#include "turns.h"

#define START_X (_BV(GRID_SIZE_LOG2 - 1))
#define START_Y START_X
#define START_MARKER (marker_t){{START_X, START_Y}, {0, 1}}

// Simple square
static marker_t demo_moves[] PROGMEM = {
    {{16, 24}, {1, 0}},
    {{26, 24}, {0, 1}},
    {{26, 6}, {-1, 0}},
    {{5, 6}, {0, 1}},
    {{5, 13}, {1, 0}},
    {{11, 13}, {0, -1}},
    {{11, 11}, {-1, 0}},
    {{8, 11}, {0, -1}},
    {{8, 8}, {1, 0}},
    {{21, 8}, {0, 1}},
    {{21, 16}, {1, 0}},
    {{25, 16}, {0, -1}},
    {{25, 8}, {1, 0}},
    {{2, 8}, {0, 1}},
    {{2, 26}, {1, 0}},
    {{12, 26}, {0, -1}},
    {{12, 16}, {1, 0}},
    {{16, 16}, {0, 1}}
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

static inline void apply_direction(marker_t * marker) {
    marker->position.x = (marker->position.x + marker->direction.x) & GRID_MASK;
    marker->position.y = (marker->position.y + marker->direction.y) & GRID_MASK;
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
