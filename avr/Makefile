# Makefile for AVR Projects

NAME = ledtris

CC = avr-gcc
OBJDIR = build
SRCDIR = src
OUTDIR = dist
INCDIR = inc

SRC = $(shell find $(SRCDIR)/ -type f -name '*.c')
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
ELF = $(OUTDIR)/$(NAME).elf
HEX = $(OUTDIR)/$(NAME).hex

DEVICE = atmega8
AVRDUDE_DEV = m8
F_CPU = 16000000UL

CFLAGS = -Wall -Wextra -pedantic -Wall -Wno-pointer-to-int-cast
CFLAGS += -I$(INCDIR) -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
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
	avrdude -cusbasp -p$(AVRDUDE_DEV) -U flash:w:$(HEX)
reset:
	avrdude -cusbasp -p$(AVRDUDE_DEV)
fuses:
	avrdude -cusbasp -p$(AVRDUDE_DEV) -U lfuse:w:0xff:m -U hfuse:w:0xD9:m 