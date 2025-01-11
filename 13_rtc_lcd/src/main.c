// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bsp.h"

#define BUFFER_SIZE         (100)
#define EEPROM_TIME_ADDR    (0) 

#define WELCOME_STR         ("\n\r"\
                            "AVR BSP Terminal\n\r"\
                            "\n\r"\
                            "CPU: ATmega328P\n\r"\
                            "Clock: 16MHz\n\r"\
                            "Flash: 32KB\n\r"\
                            "SRAM: 2KB\n\r"\
                            "EEPROM: 1KB\n\r"\
                            "\n\r> ")
#define WELCOME_STR_SIZE    (sizeof(WELCOME_STR) - 1)

#define TIME_STRING_FORMAT  ("XX:XX:XX")
#define TIME_STRING_SIZE    (sizeof(TIME_STRING_FORMAT) - 1)

#define NEWLINE_STR         ("\n\r")
#define NEWLINE_STR_SIZE    (sizeof(NEWLINE_STR) - 1)

#define TERMINAL_STR        ("\n\r> ")
#define TERMINAL_STR_SIZE   (sizeof(TERMINAL_STR) - 1)

#define STORE_STR           ("\n\rTime stored.")
#define STORE_STR_SIZE      (sizeof(STORE_STR) - 1)

#define LOAD_STR            ("\n\rTime loaded.")
#define LOAD_STR_SIZE       (sizeof(LOAD_STR) - 1)

#define INVALID_STR         ("\n\rInvalid command.")
#define INVALID_STR_SIZE    (sizeof(INVALID_STR) - 1)

#define EMPTY_STR           ("")
#define EMPTY_STR_SIZE      (sizeof(EMPTY_STR) - 1)
 
typedef struct message_t {
  char data[BUFFER_SIZE];
  int8_t size;
  int8_t index;
  bool done;
} message_t;

message_t rx_message = {
  .size = 0,
  .done = false
};
message_t tx_message = {
  .data = WELCOME_STR,
  .size = WELCOME_STR_SIZE,
  .index = 0,
  .done = false
};

char time_string[TIME_STRING_SIZE + 1];

const lcd_t lcd = {
    .address = LCD_NXP_EXPANSOR_ADDRESS,
    .backlight = true
};

bool flag_update_time = true;

static void get_rtc_time_string(const rtc_time_t *rtc_time) {
    snprintf(time_string, TIME_STRING_SIZE + 1, "%02hhd:%02hhd:%02hhd", rtc_time->hour, rtc_time->min, rtc_time->sec);
}

int main(void) {
    usart_init(BAUDRATE_115200, ENABLE_INTERRUPT, ENABLE_INTERRUPT);

    twi_init(TWI_CLOCK_SELECT_100KHZ, DISABLE_INTERRUPT);

    ext_interrupt_enable(EXT_INTERRUPT_NUM_0, EXT_INTERRUPT_TRIGGER_RISING);

    rtc_init();

    lcd_init(&lcd);

    lcd_create_big_numbers(&lcd);

    __ENABLE_INTERRUPTS();

    rtc_time_t rtc_time;

    while(1) {
        if(rx_message.done && tx_message.done) {
            if(strncmp(rx_message.data, "set", 3) == 0) {
                sscanf(rx_message.data + 4, "%02hhd:%02hhd:%02hhd", &rtc_time.hour, &rtc_time.min, &rtc_time.sec);

                while(!rtc_set_time(&rtc_time));

                strcpy(tx_message.data, EMPTY_STR);
                tx_message.size = EMPTY_STR_SIZE;
            }
            else if(strcmp(rx_message.data, "get") == 0) {
                get_rtc_time_string(&rtc_time);

                strcpy(tx_message.data, NEWLINE_STR);
                strcat(tx_message.data, time_string);
                tx_message.size = NEWLINE_STR_SIZE + TIME_STRING_SIZE;
            }
            else if(strcmp(rx_message.data, "store") == 0) {
                eeprom_write_block(EEPROM_TIME_ADDR, (const uint8_t *) &rtc_time, sizeof(rtc_time));

                strcpy(tx_message.data, STORE_STR);
                tx_message.size = STORE_STR_SIZE;
            }
            else if(strcmp(rx_message.data, "load") == 0) {
                eeprom_read_block(EEPROM_TIME_ADDR, (uint8_t *) &rtc_time, sizeof(rtc_time));

                while(!rtc_set_time(&rtc_time));
                
                strcpy(tx_message.data, LOAD_STR);
                tx_message.size = LOAD_STR_SIZE;
            }
            else if(rx_message.size != 0) {    
                strcpy(tx_message.data, INVALID_STR);
                tx_message.size = INVALID_STR_SIZE;
            }
            
            strcat(tx_message.data, TERMINAL_STR);
            tx_message.size += TERMINAL_STR_SIZE;
            tx_message.done = false;
            usart_enable_interrupt(USART_TX_INTERRUPT);

            rx_message.done = false;
            rx_message.size = 0;
        }

        if(flag_update_time) {
            rtc_read_time(&rtc_time);

            lcd_home(&lcd);
            
            lcd_write_big_number(&lcd, 0, rtc_time.hour / 10);
            lcd_write_big_number(&lcd, 2, rtc_time.hour % 10);
            lcd_write(&lcd, ':');

            lcd_write_big_number(&lcd, 5, rtc_time.min / 10);
            lcd_write_big_number(&lcd, 7, rtc_time.min % 10);
            lcd_write(&lcd, ':');

            lcd_write_big_number(&lcd, 10, rtc_time.sec / 10);
            lcd_write_big_number(&lcd, 12, rtc_time.sec % 10);

            flag_update_time = false;
        }
    } 

    __SLEEP();
}

void __vector_USART_UDRE_ISR(void) {
    if(tx_message.index < tx_message.size) {
        usart_write(tx_message.data[tx_message.index++]);
    }
    else {
        tx_message.index = 0;
        tx_message.done = true;
        usart_disable_interrupt(USART_TX_INTERRUPT);
    }
}

void __vector_USART_RX_ISR(void) {
    if(rx_message.done) {
        return;
    }

    char data = usart_read();
    
    if(data == '\0' || data == '\n' || data == '\r') {
        usart_write_blocking(data); 
        rx_message.data[rx_message.size] = '\0';
        rx_message.done = true;
    }
    else if(rx_message.size > 0 && data == '\b') {
        rx_message.size--;
        usart_write_blocking('\b'); 
        usart_write_blocking(' ');
        usart_write_blocking('\b');
    }
    else {
        rx_message.data[rx_message.size++] = data;
        usart_write_blocking(data);
    }
}

void __vector_INT0_ISR(void) {
    flag_update_time = true;
}
