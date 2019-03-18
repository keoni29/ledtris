/**
 * Filename: uart.h
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 * 
 * Usage:
 *      1. Initialize UART
 *      2. Read character from UART
 *      3. Write string to UART
 */

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void uart_init(void);
uint8_t uart_read(void);
void uart_write_string(const char *string);


#endif // _UART_H