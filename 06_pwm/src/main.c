// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

#define WELCOME_MESSAGE         ("PWM Control\n\r")

#define TMR_CALC_MS(period, ps) ((uint16_t) ((CPU_FREQ/ps)*(period/1000.0) - 1))
#define TMR_CALC_HZ(freq, ps)   ((uint16_t) ((CPU_FREQ/ps)/(freq/1.0) - 1))
#define TIMER0_PS               (1024)
#define TIMER1_PS               (8)
#define TIMER2_PS               (1024)

#define TIMER0_PERIOD           ((uint8_t) TMR_CALC_MS(1, TIMER0_PS))
#define SERVO_MIN_ANGLE_MS      (0.5) 
#define SERVO_MAX_ANGLE_MS      (2.5) 
#define PWM_PERIOD              (20)
#define PWM_PERIOD_VALUE        (TMR_CALC_MS(PWM_PERIOD, TIMER1_PS))
#define PWM_MIN_DUTY            (TMR_CALC_MS(SERVO_MIN_ANGLE_MS, TIMER1_PS))
#define PWM_MAX_DUTY            (TMR_CALC_MS(SERVO_MAX_ANGLE_MS, TIMER1_PS))

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

const gpio_t buzzer = {
    .port = IO_PORT_D, .pin = 3
};
const gpio_t servo = {
    .port = IO_PORT_B, .pin = 1
};
const gpio_t led = {
    .port = IO_PORT_B, .pin = 5
};

bool tick_timer_1ms = false;

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

uint16_t NOTES[] = {
    TMR_CALC_HZ(NOTE_A4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_B4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_C4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_D4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_E4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_F4, TIMER2_PS),
    TMR_CALC_HZ(NOTE_G4, TIMER2_PS)
};

int main(void) {
    usart_init(BAUDRATE_115200, true, true);
    
    gpio_init(&servo, IO_DIRECTION_OUT);
    gpio_init(&buzzer, IO_DIRECTION_OUT);
    gpio_init(&led, IO_DIRECTION_OUT);

    timer_init(TIMER_0, TIMER_MODE_CTC, TIMER_CLOCK_PS_1024);
    timer_config_output_compare_channel(TIMER_0, TIMER_COMPARE_A, TIMER_COMPARE_NO_OUTPUT, TIMER0_PERIOD, true);

    timer_init(TIMER_1, TIMER_MODE_FAST_PWM, TIMER_CLOCK_PS_8);
    timer_config_output_compare_channel(TIMER_1, TIMER_COMPARE_A, TIMER_COMPARE_CLEAR, PWM_MIN_DUTY, false);
    timer_set_pwm_period(TIMER_1, PWM_PERIOD_VALUE);

    timer_init(TIMER_2, TIMER_MODE_CTC, TIMER_CLOCK_NONE);
    timer_config_output_compare_channel(TIMER_2, TIMER_COMPARE_B, TIMER_COMPARE_TOGGLE, 0, false);

    ENABLE_INTERRUPTS();

    bool play_buzzer = false;
    bool increment_duty = false;
    uint16_t duty_cycle = PWM_MIN_DUTY;
    uint8_t tick_count = 0;
    
    while(1) {
        if(rx_done) {
            rx_done = false;
            
            if(rx_buffer >= 'a' && rx_buffer <= 'g') {
                int note = rx_buffer - 'a';
                timer_set_clock(TIMER_2, TIMER_CLOCK_PS_1024);
                timer_set_output_compare_value(TIMER_2, TIMER_COMPARE_B, NOTES[note]);
                tick_count = 0;
                play_buzzer = true;
            }
        }

        if(tick_timer_1ms) {
            if(play_buzzer) {
                gpio_init(&buzzer, IO_DIRECTION_OUT);
                gpio_write(&led, 1);

                tick_count++;

                if(tick_count == 150) {
                    timer_set_clock(TIMER_2, TIMER_CLOCK_NONE);
                    play_buzzer = false;
                }
            }
            else {
                gpio_write(&led, 0);
                gpio_init(&buzzer, IO_DIRECTION_IN);
            }
            
            if(increment_duty) {
                duty_cycle += 1;
                if(duty_cycle >= PWM_MAX_DUTY) {
                    duty_cycle = PWM_MAX_DUTY;
                    increment_duty = false;
                }
            }
            else {
                duty_cycle -= 1;
                if(duty_cycle <= PWM_MIN_DUTY) {
                    duty_cycle = PWM_MIN_DUTY;
                    increment_duty = true;
                }
            }
            timer_set_pwm_duty_cycle(TIMER_1, TIMER_COMPARE_A, duty_cycle);
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
