#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "blob.h"
#include "contexts.h"
#include "gameloop.h"
#include "hue.h"
#include "hw/buttons.h"
#include "hw/display.h"
#include "hw/ili934x.h"
#include "fonts.h"

#define BRIGHT 0xffff
#define DARK   0x7bef

USEBLOB(sqr8x8_4);
#define NO_OPTIONS (sizeof(options) / sizeof(*options))

__attribute__((section (".noinit")))
static uint16_t dim_palette[16];

static const char * const options[] = {
    "PLAY GAME",
    "HI SCORES",
    "CREDITS"
};
static uint8_t highlighted;

static uint16_t demo_counter;
#define DEMO_TIMEOUT 1050

USEBLOB(snake_2);

static void draw_logo(void) {
    rectangle_t r = {
        {(LCD_SIZE.y - 240) / 2, 32},
        {240, 105}
    };
    set_rect(r);

    palette = default_palette;
    gen_palette_mono(0x07e0, 5);
    palette[5] = 0x7800;

    write_cmd(MEMORY_WRITE);
    blit_2_palette(BLOB(snake_2), 0, r.size.x * r.size.y);
}

static void display_option(uint8_t option) {
    palette = option == highlighted ? white_palette : dim_palette;
    draw_string(LCD_SIZE.y / 2, 160 + option * font_pitch.y, options[option], CENTER);
}

static void highlight(uint8_t option) {
    option = option % NO_OPTIONS;
    if(option >= NO_OPTIONS) { option += NO_OPTIONS; }

    if(highlighted != option) {
        uint8_t hl_last = highlighted;
        highlighted = option;

        display_option(hl_last);
        display_option(highlighted);
    }
}

void setup_mainmenu(void) {
    demo = false;
    demo_counter = 0;

    palette = dim_palette;
    gen_palette_mono(0x7bef, 5);

    init_font_sqr();
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    draw_logo();

    int16_t spacing_x = font_pitch.x;
    font_pitch.x = 12;
    draw_string((LCD_SIZE.y - 240) / 2 + 80, 56, "SNAKE \nFORTUNA", CENTER);
    font_pitch.x = spacing_x;

    for(uint8_t i = 0; i < NO_OPTIONS; i++) {
        display_option(i);
    }
}

void tick_mainmenu(void) {
    update_buttons();

    uint8_t pressed = button_changed & ~button_state;
    if(pressed) {
        demo_counter = 0;
    } else if(++demo_counter == DEMO_TIMEOUT) {
        demo = true;
        context_switch(setup_board, tick_board);
        return;
    }

    if(pressed & _BV(PNORTH)) {
        if(highlighted == 0) {
            highlight(NO_OPTIONS - 1);
        } else {
            highlight(highlighted - 1);
        }
    } else if(pressed & _BV(PSOUTH)) {
        if(highlighted == NO_OPTIONS - 1) {
            highlight(0);
        } else {
            highlight(highlighted + 1);
        }
    } else if(pressed & _BV(PCENTER)) {
        switch(highlighted) {
            case 0:
                context_switch(setup_board, tick_board);
                break;
            case 1:
                context_switch(setup_hiscores, tick_hiscores);
                break;
            case 2:
                context_switch(setup_credits, tick_anybutton);
                break;
        }
    }
}
