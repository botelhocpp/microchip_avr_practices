// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdio.h>
#include <stdint.h>

#include "bsp.h"

#define BUFFER_SIZE (100)

char buffer[BUFFER_SIZE];

const gpio_t SPI_SS = {
    .port = IO_PORT_B, .pin = 2
};

static uint8_t sd_card_command(uint8_t cmd, uint32_t arg, uint8_t crc) {
    spi_transfer(0x40 | cmd);          // Command
    spi_transfer((arg >> 24) & 0xFF);  // Argument byte 3
    spi_transfer((arg >> 16) & 0xFF);  // Argument byte 2
    spi_transfer((arg >> 8) & 0xFF);   // Argument byte 1
    spi_transfer(arg & 0xFF);          // Argument byte 0
    spi_transfer(crc);                 // CRC (for CMD0 and CMD8)

    // Await response (8 bytes maximum)
    uint8_t response;
    for (int i = 0; i < 8; i++) {
        response = spi_transfer(0xFF);
        if (response != 0xFF) {
            break;
        }
    }
    return response;
}

static uint8_t sd_card_init(void) {
    spi_disable_chip_select(&SPI_SS);

    // Generate initial clocks with high CS (80 cycles minimum)
    for (int i = 0; i < 10; i++) {
        spi_transfer(0xFF);
    }

    spi_enable_chip_select(&SPI_SS);

    // Command CMD0 (RESET)
    uint8_t response = sd_card_command(0, 0, 0x95);
    if (response != 0x01) {
        return response;
    }

    // Command CMD8 (SEND_IF_COND) to check support to SD 2.0
    response = sd_card_command(8, 0x000001AA, 0x87);
    if (response & 0x04) {
        usart_print("SD Card does not support CMD8 (assuming SD 1.0).\n\r");

        // Try to initialize with CMD1
        uint16_t attempts = 1000;
        do {
            response = sd_card_command(1, 0, 0xF9);
        } while (response != 0x00 && --attempts);

        if (attempts == 0) {
            return 0xFF;
        }
    } else if (response == 0x01) {
        // CMD8 ssupported: read 4 byte response
        uint8_t cmd8_response[4];
        for (int i = 0; i < 4; i++) {
            cmd8_response[i] = spi_transfer(0xFF);
        }

        // Is return valid (0x000001AA)? 
        if (cmd8_response[2] != 0x01 || cmd8_response[3] != 0xAA) {
            return 0xFF;
        }

        // Send ACMD41 for SDHC/SDXC
        uint16_t attempts = 1000;
        do {
            // CMD55 (prefix to ACMD)
            response = sd_card_command(55, 0, 0x65);
            if (response > 1) {
                return 0xFF;
            }

            // Command ACMD41
            response = sd_card_command(41, 0x40000000, 0x77); // HCS = 1
        } while (response != 0x00 && --attempts);

        if (attempts == 0) {
            return 0xFF;
        }
    }

    spi_disable_chip_select(&SPI_SS);

    return 0x01;
}

int main(void) {
    usart_init(BAUDRATE_115200, DISABLE_INTERRUPT, DISABLE_INTERRUPT);
    spi_init(SPI_MODE_MASTER, SPI_CLOCK_MODE_0, SPI_PRESCALER_64, SPI_DATA_ORDER_MSB_FIRST, DISABLE_INTERRUPT);
    spi_config_chip_select(&SPI_SS);

    uint8_t response = sd_card_init();
    if (response != 0x01) {
        sprintf(buffer, "SD Card initialization error: 0x%02X\n\r", response);
        usart_print(buffer);
    } else {
        usart_print("SD Card initialized successfully!\n\r");
    }

    while (1);
}
