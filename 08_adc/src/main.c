// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include <stdint.h>

#include "bsp.h"

#define NUMBER_SAMPLES  (32)

#define POT_CHANNEL     (0)

const servo_t SERVO = {
    .channel = TIMER_COMPARE_A,
    .gpio = {.port = IO_PORT_B, .pin = 1}
};
const gpio_t builtin_led = {
    .port = IO_PORT_B, .pin = BUILTIN_LED
};

bool adc_ready = false;

int main(void) {
    gpio_init(&builtin_led, IO_DIRECTION_OUT);
    
    servo_init(&SERVO, SERVO_MIN_VALUE);

    adc_init(ADC_REFERENCE_AVCC, ADC_AUTO_TRIGGER_FREE_RUNNING, ADC_PRESCALER_64, ENABLE_INTERRUPT);
    adc_disable_digital_input(POT_CHANNEL);
    adc_start_conversion_on_channel(POT_CHANNEL);

    ENABLE_INTERRUPTS();
    
    while(1) {
        if(adc_ready) {
            uint16_t adc_value = 4*adc_read_value() + SERVO_MIN_VALUE;

            if(adc_value <= SERVO_MAX_VALUE) {
                servo_write(&SERVO, adc_value);
            }

            adc_ready = false;
        }
        SLEEP();
    }
}

void __vector_ADC_ISR(void) {
    static int adc_count = 0;
    
    adc_count++;

    if(adc_count == NUMBER_SAMPLES) {
        adc_count = 0;
        adc_ready = true;
    }
}
