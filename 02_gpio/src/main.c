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

int main() {
    const gpio_t btns[] = {
        {
            .port = IO_PORT_D,
            .pin = 2
        },
        {
            .port = IO_PORT_D,
            .pin = 3
        },
    };
    const gpio_t leds[] = {
        {
            .port = IO_PORT_D,
            .pin = 4
        },
        {
            .port = IO_PORT_D,
            .pin = 5
        },
    };

    for(int i = 0; i < 2; i++) {
        gpio_init(&btns[i], IO_DIRECTION_IN_PULLUP);
        gpio_init(&leds[i], IO_DIRECTION_OUT);
    }

    while(1) {
        for(int i = 0; i < 2; i++) {
            if(!gpio_read(&btns[i])) {
                gpio_toggle(&leds[i]);
                delay_ms(200);
            }
        }
    }
}
