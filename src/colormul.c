/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include "color.h"

uint16_t mul_color(uint16_t color, uint16_t mask) {
    uint8_t red = color >> 11;
    uint8_t green = (color >> 5) & 0x3f;
    uint8_t blue = color & 0x1f;

    // masks are padded out for a perfect shift
    uint8_t m_red = mask >> 11;
    uint8_t m_green = (mask >> 5) & 0x3f;
    uint8_t m_blue = mask & 0x1f;
    m_red += m_red >> 4;
    m_green += m_green >> 5;
    m_blue += m_blue >> 4;

    red = (red * m_red) >> 5;
    green = (green * m_green) >> 6;
    blue = (blue * m_blue) >> 5;

    return (red << 11) | (green << 5) | blue;
}

uint16_t brightness_4(uint16_t color, uint8_t brightness) {
    uint16_t red = color >> 11;
    uint16_t green = (color >> 5) & 0x3f;
    uint16_t blue = color & 0x1f;

    // pad top 8
    brightness += brightness >> 3;

    red = (red * brightness) >> 4;
    green = (green * brightness) >> 4;
    blue = (blue * brightness) >> 4;

    return (red << 11) | (green << 5) | blue;
}
