#include "lcd.h"

#include "twi.h"
#include "utils.h"

/* Private Constants */

#define LCD_CURSOR_START_FIRST_LINE			(0x80)
#define LCD_CURSOR_START_SECOND_LINE		(0xC0)

static const uint8_t LCD_BIG_NUMBER_CODES[8][8] = {
    {
        0b00000001,//0
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00000000,
    },
    {
        0b00011111,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000000,
    },
    {
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000000,
    },
    {
        0b00011111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00011111,
        0b00000000,
    },
    {
        0b00011111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {
        0b00011111,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00011111,
        0b00000000
    }
};

static const uint8_t LCD_BIG_NUMBER_COMMANDS[10][4] = {
		{0x01, 0x02, 0x4C, 0x00},
		{0x20, 0x7C, 0x20, 0x7C},
		{0x04, 0x05, 0x4C, 0x5F},
		{0x06, 0x05, 0x5F, 0x00},
		{0x4C, 0x00, 0x20, 0x03},
		{0x07, 0x04, 0x5F, 0x00},
		{0x07, 0x04, 0x4C, 0x00},
		{0x06, 0x02, 0x20, 0x03},
		{0x07, 0x05, 0x4C, 0x00},
		{0x07, 0x05, 0x20, 0x03} 
};

/* General Functions */

bool lcd_init(const lcd_t *lcd) {    
    /* Wait until voltage stabilizes */
    delay_ms(50);

    /* Start in 8-bit mode [First try] */
	if(!lcd_write_nibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
	delay_us(4100);

    /* Start in 8-bit mode [Second try] */
	if(!lcd_write_nibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
	delay_us(100);

    /* Start in 8-bit mode [Third try] */
	if(!lcd_write_nibble(lcd, LCD_8BIT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(100);

	/* Set 4-bit mode */
	if(!lcd_write_nibble(lcd, LCD_4BIT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(100);

	/* Set font size and number of lines  */
	if(!lcd_command(lcd, LCD_DEFAULT_FUNCTION_CMD)) {
        return false;
    }
    delay_us(53);

	/* Turn the display on with no cursor or blinking */
	if(!lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD)) {
        return false;
    }
    delay_us(53);

	/* Clear the Display */
	if(!lcd_clear(lcd)) {
        return false;
    }

    /* Setup the cursor to move from left to right */
	if(!lcd_command(lcd, LCD_DEFAULT_MODE_CMD)) {
        return false;
    }
    delay_us(53);

    return true;
}

bool lcd_command(const lcd_t *lcd, uint8_t command) {
    return lcd_write_nibble(lcd, LCD_HIGH_NIBBLE(command))
        && lcd_write_nibble(lcd, LCD_LOW_NIBBLE(command));
}

bool lcd_write(const lcd_t *lcd, uint8_t data) {
    return lcd_write_nibble(lcd, LCD_HIGH_NIBBLE(data) | LCD_RS) 
        && lcd_write_nibble(lcd, LCD_LOW_NIBBLE(data) | LCD_RS);
}

bool lcd_print(const lcd_t *lcd, const char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(!lcd_write(lcd, str[i])) {
            return false;
        }
    }

    return true;
}

/* Interface Functions */

bool lcd_send(const lcd_t *lcd, uint8_t data) {   
    uint8_t expander_data = data | LCD_SET_BL(lcd->backlight);
    
    return twi_write_byte(lcd->address, expander_data) == TWI_STATUS_NO_ERROR;
}

bool lcd_write_nibble(const lcd_t *lcd, uint8_t data) {
    return lcd_send(lcd, data)
        && lcd_enable_pulse(lcd, data);
}

bool lcd_enable_pulse(const lcd_t *lcd, uint8_t data) {
    if(!lcd_send(lcd, (data | LCD_EN))) {
        return false;
    }
	delay_us(1);

	if(!lcd_send(lcd, (data & ~LCD_EN))) {
        return false;
    }
	delay_us(50);
    
    return true;
}

/* Specific Commands */

bool lcd_backlight(const lcd_t *lcd) {
    return lcd_send(lcd, 0);
}

bool lcd_clear(const lcd_t *lcd) {
    if(!lcd_command(lcd, lcd_clearDISPLAY_CMD)) {
        return false;
    }
	delay_ms(2);

    return true;
}

bool lcd_home(const lcd_t *lcd) {
    if(!lcd_command(lcd, LCD_RETURNHOME_CMD)) {
        return false;
    }
	delay_ms(2);

    return true;
}

bool lcd_set_cursor(const lcd_t *lcd, uint32_t col, uint32_t row) {
    uint8_t cursor_address = (row == 0) ? LCD_CURSOR_START_FIRST_LINE : LCD_CURSOR_START_SECOND_LINE;
    return lcd_command(lcd, LCD_SETDDRAMADDR_CMD | (cursor_address + col));
}

bool lcd_create_char(const lcd_t *lcd, uint8_t char_address, const uint8_t* charmap) {
	if(!lcd_command(lcd, LCD_SETCGRAMADDR_CMD | (char_address << 3))) {
        return false;
    }
	for (int i = 0; i < 8; i++) {
		if(!lcd_write(lcd, charmap[i])) {
            return false;
        }
	}
    return true;
}

bool lcd_create_big_numbers(const lcd_t *lcd) {
	for (int i = 0, j = 0; i < 8; i++, j++) {
		if(!lcd_create_char(lcd, i, LCD_BIG_NUMBER_CODES[j])) {
            return false;
        }
	}
    return true;
}

bool lcd_write_big_number(const lcd_t *lcd, uint8_t col, uint8_t number) {
    return lcd_set_cursor(lcd, col, 0)
        && lcd_write(lcd, LCD_BIG_NUMBER_COMMANDS[number][0])
        && lcd_write(lcd, LCD_BIG_NUMBER_COMMANDS[number][1])
        && lcd_set_cursor(lcd, col, 1)
        && lcd_write(lcd, LCD_BIG_NUMBER_COMMANDS[number][2])
        && lcd_write(lcd, LCD_BIG_NUMBER_COMMANDS[number][3]);
}

bool lcd_scroll_left(const lcd_t *lcd) {
    return lcd_command(lcd, LCD_CURSORSHIFT_CMD | LCD_DISPLAYMOVE_FLAG | LCD_MOVELEFT_FLAG);
}

bool lcd_scroll_right(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_CURSORSHIFT_CMD | LCD_DISPLAYMOVE_FLAG | LCD_MOVERIGHT_FLAG);
}

bool lcd_turn_off(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD & ~LCD_DISPLAYON_FLAG);
}

bool lcd_turn_on(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD | LCD_DISPLAYON_FLAG);
}

bool lcd_hide_cursor(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD & ~LCD_CURSORON_FLAG);
}

bool lcd_show_cursor(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD | LCD_CURSORON_FLAG);
}

bool lcd_no_blink(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD & ~lcd_blinkON_FLAG);
}

bool lcd_blink(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_CONTROL_CMD | lcd_blinkON_FLAG);
}

bool lcd_left_to_right(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_MODE_CMD | LCD_CURSORINCREMENT_FLAG);
}

bool lcd_right_to_left(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_MODE_CMD & ~LCD_CURSORINCREMENT_FLAG);
}

bool lcd_autoscroll(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_MODE_CMD | LCD_ENTRYSHIFTINCREMENT_FLAG);
}

bool lcd_no_autoscroll(const lcd_t *lcd) {
	return lcd_command(lcd, LCD_DEFAULT_MODE_CMD & ~LCD_ENTRYSHIFTINCREMENT_FLAG);
}