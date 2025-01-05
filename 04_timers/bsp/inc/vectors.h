// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef VECTORS_H
#define VECTORS_H

void __vector_INT0_ISR(void) __attribute__((signal));

void __vector_INT1_ISR(void) __attribute__((signal));

void __vector_PCINT0_ISR(void) __attribute__((signal));

void __vector_PCINT1_ISR(void) __attribute__((signal));

void __vector_PCINT2_ISR(void) __attribute__((signal));

void __vector_WDT_ISR(void) __attribute__((signal));

void __vector_TIMER2_COMPA_ISR(void) __attribute__((signal));

void __vector_TIMER2_COMPB_ISR(void) __attribute__((signal));

void __vector_TIMER2_OVF_ISR(void) __attribute__((signal));

void __vector_TIMER1_CAPT_ISR(void) __attribute__((signal));

void __vector_TIMER1_COMPA_ISR(void) __attribute__((signal));

void __vector_TIMER1_COMPB_ISR(void) __attribute__((signal));

void __vector_TIMER1_OVF_ISR(void) __attribute__((signal));

void __vector_TIMER0_COMPA_ISR(void) __attribute__((signal));

void __vector_TIMER0_COMPB_ISR(void) __attribute__((signal));

void __vector_TIMER0_OVF_ISR(void) __attribute__((signal));

void __vector_SPI_STC_ISR(void) __attribute__((signal));

void __vector_USART_RX_ISR(void) __attribute__((signal));

void __vector_USART_UDRE_ISR(void) __attribute__((signal));

void __vector_USART_TX_ISR(void) __attribute__((signal));

void __vector_USART_ADC_ISR(void) __attribute__((signal));

void __vector_EE_READY_ISR(void) __attribute__((signal));

void __vector_ANALOG_COMP_ISR(void) __attribute__((signal));

void __vector_TWI_ISR(void) __attribute__((signal));

void __vector_SPM_READY_ISR(void) __attribute__((signal));

#endif // VECTORS_H
