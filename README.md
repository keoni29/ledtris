# NES Ledtris
With the software and hardware described in this repository you can play classic NES Tetris on an LED display based on the popular WS2812b led strips.


>__Disclaimer__ Script for emulator only works under UNIX-like operating systems, because of the way serial ports are accessed. Feel free to modify this to work on windows.

# Contents
This repository contains everything you need to play Tetris for NES on a WS2812b
LED display. This includes:

- Arduino/AVR sourcecode for driving the LED display(s) and optional score display
- Startup script for FCEUX (NES emulator)
- Lua script for streaming gameplay data from FCEUX to LED matrix display.
- Instructions on how to build the LED display(s)
- DXF files for laser cutter

# Hardware
You will need the following items

__Playfield display__ measuring 20x10 pixels. Build instructions can be found in `cad/`

__LED display driver__ using an Arduino board or an AVR microcontroller.

- Arduino - See `arduino/README.md` - Tested on Arduino UNO rev3
- AVR - See `avr/README.md` - Tested on ATMega8A

__5 V power supply__ that can deliver at least 12 A. This is the minimum rated current required to drive all LEDs at full intensity.

> __DO NOT__ attempt to use USB power for this. This will damage the USB port or phone charger.

__Next Block display__ measuring 4x2 pixel. (optional)

__Score display__ used to show the player's score. Contains six 7-segment display digits.(optional)

> Both the Arduino and AVR firmware support the __Score display__, however I only made schematics for the AVR version. See `avr/schematic.png`

# Software
You need the FCEUX NES emulator, the `ledtris.lua` script and a Tetris (USA version) ROM.

## NES Emulator
Install FCEUX using your favorite package manager or download it here:
https://sourceforge.net/p/fceultra/code/HEAD/tree/

 Debian : 
 ```
 apt-get install fceux
 ```

 Archlinux : 
 ```
 pacman -S fceux
 ```

## Ledtris script
The emulator's functionality can be extended using lua scripts. The script `ledtris.lua` reads the framebuffer and the memory of the emulator and sends pixel and score data to the LED display driver via a serial port. 
> Script for emulator only works under UNIX-like operating systems, because of the way serial ports are accessed. 
## Tetris ROM
You will also need to acquire a copy of the USA version of Tetris for the NES.
Do a quick google search for "Tetris NES NTSC rom." 
Optionally rename the file to tetris.nes and place it in the same directory as `start_ledtris.sh`.

>I only tested this with the USA version. The PAL release might have some subtle differences.

# Putting everything together
Connect the display(s) to the LED driver.

Connect the LED driver (either Arduino or AVR) to the PC.

Apply 5V power to the LED driver. (NOT USB POWER)

Start FCEUX using the `start_ledtris.sh` script.
Usage:
```
./start_ledtris.sh [/path/to/tetris.nes]
```
You should now see some pixels light up on the display. 
> It looks a bit weird in the menus, because the lua script samples pixel colors 
> from the framebuffer rather than looking at the game's memory for Tetris bricks.
> Just press the `start` button a few times to get to the actual game.

# Troubleshooting
If you get a message complaining about `/dev/ttyACM0` not being accessible 
Find your serial port using `ls -l /dev/tty*` and enter the name of 
the serial port in ledtris.lua
```
	local portName = '/dev/ttyACM0'
```

# Contact
If you have any questions regarding this project please contact me: 
Koen van Vliet - <8by8mail@gmail.com>
