// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

const gpio_t btns[] = {
    {.port = IO_PORT_D, .pin = 2},
    {.port = IO_PORT_D, .pin = 3},
};
const gpio_t leds[] = {
    {.port = IO_PORT_D, .pin = 4},
    {.port = IO_PORT_D, .pin = 5},
};
const gpio_t builtin_led = {
    .port = IO_PORT_B, .pin = BUILTIN_LED
};

int main(void) {
    timer1_set_clock(TIMER1_CLOCK_PS_1024);

    timer1_set_compare_channel(TIMER1_COMPARE_A, TIMER1_COMPARE_NO_OUTPUT, CPU_FREQ/1024, true);
    
    gpio_init(&builtin_led, IO_DIRECTION_OUT);

    for(int i = 0; i < 2; i++) {
        gpio_init(&btns[i], IO_DIRECTION_IN_PULLUP);
        gpio_init(&leds[i], IO_DIRECTION_OUT);
    }

    pin_change_interrupt_enable(18);
    pin_change_interrupt_enable(19);

    ENABLE_INTERRUPTS();

    while(1) {
        SLEEP();
    }
}

void __vector_PCINT2_ISR(void) {
    for(int i = 0; i < 2; i++) {
        if(!gpio_read(&btns[i])) {
            gpio_toggle(&leds[i]);
        }
    }
}

void __vector_TIMER1_COMPA_ISR(void) {
    gpio_toggle(&builtin_led);
}
