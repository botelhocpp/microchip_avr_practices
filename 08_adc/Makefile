## Copyright (c) 2025 Pedro Botelho
## All rights reserved

## General
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRDUDE = avrdude
MCU = atmega328p

## Compilation options
CFLAGS = -I$(BSP_INC) -Os -std=gnu99 -Wall -Wl,-Tbsp/src/memory.lds -nostdlib
LDFLAGS = $(CFLAGS) -Wl,-Map,obj/app.map -Wl,-e,_start -nostartfiles
TARGET_MCU = -mmcu=$(MCU)
BSP = $(BSP_INC)/bsp.h
BSP_INC = bsp/inc
LIB = avrbsp
LIB_PATH = bsp/lib

## Programmer options
PROGRAMMER_TYPE = -c arduino
PROGRAMMER_PORT = -P /dev/ttyUSB0 -b 115200

.PHONY: build flash clean dump

all: build flash

$(LIB):
	$(MAKE) -C bsp

obj/app.elf: obj/main.o
	$(CC) $(LDFLAGS) $(TARGET_MCU) $^ -o $@ -L$(LIB_PATH) -l$(LIB)

bin/app.hex: obj/app.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

obj/%.o: src/%.c $(BSP)
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

build: $(LIB) obj/app.elf bin/app.hex

flash: bin/app.hex
	$(AVRDUDE) $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_PORT) -D -U flash:w:$<:i

dump: obj/dump.txt

obj/dump.txt: obj/app.elf
	$(OBJDUMP) -D $< > $@

clean:
	$(MAKE) -C bsp clean
	rm obj/* bin/*
