#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAP_VALUE(value, in_min, in_max, out_min, out_max)\
    ((((out_max) - (out_min)) / ((in_max) - (in_min))) * ((value) - (in_min)) + (out_min))

uint16_t map(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif // UTILS_H
