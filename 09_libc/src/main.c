// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdlib.h>
#include <stdint.h>

#include "bsp.h"

#define SERVO_ADC_MAP(value)   ((4 * value) + SERVO_MIN_VALUE)

#define NUMBER_SAMPLES  (32)

#define POT_CHANNEL     (0)

const servo_t SERVO = {
    .channel = TIMER_COMPARE_A,
    .gpio = {.port = IO_PORT_B, .pin = 1}
};
const gpio_t builtin_led = {
    .port = IO_PORT_B, .pin = BUILTIN_LED
};

uint16_t adc_accumulator = 0;

bool adc_ready = false;

int main(void) {
    usart_init(BAUDRATE_115200, false, false);

    gpio_init(&builtin_led, IO_DIRECTION_OUT);
    
    servo_init(&SERVO, SERVO_MIN_VALUE);

    adc_init(ADC_REFERENCE_AVCC, ADC_AUTO_TRIGGER_FREE_RUNNING, ADC_PRESCALER_64, ENABLE_INTERRUPT);
    adc_disable_digital_input(POT_CHANNEL);
    adc_start_conversion_on_channel(POT_CHANNEL);

    __ENABLE_INTERRUPTS();
    
    int adc_count = 0;
    char buffer[16];
    while(1) {
        if(adc_ready) {
            adc_count = (adc_count + 1) % NUMBER_SAMPLES;

            if(adc_count == 0) {
                __DISABLE_INTERRUPTS();

                uint16_t adc_avg_value = adc_accumulator / NUMBER_SAMPLES;

                uint16_t servo_value = SERVO_ADC_MAP(adc_avg_value);

                usart_print("Average ADC Value: ");
                itoa(servo_value, buffer, 10);
                usart_print(buffer);
                usart_print("\r\n");

                servo_write(&SERVO, servo_value);

                adc_accumulator = 0;

                __ENABLE_INTERRUPTS();
            }
            adc_ready = false;
        }
        __SLEEP();
    }
}

void __vector_ADC_ISR(void) {
    adc_accumulator += adc_read_value();
    adc_ready = true;
}
