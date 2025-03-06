#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

static void hw_init(void);

void main(void) {
    hw_init();
    uint32_t i = 50;
    while (i--) {
        hal_uart_put_char('C');
    }
    hal_uart_put_char('\n');
    putstr("Hello, world!\n");
}

static void hw_init(void) {
    hal_uart_init();
}