#include "pico/stdlib.h" 
#include <hardware/irq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/structs/io_bank0.h>
#include <hardware/uart.h>
#include <stdio.h>
#include "hardware/adc.h"

void uart_interupted() {
    printf("nigga \n");
}

int main() {
    stdio_init_all(); 
    
    uart_init(0, 9600);
    
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_set_fifo_enabled(uart0, false);
    uart_set_hw_flow(uart0,false,false);
    
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    irq_set_exclusive_handler(UART0_IRQ, uart_interupted);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
  
    
    while(true) {
      uint16_t raw = adc_read();
      float voltage = raw * 3.3f / 4095.0f;  
      int temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
      char temperature_convert = temperature + '0';

      
      uart_putc(uart0, temperature_convert);
      printf("Temp : %c", temperature_convert);
      sleep_ms(1000);
    }
  }

