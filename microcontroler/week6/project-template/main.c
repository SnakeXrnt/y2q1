#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define ADDR 0x52

int main() {
    stdio_init_all();
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    sleep_ms(500);

    uint8_t buf[2];
    
    buf[0] = 0x00; buf[1] = 0x02;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    while (true) {
        uint8_t reg = 0b00001101;
        uint8_t data[3];
        i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
        i2c_read_blocking(i2c_default, ADDR, data, 3, false);

        uint32_t value = ((data[2] & 0x0F) << 16) | (data[1] << 8) | data[0];
        printf("Light raw = %lu\n", (unsigned long)value);
        sleep_ms(500);
    }
}
