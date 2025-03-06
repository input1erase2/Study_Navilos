#include "stdint.h"
#include "HalUart.h"

static void hw_init(void);

void main(void) {
    hw_init();
    uint32_t i = 100;
    while (i--) {
        hal_uart_put_char('C');
    }
}

static void hw_init(void) {
    hal_uart_init();
}