// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#include "atmega328p.h"

#define POWER_REDUCTION_TWI     (1U << PRTWI)
#define POWER_REDUCTION_TIMER2  (1U << PRTIM2)
#define POWER_REDUCTION_TIMER0  (1U << PRTIM0)
#define POWER_REDUCTION_TIMER1  (1U << PRTIM1)
#define POWER_REDUCTION_SPI     (1U << PRSPI)
#define POWER_REDUCTION_USART0  (1U << PRUSART0)
#define POWER_REDUCTION_ADC     (1U << PRADC)
#define POWER_REDUCTION_ALL     (POWER_REDUCTION_TWI    |   \
                                POWER_REDUCTION_TIMER2  |   \
                                POWER_REDUCTION_TIMER0  |   \
                                POWER_REDUCTION_TIMER1  |   \
                                POWER_REDUCTION_SPI     |   \
                                POWER_REDUCTION_USART0  |   \
                                POWER_REDUCTION_ADC)

typedef enum sleep_mode {
    SLEEP_MODE_IDLE,
    SLEEP_MODE_ADC_NOISE_REDUCTION,
    SLEEP_MODE_POWER_DOWN,
    SLEEP_MODE_POWER_SAVE,
    SLEEP_MODE_STANDBY = 6,
    SLEEP_MODE_EXTERNAL_STANDBY
} sleep_mode;

void cpu_setup(void);

void cpu_config_sleep(sleep_mode mode, bool enable_sleep);

void cpu_enable_sleep(void);

void cpu_disable_sleep(void);

void cpu_sleep(void);

void cpu_select_sleep_mode(sleep_mode mode);

void cpu_disable_bod_during_sleep(void);

void cpu_set_power_reduction_settings(uint8_t mask);

uint8_t cpu_get_power_reduction_settings(uint8_t mask);

#endif // CPU_H
