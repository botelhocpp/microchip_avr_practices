#include "servo.h"

#define PWM_PERIOD_VALUE    (TMR_CALC_HZ(SERVO_FREQ, SERVO_TIMER_PS))

void servo_init(const servo_t* servo, uint16_t default_value) {
    gpio_init(&servo->gpio, IO_DIRECTION_OUT);
    timer_init(SERVO_TIMER_ID, TIMER_MODE_FAST_PWM, TIMER_CLOCK_PS_8);
    timer_config_output_compare_channel(SERVO_TIMER_ID, servo->channel, TIMER_COMPARE_CLEAR, default_value, false);
    timer_set_pwm_period(SERVO_TIMER_ID, PWM_PERIOD_VALUE);
}

void servo_write(const servo_t* servo, uint16_t value) {
    timer_set_pwm_duty_cycle(SERVO_TIMER_ID, servo->channel, value);
}
