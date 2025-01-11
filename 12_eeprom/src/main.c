// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bsp.h"

#define DS3231_ADDR         (0x68)
#define DS3231_SECONDS_REG  (0x00)
#define DS3231_MINUTES_REG  (0x01)
#define DS3231_HOURS_REG    (0x02)
#define TIME_STRING_FORMAT  ("XX:XX:XX\n\r")
#define TIME_STRING_SIZE    (sizeof(TIME_STRING_FORMAT))
#define BUFFER_SIZE         (100)
#define EEPROM_TIME_ADDR    (0)

#define PRINT_NEWLINE() usart_print("\n\r")

typedef struct __attribute__((packed)) rtc_time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
} rtc_time_t;

static void print_status(const char *header, twi_status status) {
    static char buffer[16];

    usart_print("[");
    usart_print(header);
    usart_print("]: 0x");
    itoa(status, buffer, 16);
    usart_print(buffer);
    PRINT_NEWLINE();
}

void rtc_set_time(const rtc_time_t *rtc_time) {
    twi_status status = TWI_STATUS_NO_ERROR;

    status = twi_write_multiple(DS3231_ADDR, DS3231_SECONDS_REG, (const uint8_t *) rtc_time, 3);
    if(status != TWI_STATUS_NO_ERROR) {
        print_status("DS3231 Time Write error", status);
        while(1);
    }
}

void rtc_read_time(rtc_time_t *rtc_time) {
    twi_status status = TWI_STATUS_NO_ERROR;

    status = twi_read_multiple(DS3231_ADDR, DS3231_SECONDS_REG, (uint8_t *) rtc_time, 3);
    if(status != TWI_STATUS_NO_ERROR) {
        print_status("DS3231 Time Read error", status);
        while(1);
    }
}

void display_rtc_time(const rtc_time_t *rtc_time) {
    static char time_string[TIME_STRING_SIZE];

    int seconds = bcd2dec(rtc_time->sec);
    int minutes = bcd2dec(rtc_time->min);
    int hours = bcd2dec(rtc_time->hour & 0xF);

    snprintf(time_string, TIME_STRING_SIZE, "%02hhd:%02hhd:%02hhd\n\r", hours, minutes, seconds);
    usart_print(time_string);
}

int main(void) {
    usart_init(BAUDRATE_115200, DISABLE_INTERRUPT, DISABLE_INTERRUPT);
    twi_init(TWI_CLOCK_SELECT_100KHZ, DISABLE_INTERRUPT);
    
    __DISABLE_INTERRUPTS();

    rtc_time_t rtc_time;
    char buffer[BUFFER_SIZE];
    while(1) {
        usart_print("> ");
        uint8_t size = usart_scan(buffer, BUFFER_SIZE);

        if(strcmp(buffer, "set") == 0) {
            usart_scan(buffer, BUFFER_SIZE);
            PRINT_NEWLINE();

            sscanf(buffer, "%02hhd:%02hhd:%02hhd", &rtc_time.hour, &rtc_time.min, &rtc_time.sec);
            rtc_set_time(&rtc_time);
        }
        else if(strcmp(buffer, "get") == 0) {
            PRINT_NEWLINE();
            rtc_read_time(&rtc_time);
            display_rtc_time(&rtc_time);
        }
        else if(strcmp(buffer, "store") == 0) {
            eeprom_write_block(EEPROM_TIME_ADDR, (const uint8_t *) &rtc_time, sizeof(rtc_time));
            usart_print("\n\rTime stored\n\r");
        }
        else if(strcmp(buffer, "load") == 0) {
            eeprom_read_block(EEPROM_TIME_ADDR, (uint8_t *) &rtc_time, sizeof(rtc_time));
            usart_print("\n\rTime loaded\n\r");
        }
        else if(strcmp(buffer, "print") == 0) {
            PRINT_NEWLINE();
            display_rtc_time(&rtc_time); 
        }
        else if(size == 0) {
            PRINT_NEWLINE();
        }
        else {
            usart_print("\n\rInvalid command!\n\r");
        }
    }
}
