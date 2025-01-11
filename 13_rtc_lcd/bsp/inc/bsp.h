// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef BSP_H
#define BSP_H

// Register Definitions
#include "atmega328p.h"

// Components Definitions
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"
#include "vectors.h"
#include "usart.h"
#include "buzzer.h"
#include "servo.h"
#include "adc.h"
#include "twi.h"
#include "spi.h"
#include "eeprom.h"
#include "utils.h"
#include "lcd.h"
#include "rtc.h"

#define BUILTIN_LED (5) // PB5

#define ENABLE_INTERRUPT    (true)
#define DISABLE_INTERRUPT   (false)

#endif // BSP_H
