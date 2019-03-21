/**
 * Filename: score_display.h
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 * 
 * Usage:
 *      1. Initialize score display
 *      2. Show score
 */

#ifndef _SCORE_DISPLAY_H
#define _SCORE_DISPLAY_H

#include <stdint.h>

void score_display_init(void);
void score_display_set(uint32_t number);

#endif // _SCORE_DISPLAY_H