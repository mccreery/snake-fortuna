#include "turns.h"

__attribute__((section (".noinit")))
static marker_t turns[256];

static uint8_t turn_start;
static uint8_t turn_end;
static bool empty = true;

bool enqueue_turn(marker_t turn) {
    if(!empty && turn_start == turn_end) {
        // The buffer is full
        return false;
    } else {
        turns[turn_end++] = turn;
        empty = false;
        return true;
    }
}

bool has_turn(void) {
    return !empty;
}

marker_t peek_turn(void) {
    return turns[turn_start];
}

marker_t dequeue_turn(void) {
    marker_t turn = peek_turn();
    ++turn_start;

    if(turn_start == turn_end) {
        empty = true;
    }
    return turn;
}

void clear_turns(void) {
    empty = true;
    turn_end = turn_start;
}
