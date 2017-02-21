/* Filename: ledtris.ino
 * Project: NES ledtris
 * Author: Koen van Vliet
 * 
 * This sketch uses the Adafruit Neopixel library
 * which can be found here: 
 *   https://github.com/adafruit/Adafruit_NeoPixel
 *
 * Max frame rate is 30 Hz (limitation of the WS2812B)
 *
 */
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* If you do not have this extra 4x2 pixel display set it to 0. */
#define NEXT_BLOCK_DISPLAY_ENABLED 1

/* User defined data pin for driving the WS2812B */
#define PIN            A0

/* User defined LOAD/CS pin for driving the MAX7219 */
#define SS_PIN          9

/* Amount of pixels in the display */
#if NEXT_BLOCK_DISPLAY_ENABLED
#define NUMPIXELS      208
#else
#define NUMPIXELS      200
#endif

/* Seven segment display driver MAX7219 register addresses */
#define DIG 0x1
#define DECODE 0x9
#define INTENSITY 0xA
#define SCAN_LIMIT 0x0B
#define SHDN 0xC


/* Create a new neopixel object specifying the 
 * amount of pixels, data pin, data format and frequency */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void max7219_write(int address, int data)
{
  digitalWrite(SS_PIN, LOW);
  SPI.transfer(address);
  SPI.transfer(data);
  digitalWrite(SS_PIN, HIGH);
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

void max7219_display(uint16_t number)
{
  for(int i = 0; i < 6; i++)
  {
      max7219_write(DIG + i, number % 10);
      number /= 10;
  }
}

void setup()
{
  /* Initialize the library and clear the display. */
  pixels.begin();
  
  for (int i = 0; i < NUMPIXELS; ++i)
  {
    pixels.setPixelColor(i, 0);
  }
  
  pixels.show();
  
  /* Set up SPI and initialize sevseg display driver  */
  SPI.begin();
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);  // TODO make this a higher clock rate
  SPI.setBitOrder(MSBFIRST);
  digitalWrite(SS_PIN, HIGH);
  pinMode(SS_PIN, OUTPUT);
  max7219_init();
  
  /* Begin serial with maximum baudrate for uno. */
  Serial.begin(1000000);
  
  /* Flush serial receive buffer */
  while(Serial.available())
  {
    Serial.read();
  }
}

void loop()
{
  uint8_t r, g, b, s1, s2;
  uint16_t score;
  static uint16_t prevscore = -1;
  
  /* Frame synchronization */
  do
  {
    while(!Serial.available());
    r = Serial.read();
  } while (r != 0xFF);
  
  /* Receive a frame. */
  for(int i = 0; i < NUMPIXELS; ++i)
  {
    while(!Serial.available());
    r = Serial.read();

    while(!Serial.available());
    g = Serial.read();

    while(!Serial.available());
    b = Serial.read();
    
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  
  /* Receive current score and write to sevseg display if it has changed since the last frame. */
  while(!Serial.available());
  s1 = Serial.read();
  while(!Serial.available());
  s2 = Serial.read();
  
  score = (uint16_t)s1 * 256 + s2;
  
  if (score != prevscore)
  {
    max7219_display(score);
  }
  prevscore = score;

  /* Push pixels to the display */
  pixels.show();
}
