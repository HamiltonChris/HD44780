/*
 * @file hd44780.h
 *
 * @author Chris Hamilton
 * 
 * @brief driver for hd44780 LCD display
 */

#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>

/* LCD commands */
#define HD44780_CMD_DISPLAY_CLEAR       0x01
#define HD44780_CMD_RETURN_HOME         0x02
#define HD44780_CMD_ENTRY_MODE          0x04
#define HD44780_CMD_DISPLAY_ON_OFF      0x08
#define HD44780_CMD_DON_CURON           0x0E
#define HD44780_CMD_4DL_2N_5X8F         0x28

#define HD44780_FUNCTION_SET_4BIT       0x02
#define HD44780_FUNCTION_SET_8BIT       0x03

#define HD44780_INCREMENT               0x02

#define HD44780_RESET 0
#define HD44780_SET   1

typedef enum hd44780_pin_e {
    HD44780_RS = 0,
    HD44780_RW,
    HD44780_EN,
    HD44780_D0,
    HD44780_D1,
    HD44780_D2,
    HD44780_D3,
    HD44780_D4,
    HD44780_D5,
    HD44780_D6,
    HD44780_D7
} hd44780_pin_t;

typedef struct hd44780_config_s {
    void (*gpio_write)(hd44780_pin_t, uint8_t);
    void (*mdelay)(uint16_t);
    void (*udelay)(uint16_t);
    uint8_t rows;
    uint8_t columns;
    uint8_t cursor;
    uint8_t blink;
    uint8_t increment;
} hd44780_config_t;


void HD44780_init(hd44780_config_t* config);
void HD44780_send_command(const hd44780_config_t* config, uint8_t cmd);
void HD44780_print_char(const hd44780_config_t* config, uint8_t data);
void HD44780_display_clear(const hd44780_config_t* config);
void HD44780_display_return_home(const hd44780_config_t* config);
void HD44780_print_string(const hd44780_config_t* config, char *message);
void HD44780_set_cursor(const hd44780_config_t* config, uint8_t row, uint8_t column);

#endif /* HD44780_H */
