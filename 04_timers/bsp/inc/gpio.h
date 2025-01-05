// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef GPIO_H
#define GPIO_H

#include "bsp.h"

typedef enum io_port {
    IO_PORT_B,
    IO_PORT_C,
    IO_PORT_D
} io_port;

typedef enum io_direction {
    IO_DIRECTION_IN,
    IO_DIRECTION_IN_PULLUP,
    IO_DIRECTION_OUT,
} io_direction;

typedef struct gpio_t {
    io_port port;
    uint8_t pin;
} gpio_t;

void gpio_init(const gpio_t *gpio_pin, io_direction direction);

void gpio_write(const gpio_t *gpio_pin, uint8_t value);

void gpio_toggle(const gpio_t *gpio_pin);

uint8_t gpio_read(const gpio_t *gpio_pin);

#endif // GPIO_H
