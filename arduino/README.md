These are instructions for using the Arduino Uno as a LED display driver for Ledtris.

# Connections
- Connect `5V` of the display to a 5VDC, 12A power supply. This is the minimum current rating if you want to drive all LEDs at their highest intensity.
- Connect `GND` of the display to the Arduino's and the power supply's `GND`
- Connect `Din` of the display to the Arduino's `A0` pin

Optionally you can connect a MAX7219-based seven segment display for showing the score.
You need at least 6 digits to display the score.

![max7219 display module](http://embedded-lab.com/blog/wp-content/uploads/2012/12/SPI7SEGDISPTitle.jpg)

- Connect `VCC` of the MAX7219 to the Arduino's `5V` pin
- Connect `GND` of the MAX7219 to the Arduino's `GND` pin
- Connect `CLK` of the MAX7219 to the Arduino's `SPI Clock` pin (pin 13 for Arduino UNO)
- Connect `DIN` of the MAX7219 to the Arduino's `SPI MOSI` pin (pin 11 for Arduino UNO)
- Connect `CS` of the MAX7219 to the Arduino's `CS` pin. Default is pin 9, but this can be changed in the code.

# Building and uploading
1. Open `ledtris.ino` in the Arduino IDE.
2. Upload the sketch to your Arduino Uno or compatible.

To verify you did everything right

3. Start the emulator using the startup script `start_ledtris.sh`

You should now see some pixels lighting up on the display.
