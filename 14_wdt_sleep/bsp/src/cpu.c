#include "cpu.h"

#include "atmega328p.h"

#define SM_MASK     (0b111)
#define SM2         (3)
#define SM1         (2)
#define SM0         (1)
#define SE          (0)

#define BODS        (6)
#define BODSE       (5)

void cpu_setup(void) {
    cpu_set_power_reduction_settings(POWER_REDUCTION_ALL);
    
    cpu_disable_bod_during_sleep();

    cpu_config_sleep(SLEEP_MODE_IDLE, true);
}

void cpu_config_sleep(sleep_mode mode, bool enable_sleep) {
    cpu_select_sleep_mode(mode);

    if(enable_sleep) {
        cpu_enable_sleep();
    }
    else {
        cpu_disable_sleep();
    }
}

void cpu_enable_sleep(void) {
    SMCR |= (1U << SE);
}

void cpu_disable_sleep(void) {
    SMCR &= ~(1U << SE);
}

void cpu_sleep(void) {
    SMCR |= (1U << SE);

    __SLEEP();

    SMCR &= ~(1U << SE);
}

void cpu_select_sleep_mode(sleep_mode mode) {
    SMCR &= ~(SM_MASK << SM0);

    SMCR |= (mode << SM0);
}

void cpu_disable_bod_during_sleep(void) {
    MCUCR |= (1U << BODSE) | (1U << BODS);

    MCUCR &= ~(1U << BODSE);
}

void cpu_set_power_reduction_settings(uint8_t mask) {
    PRR = mask;
}

uint8_t cpu_get_power_reduction_settings(uint8_t mask) {
    return PRR;
}
