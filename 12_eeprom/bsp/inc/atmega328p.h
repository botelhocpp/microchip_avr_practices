// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include <stdint.h>
#include <stdbool.h>

#define CPU_FREQ    (16000000UL)

#define HWREG(addr) (*((volatile uint8_t *) (addr)))

#define IO_OFFSET   (0x20)

#define HIGH(x) ((x >> 8) & 0xFF)
#define LOW(x)  (x & 0xFF)

/* Instruction Macros */

#define __ENABLE_INTERRUPTS()       asm("SEI")
#define __DISABLE_INTERRUPTS()      asm("CLI")
#define __SLEEP()                   asm("SLEEP")
#define __NOP()                     asm("NOP")

/* I/O Registers */

/* TWI */

#define TWAMR       (HWREG(0xBD))
#define TWCR        (HWREG(0xBC))
#define TWDR        (HWREG(0xBB))
#define TWAR        (HWREG(0xBA))
#define TWBR        (HWREG(0xB8))
#define TWSR        (HWREG(0xB9))

/* ADC */

#define DIDR0       (HWREG(0x7E))
#define ADMUX       (HWREG(0x7C))
#define ADCSRB      (HWREG(0x7B))
#define ADCSRA      (HWREG(0x7A))
#define ADCH        (HWREG(0x79))
#define ADCL        (HWREG(0x78))

/* USART0 */

#define UDR0        (HWREG(0xC6))
#define UBRR0H      (HWREG(0xC5))
#define UBRR0L      (HWREG(0xC4))
#define UCSR0C      (HWREG(0xC2))
#define UCSR0B      (HWREG(0xC1))
#define UCSR0A      (HWREG(0xC0))

/* TIMER2 */

#define OCR2B_ADDR  (0xB3)
#define OCR2A_ADDR  (0xB4)
#define TCNT2_ADDR  (0xB2)
#define TCCR2B_ADDR (0xB1)
#define TCCR2A_ADDR (0xB0)
#define TIMSK2_ADDR (0x70)
#define TIFR2_ADDR  (0x17 + IO_OFFSET)

#define OCR2B       (HWREG(OCR2B_ADDR))
#define OCR2A       (HWREG(OCR2A_ADDR))
#define TCNT2       (HWREG(TCNT2_ADDR))
#define TCCR2B      (HWREG(TCCR2B_ADDR))
#define TCCR2A      (HWREG(TCCR2A_ADDR))
#define TIMSK2      (HWREG(TIMSK2_ADDR))
#define TIFR2       (HWREG(TIFR2_ADDR))

/* TIMER1 */

#define OCR1BH_ADDR (0x8B)
#define OCR1BL_ADDR (0x8A)
#define OCR1AH_ADDR (0x89)
#define OCR1AL_ADDR (0x88)
#define ICR1H_ADDR  (0x87)
#define ICR1L_ADDR  (0x86)
#define TCNT1H_ADDR (0x85)
#define TCNT1L_ADDR (0x84)
#define TCCR1C_ADDR (0x82)
#define TCCR1B_ADDR (0x81)
#define TCCR1A_ADDR (0x80)
#define TIMSK1_ADDR (0x6F)
#define TIFR1_ADDR  (0x16 + IO_OFFSET)

#define OCR1BH      (HWREG(OCR1BH_ADDR))
#define OCR1BL      (HWREG(OCR1BL_ADDR))
#define OCR1AH      (HWREG(OCR1AH_ADDR))
#define OCR1AL      (HWREG(OCR1AL_ADDR))
#define ICR1H       (HWREG(ICR1H_ADDR))
#define ICR1L       (HWREG(ICR1L_ADDR))
#define TCNT1H      (HWREG(TCNT1H_ADDR))
#define TCNT1L      (HWREG(TCNT1L_ADDR))
#define TCCR1C      (HWREG(TCCR1C_ADDR))
#define TCCR1B      (HWREG(TCCR1B_ADDR))
#define TCCR1A      (HWREG(TCCR1A_ADDR))
#define TIMSK1      (HWREG(TIMSK1_ADDR))
#define TIFR1       (HWREG(TIFR1_ADDR))

/* SPI */

#define SPDR        (HWREG(0x2E + IO_OFFSET))
#define SPSR        (HWREG(0x2D + IO_OFFSET))
#define SPCR        (HWREG(0x2C + IO_OFFSET))

/* TIMER0 */

#define OCR0B_ADDR  (0x28 + IO_OFFSET)
#define OCR0A_ADDR  (0x27 + IO_OFFSET)
#define TCNT0_ADDR  (0x26 + IO_OFFSET)
#define TCCR0B_ADDR (0x25 + IO_OFFSET)
#define TCCR0A_ADDR (0x24 + IO_OFFSET)
#define TIMSK0_ADDR (0x6E)
#define TIFR0_ADDR  (0x15 + IO_OFFSET)

#define OCR0B       (HWREG(OCR0B_ADDR))
#define OCR0A       (HWREG(OCR0A_ADDR))
#define TCNT0       (HWREG(TCNT0_ADDR))
#define TCCR0B      (HWREG(TCCR0B_ADDR))
#define TCCR0A      (HWREG(TCCR0A_ADDR))
#define TIMSK0      (HWREG(TIMSK0_ADDR))
#define TIFR0       (HWREG(TIFR0_ADDR))

/* EEPROM */

#define EEARH       (HWREG(0x22 + IO_OFFSET))
#define EEARL       (HWREG(0x21 + IO_OFFSET))
#define EEDR        (HWREG(0x20 + IO_OFFSET))
#define EECR        (HWREG(0x1F + IO_OFFSET))

/* INTERRUPTS */

#define PCMSK2_ADDR (0x6D)
#define PCMSK1_ADDR (0x6C)
#define PCMSK0_ADDR (0x6B)

#define PCMSK2      (HWREG(PCMSK2_ADDR))
#define PCMSK1      (HWREG(PCMSK1_ADDR))
#define PCMSK0      (HWREG(PCMSK0_ADDR))
#define EICRA       (HWREG(0x69))
#define PCICR       (HWREG(0x68))
#define EIMSK       (HWREG(0x1D + IO_OFFSET))
#define EIFR        (HWREG(0x1C + IO_OFFSET))
#define PCIFR       (HWREG(0x1B + IO_OFFSET))

/* GPIO */

#define PORTD_ADDR  (0x0B + IO_OFFSET)
#define DDRD_ADDR   (0x0A + IO_OFFSET)
#define PIND_ADDR   (0x09 + IO_OFFSET)
#define PORTC_ADDR  (0x08 + IO_OFFSET)
#define DDRC_ADDR   (0x07 + IO_OFFSET)
#define PINC_ADDR   (0x06 + IO_OFFSET)
#define PORTB_ADDR  (0x05 + IO_OFFSET)
#define DDRB_ADDR   (0x04 + IO_OFFSET)
#define PINB_ADDR   (0x03 + IO_OFFSET)

#define PORTD       (HWREG(PORTD_ADDR))
#define DDRD        (HWREG(DDRD_ADDR))
#define PIND        (HWREG(PIND_ADDR))
#define PORTC       (HWREG(PORTC_ADDR))
#define DDRC        (HWREG(DDRC_ADDR))
#define PINC        (HWREG(PINC_ADDR))
#define PORTB       (HWREG(PORTB_ADDR))
#define DDRB        (HWREG(DDRB_ADDR))
#define PINB        (HWREG(PINB_ADDR))

#endif // ATMEGA328P_H
