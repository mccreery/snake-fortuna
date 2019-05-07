/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
#include <util/delay.h>

#include "hw/display.h"
#include "hw/ili934x.h"

uint16_t display_width = LCD_WIDTH,
    display_height = LCD_HEIGHT;

void init_lcd(void) {
    // Use PORTC 1:0 for external memory address 9:8
    XMCRB = _BV(XMM2) | _BV(XMM1);
    // Enable external memory
    XMCRA = _BV(SRE);

    // Enable reset and backlight output pins
    DDRC |= _BV(RESET);
    DDRB |= _BV(BLC);
    _delay_ms(1);

    // Hardware reset cycle
    PORTC &= ~_BV(RESET);
    _delay_ms(20);
    PORTC |= _BV(RESET);
    _delay_ms(120);

    // Turn off the display
    write_cmd(DISPLAY_OFF);
    // Leave sleep mode
    write_cmd(SLEEP_OUT);
    _delay_ms(60);

    write_cmd_data(INTERNAL_IC_SETTING,          0x01);
    write_cmd(POWER_CONTROL_1);
        write_data16(0x2608);
    write_cmd_data(POWER_CONTROL_2,              0x10);
    write_cmd(VCOM_CONTROL_1);
        write_data16(0x353E);
    write_cmd_data(VCOM_CONTROL_2, 0xB5);
    write_cmd_data(INTERFACE_CONTROL, 0x01);
        write_data16(0x0000);

    // 16-bit color
    write_cmd_data(PIXEL_FORMAT_SET, 0x55);

    // Clear screen (not sure why needed after reset)
    set_screen_orientation(EAST);
    fill_rect(TRANSPOSE(LCD_BOUNDS), 0);

    // Turn on the display
    write_cmd(DISPLAY_ON);
    _delay_ms(50);

    // Enable VBLANK signal, disable HBLANK signal
    write_cmd_data(TEARING_EFFECT_LINE_ON, 0);

    // Turn on backlight
    PORTB |= _BV(BLC);
}

void fill_rect(const rectangle_t r, const uint16_t color) {
    set_rect(r);
    write_cmd(MEMORY_WRITE);

    for(uint32_t i = 0; i < (uint32_t)r.size.x * r.size.y; i++) {
        write_data16(color);
    }
}

void lcd_brightness(uint8_t i) {
    /* configure timer 2 fast PWM mode 3 */
    TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20);
    OCR2A = i;
}

void set_frame_rate_hz(uint8_t f) {
    uint8_t diva, rtna, period;
    if (f>118)
        f = 118;
    if (f<8)
        f = 8;
    if (f>60) {
        diva = 0x00;
    } else if (f>30) {
        diva = 0x01;
    } else if (f>15) {
        diva = 0x02;
    } else {
        diva = 0x03;
    }
    /*   !!! FIXME !!!  [KPZ-30.01.2015] */
    /*   Check whether this works for diva > 0  */
    /*   See ILI9341 datasheet, page 155  */
    period = 1920.0/f;
    rtna = period >> diva;
    write_cmd(FRAME_CONTROL_IN_NORMAL_MODE);
    write_data(diva);
    write_data(rtna);
}

void set_screen_orientation(orientation_t orientation) {
    write_cmd_data(MEMORY_ACCESS_CONTROL, orientation);

    if(orientation & _BV(MV)) {
        display_width = LCD_HEIGHT;
        display_height = LCD_WIDTH;
    } else {
        display_width = LCD_WIDTH;
        display_height = LCD_HEIGHT;
    }
}

void set_rect(const rectangle_t r) {
    write_cmd(COLUMN_ADDRESS_SET);
    write_data16(r.pos.x);
    write_data16(r.pos.x + r.size.x - 1);
    write_cmd(PAGE_ADDRESS_SET);
    write_data16(r.pos.y);
    write_data16(r.pos.y + r.size.y - 1);
}
