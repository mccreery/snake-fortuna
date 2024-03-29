/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include <string.h>

#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "grid.h"
#include "hw/buttons.h"
#include "hw/display.h"
#include "levels.h"
#include "markers.h"
#include "rng.h"
#include "scoreio.h"

#define SNAKE_COLOR 0x07E0
#define APPLE_COLOR 0xF800

#define SCORE_LEFT ((grid_position.x - 24) / 2)
#define SCORE_RIGHT (320 - SCORE_LEFT - 24)
#define SCORE_Y 24

static const point_t DIRECTIONS[4] = {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0}
};

bool demo;

static const point_t demo_apples[] PROGMEM = {
    {11, 12},
    {28, 18},
    {15, 29},
    {1, 28},
    {25, 3}
};
static uint8_t apple_index;

static uint8_t level_countdown;
static uint8_t level;
static int8_t step;
static char level_text[] = "LEVEL XX";

static point_t get_demo_apple(void) {
    point_t apple;

    for(uint8_t i = 0; i < sizeof(apple); i++) {
        ((uint8_t *)&apple)[i] = pgm_read_byte(&((uint8_t *)&demo_apples[apple_index])[i]);
    }
    apple_index = (apple_index + 1) % (sizeof(demo_apples) / sizeof(demo_apples[0]));

    return apple;
}

static void process_buttons(void) {
    update_buttons();
    const uint8_t pressed = ~button_state & button_changed;

    for(uint8_t i = 0; i < 4; i++) {
        if((pressed & _BV(PNORTH + i)) &&
                !point_eq(NEGATE_POINT(head.direction), DIRECTIONS[i])) {
            next_turn = DIRECTIONS[i];
        }
    }
}

static void draw_score_suffix(int16_t x, int16_t y, const score_t score, uint8_t i) {
    if(demo) {
        init_font_seg();
        palette = white_palette;
    }
    y += i * font_pitch.y;

    for(; i < 4; i++) {
        draw_char(x, y, '0' + score.score[i]);
        y += font_pitch.y;
    }
    if(demo) init_font_sqr();
}

static void place_apple(void) {
    point_t pos;
    if(demo) {
        pos = get_demo_apple();
    } else {
        pos.x = -1;
    }

    while(pos.x == -1 || read_cell(pos) != 0) {
        pos.x = rng8() & GRID_MASK;
        pos.y = rng8() & GRID_MASK;
    }

    write_cell(pos, APPLE_COLOR);
}

static void start_level(void) {
    uint16_t real_score = get_score(score);
    level = real_score >> 4;
    load_level(level & 3);

    uint8_t tens = level / 10;
    level_text[sizeof(level_text) - 3] = '0' + tens;
    level_text[sizeof(level_text) - 2] = '0' + (level - tens * 10);

    // Increase speed every time the levels repeat
    step = 5 - (level >> 2);
    if(step < 0) step = 0;

    clear_turns();
    tail = head = (marker_t){{1, 1}, {0, 1}};
    tail.position.y -= get_score(score) + 1;

    int8_t y = tail.position.y + 1;
    if(y < 0) y = 0;

    for(; y <= head.position.y; y++) {
        write_cell((point_t){tail.position.x, y}, SNAKE_COLOR);
    }
    level_countdown = 175;
    place_apple();
}

void tick_board(void) {
    static uint8_t step_counter;

    if(level_countdown > 0) {
        --level_countdown;

        // Don't draw level number on the demo
        if(!demo && (level_countdown & 31) == 0) {
            init_font_sqr();

            if((level_countdown & 63) == 0) {
                draw_string(LCD_SIZE.y / 2, 0, level_text, CENTER);
            } else {
                font_blitter = blit_clear;
                draw_string(LCD_SIZE.y / 2, 0, level_text, CENTER);
                font_blitter = blit_2_palette;
            }

            init_font_seg();
        }
        return;
    }

    if(demo) {
        tick_anybutton();
    } else {
        process_buttons();
    }

    // We've been switched out
    if(tick_func_last == tick_board) return;

    if(++step_counter == step) {
        step_counter = 0;
    } else {
        return;
    }

    move_head();
    bool eaten_apple = read_cell(head.position) == APPLE_COLOR;

    if(eaten_apple) {
        // Handle previous apple eaten
        int8_t i = increment_score(&score);
        draw_score_suffix(SCORE_LEFT, SCORE_Y, score, i);

        if((get_score(score) & 15) == 0) {
            if(demo) {
                // Don't let the demo get past level 0
                context_switch(setup_mainmenu, tick_mainmenu);
            } else {
                start_level();
            }
            return;
        }
    } else {
        // Clear up tail
        move_tail();
        write_cell(tail.position, 0);
    }

    if(!eaten_apple && read_cell(head.position) != 0) {
        // Game over
        if(demo) {
            context_switch(setup_mainmenu, tick_mainmenu);
        } else {
            context_switch(setup_hiscores, tick_hiscores);
        }
    } else {
        // No game over, so paint head
        write_cell(head.position, SNAKE_COLOR);

        if(eaten_apple) {
            // Paint a new apple (previous apple overwritten by head)
            place_apple();
        }
    }
}

void setup_board(void) {
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    grid_size = _BV(GRID_SIZE_LOG2);
    fit_grid(TRANSPOSE(LCD_BOUNDS));
    setup_markers();
    clear_score(&score);

    palette = white_palette;
    init_font_sqr();

    int16_t y_spacing = font_pitch.y;
    font_pitch.y = font_size.y;
    draw_string(SCORE_LEFT + 12, 0, "YOUR\nSCORE", CENTER);
    draw_string(SCORE_RIGHT + 12, 0, "HIGH\nSCORE", CENTER);
    font_pitch.y = y_spacing;

    if(demo) {
        draw_string(LCD_SIZE.y / 2, 0, "D E M O", CENTER);
        next_setup = setup_mainmenu;
        next_tick = tick_mainmenu;
        apple_index = 0;
    }

    init_font_seg();
    draw_score_suffix(SCORE_LEFT, SCORE_Y, score, 0);
    draw_score_suffix(SCORE_RIGHT, SCORE_Y, hiscores[0], 0);
    if(demo) init_font_sqr();

    start_level();
}
