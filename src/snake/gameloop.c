#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "gameloop.h"
#include "hw/display.h"
#include "hw/ili934x.h"
#include "hw/led.h"

tick_t tick_func_last;
uint8_t frame_counter;

static tick_t tick_func;

void context_switch(tick_t setup, tick_t tick) {
    // Disable frame interrupt
    EIMSK &= ~_BV(INT6);

    tick_func_last = tick_func;
    tick_func = tick;

    if(setup) {
        //write_cmd(DISPLAY_OFF);
        setup();
        //write_cmd(DISPLAY_ON);
    }
    EIMSK |= _BV(INT6);
}

void game_loop(tick_t setup, tick_t tick) {
    init_lcd();

#ifdef DEBUG
    init_led();
    DDRE &= ~_BV(INT6);
    PORTE |= _BV(INT6);
#endif

    // Enable frame interrupt (rising edge)
    EICRB |= _BV(ISC61) | _BV(ISC60);
    EIMSK |= _BV(INT6);

    // Enable sleep and wakeup
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();

    sleep_cpu();
    context_switch(setup, tick);

    do {
        sleep_cpu();
        if(tick_func) tick_func();

#ifdef DEBUG
        set_led(~PINE & _BV(INT6));
#endif
        ++frame_counter;
    } while(true);
}

EMPTY_INTERRUPT(INT6_vect)
