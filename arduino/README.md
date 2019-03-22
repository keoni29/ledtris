These are instructions for using the Arduino Uno as a LED display driver for Ledtris.

# Connections
- Connect `5V` of the display to a 5VDC, 12A power supply. This is the minimum current rating if you want to drive all LEDs at their highest intensity.
- Connect `GND` of the display to the Arduino's and the power supply's `GND`
- Connect `Din` of the display to the Arduino's `A0` pin

# Building and uploading
1. Open `ledtris.ino` in the Arduino IDE.
2. Upload the sketch to your Arduino Uno or compatible.

To verify you did everything right

3. Start the emulator using the startup script `start_ledtris.sh`

You should now see some pixels lighting up on the display.
