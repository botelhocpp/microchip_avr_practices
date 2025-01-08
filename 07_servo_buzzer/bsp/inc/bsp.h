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

#define BUILTIN_LED (5) // PB5

#endif // BSP_H
