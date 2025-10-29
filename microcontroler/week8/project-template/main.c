#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "drivers/ambeient.c"
#include "drivers/io_expander.c"
#include "drivers/temperature.c"
#include "drivers/i2c.c"
#include "drivers/spi.c"
#include "drivers/button.c"

#define LIGHT_ADDR 0x52
#define TEMP_ADDR  0x49
#define BUTTON_PIN 15
#define INT_PIN    14
#define OS_PIN 13

#define PIN_CS   17
#define PIN_MISO 16
#define PIN_MOSI 19
#define PIN_SCK  18
#define PIN_SYNC 20

#define PIN_SDA 4 
#define PIN_SCL 5 

// ====== Mode/LED state ======
static volatile uint8_t mode_light = 1;      // 1 = show light, 0 = show temperature
static volatile bool mode_changed = false;
static volatile bool flash_request = false;  // set by INT ISR, handled in main()
static uint8_t last_led_byte = 0x00;    

void gpio_callback(uint gpio, uint32_t events) {
    static uint64_t last_us = 0;
    uint64_t now = time_us_64();
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_RISE)) {
        if (now - last_us > 200000) {  // 200 ms debounce
            mode_light ^= 1;
            mode_changed = true;
            last_us = now;
        }
    }
}

static void gpio_all_irq(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_RISE)) {
        gpio_callback(gpio, events);                // your toggle/debounce
    } else if (gpio == INT_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        // just set a flag; do work in main loop
        flash_request = true;
    } else if (gpio == OS_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        flash_request = true;
    }
}


int main(void) {
    stdio_init_all();

    // I2C, SPI init, sensors, thresholds...
    initialize_i2c();
    initialize_spi();
    io_exp_write(0x00, 0);     // whatever your IO expander needs
    amb_setup_thresholds(100, 160000, 4); // make sure this enables INT in the sensor!
    temp_treshold_up(0x3C);
    temp_treshold_down(0x32);

    // --- GPIO directions & pulls ---
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);                 // button active-high

    gpio_init(INT_PIN);
    gpio_set_dir(INT_PIN, GPIO_IN);
    gpio_pull_up(INT_PIN);                      // ambient INT is open-drain, active-LOW

    gpio_init(OS_PIN);
    gpio_set_dir(OS_PIN, GPIO_IN);
    gpio_pull_up(OS_PIN);                       // assuming active-LOW; flip if not

    // --- register ONE global callback once ---
    gpio_set_irq_enabled_with_callback(/*any pin*/ BUTTON_PIN, 0, true, &gpio_all_irq);

    // --- now enable the specific edges per pin ---
    gpio_set_irq_enabled(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(INT_PIN,   GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(OS_PIN,    GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (mode_changed) {
            mode_changed = false;
            if (mode_light) {
                gpio_set_irq_enabled(INT_PIN,   GPIO_IRQ_EDGE_FALL, true);
                gpio_set_irq_enabled(OS_PIN,    GPIO_IRQ_EDGE_FALL, false);
            } else {
                gpio_set_irq_enabled(INT_PIN,   GPIO_IRQ_EDGE_FALL, false);
                gpio_set_irq_enabled(OS_PIN,    GPIO_IRQ_EDGE_FALL, true);
            }
            printf("Mode: %s\n", mode_light ? "LIGHT" : "TEMPERATURE");
        }

        if (flash_request) {                // do the flashing OUTSIDE the ISR
            flash_request = false;
            flash_leds_3x();
            clear_interupt();
        }

        if (mode_light) {

            uint32_t light = read_ambeient();   // also: use 0x29 (7-bit) in that driver
            printf("Light raw: %lu\n", (unsigned long)light);
            last_led_byte = (uint8_t)light;
            io_exp_write(0x09, last_led_byte);
        } else {

            float temp = read_temperature();
            printf("Temperature: %.3f C\n", temp);
            last_led_byte = (uint8_t)temp;
            io_exp_write(0x09, last_led_byte);
        }

        sleep_ms(300);
    }
}
