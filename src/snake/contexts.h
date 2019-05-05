#ifndef MENUS_H_
#define MENUS_H_

#include <stdbool.h>

#include "gameloop.h"

void setup_mainmenu(void);
void tick_mainmenu(void);

extern bool demo;
void setup_board(void);
void tick_board(void);

void setup_hiscores(void);
void tick_hiscores(void);
void setup_credits(void);

extern tick_t next_setup;
extern tick_t next_tick;
void tick_anybutton(void);

void setup_entername(void);
void tick_entername(void);

#endif
