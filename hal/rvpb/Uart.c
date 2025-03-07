#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"

extern volatile PL011_t* uart;

static void interrupt_handler(void);

void hal_uart_init(void) {
    // Enable UART
    uart->uartcr.bits.UARTEN = 0;
    uart->uartcr.bits.TXE = 1;
    uart->uartcr.bits.RXE = 1;
    uart->uartcr.bits.UARTEN = 1;
    // Enable UART input interrupt
    uart->uartimsc.bits.RXIM = 1;       // Interrupt Mask Set/Clear Register
    // Register UART interrupt handler
    hal_interrupt_enable(UART_INTERRUPT0);
    hal_interrupt_register_handler(UART_INTERRUPT0, interrupt_handler);
}

void hal_uart_put_char(uint8_t ch) {
    while (uart->uartfr.bits.TXFF);     // Waiting buffer is empty
    uart->uartdr.all = (ch & 0xFF);
}

/* 
// Unoptimized code (5 memory access for every uart get request)
uint8_t hal_uart_get_char(void) {
    uint32_t data;
    while (uart->uartfr.bits.RXFE);
    
    if (uart->uartdr.bits.BE || uart->uartdr.bits.FE ||
        uart->uartdr.bits.OE || uart->uartdr.bits.PE) {
        // clear the errors
        uart->uartrsr.bits.BE = 1;
        uart->uartrsr.bits.FE = 1;
        uart->uartrsr.bits.OE = 1;
        uart->uartrsr.bits.PE = 1;
        return 0;
    }
    data = uart->uartdr.bits.DATA;
    return data;
}
 */

// optimized code (1 memory access for every uart get request)
uint8_t hal_uart_get_char(void) {
    uint32_t data;
    while (uart->uartfr.bits.RXFE);
    data = uart->uartdr.all;
    if (data & 0xFFFFFF00) {
        // clear the errors
        uart->uartrsr.all = 0xFF;
        return 0;
    }
    return (uint8_t)(data & 0xFF);
}

static void interrupt_handler(void) {
    uint8_t ch = hal_uart_get_char();
    hal_uart_put_char(ch);
}