// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef TIMER_H
#define TIMER_H 

#include "bsp.h"

typedef enum timer1_clock_select {
    TIMER1_CLOCK_NONE,
    TIMER1_CLOCK_NO_PS,
    TIMER1_CLOCK_PS_8,
    TIMER1_CLOCK_PS_64,
    TIMER1_CLOCK_PS_256,
    TIMER1_CLOCK_PS_1024,
    TIMER1_CLOCK_EXT_FALLING,
    TIMER1_CLOCK_EXT_RISING
} timer1_clock_select;

typedef enum timer1_compare_output_mode {
    TIMER1_COMPARE_NO_OUTPUT,
    TIMER1_COMPARE_TOGGLE,
    TIMER1_COMPARE_CLEAR,
    TIMER1_COMPARE_SET
} timer1_compare_output_mode;

typedef enum timer1_compare_channel {
    TIMER1_COMPARE_A,
    TIMER1_COMPARE_B
} timer1_compare_channel;

void timer1_set_clock(timer1_clock_select clock_select);

void timer1_set_counter(uint16_t counter);

void timer1_set_compare_channel(timer1_compare_channel channel, timer1_compare_output_mode mode, uint16_t period, bool generate_interrupt);

#endif // TIMER_H
