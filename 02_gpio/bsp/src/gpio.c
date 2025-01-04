// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include "gpio.h"

uint16_t gpio_port_registers[] = {PORTB_ADDR, PORTC_ADDR, PORTD_ADDR};
uint16_t gpio_ddr_registers[] = {DDRB_ADDR, DDRC_ADDR, DDRD_ADDR};
uint16_t gpio_pin_registers[] = {PINB_ADDR, PINC_ADDR, PIND_ADDR};

void gpio_init(const gpio_t *gpio_pin, io_direction direction) {
    if(direction == IO_DIRECTION_OUT) {
        HWREG(gpio_ddr_registers[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(gpio_ddr_registers[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }

    if(direction == IO_DIRECTION_IN_PULLUP) {
        HWREG(gpio_port_registers[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(gpio_port_registers[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }
}

void gpio_write(const gpio_t *gpio_pin, uint8_t value) {
    if(value) {
        HWREG(gpio_port_registers[gpio_pin->port]) |= (1 << gpio_pin->pin);
    }
    else {
        HWREG(gpio_port_registers[gpio_pin->port]) &= ~(1 << gpio_pin->pin);
    }
}

void gpio_toggle(const gpio_t *gpio_pin) {
    HWREG(gpio_port_registers[gpio_pin->port]) ^= (1 << gpio_pin->pin);
}

uint8_t gpio_read(const gpio_t *gpio_pin) {
    return (HWREG(gpio_pin_registers[gpio_pin->port]) >> gpio_pin->pin) & 1;
}