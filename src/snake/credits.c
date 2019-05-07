/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include <string.h>

#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "hw/buttons.h"
#include "hw/display.h"

static const char * const CREDITS =
        "GAME BY SAM MCCREERY\n"
        "DISPLAY FUNCTIONS MODIFIED\n"
        "FROM STEVE GUNN'S LIBRARY\n"
        "SPECIAL THANKS TO\n"
        "WIKIPEDIA AND STACKOVERFLOW";

void setup_credits(void) {
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    palette = white_palette;
    init_font_sqr();
    draw_string(LCD_SIZE.y / 2, 64, CREDITS, CENTER);

    next_setup = setup_mainmenu;
    next_tick = tick_mainmenu;
}
