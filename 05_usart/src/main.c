// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

#define WELCOME_MESSAGE   ("Welcome!\n\r")

typedef struct message_t {
  const uint8_t *data;
  int8_t size;
  int8_t index;
  bool done;
} message_t;

message_t welcome_message = {
  .data = (const uint8_t *) WELCOME_MESSAGE,
  .size = sizeof(WELCOME_MESSAGE) - 1,
  .index = 0,
  .done = false
};

uint8_t led_select = 0;
bool rx_done = false;

const gpio_t btns[] = {
    {.port = IO_PORT_D, .pin = 2},
    {.port = IO_PORT_D, .pin = 3},
};
const gpio_t leds[] = {
    {.port = IO_PORT_D, .pin = 4},
    {.port = IO_PORT_D, .pin = 5},
};
const gpio_t builtin_led = {
    .port = IO_PORT_B, .pin = BUILTIN_LED
};

int main(void) {
    usart_init(BAUDRATE_115200, true, true);
    
    timer1_set_clock(TIMER1_CLOCK_PS_1024);

    timer1_set_compare_channel(TIMER1_COMPARE_A, TIMER1_COMPARE_NO_OUTPUT, CPU_FREQ/1024, true);
    
    gpio_init(&builtin_led, IO_DIRECTION_OUT);

    for(int i = 0; i < 2; i++) {
        gpio_init(&btns[i], IO_DIRECTION_IN_PULLUP);
        gpio_init(&leds[i], IO_DIRECTION_OUT);
    }

    pin_change_interrupt_enable(18);
    pin_change_interrupt_enable(19);

    ENABLE_INTERRUPTS();

    while(1) {
        SLEEP();

        if(rx_done && led_select < 2) {
            gpio_toggle(&leds[led_select]);
            rx_done = false;
        }
    }
}

void __vector_PCINT2_ISR(void) {
    for(int i = 0; i < 2; i++) {
        if(!gpio_read(&btns[i])) {
            gpio_toggle(&leds[i]);
        }
    }
}

void __vector_TIMER1_COMPA_ISR(void) {
    gpio_toggle(&builtin_led);
}

void __vector_USART_RX_ISR(void) {
    rx_done = true;
    led_select = usart_read() - '0';
}

void __vector_USART_UDRE_ISR(void) {
    if(welcome_message.index < welcome_message.size) {
      usart_write(welcome_message.data[welcome_message.index++]);
    }
    else {
      welcome_message.done = true;
      usart_disable_interrupt(USART_TX_INTERRUPT);
    }
}
