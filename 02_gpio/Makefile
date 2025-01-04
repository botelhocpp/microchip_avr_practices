## Copyright (c) 2025 Pedro Botelho
## All rights reserved

## General
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
MCU = atmega328p

## Compilation options
CFLAGS = -I$(BSP_INC) -Os -g -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections 
LDFLAGS = -I$(BSP_INC) -Wl,-Map,obj/app.map -Wl,--gc-sections 
TARGET_CPU = -mmcu=$(MCU)
BSP = $(BSP_INC)/bsp.h
BSP_INC = bsp/inc
LIB = avrbsp
LIB_PATH = bsp/lib

## Programmer options
PROGRAMMER_TYPE = -c arduino
PROGRAMMER_PORT = -P /dev/ttyUSB0 -b 115200

.PHONY: build flash clean

all: $(LIB) build flash

$(LIB):
	$(MAKE) -C bsp

obj/app.elf: obj/main.o
	$(CC) $(LDFLAGS) $(TARGET_CPU) $^ -o $@ -L$(LIB_PATH) -l$(LIB)

bin/app.hex: obj/app.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

obj/%.o: src/%.c $(BSP)
	$(CC) $(CFLAGS) $(TARGET_CPU) -c $< -o $@

build: obj/app.elf bin/app.hex

flash: bin/app.hex
	$(AVRDUDE) $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_PORT) -D -U flash:w:$<:i

clean:
	$(MAKE) -C bsp clean
	rm obj/* bin/*
