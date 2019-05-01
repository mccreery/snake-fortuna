#include <string.h>

#include "contexts.h"
#include "fonts.h"
#include "gameloop.h"
#include "hw/buttons.h"
#include "hw/display.h"
#include "scoreio.h"
#include "hue.h"

#define SCORE_LEFT ((LCD_SIZE.y - 128) / 2)
#define SCORE_RIGHT ((LCD_SIZE.y + 128) / 2)
#define SCORE_TOP 32

static void draw_hiscore(uint8_t i) {
    char left[7];

    if(i == 9) {
        left[0] = '1';
        left[1] = '0';
    } else {
        left[0] = ' ';
        left[1] = '1' + i;
    }
    left[2] = ' ';

    // Load names here
    memcpy(left + 3, hiscores[i].name, 3);
    left[6] = '\0';

    char score[5];
    get_score_text(hiscores[i], score);

    int16_t y = SCORE_TOP + i * 16;
    draw_string(SCORE_LEFT, y, left, LEFT);
    draw_string(SCORE_RIGHT, y, score, RIGHT);
}

__attribute__((section (".noinit")))
static score_t rising_score;
static uint8_t score_rank;

void setup_hiscores(void) {
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    palette = white_palette;
    init_font_sqr();

    draw_string(LCD_SIZE.y / 2, 16, "HI SCORES", CENTER);
    clear_score(&rising_score);
    score_rank = 10;

    char str[7];
    str[0] = ' ';
    str[1] = ' ';
    str[5] = '\0';

    for(uint8_t i = 0; i < 10; i++) {
        draw_hiscore(i);
    }

    // Draw initial score
    if(tick_func_last == tick_board) {
        char buf[5];
        get_score_text(rising_score, buf);
        draw_string(SCORE_RIGHT + font_pitch.x,
                SCORE_TOP + score_rank * font_pitch.y, buf, LEFT);
    }

    next_setup = setup_mainmenu;
    next_tick = tick_mainmenu;
}

static void tick_new_title(void) {
    static uint8_t hue;

    hue += 2;
    if(hue >= 192) hue = 0;

    palette = default_palette;
    gen_palette_mono(hue192to565(hue));
    draw_string(LCD_SIZE.y / 2, 16, "NEW HI SCORE!", CENTER);
}

void tick_hiscores(void) {
    static char buf[5];

    if(tick_func_last == tick_board && compare_score(rising_score, score) < 0) {
        if(frame_counter & 3) return;

        int8_t msd = increment_score(&rising_score);
        get_score_text(rising_score, buf);

        uint8_t new_score_rank = score_rank;
        while(new_score_rank > 0 && compare_score(rising_score, hiscores[new_score_rank - 1]) > 0) {
            --new_score_rank;
        }

        if(new_score_rank != score_rank) {
            next_setup = setup_entername;
            next_tick = tick_entername;

            // Clear text
            blitter_t * b_temp = font_blitter;
            font_blitter = blit_clear;
            draw_string(SCORE_RIGHT + font_size.x, SCORE_TOP + score_rank * font_pitch.y, buf, LEFT);
            font_blitter = b_temp;

            score_rank = new_score_rank;
            msd = 0;
        }

        draw_substring(SCORE_RIGHT + font_pitch.x + msd * font_pitch.x,
                SCORE_TOP + score_rank * font_pitch.y, buf, msd, 4, LEFT);
    } else {
        if(score_rank < 10) { tick_new_title(); }
        tick_anybutton();
    }
}
