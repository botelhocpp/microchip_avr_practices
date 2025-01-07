// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef TIMER_H
#define TIMER_H 

#include "bsp.h"

typedef enum timer_clock_select {
    TIMER_CLOCK_NONE,
    TIMER_CLOCK_NO_PS,
    TIMER_CLOCK_PS_8,
    TIMER_CLOCK_PS_32,
    TIMER_CLOCK_PS_64,
    TIMER_CLOCK_PS_128,
    TIMER_CLOCK_PS_256,
    TIMER_CLOCK_PS_1024,
    TIMER_CLOCK_EXT_FALLING,
    TIMER_CLOCK_EXT_RISING
} timer_clock_select;

typedef enum timer_compare_output_mode {
    TIMER_COMPARE_NO_OUTPUT,
    TIMER_COMPARE_TOGGLE,
    TIMER_COMPARE_CLEAR,
    TIMER_COMPARE_SET
} timer_compare_output_mode;

typedef enum timer_compare_channel {
    TIMER_COMPARE_A,
    TIMER_COMPARE_B
} timer_compare_channel;

typedef enum timer_mode {
    TIMER_MODE_CTC,
    TIMER_MODE_FAST_PWM
} timer_mode;

typedef enum timer_interrupt {
    TIMER_OVERFLOW_INTERRUPT = 0,
    TIMER_OUTPUT_COMPARE_A_INTERRUPT = 1,
    TIMER_OUTPUT_COMPARE_B_INTERRUPT = 2,
    TIMER_INPUT_CAPTURE_INTERRUPT = 5
} timer_interrupt;

typedef enum timer_id {
    TIMER_0,
    TIMER_1,
    TIMER_2
} timer_id;

#define timer_set_pwm_period timer_set_input_compare_value

#define timer_set_pwm_duty_cycle timer_set_output_compare_value 

void timer_init(timer_id id, timer_mode mode, timer_clock_select clock_select);

void timer_set_clock(timer_id id, timer_clock_select clock_select);

void timer_set_counter(timer_id id, uint16_t counter);

void timer_set_input_compare_value(timer_id id, uint16_t value);

void timer_set_output_compare_value(timer_id id, timer_compare_channel channel, uint16_t value);

void timer_config_output_compare_channel(timer_id id, timer_compare_channel channel, timer_compare_output_mode mode, uint16_t period, bool generate_interrupt);

void timer_enable_interrupt(timer_id id, timer_interrupt interrupt);

void timer_disable_interrupt(timer_id id, timer_interrupt interrupt);

#endif // TIMER_H
