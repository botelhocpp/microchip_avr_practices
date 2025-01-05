// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "bsp.h"

#define ENABLE_INTERRUPTS()     asm("SEI")
#define DISABLE_INTERRUPTS()    asm("CLI")
#define SLEEP()                 asm("SLEEP")

typedef enum ext_interrupt_num {
    EXT_INTERRUPT_NUM_0,
    EXT_INTERRUPT_NUM_1
} ext_interrupt_num;

typedef enum ext_interrupt_trigger {
    EXT_INTERRUPT_TRIGGER_LOW_LEVEL,
    EXT_INTERRUPT_TRIGGER_CHANGE,
    EXT_INTERRUPT_TRIGGER_FALLING,
    EXT_INTERRUPT_TRIGGER_RISING,
} ext_interrupt_trigger;

void ext_interrupt_enable(ext_interrupt_num num, ext_interrupt_trigger trigger);

void ext_interrupt_disable(ext_interrupt_num num);

void pin_change_interrupt_group_enable(uint8_t pcint);

void pin_change_interrupt_group_disable(uint8_t pcint);

void pin_change_interrupt_enable(uint8_t pcint);

void pin_change_interrupt_disable(uint8_t pcint);

#endif // INTERRUPTS_H
