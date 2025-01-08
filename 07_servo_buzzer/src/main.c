// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

#define WELCOME_MESSAGE         ("PWM Control\n\r")

#define TIMER0_PS               (1024)
#define TIMER2_PS               (1024)

#define TIMER0_PERIOD           ((uint8_t) TMR_CALC_MS(1, TIMER0_PS))

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

uint8_t rx_buffer = 0;
bool rx_done = false;

const buzzer_t buzzer = {
    .id = TIMER_2,
    .channel = TIMER_COMPARE_B,
    .gpio = {.port = IO_PORT_D, .pin = 3}
};
const servo_t servo = {
    .channel = TIMER_COMPARE_A,
    .gpio = {.port = IO_PORT_B, .pin = 1}
};
const gpio_t led = {
    .port = IO_PORT_B, .pin = 5
};

bool tick_timer_1ms = false;

const uint16_t NOTES[] = {
    NOTE_A4,
    NOTE_B4,
    NOTE_C4,
    NOTE_D4,
    NOTE_E4,
    NOTE_F4,
    NOTE_G4
};

static void system_timer_init(void) {
    timer_init(TIMER_0, TIMER_MODE_CTC, TIMER_CLOCK_PS_1024);
    timer_config_output_compare_channel(TIMER_0, TIMER_COMPARE_A, TIMER_COMPARE_NO_OUTPUT, TIMER0_PERIOD, true);
}

int main(void) {
    usart_init(BAUDRATE_115200, true, true);
    
    gpio_init(&led, IO_DIRECTION_OUT);

    system_timer_init();
    servo_init(&servo, SERVO_MIN_VALUE);
    buzzer_init(&buzzer);

    ENABLE_INTERRUPTS();
    
    bool play_buzzer = false;
    bool increment_duty = false;
    uint16_t duty_cycle = SERVO_MIN_VALUE;
    uint8_t tick_count = 0;
    
    while(1) {
        if(rx_done) {
            rx_done = false;
            
            if(rx_buffer >= 'a' && rx_buffer <= 'g') {
                int note = rx_buffer - 'a';
                buzzer_tone(&buzzer, NOTES[note]);
                tick_count = 0;
                play_buzzer = true;
            }
        }

        if(tick_timer_1ms) {
            if(play_buzzer) {
                gpio_write(&led, 1);
                tick_count++;

                if(tick_count == 150) {
                    buzzer_no_tone(&buzzer);
                    play_buzzer = false;
                    gpio_write(&led, 0);
                }
            }

            if(increment_duty) {
                duty_cycle += 1;
                if(duty_cycle >= SERVO_MAX_VALUE) {
                    duty_cycle = SERVO_MAX_VALUE;
                    increment_duty = false;
                }
            }
            else {
                duty_cycle -= 1;
                if(duty_cycle <= SERVO_MIN_VALUE) {
                    duty_cycle = SERVO_MIN_VALUE;
                    increment_duty = true;
                }
            }
            servo_write(&servo, duty_cycle);
            tick_timer_1ms = false;
        }
        SLEEP();
    }
}

void __vector_TIMER0_COMPA_ISR(void) {
    tick_timer_1ms = true;
}

void __vector_USART_RX_ISR(void) {
    rx_done = true;
    rx_buffer = usart_read();
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
