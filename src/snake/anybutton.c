#include "contexts.h"
#include "font/font.h"
#include "gameloop.h"
#include "hw/buttons.h"
#include "hw/display.h"

tick_t next_setup;
tick_t next_tick;

static const char * const ANY_BUTTON = "PRESS ANY BUTTON";

void tick_anybutton(void) {
    // Don't really care what value this has initially
    static uint8_t frame_counter;

    update_buttons();
    if(button_changed & ~button_state) {
        context_switch(next_setup, next_tick);
    } else {
        frame_counter += 2;

        // grab the lower 6 bits of the counter and invert them every cycle
        uint8_t f = frame_counter ^ -((frame_counter >> 6) & 1);
        f &= 0x3f;

        uint16_t color =
            (f >> 1 << 11) | // red
            (f << 5) | // green
            (f >> 1); // blue

        palette = default_palette;
        gen_palette_mono(color);
        draw_string(LCD_SIZE.y / 2, 224, "PRESS ANY BUTTON", CENTER);
    }
}
