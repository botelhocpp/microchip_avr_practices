#include "utils.h"

#include "atmega328p.h"

uint16_t map(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
	return MAP_VALUE(value, in_min, in_max, out_min, out_max);
}

uint8_t bcd2dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t dec2bcd(uint8_t dec) {
	return (dec / 10 << 4) + (dec%10);
}

void delay_ms(uint16_t ms) {
	while (ms--) {		
		for (uint16_t i = 0; i < (CPU_FREQ / 4000); i++) {
			__NOP();
		}
    }
}

void delay_us(uint16_t us) {
	while (us--) {		
		for (uint16_t i = 0; i < (CPU_FREQ / 4000000); i++) {
			__NOP();
		}
    }
}
