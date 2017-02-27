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

struct cRGB led[NUMLEDS];

// void max7219_write(int address, int data)
// {
// 	digitalWrite(SS_PIN, LOW);
// 	SPI.transfer(address);
// 	SPI.transfer(data);
// 	digitalWrite(SS_PIN, HIGH);
// }

// void max7219_init()
// {
// 	int i;
// 	max7219_write(DECODE, 0xFF);  /* Decode all digits */
// 	max7219_write(SCAN_LIMIT, 5);  /* Limit scan digits 0..5 */
// 	max7219_write(INTENSITY, 0x7); /* Half intensity */
// 	max7219_write(SHDN, 0x1);  /* Turn ON display */ 

// 	/* Turn all segments off */  
// 	for(i = 0; i < 6; i++)
// 	{
// 		max7219_write(DIG + i, 0xF);
// 	}
// }

// void max7219_display(uint16_t number)
// {
// 	for(int i = 0; i < 6; i++)
// 	{
// 		max7219_write(DIG + i, number % 10);
// 		number /= 10;
// 	}
// }

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
	uint8_t i;
	char c;

	UBRRL = UB_L;
	UBRRH = UB_H;

	UCSRB = (1<<RXEN)|(1<<TXEN);

	putstr(hello);

	// SPI.begin();
	// SPI.setDataMode(SPI_MODE1);
	// SPI.setClockDivider(SPI_CLOCK_DIV16);  // TODO make this a higher clock rate
	// SPI.setBitOrder(MSBFIRST);
	// digitalWrite(SS_PIN, HIGH);
	// pinMode(SS_PIN, OUTPUT);
	// max7219_init();

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

		/* Push frame to display. */
		ws2812_setleds(led, NUMLEDS);
	}
}
