/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef GAMELOOP_H_
#define GAMELOOP_H_

#include <stdint.h>

typedef void (*tick_t)(void);

// Can be used for transitions between screens
extern tick_t tick_func_last;
extern uint8_t frame_counter;

void game_loop(tick_t setup, tick_t tick);
void context_switch(tick_t setup, tick_t tick);

#endif
