// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            A0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      200

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
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
  
  /* Receive a frame */
  for(i = 0; i<NUMPIXELS; i++)
  {
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }  
  pixels.show();
  
}
