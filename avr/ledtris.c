/* AVR version of ledtris display driver using the light_ws2812 library.
 * TODO give credit and clean up stuff.
 */

#include <util/delay.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
#include "light_ws2812.h"

#define NUMLEDS 208

/* Highest possible baudrate @ 16MHz */
#define BAUDRATE 500000UL
#define UB ((F_CPU / (BAUDRATE * 16UL)) - 1)
#define UB_L (UB & 0xFF)
#define UB_H (UB >> 8)

#define PORT_SPI PORTB
#define DDR_SPI DDRB
#define DD_MOSI 3
#define DD_SCK 5
#define DD_SS 2

#define PORT_ALIVE PORTD
#define DDR_ALIVE DDRD
#define DD_ALIVE 5

/* Seven segment display driver MAX7219 register addresses */
#define DIG 0x1
#define DECODE 0x9
#define INTENSITY 0xA
#define SCAN_LIMIT 0x0B
#define SHDN 0xC


struct cRGB led[NUMLEDS];

void max7219_write(int address, int data)
{
	PORT_SPI |= (1<<DD_SS);

	SPDR = address;
	while(!(SPSR & (1<<SPIF)));
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));

	PORT_SPI &= ~(1<<DD_SS);
}

void max7219_init()
{
	int i;
	max7219_write(DECODE, 0xFF);  /* Decode all digits */
	max7219_write(SCAN_LIMIT, 5);  /* Limit scan digits 0..5 */
	max7219_write(INTENSITY, 0x7); /* Half intensity */
	max7219_write(SHDN, 0x1);  /* Turn ON display */ 

	/* Turn all segments off */  
	for(i = 0; i < 6; i++)
	{
		max7219_write(DIG + i, 0xF);
	}
}

void max7219_display(uint32_t number)
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

/** Blocking uart send string */
void putstr(const char *str)
{
	/* Wait until sender is ready. Then send a character. */
	while(*str)
	{
		while (!(UCSRA & (1 << UDRE)));
		UDR = *str;
		++str;
	}
}

int main(void)
{
	const char hello[] = "LEDTRIS - kv 2017";
	uint8_t i, s1, s2;
	uint16_t score;

	UBRRL = UB_L;
	UBRRH = UB_H;

	UCSRB = (1<<RXEN)|(1<<TXEN);

	putstr(hello);

	//PORT_SPI = (1<<DD_SS);
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	/* Master SPI mode enabled clk/64 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);

	DDR_ALIVE |= (1 << DD_ALIVE);

	max7219_init();
	max7219_display(123456);

	_delay_ms(100);


	while(1)
	{
		/* Wait for sync character */
		do {
			while(!(UCSRA & (1 << RXC)));
		} while(UDR != 0xFF);

		/* Receive and store frame in framebuffer. */
		for (i = 0; i < NUMLEDS; ++i)
		{
			while(!(UCSRA & (1 << RXC)));
			led[i].r = UDR;
			while(!(UCSRA & (1 << RXC)));
			led[i].g = UDR;
			while(!(UCSRA & (1 << RXC)));
			led[i].b = UDR;
		}

		while(!(UCSRA & (1 << RXC)));
		s1 = UDR;
		while(!(UCSRA & (1 << RXC)));
		s2 = UDR;

		score = (uint16_t)s1 * 256 + s2;
		max7219_display(score);

		/* Push frame to display. */
		ws2812_setleds(led, NUMLEDS);
	}
}
