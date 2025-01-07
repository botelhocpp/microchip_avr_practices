## Copyright (c) 2025 Pedro Botelho
## All rights reserved

## General
AS = avr-as
CC = avr-gcc
AR = ar
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

.PHONY: build clean

all: lib/$(LIB)

lib/$(LIB): obj/start.o $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

obj/%.o: src/%.c inc/%.h inc/bsp.h
	$(CC) $(CFLAGS) $(TARGET_MCU) -c $< -o $@

obj/start.o: src/start.s
	$(AS) $(TARGET_MCU) -c $< -o $@

build: obj/app.elf bin/app.hex

clean:
	rm obj/* lib/*
