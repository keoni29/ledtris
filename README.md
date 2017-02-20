### README
With the software and hardware described in this repository you can play classic NES tetris on an LED display based on the popular WS2812b led strips.
__Disclaimer__ Script for emulator only works under unix-like operating systems, because of the way serial ports are accessed. May be ported to windows some day if enough people poke me.

# Contents
This repository contains everything you need to play tetris for NES on a WS2812b LED display. This includes:

- Arduino/AVR sourcecode for driving the LED display and optional seven segment display
- Startup script for FCEUX
- Lua script for streaming gameplay data from FCEUX to LED matrix display.
- Instructions on how to build the LED display
- DXF files for laser cutter


# Usage
This is a step-by-step guide on how to get everything up and running.

## Arduino Firmware
Upload the arduino firmware to your arduino uno or compatible. (Only tested on uno rev3)

###Acquiring the rom
Do a quick google search for "Tetris NES NTSC rom." (I only tested this with the USA version. The PAL release might have some subtle differences.)

## Getting your host ready
Install FCEUX on your desktop/laptop/raspberry pi.

Debian:
`apt-get install fceux`

Archlinux:
`pacman -S fceux` 

## Starting the emulator
Start FCEUX using the `start_fceux.sh` script.
Usage:
```
	./start_ledtris.sh tetris.nes
```

# Troubleshooting
Some troubleshooting tips.

## ttyACM0 not accessible
If you get a message complaining about /dev/ttyACM0 not being accessible check if your arduino is plugged in.
Find your serial port using `ls -l /dev/tty*`. Change the line in sendall.lua that says
```
	local portName = '/dev/ttyACM0'
```
To whatever name your serial port has.

# Contact
If you have any questions regarding this project please contact me:
Koen van Vliet <8by8mail@gmail.com>
