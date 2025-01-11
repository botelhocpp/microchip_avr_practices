#include "rtc.h"

#include "twi.h"
#include "utils.h"

#define RTC_DS3231_ADDR             (0x68)
#define RTC_DS3231_SECONDS_REG      (0x00)
#define RTC_DS3231_MINUTES_REG      (0x01)
#define RTC_DS3231_HOURS_REG        (0x02)
#define RTC_DS3231_CONTROL_REG      (0x0E)

#define RTC_DS3231_12_24            (1U << 6)
#define RTC_DS3231_INTCN            (1U << 2)
#define RTC_DS3231_RS2              (1U << 4)
#define RTC_DS3231_RS1              (1U << 3)
#define RTC_DS3231_RS               (0b11U << 3)

bool rtc_init(void) {
    twi_status status = TWI_STATUS_NO_ERROR;

    uint8_t buffer = 0;

    status = twi_read_multiple(RTC_DS3231_ADDR, RTC_DS3231_HOURS_REG, (uint8_t *) &buffer, 1);
    if(status != TWI_STATUS_NO_ERROR) {
        return false;
    }

    buffer &= ~RTC_DS3231_12_24;

    status = twi_write_multiple(RTC_DS3231_ADDR, RTC_DS3231_HOURS_REG, (const uint8_t *) &buffer, 1);
    if(status != TWI_STATUS_NO_ERROR) {
        return false;
    }

    status = twi_read_multiple(RTC_DS3231_ADDR, RTC_DS3231_CONTROL_REG, (uint8_t *) &buffer, 1);
    if(status != TWI_STATUS_NO_ERROR) {
        return false;
    }

    buffer &= ~RTC_DS3231_INTCN & ~RTC_DS3231_RS;

    status = twi_write_multiple(RTC_DS3231_ADDR, RTC_DS3231_CONTROL_REG, (const uint8_t *) &buffer, 1);
    

    return status == TWI_STATUS_NO_ERROR;
}

bool rtc_set_time(const rtc_time_t *rtc_time) {
    twi_status status = TWI_STATUS_NO_ERROR;

    rtc_time_t rtc_time_bcd = {
        .sec = dec2bcd(rtc_time->sec),
        .min = dec2bcd(rtc_time->min),
        .hour = dec2bcd(rtc_time->hour)
    };

    status = twi_write_multiple(RTC_DS3231_ADDR, RTC_DS3231_SECONDS_REG, (const uint8_t *) &rtc_time_bcd, 3);

    return status == TWI_STATUS_NO_ERROR;
}

bool rtc_read_time(rtc_time_t *rtc_time) {
    twi_status status = TWI_STATUS_NO_ERROR;

    status = twi_read_multiple(RTC_DS3231_ADDR, RTC_DS3231_SECONDS_REG, (uint8_t *) rtc_time, 3);

    rtc_time->sec = bcd2dec(rtc_time->sec);
    rtc_time->min = bcd2dec(rtc_time->min);
    rtc_time->hour = bcd2dec(rtc_time->hour & 0x3F);
    
    return status == TWI_STATUS_NO_ERROR;
}
