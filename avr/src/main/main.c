/**
 * Filename: main.c
 * Project: NES Ledtris
 * Author: Koen van Vliet <8by8mail@gmail.com>
 * 
 * AVR version of ledtris display driver
 *
 * Uses the light_ws2812 library, which can be found here:
 *   https://github.com/cpldcpu/light_ws2812
 * 
 * Usage:
 *      Send serial data to uart RXD pin (In case of the ATMega8A this is PD0)
 *      1. Send FRAME_SYNC (default 0xFF) to synchronize the display
 *          It is advised to reserve the FRAME_SYNC character for synchronization
 *      
 *      2. Send the pixel RGB data. Each pixel is 3 bytes in order 
 *          red, green, blue. In total 3 x NOF_LEDS bytes are sent.
 *      
 *      3. Only if score display is enabled using ENABLE_SCORE_DISPLAY
 *          Send 3 bytes for the score display MSB first.
 *      
 */

#include "system_config.h"
////#include <util/delay.h>
#include "light_ws2812.h"
#include "score_display.h"
#include "uart.h"
#include "alive_led.h"

/**
 * The number of LEDs in the display(s) combined.
 */
#define NOF_LEDS 208

/**
 * Enables the seven segment score display.
 */
#define ENABLE_SCORE_DISPLAY

/** Synchronization character indicating the start of a frame. */
#define FRAME_SYNC 0xFF

/** Framebuffer */
struct cRGB led[NOF_LEDS];


void update_score_display(void);


int main(void)
{
	const char hello[] = "LEDTRIS - kv 2017\n";
    uint8_t i;

	uart_init();
    alive_led_init();
    ////_delay_ms(100);
	score_display_init();
	score_display_set(0);

	uart_write_string(hello);

	while(1)
	{
		/* Wait for frame synchronization character. */
        while(uart_read() != FRAME_SYNC)
        {
            ;
        }

		/* Receive pixel RGB data and store frame in framebuffer. */
		for (i = 0; i < NOF_LEDS; ++i)
		{
			led[i].r = uart_read();
			led[i].g = uart_read();
			led[i].b = uart_read();
		}

        update_score_display();

		/* Push frame to display(s). */
		ws2812_setleds(led, NOF_LEDS);

        /** 
         * Toggle 'alive' LED to indicate the frame was 
         * received and processed. 
         */
		alive_led_toggle();
	}
}


/** Read score from UART and update the score display if score has changed. */
inline void update_score_display(void)
{
#ifdef ENABLE_SCORE_DISPLAY
    uint8_t s1, s2, s3;
    uint32_t score;
    static uint32_t score_prev = 0;

    /* Read score bytes */ 
    s1 = uart_read();;  
    s2 = uart_read();;
    s3 = uart_read();;

    /* Pack score bytes into a single 32 bit value */
    score = ((uint32_t)s1 << 16) + ((uint32_t)s2 << 8)  + s3;

    /* Only update score display when score changes. */
    if (score != score_prev)
    {	
        score_display_set(score);
    }
    score_prev = score;
#endif // ENABLE_SCORE_DISPLAY
}

