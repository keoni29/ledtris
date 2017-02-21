/* Filename: ledtris.ino
 * Project: NES ledtris
 * Author: Koen van Vliet
 * 
 * This sketch uses the Adafruit Neopixel library
 * which can be found here: 
 *   https://github.com/adafruit/Adafruit_NeoPixel
 *
 * Max frame rate is 30 Hz (limitation of the WS2812B)
 * Display size is 10x20 pixels
 *
 */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* If you do not have this extra 4x2 pixel display set it to 0. */
#define NEXT_BLOCK_DISPLAY_ENABLED 1

/* User defined data pin for driving the WS2812B */
#define PIN            A0

/* Amount of pixels in the display */
#if NEXT_BLOCK_DISPLAY_ENABLED
#define NUMPIXELS      208
#else
#define NUMPIXELS      200
#endif

/* Create a new neopixel object specifying the 
 * amount of pixels, data pin, data format and frequency */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  /* Initialize the library and clear the display. */
  pixels.begin();
  pixels.show();
  
  /* Begin serial with maximum baudrate for uno. */
  Serial.begin(1000000);
  
  /* Flush input */
  while(Serial.available())
  {
    Serial.read();
  }
}

void loop()
{
  uint8_t r, g, b;
  uint16_t i;
  
  /* Frame synchronization */
  do{
    while(!Serial.available());
    r = Serial.read();
  } while (r != 0xFF);
  
  /* Receive a frame. */
  for(i = 0; i < NUMPIXELS; ++i)
  {
    while(!Serial.available());
    r = Serial.read();

    while(!Serial.available());
    g = Serial.read();

    while(!Serial.available());
    b = Serial.read();
    
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }

  /* Push pixels to the display */
  pixels.show();
  
}
