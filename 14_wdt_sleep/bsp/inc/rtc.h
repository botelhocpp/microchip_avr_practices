// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) rtc_time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
} rtc_time_t;

bool rtc_init(void);

bool rtc_set_time(const rtc_time_t *rtc_time);

bool rtc_read_time(rtc_time_t *rtc_time);

#endif // RTC_H
