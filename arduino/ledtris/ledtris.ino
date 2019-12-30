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

/* User defined data pin for driving the WS2812B */
#define PIN            A0

/* User defined LOAD/CS pin for driving the MAX7219 */
#define SS_PIN          9

/* The number of LEDs in the display(s) combined */
#define NOF_PIXELS      208

/* Seven segment display driver MAX7219 register addresses */
#define DIG 0x1
#define DECODE 0x9
#define INTENSITY 0xA
#define SCAN_LIMIT 0x0B
#define SHDN 0xC
#define DISPLAY_TEST 0x0F

/* Create a new neopixel object specifying the
 * amount of pixels, data pin, data format and frequency */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NOF_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
  max7219_write(DISPLAY_TEST, 0x0); /* Disable display test mode! */

  /* Turn all segments off */
  for(i = 0; i < 6; i++)
  {
    max7219_write(DIG + i, 0xF);
  }
}

void max7219_display(uint32_t number)
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

  for (int i = 0; i < NOF_PIXELS; ++i)
  {
    pixels.setPixelColor(i, 0);
  }

  pixels.show();

  /* Set up SPI and initialize sevseg display driver  */
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
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
  uint8_t r, g, b, s1, s2, s3;
  uint32_t score;
  static uint32_t prevscore = -1;

  /* Frame synchronization */
  do
  {
    while(!Serial.available());
    r = Serial.read();
  } while (r != 0xFF);

  /* Receive a frame. */
  for(int i = 0; i < NOF_PIXELS; ++i)
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
  while(!Serial.available());
  s3 = Serial.read();

  score = ((uint32_t)s1 << 16)  + ((uint16_t)s2 << 8) + s3;

  if (score != prevscore)
  {
    max7219_display(score);
  }
  prevscore = score;

  /* Push pixels to the display */
  pixels.show();
}
