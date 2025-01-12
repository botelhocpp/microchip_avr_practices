// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef BSP_H
#define BSP_H

// Register Definitions
#include "atmega328p.h"

// Components Definitions
#include "adc.h"
#include "eeprom.h"
#include "gpio.h"
#include "interrupt.h"
#include "lcd.h"
#include "rtc.h"
#include "servo.h"
#include "spi.h"
#include "timer.h"
#include "twi.h"
#include "usart.h"
#include "utils.h"
#include "vectors.h"
#include "wdt.h"
#include "cpu.h"

#define BUILTIN_LED (5) // PB5

#define ENABLE_INTERRUPT    (true)
#define DISABLE_INTERRUPT   (false)

#endif // BSP_H
