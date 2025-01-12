// Copyright (c) 2025 Pedro Botelho
// All rights reserved

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

/*          Bit/Pin mappings        */
/* I2C Data D7-D6-D5-D4-D3-D2-D1-D0 */
/* LCD Pins D7-D6-D5-D4-BL-EN-RW-RS */

/* Data Type */

typedef struct lcd_t {
    uint8_t address;
    bool backlight;
} lcd_t;

/* Public Constants */

#define LCD_NXP_EXPANSOR_ADDRESS        (0x27)
#define LCD_TI_EXPANSOR_ADDRESS         (0x3F)

#define LCD_BL                          (1U << 3)
#define LCD_EN                          (1U << 2)
#define LCD_RW                          (1U << 1)
#define LCD_RS                          (1U << 0)

/* Commands */

#define lcd_clearDISPLAY_CMD            (0x01)
#define LCD_RETURNHOME_CMD              (0x02)
#define LCD_ENTRYMODESET_CMD            (0x04)
#define LCD_DISPLAYCONTROL_CMD          (0x08)
#define LCD_CURSORSHIFT_CMD             (0x10)
#define LCD_FUNCTIONSET_CMD             (0x20)
#define LCD_SETCGRAMADDR_CMD            (0x40)
#define LCD_SETDDRAMADDR_CMD            (0x80)

/* Flags for display entry mode */

#define LCD_CURSORDECREMENT_FLAG        (0x00)
#define LCD_CURSORINCREMENT_FLAG        (0x02)
#define LCD_ENTRYSHIFTINCREMENT_FLAG    (0x01)
#define LCD_ENTRYSHIFTDECREMENT_FLAG    (0x00)

/* Flags for display on/off control */

#define LCD_DISPLAYON_FLAG              (0x04)
#define LCD_DISPLAYOFF_FLAG             (0x00)
#define LCD_CURSORON_FLAG               (0x02)
#define LCD_CURSOROFF_FLAG              (0x00)
#define lcd_blinkON_FLAG                (0x01)
#define lcd_blinkOFF_FLAG               (0x00)

/* Flags for display/cursor shift */

#define LCD_DISPLAYMOVE_FLAG            (0x08)
#define LCD_CURSORMOVE_FLAG             (0x00)
#define LCD_MOVERIGHT_FLAG              (0x04)
#define LCD_MOVELEFT_FLAG               (0x00)
 
/* Flags for function set */

#define LCD_8BITMODE_FLAG               (0x10)
#define LCD_4BITMODE_FLAG               (0x00)
#define LCD_2LINE_FLAG                  (0x08)
#define LCD_1LINE_FLAG                  (0x00)
#define LCD_5x10DOTS_FLAG               (0x04)
#define LCD_5x8DOTS_FLAG                (0x00)

/* Default Commands + Flags */

#define LCD_8BIT_FUNCTION_CMD           (LCD_FUNCTIONSET_CMD    \
                                        | LCD_8BITMODE_FLAG)

#define LCD_4BIT_FUNCTION_CMD           (LCD_FUNCTIONSET_CMD    \
                                        | LCD_4BITMODE_FLAG)

#define LCD_DEFAULT_FUNCTION_CMD        (LCD_FUNCTIONSET_CMD    \
                                        | LCD_4BITMODE_FLAG     \
                                        | LCD_2LINE_FLAG        \
                                        | LCD_5x8DOTS_FLAG)

#define LCD_DEFAULT_CONTROL_CMD         (LCD_DISPLAYCONTROL_CMD \
                                        | LCD_DISPLAYON_FLAG    \
                                        | LCD_CURSOROFF_FLAG    \
                                        | lcd_blinkOFF_FLAG)

#define LCD_DEFAULT_MODE_CMD            (LCD_ENTRYMODESET_CMD   \
                                        | LCD_CURSORINCREMENT_FLAG    \
                                        | LCD_ENTRYSHIFTDECREMENT_FLAG)

/* Public Macros */

#define LCD_MASK                        (0xF0)
#define LCD_HIGH_NIBBLE(data)           (data & LCD_MASK)
#define LCD_LOW_NIBBLE(data)            ((data << 4) & LCD_MASK)
#define LCD_SET_BL(_bl)                 (_bl << 3)

#define LCD_RIGHT_JUSTIFY(lcd_handle)   (lcd_autoscroll(lcd_handle))
#define LCD_LEFT_JUSTIFY(lcd_handle)    (lcd_no_autoscroll(lcd_handle))

/* General Functions */

bool lcd_init(const lcd_t *lcd);

bool lcd_command(const lcd_t *lcd, uint8_t command);

bool lcd_write(const lcd_t *lcd, uint8_t data);

bool lcd_print(const lcd_t *lcd, const char *str);

/* Interface Functions */

bool lcd_send(const lcd_t *lcd, uint8_t data);

bool lcd_write_nibble(const lcd_t *lcd, uint8_t data);

bool lcd_enable_pulse(const lcd_t *lcd, uint8_t data);

/* Specific Commands */

bool lcd_backlight(const lcd_t *lcd);

bool lcd_clear(const lcd_t *lcd);

bool lcd_home(const lcd_t *lcd);

bool lcd_set_cursor(const lcd_t *lcd, uint32_t col, uint32_t row);

bool lcd_create_char(const lcd_t *lcd, uint8_t char_address, const uint8_t* charmap);

bool lcd_create_big_numbers(const lcd_t *lcd);

bool lcd_write_big_number(const lcd_t *lcd, uint8_t col, uint8_t number);

bool lcd_scroll_left(const lcd_t *lcd);

bool lcd_scroll_right(const lcd_t *lcd);

bool lcd_turn_off(const lcd_t *lcd);

bool lcd_turn_on(const lcd_t *lcd);

bool lcd_hide_cursor(const lcd_t *lcd);

bool lcd_show_cursor(const lcd_t *lcd);

bool lcd_no_blink(const lcd_t *lcd);

bool lcd_blink(const lcd_t *lcd);

bool lcd_left_to_right(const lcd_t *lcd);

bool lcd_right_to_left(const lcd_t *lcd);

bool lcd_autoscroll(const lcd_t *lcd);

bool lcd_no_autoscroll(const lcd_t *lcd);

#endif // LCD_H
