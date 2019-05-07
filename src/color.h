/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

uint16_t mul_color(uint16_t color, uint16_t mask);
uint16_t brightness_4(uint16_t color, uint8_t brightness);

#endif
