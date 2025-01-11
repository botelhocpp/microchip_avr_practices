#include "spi.h"

#include "atmega328p.h"

#define SPIF_BIT    (7)
#define WCOL_BIT    (6)
#define SPIF        (1U << SPIF_BIT)
#define WCOL        (1U << WCOL_BIT)
#define SPI2X       (1U << 0)

#define SPIE        (1U << 7)
#define SPE         (1U << 6)
#define DORD        (1U << 5)
#define MSTR        (1U << 4)
#define CPOL        (1U << 3)
#define CPHA        (1U << 2)
#define SPR1        (1U << 1)
#define SPR0        (1U << 0)

const gpio_t SPI_SCK = {
    .port = IO_PORT_B, .pin = 5
};
const gpio_t SPI_MISO = {
    .port = IO_PORT_B, .pin = 4
};
const gpio_t SPI_MOSI = {
    .port = IO_PORT_B, .pin = 3
};

void spi_init(spi_mode mode, spi_clock_mode clock_mode, spi_prescaler prescaler, spi_data_order data_order, bool enable_interrupt) {
    gpio_init(&SPI_SCK, IO_DIRECTION_OUT);
    gpio_init(&SPI_MOSI, IO_DIRECTION_OUT);
    gpio_init(&SPI_MISO, IO_DIRECTION_IN);

    spi_set_mode(mode);
    spi_set_clock_mode(clock_mode);
    spi_set_prescaler(prescaler);
    spi_set_data_order(data_order);
    
    if(enable_interrupt) {
        spi_enable_interrupt();
    }
    else {
        spi_disable_interrupt();
    }

    spi_enable();
}

void spi_enable(void) {
    SPCR |= SPE;
}

void spi_disable(void) {
    SPCR &= ~SPE;
}

void spi_set_mode(spi_mode mode) {
    if(mode == SPI_MODE_MASTER) {
        SPCR |= MSTR;
    }
    else {
        SPCR &= ~MSTR;
    }
}

void spi_set_clock_mode(spi_clock_mode clock_mode) {
    if(clock_mode & 0b1) {
        SPCR |= CPHA;
    }
    else {
        SPCR &= ~CPHA;
    }

    if(clock_mode & 0b10) {
        SPCR |= CPOL;
    }
    else {
        SPCR &= ~CPOL;
    }
}

void spi_set_prescaler(spi_prescaler prescaler) {
    SPCR &= ~(0b11);
    SPCR |= (prescaler & 0b11);
    
    if(prescaler & 0b100) {
        SPSR |= SPI2X;
    }
    else {
        SPSR &= ~SPI2X;
    }
}

void spi_set_data_order(spi_data_order data_order) {
    if(data_order == SPI_DATA_ORDER_LSB_FIRST) {
        SPCR |= DORD;
    }
    else {
        SPCR &= ~DORD;
    }
}

void spi_enable_interrupt(void) {
    SPCR |= SPIE;
}

void spi_disable_interrupt(void) {
    SPCR &= ~SPIE;
}

void spi_write(uint8_t data) {
    SPDR = data;
}

uint8_t spi_read(void) {
    return SPDR;
}

uint8_t spi_transfer(uint8_t data) {
    spi_write(data);
    while(!spi_check_completion());
    return spi_read();
}

void spi_config_chip_select(const gpio_t *cs_gpio) {
    gpio_init(cs_gpio, IO_DIRECTION_OUT);
}

void spi_enable_chip_select(const gpio_t *cs_gpio) {
    gpio_write(cs_gpio, 0);
}

void spi_disable_chip_select(const gpio_t *cs_gpio) {
    gpio_write(cs_gpio, 1);
}

bool spi_check_completion(void) {
    return (SPSR >> SPIF_BIT);
}

bool spi_check_collision(void) {
    return (SPSR >> WCOL_BIT) & 1;
}
