## General
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
MCU = atmega328p

## Compilation options
CFLAGS = -Os -g -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections 
LDFLAGS = -Wl,-Map,obj/app.map -Wl,--gc-sections 
TARGET_CPU = -mmcu=$(MCU)

## Programmer options
PROGRAMMER_TYPE = -c arduino
PROGRAMMER_PORT = -P /dev/ttyUSB0 -b 115200

.PHONY: flash clean

all: build flash

obj/app.elf: obj/main.o
	$(CC) $(LDFLAGS) $(TARGET_CPU) $^ -o $@ 

bin/app.hex: obj/app.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(TARGET_CPU) -c $< -o $@

build: obj/app.elf bin/app.hex

flash: bin/app.hex
	$(AVRDUDE) $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_PORT) -D -U flash:w:$<:i

clean:
	rm obj/* bin/*
