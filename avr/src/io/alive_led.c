/**
 * Filename: alive_led.c
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 */

#include <avr/io.h>
#include "alive_led.h"

#define PORT_ALIVE PORTD
#define DDR_ALIVE DDRD
#define DD_ALIVE 5


inline void alive_led_init(void)
{
    DDR_ALIVE |= (1 << DD_ALIVE);
}

inline void alive_led_toggle(void)
{
    PORT_ALIVE ^= (1 << DD_ALIVE);
}
