// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include <stdint.h>
#include <stdbool.h>

#define HWREG(addr) (*((volatile uint8_t *) (addr)))

#define IO_OFFSET   (0x20)

/* IO Register Addresses */

#define PCMSK2_ADDR (0x6D)
#define PCMSK1_ADDR (0x6C)
#define PCMSK0_ADDR (0x6B)

#define PORTD_ADDR  (0x0B + IO_OFFSET)
#define DDRD_ADDR   (0x0A + IO_OFFSET)
#define PIND_ADDR   (0x09 + IO_OFFSET)

#define PORTC_ADDR  (0x08 + IO_OFFSET)
#define DDRC_ADDR   (0x07 + IO_OFFSET)
#define PINC_ADDR   (0x06 + IO_OFFSET)

#define PORTB_ADDR  (0x05 + IO_OFFSET)
#define DDRB_ADDR   (0x04 + IO_OFFSET)
#define PINB_ADDR   (0x03 + IO_OFFSET)

/* IO Register Access */

#define PCMSK2      (HWREG(PCMSK2_ADDR))
#define PCMSK1      (HWREG(PCMSK1_ADDR))
#define PCMSK0      (HWREG(PCMSK0_ADDR))
#define EICRA       (HWREG(0x69))
#define PCICR       (HWREG(0x68))

#define EIMSK       (HWREG(0x1D + IO_OFFSET))
#define EIFR        (HWREG(0x1C + IO_OFFSET))
#define PCIFR       (HWREG(0x1B + IO_OFFSET))

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
