#include "wdt.h"

#include "atmega328p.h"

#define WDRF_BIT    (3)
#define WDRF        (1U << WDRF_BIT)
#define BORF        (1U << 2)
#define EXTRF       (1U << 1)
#define PORF        (1U << 0)

#define WDIF        (1U << 7)
#define WDIE        (1U << 6)
#define WDP3        (1U << 5)
#define WDCE        (1U << 4)
#define WDE         (1U << 3)
#define WDP2        (1U << 2)
#define WDP1        (1U << 1)
#define WDP0        (1U << 0)

void wdt_init(wdt_timeout timeout, bool enable_interrupt) {
    __reset_watchdog();
    
    if(wdt_check_occurence()) {
        MCUSR &= ~WDRF;
    }
    
    wdt_set_timeout(timeout);

    if(enable_interrupt) {
        wdt_enable_interrupt();
    }
    else {
        wdt_disable_interrupt();
    }

    wdt_enable_reset();
}

void wdt_reset(void) {
    __reset_watchdog();
}

void wdt_enable_reset(void) {
    WDTCSR |= WDE;
}

void wdt_disable_reset(void) {
    __reset_watchdog();
    
    MCUSR &= ~WDRF;

    WDTCSR |= (WDCE | WDE);
    
    WDTCSR &= ~WDE;
}

void wdt_enable_interrupt(void) {
    WDTCSR |= WDIE;
}

void wdt_disable_interrupt(void) {
    WDTCSR &= ~WDIE;
}

void wdt_set_timeout(wdt_timeout timeout) {
    __reset_watchdog();

    uint8_t wdtcsr_value = WDTCSR;

    wdtcsr_value &= ~(WDP3 | WDP2 | WDP1 | WDP0);
    
    wdtcsr_value |= ((timeout & 0b1000) << 2) | (timeout & 0b111);

    WDTCSR |= (WDCE | WDE);

    WDTCSR = wdtcsr_value;
}

bool wdt_check_occurence(void) {
    return (MCUSR >> WDRF_BIT) & 1U;
}
