# Makefile for AVR Projects

NAME = ledtris

CC = avr-gcc
OBJDIR = build
SRCDIR = src
OUTDIR = dist

SRC = $(shell find $(SRCDIR)/ -type f -name '*.c')
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
ELF = $(OUTDIR)/$(NAME).elf
HEX = $(OUTDIR)/$(NAME).hex

DEVICE = atmega8
AVRDUDE_DEV = m8
AVRDUDE_TOOL = usbasp

CFLAGS = -Wall -Wextra -pedantic -Wall -Wno-pointer-to-int-cast
CFLAGS += -Iinc/io -Iinc/light_ws2812 -Iinc/main -mmcu=$(DEVICE)
CFLAGS+= -g2 -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  

LDFLAGS = 
#-Wl,--relax,--section-start=.text=0,-Map=main.map

all: $(HEX)

$(HEX):$(ELF)
	@mkdir -p "$(@D)"
	avr-objcopy -j .text -j .data -O ihex $< $@
	
$(ELF): $(OBJ)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(OBJ) -o $@  && avr-size $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean purge flash reset fuses
clean:
	@rm -f $(OBJ) $(ELF) $(HEX) && echo "clean succesful" || echo "clean failed"
	
purge:
	@rm -rf $(OBJDIR) $(OUTDIR) && echo "purge succesful" || echo "purge failed"

flash:
	avrdude -c$(AVRDUDE_TOOL) -p$(AVRDUDE_DEV) -U flash:w:$(HEX)
reset:
	avrdude -c$(AVRDUDE_TOOL) -p$(AVRDUDE_DEV)
fuses:
	avrdude -c$(AVRDUDE_TOOL) -p$(AVRDUDE_DEV) -U lfuse:w:0xff:m -U hfuse:w:0xD9:m 