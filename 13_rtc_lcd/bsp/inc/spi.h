// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h" 

typedef enum spi_clock_mode {
    SPI_CLOCK_MODE_0,
    SPI_CLOCK_MODE_1,
    SPI_CLOCK_MODE_2,
    SPI_CLOCK_MODE_3,
} spi_clock_mode;

typedef enum spi_mode {
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE
} spi_mode;

typedef enum spi_data_order {
    SPI_DATA_ORDER_LSB_FIRST,
    SPI_DATA_ORDER_MSB_FIRST
} spi_data_order;

typedef enum spi_prescaler {
    SPI_PRESCALER_4,
    SPI_PRESCALER_16,
    SPI_PRESCALER_64,
    SPI_PRESCALER_128,
    SPI_PRESCALER_2,
    SPI_PRESCALER_8,
    SPI_PRESCALER_32
} spi_prescaler;

void spi_init(spi_mode mode, spi_clock_mode clock_mode, spi_prescaler prescaler, spi_data_order data_order, bool enable_interrupt);

void spi_enable(void);

void spi_disable(void);

void spi_set_mode(spi_mode mode);

void spi_set_clock_mode(spi_clock_mode clock_mode);

void spi_set_prescaler(spi_prescaler prescaler);

void spi_set_data_order(spi_data_order data_order);

void spi_enable_interrupt(void);

void spi_disable_interrupt(void);

void spi_write(uint8_t data);

uint8_t spi_read(void);

uint8_t spi_transfer(uint8_t data);

void spi_config_chip_select(const gpio_t *cs_gpio);

void spi_enable_chip_select(const gpio_t *cs_gpio);

void spi_disable_chip_select(const gpio_t *cs_gpio);

bool spi_check_completion(void);

bool spi_check_collision(void);

#endif // SPI_H
