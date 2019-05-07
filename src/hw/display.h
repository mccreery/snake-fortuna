/*
 * Original work: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 * Modification: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>

#include "geom.h"

// Memory Access Control register bits
#define MY  7
#define MX  6
#define MV  5
#define ML  4
#define BGR 3
#define MH  2

#define LCD_WIDTH (240)
#define LCD_HEIGHT (320)
#define LCD_SIZE (point_t){LCD_WIDTH, LCD_HEIGHT}
#define LCD_BOUNDS (rectangle_t){POINT_ZERO, LCD_SIZE}

#define TRANSPOSE(r) (rectangle_t){r.pos, (point_t){r.size.y, r.size.x}}

extern point_t local_display_size;

typedef enum {
    NORTH = _BV(BGR) | _BV(MX),
    EAST  = _BV(BGR) | _BV(MX) | _BV(MY) | _BV(MV),
    SOUTH = _BV(BGR) | _BV(MY),
    WEST  = _BV(BGR) | _BV(MV)
} orientation_t;

// Functions ported from Steve Gunn's library
void init_lcd(void);
void lcd_brightness(uint8_t i);
void set_frame_rate_hz(uint8_t f);

/*
 * sets the physical screen orientation.
 * the orientation is the direction the top of the screen
 * (furthest from the ribbon) is facing.
 */
void set_screen_orientation(orientation_t orientation);

/*
 * sets the drawing rectangle on the display.
 */
void set_rect(const rectangle_t r);

/**
 * @brief Fill the rectangle with the specified color.
 */
void fill_rect(const rectangle_t r, const uint16_t color);

#endif
