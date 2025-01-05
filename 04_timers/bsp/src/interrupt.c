// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include "interrupt.h"

const uint16_t pin_change_mask_registers[] = {PCMSK0_ADDR, PCMSK1_ADDR, PCMSK2_ADDR};

static uint8_t get_pcint_index(uint8_t pcint);

void ext_interrupt_enable(ext_interrupt_num num, ext_interrupt_trigger trigger) { 
    EICRA |= (trigger << 2*num);
    EIMSK |= (1 << num);
}

void ext_interrupt_disable(ext_interrupt_num num) {
    EIMSK &= ~(1 << num);
}

void pin_change_interrupt_group_enable(uint8_t pcint) {
    uint8_t pcint_index = get_pcint_index(pcint);

    PCICR |= (1 << pcint_index);
}

void pin_change_interrupt_group_disable(uint8_t pcint) {
    uint8_t pcint_index = get_pcint_index(pcint);

    PCICR &= ~(1 << pcint_index);
}

void pin_change_interrupt_enable(uint8_t pcint) {
    uint8_t pcint_index = get_pcint_index(pcint);

    HWREG(pin_change_mask_registers[pcint_index]) |= (1 << (pcint - 8*pcint_index));

    pin_change_interrupt_group_enable(pcint);
}

void pin_change_interrupt_disable(uint8_t pcint) {
    uint8_t pcint_index = get_pcint_index(pcint);

    HWREG(pin_change_mask_registers[pcint_index]) &= ~(1 << (pcint - 8*pcint_index));
}

uint8_t get_pcint_index(uint8_t pcint) {
    uint8_t index = 2;

    if(pcint >= 0 && pcint <= 7) {
        index = 0;
    }
    else if(pcint >= 8 && pcint <= 14) {
        index = 1;
    }

    return index;
}
