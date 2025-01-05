; Copyright (c) 2025 Pedro Botelho
; All Rights Reserved

.equ SREG, 0x3F 
.equ SPL, 0x3D 
.equ SPH, 0x3E

/* Macro for creating weak functions */
.macro vector name
.weak \name
.set \name, __dummy_function
    JMP \name
.endm

.weak main
.set main, __dummy_function

/* Vector Table */
.section .vectors, "ax"
.global __vectors
    JMP _start
    vector __vector_INT0_ISR
    vector __vector_INT1_ISR
    vector __vector_PCINT0_ISR
    vector __vector_PCINT1_ISR
    vector __vector_PCINT2_ISR
    vector __vector_WDT_ISR
    vector __vector_TIMER2_COMPA_ISR
    vector __vector_TIMER2_COMPB_ISR
    vector __vector_TIMER2_OVF_ISR
    vector __vector_TIMER1_CAPT_ISR
    vector __vector_TIMER1_COMPA_ISR
    vector __vector_TIMER1_COMPB_ISR
    vector __vector_TIMER1_OVF_ISR
    vector __vector_TIMER0_COMPA_ISR
    vector __vector_TIMER0_COMPB_ISR
    vector __vector_TIMER0_OVF_ISR
    vector __vector_SPI_STC_ISR
    vector __vector_USART_RX_ISR
    vector __vector_USART_UDRE_ISR
    vector __vector_USART_TX_ISR
    vector __vector_USART_ADC_ISR
    vector __vector_EE_READY_ISR
    vector __vector_ANALOG_COMP_ISR
    vector __vector_TWI_ISR
    vector __vector_SPM_READY_ISR

/* Reset Handler */
.section .text
.global _start
_start:
    EOR R20, R20
    LDI R21, lo8(__stack_top)
    LDI R22, hi8(__stack_top)
    
    OUT SREG, R20
    OUT SPL, R21
    OUT SPH, R22

    RCALL __copy_data
    RCALL __clear_bss

    CALL main

    JMP .

.global __copy_data
__copy_data:
    ; .data End [R17:R16]
    LDI R17, hi8(__data_end)
    LDI R16, lo8(__data_end)

    ; X Register [R27:R26] (VMA)
    LDI R27, hi8(__data_start)
    LDI R26, lo8(__data_start)

    ; Z Register [R31:R30] (LMA)
    LDI R31, hi8(__data_load_start)
    LDI R30, lo8(__data_load_start)

    RJMP .copy_data_start
    
.copy_data_loop:
    LPM	R0, Z+           
    ST	X+, R0   

.copy_data_start:  
    CP R26, R16	
    CPC	R27, R17
    BRNE .copy_data_loop

    RET

.global __clear_bss
__clear_bss:
    EOR R1, R1

    ; .bss End [R17:R16]
    LDI R17, hi8(__bss_end)
    LDI R16, lo8(__bss_end)

    ; X Register [R27:R26]
    LDI R27, hi8(__bss_start)
    LDI R26, lo8(__bss_start)
    
    RJMP .clear_bss_start

.clear_bss_loop:
    ST	X+, R1

.clear_bss_start:
    CP	R26, R16	; 4
 	CPC	R27, R17
 	BRNE .clear_bss_loop

    RET

/* Default ISR Handler */
.global __dummy_function
__dummy_function:
    JMP __dummy_function
