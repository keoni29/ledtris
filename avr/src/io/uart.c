/**
 * Filename: uart.c
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 */

#include <avr/io.h>
#include "system_config.h"
#include "uart.h"

/* Highest possible baudrate @ 16MHz */
#define BAUDRATE 1000000UL

#define UB ((F_CPU / (BAUDRATE * 16UL)) - 1)
#define UB_L (UB & 0xFF)
#define UB_H (UB >> 8)

/**
 * Initialize the UART.
 */
inline void uart_init(void)
{
    UBRRL = UB_L;
	UBRRH = UB_H;

	UCSRB = (1<<RXEN)|(1<<TXEN);
}


/**
 * Read a character from the UART.
 * Waits until a character is received.
 * @return The received character
 */
inline uint8_t uart_read(void)
{
    while(!(UCSRA & (1 << RXC)));
    return UDR;
}


/** 
 * Write a string to the UART.
 * Waits for the whole string to be sent.
 */
void uart_write_string(const char *string)
{
	/* Wait until sender is ready. Then send a character. */
	while(*string)
	{
		while (!(UCSRA & (1 << UDRE)));
		UDR = *string;
		++string;
	}
}
