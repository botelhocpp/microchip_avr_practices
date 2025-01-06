#include "timer.h"

void timer1_set_clock(timer1_clock_select clock_select) {
    // Set CTC mode
    TCCR1B = (0b01 << 3) | (clock_select << 0);

    timer1_set_counter(0);
}

void timer1_set_counter(uint16_t counter) {
    TCNT1H = HIGH(counter);
    TCNT1L = LOW(counter);
}

void timer1_set_compare_channel(timer1_compare_channel channel, timer1_compare_output_mode mode, uint16_t period, bool generate_interrupt) {
    uint8_t period_low = LOW(period); 
    uint8_t period_high = HIGH(period); 
    uint8_t interrupt_mask = 0;

    if(channel == TIMER1_COMPARE_A) {
        OCR1AH = period_high; 
        OCR1AL = period_low; 
        interrupt_mask = (1 << 1);
        TCCR1A |= (mode << 6);
    }
    else {
        OCR1BH = period_high; 
        OCR1BL = period_low; 
        interrupt_mask = (1 << 2);
        TCCR1A |= (mode << 4);
    }

    if(generate_interrupt) {
        TIMSK1 |= interrupt_mask;
    }
}
