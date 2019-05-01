#include "markers.h"
#include "turns.h"

#define START_MARKER (marker_t){{_BV(GRID_SIZE_LOG2 - 1), _BV(GRID_SIZE_LOG2 - 1)}, {0, 1}}

marker_t tail, head;
point_t next_turn;

void setup_markers(void) {
    clear_turns();
    tail = head = START_MARKER;
    next_turn = POINT_ZERO;

    // Add a single cell to the snake
    move_head();
}

static inline void apply_direction(marker_t * marker) {
    marker->position.x = (marker->position.x + marker->direction.x) & GRID_MASK;
    marker->position.y = (marker->position.y + marker->direction.y) & GRID_MASK;
}

void move_head(void) {
    if(!point_eq(next_turn, POINT_ZERO)) {
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
