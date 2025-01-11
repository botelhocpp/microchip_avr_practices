# Microchip AVR Practices with ATmega328P

## Full Bare-metal Codes for Microchip's AVR ATmega328P microcontroller using Arduino Uno board.

This repository has the intent of bringing each functionality of the MCU without any underlying layer. 

Basically I build a static library called "avrbsp". It has a simple linker script, a Makefile, as well as functions and definitions for each peripheral of the MCU. My objective is to enable easy development with this MCU.

## Contents

- [X] Blinky
- [X] GPIO
- [X] External Interrupts
- [X] Timers
- [X] USART
- [X] PWM
- [X] Servo/Buzzer
- [X] ADC
- [X] libc*
- [X] TWI (I2C)
- [X] SPI
- [X] EEPROM
- [X] RTC/LCD**

*Standard C Library for AVR (libc)
**DS3231 and LCD 16x2 via TWI (I2C)
