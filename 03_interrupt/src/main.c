// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

void delay_ms(int ms) {
    for(int i = 0; i < ms; i++) {
        for(int j = 0; j < 3000; j++) {
            asm("NOP");
        }
    }
}

const gpio_t btns[] = {
    {.port = IO_PORT_D, .pin = 2},
    {.port = IO_PORT_D, .pin = 3},
};
const gpio_t leds[] = {
    {.port = IO_PORT_D, .pin = 4},
    {.port = IO_PORT_D, .pin = 5},
};

int main() {
    for(int i = 0; i < 2; i++) {
        gpio_init(&btns[i], IO_DIRECTION_IN_PULLUP);
        gpio_init(&leds[i], IO_DIRECTION_OUT);
    }

    ext_interrupt_enable(EXT_INTERRUPT_NUM_0, EXT_INTERRUPT_TRIGGER_FALLING);
    
    pin_change_interrupt_group_enable(19);
    pin_change_interrupt_enable(19);

    ENABLE_INTERRUPTS();

    while(1) {
        SLEEP();
    }
}

void __vector_INT0_ISR(void) {
    gpio_toggle(&leds[0]);
}

void __vector_PCINT2_ISR(void) {
    gpio_toggle(&leds[1]);
}
