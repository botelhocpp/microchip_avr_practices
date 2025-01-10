#include "twi.h"

#include "gpio.h"

#include "atmega328p.h"

#define TWS_MASK                (0xF8)
#define TWINT_BIT               (7)
#define TWINT                   (1U << 7)
#define TWEA                    (1U << 6)
#define TWSTA                   (1U << 5)
#define TWSTO                   (1U << 4)
#define TWWC                    (1U << 3)
#define TWEN                    (1U << 2)
#define TWIE                    (1U << 0)

#define TWI_PS                  (1)

#define TWI_CALC_TWBR(scl, ps)  ((((CPU_FREQ) / ((scl)*(ps))) - 16) / 2)

const gpio_t SDA = {
    .port = IO_PORT_C, .pin = 4
};
const gpio_t SCL = {
    .port = IO_PORT_C, .pin = 5
};
const uint8_t TWI_TWBR[] = {
    [TWI_CLOCK_SELECT_100KHZ] = TWI_CALC_TWBR(100000, TWI_PS),
    [TWI_CLOCK_SELECT_400KHZ] = TWI_CALC_TWBR(400000, TWI_PS)
};

static bool twi_interrupt_is_enabled = false;

void twi_init(twi_clock_select clock, bool enable_interrupt) {
    TWBR = TWI_TWBR[clock];

    gpio_init(&SDA, IO_DIRECTION_IN_PULLUP);
    gpio_init(&SCL, IO_DIRECTION_IN_PULLUP);
    
    if(enable_interrupt) {
        twi_enable_interrupt();
    }
    else {
        twi_disable_interrupt();
    }

    twi_enable();
}

void twi_enable(void) {
    TWCR |= TWEN;
}

void twi_disable(void) {
    TWCR &= ~TWEN;
}

void twi_send_ack(void) {
    TWCR |= TWEA;
}

bool twi_check_completion(void) {
    return (TWCR & TWINT);
}

void twi_clear_flag(void) {
    TWCR = TWINT | TWEN | twi_interrupt_is_enabled;
}

void twi_enable_interrupt(void) {
    twi_interrupt_is_enabled = true;
    TWCR |= TWIE;
}

void twi_disable_interrupt(void) {
    twi_interrupt_is_enabled = false;
    TWCR &= ~TWIE;
}

twi_status twi_get_status(void) {
    return (TWSR & TWS_MASK);
}

/* TWI Non-Blocking Directives */

void twi_send_start(void) {
    TWCR = TWINT | TWSTA | TWEN | twi_interrupt_is_enabled;
}

void twi_send_stop(void) {
    TWCR = TWINT | TWSTO | TWEN | twi_interrupt_is_enabled;
}

void twi_write_data(uint8_t data) {
    TWDR = data;

    twi_clear_flag();
}

uint8_t twi_read_data(void) {
    return TWDR;
}

/* TWI Blocking Directives */

twi_status twi_start(void) {
    twi_send_start();

    while(!twi_check_completion());

    twi_status status = twi_get_status();

    if(status == TWI_STATUS_START_TRANSMITTED || status == TWI_STATUS_RE_START_TRANSMITTED) {
        status = TWI_STATUS_NO_ERROR;
    }

    return status;
}

twi_status twi_stop(void) { 
    twi_send_stop();

    while(TWCR & TWSTO);

    twi_status status = twi_get_status();

    if(status == TWI_STATUS_IDLE) {
        status = TWI_STATUS_NO_ERROR;
    }

    return status;
}

twi_status twi_write(uint8_t data) {
    while(!twi_check_completion());

    twi_write_data(data);

    while(!twi_check_completion());

    twi_status status = twi_get_status();

    if(
        status == TWI_STATUS_SLAVE_ADDR_WRITE_TRANSMITTED_ACK_RECV || 
        status == TWI_STATUS_SLAVE_ADDR_READ_TRANSMITTED_ACK_RECV ||  
        status == TWI_STATUS_DATA_TRANSMITTED_ACK_RECV
    ) {
        status = TWI_STATUS_NO_ERROR;
    }

    return status;
}

twi_status twi_read(uint8_t *data, bool send_ack) {
    twi_clear_flag();

    if(send_ack) {
        twi_send_ack();
    }

    while(!twi_check_completion());

    *data = twi_read_data();

    twi_status status = twi_get_status();

    if(
        (send_ack && status == TWI_STATUS_DATA_RECEIVED_WITH_ACK) ||
        (!send_ack && status == TWI_STATUS_DATA_RECEIVED_WITH_NACK)
    ) {
        status = TWI_STATUS_NO_ERROR;
    }

    return status;
}

twi_status twi_write_byte(uint8_t slave_address, uint8_t data) {
    twi_status status = TWI_STATUS_NO_ERROR;

    uint8_t slave_address_8bit = slave_address << 1;

    status = twi_start();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(slave_address_8bit);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(data);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_stop();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    return TWI_STATUS_NO_ERROR;
}

twi_status twi_read_byte(uint8_t slave_address, uint8_t *data) {
    twi_status status = TWI_STATUS_NO_ERROR;

    uint8_t slave_address_8bit = slave_address << 1;

    status = twi_start();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(slave_address_8bit | 1);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_read(data, TWI_SEND_NACK);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_stop();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    return TWI_STATUS_NO_ERROR;
}

twi_status twi_write_multiple(uint8_t slave_address, uint8_t first_register_address, const uint8_t *data, uint8_t size) {
    twi_status status = TWI_STATUS_NO_ERROR;

    uint8_t slave_address_8bit = slave_address << 1;

    status = twi_start();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(slave_address_8bit);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(first_register_address);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    for(uint8_t i = 0; i < size; i++) {
        status = twi_write(data[i]);
        if(status != TWI_STATUS_NO_ERROR) {
            return status;
        }
    }

    status = twi_stop();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    return TWI_STATUS_NO_ERROR;
}

twi_status twi_read_multiple(uint8_t slave_address, uint8_t first_register_address, uint8_t *data, uint8_t size) {
    twi_status status = TWI_STATUS_NO_ERROR;

    uint8_t slave_address_8bit = slave_address << 1;

    status = twi_start();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(slave_address_8bit);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(first_register_address);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_repeated_start();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    status = twi_write(slave_address_8bit | 1);
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    for(uint8_t i = 0; i < size; i++) {
        bool send_ack = (i == size - 1)? TWI_SEND_NACK : TWI_SEND_ACK;
        status = twi_read(&data[i], send_ack);
        if(status != TWI_STATUS_NO_ERROR) {
            return status;
        }
    }

    status = twi_stop();
    if(status != TWI_STATUS_NO_ERROR) {
        return status;
    }

    return TWI_STATUS_NO_ERROR;
}
