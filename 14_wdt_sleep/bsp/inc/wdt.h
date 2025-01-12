// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef WDT_H
#define WDT_H

#include <stdint.h>
#include <stdbool.h>

typedef enum wdt_timeout {
    WDT_TIMEOUT_16MS,
    WDT_TIMEOUT_32MS,
    WDT_TIMEOUT_64MS,
    WDT_TIMEOUT_125MS,
    WDT_TIMEOUT_250MS,
    WDT_TIMEOUT_500MS,
    WDT_TIMEOUT_1S,
    WDT_TIMEOUT_2S,
    WDT_TIMEOUT_4S,
    WDT_TIMEOUT_8S
} wdt_timeout;

void wdt_init(wdt_timeout timeout, bool enable_interrupt);

void wdt_reset(void);

void wdt_enable_reset(void);

void wdt_disable_reset(void);

void wdt_enable_interrupt(void);

void wdt_disable_interrupt(void);

void wdt_set_timeout(wdt_timeout timeout);

bool wdt_check_occurence(void);

#endif // WDT_H
