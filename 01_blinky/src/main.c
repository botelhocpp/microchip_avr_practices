#include <stdint.h>

#define PORTD       (0x0B)
#define DDRD        (0x0A)
#define PIND        (0x09)

#define PORTC       (0x08)
#define DDRC        (0x07)
#define PINC        (0x06)

#define PORTB       (0x05)
#define DDRB        (0x04)
#define PINB        (0x03)

#define HWREG(addr) (*((volatile uint8_t *) (addr)))

#define IO_OFFSET   (0x20)

#define LED_PIN     (5) // PB5

void delay_ms(int ms) {
    for(int i = 0; i < ms; i++) {
        for(int j = 0; j < 3000; j++) {
            asm("NOP");
        }
    }
}

int main() {
    HWREG(DDRB + IO_OFFSET) |= (1U << LED_PIN); 

    while(1) {
        HWREG(PORTB + IO_OFFSET) |= (1U << LED_PIN); 
        delay_ms(1000);
        HWREG(PORTB + IO_OFFSET) &= ~(1U << LED_PIN); 
        delay_ms(1000);
    }
}
