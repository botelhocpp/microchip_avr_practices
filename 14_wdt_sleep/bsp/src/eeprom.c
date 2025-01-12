#include "eeprom.h"

#include "atmega328p.h"

#define EEPM        (0b11U << 4)
#define EEPM1       (1U << 5)
#define EEPM0       (1U << 4)
#define EERIE       (1U << 3)
#define EEMPE       (1U << 2)
#define EEPE_BIT    (1)
#define EEPE        (1U << EEPE_BIT)
#define EERE        (1U << 0)

void eeprom_init(eeprom_mode mode, bool enable_interrupt) {
    eeprom_set_mode(mode);

    if(enable_interrupt) {
        eeprom_enable_interrupt();
    }
    else {
        eeprom_disable_interrupt();
    }
}

void eeprom_enable_interrupt(void) {
    EECR |= EERIE;
}

void eeprom_disable_interrupt(void) {
    EECR &= ~EERIE;
}

void eeprom_set_mode(eeprom_mode mode) {
    EECR &= ~EEPM;
    EECR |= (mode << 4);
}

void eeprom_write_data(uint16_t address, uint8_t data) {
    EEARH = (address >> 8);
    EEARL = address;

    EEDR = data;

    EECR |= EEMPE;
    EECR |= EEPE;
}

uint8_t eeprom_read_data(uint16_t address) {
    EEARH = (address >> 8);
    EEARL = address;

    EECR |= EERE;

    return EEDR;
}

void eeprom_write(uint16_t address, uint8_t data) {
    while(eeprom_check_write_busy());

    eeprom_write_data(address, data);
}

uint8_t eeprom_read(uint16_t address) {
    while(eeprom_check_write_busy());

    return eeprom_read_data(address);
}

void eeprom_write_block(uint16_t initial_address, const uint8_t *data, uint8_t size) {
    for(uint8_t i = 0; i < size; i++) {
        eeprom_write(initial_address++, data[i]);
    }
}

void eeprom_read_block(uint16_t initial_address, uint8_t *data, uint8_t size) {
    for(uint8_t i = 0; i < size; i++) {
        data[i] = eeprom_read(initial_address++);
    }
}

bool eeprom_check_write_busy(void) {
    return ((EECR >> EEPE_BIT) & 1U);
}
