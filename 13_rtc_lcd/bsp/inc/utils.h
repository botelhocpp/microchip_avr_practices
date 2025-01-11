#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAP_VALUE(value, in_min, in_max, out_min, out_max)\
    ((((out_max) - (out_min)) / ((in_max) - (in_min))) * ((value) - (in_min)) + (out_min))

uint16_t map(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

uint8_t bcd2dec(uint8_t bcd);

uint8_t dec2bcd(uint8_t dec);

void delay_ms(uint16_t ms);

void delay_us(uint16_t us);

#endif // UTILS_H
