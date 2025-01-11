#include "usart.h"

#include "atmega328p.h"

#define UBRR_FOR_2400   (416)
#define UBRR_FOR_4800   (207) 
#define UBRR_FOR_9600   (103)  
#define UBRR_FOR_19200  (51)  
#define UBRR_FOR_38400  (25) 
#define UBRR_FOR_115200 (8)

const uint16_t baud_rates[] = {
    UBRR_FOR_2400,
    UBRR_FOR_4800,
    UBRR_FOR_9600,
    UBRR_FOR_19200,
    UBRR_FOR_38400,
    UBRR_FOR_115200
};

void usart_init(usart_baud_rate baud_rate, bool tx_interrupt, bool rx_interrupt) {
    // Enable TX and RX
    UCSR0B  |= (1U << 4) | (1U << 3);

    uint16_t ubrr_value = baud_rates[baud_rate];

    UBRR0H = HIGH(ubrr_value);
    UBRR0L = LOW(ubrr_value);

    if(tx_interrupt) {
        usart_enable_interrupt(USART_TX_INTERRUPT);
    }
    if(rx_interrupt) {
        usart_enable_interrupt(USART_RX_INTERRUPT);
    }
}

void usart_enable_interrupt(usart_interrupt interrupt) {
    uint8_t mask = 0;

    switch(interrupt) {
        case USART_TX_INTERRUPT:
            mask = (1U << 5);
            break;
        case USART_RX_INTERRUPT:
            mask = (1U << 7);
            break;
    }

    UCSR0B |= mask;
}

void usart_disable_interrupt(usart_interrupt interrupt) {
    uint8_t mask = 0;

    switch(interrupt) {
        case USART_TX_INTERRUPT:
            mask = (1U << 5);
            break;
        case USART_RX_INTERRUPT:
            mask = (1U << 7);
            break;
    }

    UCSR0B &= ~mask;

}

void usart_write(uint8_t value) {
    UDR0 = value;
}

uint8_t usart_read(void) {
    return UDR0;
}

bool usart_has_data(void) {
    return UCSR0A & (1U << 7);
}

bool usart_is_empty(void) {
    return UCSR0A & (1U << 5);
}

void usart_write_blocking(uint8_t value) {
    while(!usart_is_empty());

    usart_write(value);
}

uint8_t usart_read_blocking(void) {
    while(!usart_has_data());

    return usart_read();
}

void usart_write_string_blocking(const char *str) {
    for(uint8_t i = 0; str[i] != '\0'; i++) {
        usart_write_blocking(str[i]);
    }
}

uint8_t usart_read_string_blocking(char *str, uint8_t max_size) {
    uint8_t size = 0;

    while(size < max_size) {
        str[size] = usart_read_blocking();

        if(str[size] == '\0') {
            str[size] = '\0';
            break;
        }

        size++;
    }

    return size - 1;
}

uint8_t usart_scan(char *str, uint8_t max_size) {
    uint8_t size = 0;

    while(size < max_size) {
        str[size] = usart_read_blocking();

        if(str[size] == '\0' || str[size] == '\n' || str[size] == '\r' || str[size] == ' ') {
            usart_write_blocking(str[size]); 
            break;
        }
        else if(size >0 && str[size] == '\b') {
            size--;
            usart_write_blocking('\b'); 
            usart_write_blocking(' ');
            usart_write_blocking('\b');
        }
        else {
            usart_write_blocking(str[size]);
            size++;
        }
    }

    str[size] = '\0';
    return (size == 0)? 0 : size - 1;
}
