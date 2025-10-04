#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include <stdio.h>

// === Shared SPI pins ===
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_MOSI 19
#define PIN_SCK 18

// === DAC101S101 ===
#define PIN_CS_DAC 17

// === MCP23S08 IO-expander ===
#define PIN_CS_IOE 20
#define REG_IODIR 0x00
#define REG_GPIO  0x09      // Use GPIO register for immediate output
#define OPCODE_WRITE 0x40

// DAC write function
// mode: 0 = normal, 1 = PD 1k, 2 = PD 100k, 3 = Hi-Z
void dac101s101_write(uint16_t value, uint8_t mode) {
    uint16_t command = ((mode & 0x3) << 12) | ((value & 0x03FF) << 2);
    uint8_t buf[2] = {command >> 8, command & 0xFF};
    
    gpio_put(PIN_CS_DAC, 0);
    sleep_us(1);
    spi_write_blocking(SPI_PORT, buf, 2);
    sleep_us(1);
    gpio_put(PIN_CS_DAC, 1);
}

// IO-expander write
void mcp23s08_write(uint8_t reg, uint8_t data) {
    uint8_t buf[3] = {OPCODE_WRITE, reg, data};
    
    gpio_put(PIN_CS_IOE, 0);
    sleep_us(1);
    spi_write_blocking(SPI_PORT, buf, 3);
    sleep_us(1);
    gpio_put(PIN_CS_IOE, 1);
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("\n=== DAC + IO Expander Test ===\n\n");
    
    // Setup SPI
    spi_init(SPI_PORT, 1000*1000); // 1 MHz
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    
    // DAC CS
    gpio_init(PIN_CS_DAC);
    gpio_set_dir(PIN_CS_DAC, GPIO_OUT);
    gpio_put(PIN_CS_DAC, 1);
    
    // IO-expander CS
    gpio_init(PIN_CS_IOE);
    gpio_set_dir(PIN_CS_IOE, GPIO_OUT);
    gpio_put(PIN_CS_IOE, 1);
    
    sleep_ms(10);
    
    // Init IO-expander (all pins output)
    printf("Initializing MCP23S08...\n");
    mcp23s08_write(REG_IODIR, 0x00);  // All pins as outputs
    mcp23s08_write(REG_GPIO, 0xFF);   // Start with all LEDs OFF (inverted logic!)
    sleep_ms(10);
    
    // Init ADC
    printf("Initializing ADC...\n");
    adc_init();
    adc_gpio_init(26); // GP26 = ADC0
    adc_select_input(0);
    
    printf("\nStarting voltage sweep...\n");
    printf("DAC Value | LED Value | LED Binary | ADC Raw | Voltage\n");
    printf("----------|-----------|------------|---------|--------\n");
    
    while (1) {
        for (uint16_t dacval = 0; dacval <= 1023; dacval += 64) {
            // Write to DAC
            dac101s101_write(dacval, 0);
            sleep_us(100);  // Small delay between devices
            
            // LED value = scaled DAC (0-1023 mapped to 0-255)
            uint8_t ledval = dacval >> 2;
            
            // INVERT the LED value because MCP23S08 has inverted logic!
            // 0 = LED ON, 1 = LED OFF
            uint8_t ledval_inverted = ~ledval;
            
            // Write to IO-expander
            mcp23s08_write(REG_GPIO, ledval_inverted);
            
            sleep_ms(10);  // Allow settling
            
            // ADC readback
            uint16_t raw = adc_read();
            float voltage = raw * 3.3f / 4095.0f;
            
            // Print LED pattern in binary for visualization
            printf("%4u/1023 | %3u/255   | ", dacval, ledval);
            for (int bit = 7; bit >= 0; bit--) {
                printf("%c", (ledval & (1 << bit)) ? '1' : '0');
            }
            printf(" | %4u    | %.3fV\n", raw, voltage);
            
            sleep_ms(500);
        }
        
        printf("\n--- Cycle complete, restarting ---\n\n");
        sleep_ms(1000);
    }
}
