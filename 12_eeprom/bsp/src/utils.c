#include "utils.h"

uint16_t map(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
	return MAP_VALUE(value, in_min, in_max, out_min, out_max);
}

uint8_t bcd2dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
