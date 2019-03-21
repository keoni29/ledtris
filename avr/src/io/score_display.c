/**
 * Filename: score_display.c
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 * 
 * A MAX7219 based 6 digit seven segment score display.
 */

#include <avr/io.h>
#include "score_display.h"


#define PORT_SPI PORTB
#define DDR_SPI DDRB
#define DD_MOSI 3
#define DD_SCK 5
#define DD_SS 2

/* Seven segment display driver MAX7219 register addresses */
#define DIG 0x01
#define DECODE 0x09
#define INTENSITY 0x0A
#define SCAN_LIMIT 0x0B
#define SHDN 0x0C
#define DISPLAY_TEST 0x0F


static void spi_init(void);
static void max7219_init(void);
static void max7219_write(int address, int data);


void score_display_init(void)
{
    spi_init();
    max7219_init();
}


void score_display_set(uint32_t number)
{
    int i;
	for(i = 0; i < 3; i++)
	{
		max7219_write(DIG + i + 3, number % 10);
		number /= 10;
	}

	for(i = 0; i < 4; i++)
	{
		max7219_write(DIG + i, number % 10);
		number /= 10;
	}
}


static inline void spi_init(void)
{
    ////PORT_SPI = (1<<DD_SS);
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	/* Master SPI mode enabled clk/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


static inline void max7219_init(void)
{
	int i;
	max7219_write(DECODE, 0xFF);  /* Decode all digits */
	max7219_write(SCAN_LIMIT, 5);  /* Limit scan digits 0..5 */
	max7219_write(INTENSITY, 0x7); /* Half intensity */
	max7219_write(SHDN, 0x1);  /* Turn ON display */
	max7219_write(DISPLAY_TEST, 0x0); /* Disable display test mode! */

	/* Turn all segments off */  
	for(i = 0; i < 6; i++)
	{
		max7219_write(DIG + i, 0xF);
	}
}


static void max7219_write(int address, int data)
{
	PORT_SPI |= (1<<DD_SS);

	SPDR = address;
	while(!(SPSR & (1<<SPIF)));
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));

	PORT_SPI &= ~(1<<DD_SS);
}
