/**
 * Filename: alive_led.h
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 * 
 * Blink an LED to indicate the system is still alive.
 * 
 * Usage:
 *      1. Initialize alive LED
 *      2. Toggle alive LED
 */

#ifndef _ALIVE_LED_H
#define _ALIVE_LED_H

void alive_led_init(void);
void alive_led_toggle(void);

#endif // _ALIVE_LED_H