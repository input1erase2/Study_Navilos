#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t* uart;

void hal_uart_init(void) {
    // Enable UART
    uart->uartcr.bits.UARTEN = 0;
    uart->uartcr.bits.TXE = 1;
    uart->uartcr.bits.RXE = 1;
    uart->uartcr.bits.UARTEN = 1;
}

void hal_uart_put_char(uint8_t ch) {
    while (uart->uartfr.bits.TXFF);     // Waiting buffer is empty
    uart->uartdr.all = (ch & 0xFF);     // bottom
}
