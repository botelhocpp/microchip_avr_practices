// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

typedef enum adc_voltage_reference {
    ADC_REFERENCE_AREF,
    ADC_REFERENCE_AVCC,
    ADC_REFERENCE_INTERNAL_VREF = 3
} adc_voltage_reference;

typedef enum adc_auto_trigger {
    ADC_AUTO_TRIGGER_FREE_RUNNING,
    ADC_AUTO_TRIGGER_ANALOG_COMPARATOR,
    ADC_AUTO_TRIGGER_EXT_INT0,
    ADC_AUTO_TRIGGER_TIMER0_COMPARE_A,
    ADC_AUTO_TRIGGER_TIMER0_OVERFLOW,
    ADC_AUTO_TRIGGER_TIMER1_COMPARE_B,
    ADC_AUTO_TRIGGER_TIMER1_OVERFLOW,
    ADC_AUTO_TRIGGER_TIMER1_CAPTURE,
    ADC_AUTO_TRIGGER_NONE,
} adc_auto_trigger;

typedef enum adc_prescaler {
    ADC_PRESCALER_2 = 1,
    ADC_PRESCALER_4,
    ADC_PRESCALER_8,
    ADC_PRESCALER_16,
    ADC_PRESCALER_32,
    ADC_PRESCALER_64,
    ADC_PRESCALER_128
} adc_prescaler;

void adc_init(adc_voltage_reference reference, adc_auto_trigger trigger, adc_prescaler prescaler, bool enable_interrupt);

void adc_enable(void);

void adc_disable(void);

void adc_enable_interrupt(void);

void adc_disable_interrupt(void);

void adc_select_channel(uint8_t channel);

void adc_select_reference(adc_voltage_reference reference);

void adc_select_prescaler(adc_prescaler prescaler);

void adc_select_auto_trigger(adc_auto_trigger trigger);

void adc_start_conversion(void);

void adc_start_conversion_on_channel(uint8_t channel);

bool adc_check_completion(void);

uint16_t adc_read_value(void);

void adc_enable_digital_input(uint8_t channel);

void adc_disable_digital_input(uint8_t channel);

/**
 * Start a conversion, wait until it's done and
 * return the result.
 */
uint16_t adc_read(uint8_t channel);

#endif // ADC_H
