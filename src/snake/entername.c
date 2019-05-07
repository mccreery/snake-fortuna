/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "hw/buttons.h"
#include "hw/display.h"
#include "scoreio.h"

static uint8_t blinking;

#define CHAR_X(i) (LCD_SIZE.y / 2 - 4 + ((i) - 1) * 16)
#define CHAR_Y 96

static char get_next_char(char c) {
    if(c == 'Z') {
        return 'A';
    } else {
        return c + 1;
    }
}

static char get_prev_char(char c) {
    if(c == 'A') {
        return 'Z';
    } else {
        return c - 1;
    }
}

static void draw_name_char(uint8_t i) {
    draw_char(CHAR_X(i), CHAR_Y, score.name[i]);
}

static void insert_new_score(void) {
    uint8_t i;
    for(i = 10; i > 0 && compare_score(score, hiscores[i - 1]) > 0; i--);

    if(i < 10) {
        // Shift existing scores right
        for(uint8_t j = 9; j > i; j--) {
            hiscores[j] = hiscores[j-1];
        }
        hiscores[i] = score;
        update_scores();
    }
}

void setup_entername(void) {
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    palette = white_palette;
    init_font_sqr();
    blinking = 0;

    draw_string(LCD_SIZE.y / 2, 64, "ENTER YOUR NAME", CENTER);
    for(uint8_t i = 0; i < 3; i++) {
        draw_name_char(i);
    }
}

void tick_entername(void) {
    static uint8_t blink_counter;

    ++blink_counter;
    update_buttons();

    if(BUTTONS_PRESSED & _BV(PNORTH)) {
        score.name[blinking] = get_prev_char(score.name[blinking]);
        blink_counter = 0;
    } else if(BUTTONS_PRESSED & _BV(PSOUTH)) {
        score.name[blinking] = get_next_char(score.name[blinking]);
        blink_counter = 0;
    } else if(BUTTONS_PRESSED & _BV(PCENTER)) {
        draw_name_char(blinking);
        ++blinking;

        // When name is finished
        if(blinking >= 3) {
            insert_new_score();
            context_switch(setup_mainmenu, tick_mainmenu);
            return;
        }
    }

    switch(blink_counter & 63) {
        case 0:
            draw_name_char(blinking);
            break;
        case 31:
            fill_rect((rectangle_t){{CHAR_X(blinking), CHAR_Y}, font_size}, 0);
            break;
    }
}
