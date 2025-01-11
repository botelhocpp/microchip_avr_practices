// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

typedef enum eeprom_mode {
    EEPROM_MODE_ERASE_WRITE,
    EEPROM_MODE_ERASE_ONLY,
    EEPROM_MODE_WRITE_ONLY
} eeprom_mode;

void eeprom_init(eeprom_mode mode, bool enable_interrupt);

void eeprom_enable_interrupt(void);

void eeprom_disable_interrupt(void);

void eeprom_set_mode(eeprom_mode mode);

void eeprom_write_data(uint16_t address, uint8_t data);

uint8_t eeprom_read_data(uint16_t address);

void eeprom_write(uint16_t address, uint8_t data);

uint8_t eeprom_read(uint16_t address);

void eeprom_write_block(uint16_t initial_address, const uint8_t *data, uint8_t size);

void eeprom_read_block(uint16_t initial_address, uint8_t *data, uint8_t size);

bool eeprom_check_write_busy(void);

#endif // EEPROM_H
