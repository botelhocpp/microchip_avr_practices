## Copyright (c) 2025 Pedro Botelho
## All rights reserved

## General
CC = avr-gcc
AR = ar
OBJCOPY = avr-objcopy
MCU = atmega328p
LIB = libavrbsp.a

## Compilation options
CFLAGS = $(BSP_INC) -Os -g -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections 
LDFLAGS = $(BSP_INC) -Wl,-Map,obj/app.map -Wl,--gc-sections 
ARFLAGS = rcs
TARGET_CPU = -mmcu=$(MCU)
BSP_INC = -Iinc

## BSP Files
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: build clean

all: lib/$(LIB)

lib/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

obj/%.o: src/%.c inc/%.h inc/bsp.h
	$(CC) $(CFLAGS) $(TARGET_CPU) -c $< -o $@

build: obj/app.elf bin/app.hex

clean:
	rm obj/* lib/*
