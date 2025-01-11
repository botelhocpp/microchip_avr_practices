#include "timer.h"

#include "atmega328p.h"

const uint16_t TIMER_TCCRA_REGS[] = {TCCR0A_ADDR, TCCR1A_ADDR, TCCR2A_ADDR};
const uint16_t TIMER_TCCRB_REGS[] = {TCCR0B_ADDR, TCCR1B_ADDR, TCCR2B_ADDR};
const uint16_t TIMER_TCNT_REGS[] = {TCNT0_ADDR, TCNT1L_ADDR, TCNT2_ADDR};
const uint16_t TIMER_OCRA_REGS[] = {OCR0A_ADDR, OCR1AL_ADDR, OCR2A_ADDR};
const uint16_t TIMER_OCRB_REGS[] = {OCR0B_ADDR, OCR1BL_ADDR, OCR2B_ADDR};
const uint16_t TIMER_TIMSK_REGS[] = {TIMSK0_ADDR, TIMSK1_ADDR, TIMSK2_ADDR};
const uint16_t TIMER_TIFR_REGS[] = {TIFR0_ADDR, TIFR1_ADDR, TIFR2_ADDR};

const uint8_t TIMER_WAVEFORM_MODE[] = {
    [TIMER_MODE_CTC] = 0b0100,
    [TIMER_MODE_FAST_PWM] = 0b1110
};

void timer_init(timer_id id, timer_mode mode, timer_clock_select clock_select) {
    // Mask WGMn3:WGMn0
    uint8_t mode_mask = TIMER_WAVEFORM_MODE[mode];

    if(id != TIMER_1) {
        mode_mask >>= 1;
    }

    // Clear WGM
    HWREG(TIMER_TCCRA_REGS[id]) &= ~(0b11); // WGMn1:WGMn0
    HWREG(TIMER_TCCRB_REGS[id]) &= ~(0b11U << 3); // WGMn3:WGMn2

    HWREG(TIMER_TCCRA_REGS[id]) |= ((mode_mask & 0b11));   // WGMn1:WGMn0
    HWREG(TIMER_TCCRB_REGS[id]) |= ((mode_mask >> 2) << 3);     // WGMn3:WGMn2

    timer_set_clock(id, clock_select);
    timer_set_counter(id, 0);
}

void timer_set_clock(timer_id id, timer_clock_select clock_select) {
    uint8_t clock_select_value = clock_select;

    if(id != TIMER_2) {
        if(clock_select > TIMER_CLOCK_PS_64) {
            clock_select_value -= 2;
        }
        else if(clock_select > TIMER_CLOCK_PS_8) {
            clock_select_value -= 1;
        }
    }

    HWREG(TIMER_TCCRB_REGS[id]) &= ~(0b111);
    HWREG(TIMER_TCCRB_REGS[id]) |= (clock_select_value);
}

void timer_set_counter(timer_id id, uint16_t counter) {
    if(id == TIMER_1) {
        TCNT1H = HIGH(counter);
    }
    HWREG(TIMER_TCNT_REGS[id]) = LOW(counter);
}

void timer_set_input_compare_value(timer_id id, uint16_t value) {
    if(id == TIMER_1) {
        ICR1H = HIGH(value);
        ICR1L = LOW(value);
    }
}

void timer_set_output_compare_value(timer_id id, timer_compare_channel channel, uint16_t value) {
    const static uint16_t TIMER_OCR1H_REGS[] = {OCR1AH_ADDR, OCR1BH_ADDR};
    const static uint16_t *TIMER_OCR_REGS[] = {TIMER_OCRA_REGS, TIMER_OCRB_REGS};

    if(id == TIMER_1) {
        HWREG(TIMER_OCR1H_REGS[channel]) = HIGH(value);
    }
    HWREG(TIMER_OCR_REGS[channel][id]) = LOW(value);
}

void timer_config_output_compare_channel(timer_id id, timer_compare_channel channel, timer_compare_output_mode mode, uint16_t period, bool generate_interrupt) {
    uint8_t mode_mask = 0;
    timer_interrupt interrupt = TIMER_OUTPUT_COMPARE_A_INTERRUPT;

    timer_set_output_compare_value(id, channel, period);

    if(channel == TIMER_COMPARE_A) {
        mode_mask = (mode << 6);
        interrupt = TIMER_OUTPUT_COMPARE_A_INTERRUPT;
    }
    else {
        mode_mask = (mode << 4);
        interrupt = TIMER_OUTPUT_COMPARE_B_INTERRUPT;
    }

    HWREG(TIMER_TCCRA_REGS[id]) |= mode_mask;

    if(generate_interrupt) {
        timer_enable_interrupt(id, interrupt);
    }
}

void timer_enable_interrupt(timer_id id, timer_interrupt interrupt) {
    HWREG(TIMER_TIMSK_REGS[id]) |= (1U << interrupt);
}

void timer_disable_interrupt(timer_id id, timer_interrupt interrupt) {
    HWREG(TIMER_TIMSK_REGS[id]) &= ~(1U << interrupt);
}
