#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#define NUM_MODULES 4
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7

// MCP23S08 SPI opcodes (based on datasheet Figure 1-3)
#define MCP23S08_WRITE_CMD  0x40  // 0100 00A1 A0 0 (assuming A1=A0=0)
#define MCP23S08_READ_CMD   0x41  // 0100 00A1 A0 1 (assuming A1=A0=0)

// MCP23S08 register addresses (from datasheet Table 1-2)
#define IODIR_REG    0x00  // I/O Direction register
#define IPOL_REG     0x01  // Input Polarity register  
#define GPINTEN_REG  0x02  // Interrupt-on-change control
#define DEFVAL_REG   0x03  // Default compare for interrupt
#define INTCON_REG   0x04  // Interrupt control
#define IOCON_REG    0x05  // Configuration register
#define GPPU_REG     0x06  // Pull-up resistor config
#define INTF_REG     0x07  // Interrupt flag (read-only)
#define INTCAP_REG   0x08  // Interrupt capture (read-only)
#define GPIO_REG     0x09  // Port register
#define OLAT_REG     0x0A  // Output latch register

static inline void cs_select() {
  asm volatile("nop \n nop \n nop");
  gpio_put(PIN_CS, 0); // Active low
  asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
  asm volatile("nop \n nop \n nop");
  gpio_put(PIN_CS, 1);
  asm volatile("nop \n nop \n nop");
}

static void write_register(uint8_t reg, uint8_t data) {
  uint8_t buf[3];
  buf[0] = MCP23S08_WRITE_CMD;  // Fixed: proper SPI command
  buf[1] = reg;                 // Register address
  buf[2] = data;                // Data to write
  
  cs_select();
  spi_write_blocking(spi_default, buf, 3);
  cs_deselect();
  sleep_ms(1);
}

static uint8_t read_register(uint8_t reg) {
  uint8_t tx_buf[3] = {MCP23S08_READ_CMD, reg, 0x00};
  uint8_t rx_buf[3];
  
  cs_select();
  spi_write_read_blocking(spi_default, tx_buf, rx_buf, 3);
  cs_deselect();
  
  return rx_buf[2]; // Data is in the third byte
}

static void init_mcp23s08() {
  // Enable hardware addressing (required for MCP23S08)
  write_register(IOCON_REG, 0x08); // Set HAEN bit (bit 3)
  
  // Set all pins as outputs (0 = output, 1 = input)
  write_register(IODIR_REG, 0x00);
  
  // Clear all outputs initially
  write_register(GPIO_REG, 0x00);
  
  printf("MCP23S08 initialized\n");
}

int main() {
  stdio_init_all();
  
  // Initialize SPI
  spi_init(spi_default, 1000 * 1000); // 1MHz, well within 10MHz limit
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
  
  // Initialize CS pin
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1); // CS high (inactive)
  
  sleep_ms(100); // Allow power-up
  
  printf("Initializing MCP23S08...\n");
  init_mcp23s08();
  
  // Test reading back the configuration
  uint8_t iodir_val = read_register(IODIR_REG);
  printf("IODIR register: 0x%02X (should be 0x00)\n", iodir_val);
  
  printf("Starting LED test...\n");
  
  while (true) {
    // Turn on all LEDs (assuming LEDs are connected to all 8 pins)
    printf("LEDs ON - All pins HIGH (0xFF)\n");
    write_register(GPIO_REG, 0xFF);
    sleep_ms(1000);
    
    // Turn off all LEDs  
    printf("LEDs OFF - All pins LOW (0x00)\n");
    write_register(GPIO_REG, 0x00);
    sleep_ms(1000);
    
    // Individual pin test
    for (int pin = 0; pin < 8; pin++) {
      uint8_t pin_value = 1 << pin;
      printf("LED on pin %d - Value: 0x%02X\n", pin, pin_value);
      write_register(GPIO_REG, pin_value);
      sleep_ms(500);
    }
    
    // All off
    write_register(GPIO_REG, 0x00);
    sleep_ms(1000);
  }
  
  return 0;
}
