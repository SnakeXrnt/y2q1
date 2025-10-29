#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define TEMP_ADDR  0x49
#define PIN_SDA 4 
#define PIN_SCL 5


#define REG_TOS 0x03 // upper treshold 
#define REG_THYST 0x02 // lower treshold
#define REG_TEMP 0X00

static void temp_register_write(uint8_t reg, uint8_t msb, uint8_t lsb) {
    uint8_t buffer[3] = {reg, msb, lsb};
    i2c_write_blocking(i2c_default, TEMP_ADDR, buffer, 3, false);
}

static void temp_treshold_up(uint32_t value9bit) {
    uint8_t msb = (uint8_t)(value9bit & 0xFF);
    uint8_t lsb = 0x00;
    temp_register_write(REG_TOS, msb, lsb);

}


static void temp_treshold_down(uint32_t value9bit) {
    uint8_t msb = (uint8_t)(value9bit & 0xFF);
    uint8_t lsb = 0x00;
    temp_register_write(REG_THYST, msb, lsb);
}


float read_temperature(void)
{
    uint8_t reg = 0x00;
    uint8_t buf[2];

    i2c_write_blocking(i2c_default, TEMP_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, TEMP_ADDR, buf, 2, false);

    int16_t raw = (int16_t)((buf[0] << 8) | buf[1]);
    raw >>= 5;
    if (raw & 0x0400) raw |= 0xF800;

    return raw * 0.125f;
}

static inline void clear_os(void) {
    uint8_t reg = REG_TEMP;
    uint8_t buf[2];
    i2c_write_blocking(i2c_default, TEMP_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, TEMP_ADDR, buf, 2, false);
}


