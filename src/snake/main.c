#include <avr/interrupt.h>
#include <avr/io.h>

#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "hw/buttons.h"
#include "hw/clock.h"
#include "scoreio.h"

uint16_t white_palette[16];

void main(void) {
    set_prescaler_log2(0);
    cli();
    EIMSK = 0;
    init_buttons();

    palette = white_palette;
    gen_palette_mono(0xffff, 1);

    load_scores();
    game_loop(setup_mainmenu, tick_mainmenu);
}
