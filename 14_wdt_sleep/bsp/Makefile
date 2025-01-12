## Copyright (c) 2025 Pedro Botelho
## All rights reserved

## General
AS = avr-as
CC = avr-gcc
AR = ar
OBJDUMP = avr-objdump
MCU = atmega328p
LIB = libavrbsp.a

## Compilation options
CFLAGS = -I$(BSP_INC) -Os -std=gnu99 -Wall -Wl,-Tsrc/memory.lds -nostdlib
LDFLAGS = $(CFLAGS) -Wl,-e,_start -nostartfiles
ARFLAGS = rcs
TARGET_MCU = -mmcu=$(MCU)
BSP_INC = inc

## BSP Files
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: dump build clean

all: build

lib/$(LIB): obj/start.o $(OBJS) inc/bsp.h inc/vectors.h
	$(AR) $(ARFLAGS) $@ $^ -lgcc

obj/%.o: src/%.c inc/%.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/twi.o: src/twi.c inc/twi.h inc/gpio.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/spi.o: src/spi.c inc/spi.h inc/gpio.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/rtc.o: src/rtc.c inc/rtc.h inc/timer.h inc/utils.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/lcd.o: src/lcd.c inc/lcd.h inc/timer.h inc/utils.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/servo.o: src/servo.c inc/servo.h inc/gpio.h inc/timer.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/buzzer.o: src/buzzer.c inc/buzzer.h inc/gpio.h inc/timer.h inc/atmega328p.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/start.o: src/start.s
	$(AS) $(TARGET_MCU) -c $< -o $@

build: lib/$(LIB)

dump: obj/dump.txt

obj/dump.txt: lib/$(LIB)
	$(OBJDUMP) -D $< > $@

clean:
	rm obj/* lib/*
