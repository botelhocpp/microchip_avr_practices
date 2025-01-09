// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef SERVO_H
#define SERVO_H

#include "timer.h"
#include "gpio.h"
#include "utils.h"

#define SERVO_TIMER_ID              (TIMER_1)
#define SERVO_TIMER_PS              (8)
#define SERVO_FREQ                  (50)
#define SERVO_MIN_ANGLE             (0) 
#define SERVO_MAX_ANGLE             (180) 
#define SERVO_MIN_ANGLE_MS          (0.5) 
#define SERVO_MAX_ANGLE_MS          (2.5) 
#define SERVO_MIN_VALUE             (TMR_CALC_MS(SERVO_MIN_ANGLE_MS, SERVO_TIMER_PS))
#define SERVO_MAX_VALUE             (TMR_CALC_MS(SERVO_MAX_ANGLE_MS, SERVO_TIMER_PS))
#define SERVO_ANGLE_VALUE(angle)    (map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_VALUE, SERVO_MAX_VALUE))    

typedef struct servo_t {
    timer_compare_channel channel;
    gpio_t gpio;
} servo_t;

void servo_init(const servo_t* servo, uint16_t default_value);

void servo_write(const servo_t* servo, uint16_t value);

#endif // SERVO_H
