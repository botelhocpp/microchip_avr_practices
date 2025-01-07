// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include "gpio.h"

const uint16_t GPIO_PORT_REGS[] = {PORTB_ADDR, PORTC_ADDR, PORTD_ADDR};
const uint16_t GPIO_DDR_REGS[] = {DDRB_ADDR, DDRC_ADDR, DDRD_ADDR};
const uint16_t GPIO_PIN_REGS[] = {PINB_ADDR, PINC_ADDR, PIND_ADDR};

void gpio_init(const gpio_t *gpio_pin, io_direction direction) {
    if(direction == IO_DIRECTION_OUT) {
        HWREG(GPIO_DDR_REGS[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(GPIO_DDR_REGS[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }

    if(direction == IO_DIRECTION_IN_PULLUP) {
        HWREG(GPIO_PORT_REGS[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(GPIO_PORT_REGS[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }
}

void gpio_write(const gpio_t *gpio_pin, uint8_t value) {
    if(value) {
        HWREG(GPIO_PORT_REGS[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(GPIO_PORT_REGS[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }
}

void gpio_toggle(const gpio_t *gpio_pin) {
    HWREG(GPIO_PORT_REGS[gpio_pin->port]) ^= (1 << gpio_pin->pin);
}

uint8_t gpio_read(const gpio_t *gpio_pin) {
    return (HWREG(GPIO_PIN_REGS[gpio_pin->port]) >> gpio_pin->pin) & 1;
}