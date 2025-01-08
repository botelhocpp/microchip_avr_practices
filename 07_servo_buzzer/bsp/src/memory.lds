OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:5)

MEMORY
{
    FLASH (RX) : ORIGIN = 0, LENGTH = 32K
    SRAM (RW!X) : ORIGIN = 0x800100, LENGTH = 2K
}

PROVIDE(__stack_top = ORIGIN(SRAM) + LENGTH(SRAM) - 1);

SECTIONS
{
    .text : {
        KEEP(*(.vectors))
        . = ALIGN(2);
        *(.text)
    }> FLASH

    PROVIDE(__data_load_start = LOADADDR(.data));
    .data : {
        PROVIDE(__data_start = .);
        *(.data)
        KEEP(*(.rodata))
        *(.rodata)
        *(.rodata*)
        PROVIDE(__data_end = .);
    }> SRAM AT> FLASH

    .bss : {
        PROVIDE(__bss_start = .);
        *(.bss)
        *(.COMMON)
        PROVIDE(__bss_end = .);
    }> SRAM
} 
