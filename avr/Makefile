# TODO Fix this makefile

F_CPU = 8000000UL
DEVICE = atmega16

# Tools:
CC = avr-gcc

LIB       = light_ws2812
EXAMPLES  = ledtris
DEP		  = Light_WS2812/ws2812_config.h Light_WS2812/light_ws2812.h

#TODO -O1 for future releases
CFLAGS = -I. -ILight_WS2812 -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
CFLAGS+= -g2 -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast
#CFLAGS+= -Wa,-ahls=$<.lst

LDFLAGS = -Wl,--relax,--section-start=.text=0,-Map=main.map

all:	$(EXAMPLES) 

$(LIB): $(DEP)
	@$(CC) $(CFLAGS) -o Objects/$@.o -c Light_WS2812/$@.c 

$(EXAMPLES): $(LIB) 
	@$(CC) $(CFLAGS) -o Objects/$@.o $@.c Light_WS2812/$^.c
	@avr-size Objects/$@.o
	@avr-objcopy -j .text  -j .data -O ihex Objects/$@.o $@.hex
	@avr-objdump -d -S Objects/$@.o >Objects/$@.lss

flash:
	avrdude -cusbasp -pm16 -U flash:w:ledtris.hex
reset:
	avrdude -cusbasp -pm16
fuses:
	avrdude -cusbasp -pm16 -U lfuse:w:0xff:m -U hfuse:w:0x94:m 

.PHONY:	clean

clean:
	rm -f *.hex Objects/*.o Objects/*.lss
