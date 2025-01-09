#include "buzzer.h"

#define GPIO_SET_HIGH_IMPEDANCE(gpio_ptr)   gpio_init(gpio_ptr, IO_DIRECTION_IN)

static void prv_buzzer_set_configuration(const buzzer_t* buzzer, timer_clock_select clock_select, uint16_t tone);

void buzzer_init(const buzzer_t* buzzer) {
    timer_init(buzzer->id, TIMER_MODE_CTC, TIMER_CLOCK_NONE);
    timer_config_output_compare_channel(buzzer->id, buzzer->channel, TIMER_COMPARE_TOGGLE, NO_NOTE, false);
}

void buzzer_tone(const buzzer_t* buzzer, uint16_t tone) {
    gpio_init(&buzzer->gpio, IO_DIRECTION_OUT);
    prv_buzzer_set_configuration(buzzer, TIMER_CLOCK_PS_1024, tone);
}

void buzzer_no_tone(const buzzer_t* buzzer) {
    GPIO_SET_HIGH_IMPEDANCE(&buzzer->gpio);
    prv_buzzer_set_configuration(buzzer, TIMER_CLOCK_NONE, NO_NOTE);
}

void prv_buzzer_set_configuration(const buzzer_t* buzzer, timer_clock_select clock_select, uint16_t tone) {
    timer_set_clock(buzzer->id, clock_select);
    timer_set_output_compare_value(buzzer->id, buzzer->channel, tone);
}
