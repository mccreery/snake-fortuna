#include <string.h>

#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "grid.h"
#include "hw/buttons.h"
#include "hw/display.h"
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

static void draw_score_suffix(int16_t x, int16_t y, const score_t score, int8_t i) {
    y += i * font_pitch.y;

    for(; i < 4; i++) {
        draw_char(x, y, '0' + score.score[i]);
        y += font_pitch.y;
    }
}

static void place_apple(void) {
    point_t pos;

    do {
        pos.x = rng8() & GRID_MASK;
        pos.y = rng8() & GRID_MASK;
    } while(read_cell(pos) == SNAKE_COLOR);

    write_cell(pos, APPLE_COLOR);
}

void tick_board(void) {
    process_buttons();
    if(frame_counter & 3) return;

    move_head();
    bool eaten_apple = read_cell(head.position) == APPLE_COLOR;

    if(eaten_apple) {
        // Handle previous apple eaten
        int8_t i = increment_score(&score);
        draw_score_suffix(SCORE_LEFT, SCORE_Y, score, i);
    } else {
        // Clear up tail
        move_tail();
        write_cell(tail.position, 0);
    }

    if(read_cell(head.position) == SNAKE_COLOR) {
        // Game over
        context_switch(setup_hiscores, tick_hiscores);
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

    init_font_seg();
    draw_score_suffix(SCORE_LEFT, SCORE_Y, score, 0);
    draw_score_suffix(SCORE_RIGHT, SCORE_Y, hiscores[0], 0);

    clear_grid();
    place_apple();
}