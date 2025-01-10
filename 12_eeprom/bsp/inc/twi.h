// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef TWI_H
#define TWI_H

#include <stdint.h>
#include <stdbool.h>

#define TWI_SEND_ACK    (true)
#define TWI_SEND_NACK   (false)

#define TWI_WRITE_BIT   (0)
#define TWI_READ_BIT    (1)

#define TWI_STATUS_BUS_ERROR                                (0x00)
#define TWI_STATUS_START_TRANSMITTED                        (0x08)
#define TWI_STATUS_RE_START_TRANSMITTED                     (0x10)
#define TWI_STATUS_SLAVE_ADDR_WRITE_TRANSMITTED_ACK_RECV    (0x18)
#define TWI_STATUS_SLAVE_ADDR_WRITE_TRANSMITTED_NACK_RECV   (0x20)
#define TWI_STATUS_DATA_TRANSMITTED_ACK_RECV                (0x28)
#define TWI_STATUS_DATA_TRANSMITTED_NACK_RECV               (0x30)
#define TWI_STATUS_ARBITRATION_LOST                         (0x38)
#define TWI_STATUS_SLAVE_ADDR_READ_TRANSMITTED_ACK_RECV     (0x40)
#define TWI_STATUS_SLAVE_ADDR_READ_TRANSMITTED_NACK_RECV    (0x48)
#define TWI_STATUS_DATA_RECEIVED_WITH_ACK                   (0x50)
#define TWI_STATUS_DATA_RECEIVED_WITH_NACK                  (0x58)
#define TWI_STATUS_IDLE                                     (0xF8)
#define TWI_STATUS_NO_ERROR                                 (0xFF)

typedef enum twi_clock_select {
    TWI_CLOCK_SELECT_100KHZ, 
    TWI_CLOCK_SELECT_400KHZ
} twi_clock_select;

typedef uint8_t twi_status;

void twi_init(twi_clock_select clock, bool enable_interrupt);

void twi_enable(void);

void twi_disable(void);

void twi_send_ack(void);

bool twi_check_completion(void);

void twi_clear_flag(void);

void twi_enable_interrupt(void);

void twi_disable_interrupt(void);

twi_status twi_get_status(void);

/* TWI Non-Blocking Directives */

#define twi_send_repeated_start twi_send_start

void twi_send_start(void);

void twi_send_stop(void);

void twi_write_data(uint8_t data);

uint8_t twi_read_data(void);

/* TWI Blocking Directives */

#define twi_repeated_start twi_start

twi_status twi_start(void);

twi_status twi_stop(void);

twi_status twi_write(uint8_t data);

twi_status twi_read(uint8_t *data, bool send_ack);

twi_status twi_write_byte(uint8_t slave_address, uint8_t data);

twi_status twi_read_byte(uint8_t slave_address, uint8_t *data);

twi_status twi_write_multiple(uint8_t slave_address, uint8_t first_register_address, const uint8_t *data, uint8_t size);

twi_status twi_read_multiple(uint8_t slave_address, uint8_t first_register_address, uint8_t *data, uint8_t size);

#endif // TWI_H
