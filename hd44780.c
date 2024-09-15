/*
 * @file hd44780.c
 *
 * @author Chris Hamilton
 * 
 * @brief driver implementation for hd44780 LCD display
 */

#include "hd44780.h"


static void write_4_bits(const hd44780_config_t* config, uint8_t value);
static void enable(const hd44780_config_t* config);

void HD44780_init(hd44780_config_t* config)
{
    // Reset all pins
    config->gpio_write(HD44780_RS, HD44780_RESET);
    config->gpio_write(HD44780_RW, HD44780_RESET);
    config->gpio_write(HD44780_EN, HD44780_RESET);
    config->gpio_write(HD44780_D4, HD44780_RESET);
    config->gpio_write(HD44780_D5, HD44780_RESET);
    config->gpio_write(HD44780_D6, HD44780_RESET);
    config->gpio_write(HD44780_D7, HD44780_RESET);

    // Power-on initialization as per the HD44780 data sheet
    config->mdelay(40); // Delay atleast 15ms

    config->gpio_write(HD44780_RS, HD44780_RESET);
    config->gpio_write(HD44780_RW, HD44780_RESET);

    write_4_bits(config, HD44780_FUNCTION_SET_8BIT);

    config->mdelay(5); // Delay atleast 4.1ms

    write_4_bits(config, HD44780_FUNCTION_SET_8BIT);

    config->udelay(150);

    write_4_bits(config, HD44780_FUNCTION_SET_8BIT);
    write_4_bits(config, HD44780_FUNCTION_SET_4BIT);

    // Function set command
    HD44780_send_command(config, HD44780_CMD_4DL_2N_5X8F);

    // Display On and Cursor On
    HD44780_send_command(config, HD44780_CMD_DON_CURON);

    // Display Clear
    HD44780_display_clear(config);

    // Entry mode set
    HD44780_send_command(config, HD44780_CMD_ENTRY_MODE | HD44780_INCREMENT);
}


void HD44780_send_command(const hd44780_config_t* config, uint8_t cmd)
{
    config->gpio_write(HD44780_RS, HD44780_RESET);
    config->gpio_write(HD44780_RW, HD44780_RESET);

    write_4_bits(config, cmd >> 4);
    write_4_bits(config, cmd & 0xF);
}


void HD44780_print_char(const hd44780_config_t* config, uint8_t data)
{
    config->gpio_write(HD44780_RS, HD44780_SET);
    config->gpio_write(HD44780_RW, HD44780_RESET);

    write_4_bits(config, data >> 4);
    write_4_bits(config, data & 0xF);
}

void HD44780_display_clear(const hd44780_config_t* config)
{
    HD44780_send_command(config, HD44780_CMD_DISPLAY_CLEAR);
}


void HD44780_display_return_home(const hd44780_config_t* config)
{
    HD44780_send_command(config, HD44780_CMD_DISPLAY_CLEAR);
    config->mdelay(2);
}

void HD44780_print_string(const hd44780_config_t* config, char *message)
{
    do
    {
        HD44780_print_char(config, (uint8_t)*message++);
    } while (*message != '\0');
}

void HD44780_set_cursor(const hd44780_config_t* config, uint8_t row, uint8_t column)
{
    column--;
    row++;
    HD44780_send_command(config, (column | (row * 8 * config->columns)));
}

static void write_4_bits(const hd44780_config_t* config, uint8_t value)
{
    config->gpio_write(HD44780_D4, value & 0x1);
    config->gpio_write(HD44780_D5, (value >> 1) & 0x1);
    config->gpio_write(HD44780_D6, (value >> 2) & 0x1);
    config->gpio_write(HD44780_D7, (value >> 3) & 0x1);
    enable(config);
}

static void enable(const hd44780_config_t* config)
{
    config->gpio_write(HD44780_EN, HD44780_SET);
    config->udelay(10);
    config->gpio_write(HD44780_EN, HD44780_RESET);
    config->udelay(50); // delay longer than 37us
}
