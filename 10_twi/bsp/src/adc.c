// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#include "adc.h"

#include "atmega328p.h"

void adc_init(adc_voltage_reference reference, adc_auto_trigger trigger, adc_prescaler prescaler, bool enable_interrupt) {
    adc_select_reference(reference);

    if(enable_interrupt) {
        adc_enable_interrupt();
    }
    else {
        adc_disable_interrupt();
    }

    adc_select_auto_trigger(trigger);
    adc_select_prescaler(prescaler);
    adc_enable();
}

void adc_enable(void) {
    ADCSRA |= (1U << 7);
}

void adc_disable(void) {
    ADCSRA &= ~(1U << 7);
}

void adc_enable_interrupt(void) {
    ADCSRA |= (1U << 3);
}

void adc_disable_interrupt(void) {
    ADCSRA &= ~(1U << 3);
}

void adc_select_channel(uint8_t channel) {
    ADMUX &= ~(0b1111);
    ADMUX |= (channel);
}

void adc_select_reference(adc_voltage_reference reference) {
    ADMUX &= ~(0b11U << 6);
    ADMUX |= (reference << 6);
}

void adc_select_prescaler(adc_prescaler prescaler) {
    ADCSRA &= ~(0b111);
    ADCSRA |= (prescaler);
}

void adc_select_auto_trigger(adc_auto_trigger trigger) {
    if(trigger == ADC_AUTO_TRIGGER_NONE) {
        ADCSRA &= ~(1U << 5);
    }
    else {
        ADCSRA |= (1U << 5);

        ADCSRB &= ~(0b111);
        ADCSRB |= (trigger);
    }
}

void adc_start_conversion() {
    ADCSRA |= (1U << 6);
}

void adc_start_conversion_on_channel(uint8_t channel) {
    adc_select_channel(channel);
    adc_start_conversion();
}

bool adc_check_completion(void) {
    bool is_complete = (ADCSRA >> 4) & 1;

    ADCSRA |= (1U << 4);

    return is_complete;
}

uint16_t adc_read_value(void) {
    uint8_t adcl_value = ADCL;
    uint8_t adch_value = ADCH;

    return (adch_value << 8) | adcl_value;
}

void adc_enable_digital_input(uint8_t channel) {
    DIDR0 &= ~(1U << channel);
}

void adc_disable_digital_input(uint8_t channel) {
    DIDR0 |= (1U << channel);
}

uint16_t adc_read(uint8_t channel) {
    adc_start_conversion_on_channel(channel);
    while(!adc_check_completion());
    return adc_read_value();
}
