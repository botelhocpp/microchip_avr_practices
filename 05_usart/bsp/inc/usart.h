// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef USART_H
#define USART_H 

#include "bsp.h"

typedef enum usart_baud_rate {
    BAUDRATE_2400,
    BAUDRATE_4800,
    BAUDRATE_9600,
    BAUDRATE_19200,   
    BAUDRATE_38400,
    BAUDRATE_115200
} usart_baud_rate;

typedef enum usart_interrupt {
    USART_RX_INTERRUPT,
    USART_TX_INTERRUPT
} usart_interrupt;

void usart_init(usart_baud_rate baud_rate, bool tx_interrupt, bool rx_interrupt);

void usart_enable_interrupt(usart_interrupt interrupt);

void usart_disable_interrupt(usart_interrupt interrupt);

void usart_write(uint8_t value);

uint8_t usart_read(void);

bool usart_has_data(void);

bool usart_is_empty(void);

void usart_write_blocking(uint8_t value);

uint8_t usart_read_blocking(void);

void usart_write_string_blocking(const uint8_t *str);

uint8_t usart_read_string_blocking(uint8_t *str, uint8_t max_size);

#endif // USART_H
