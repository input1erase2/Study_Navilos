#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

static void hw_init(void);
static void printf_test(void);

void main(void) {
    hw_init();
    uint32_t i = 50;
    while (i--) {
        hal_uart_put_char('C');
    }
    hal_uart_put_char('\n');
    putstr("Hello, world!\n");

    i = 100;
    while (i--) {
        uint8_t ch = hal_uart_get_char();
        hal_uart_put_char(ch);
    }
}

static void hw_init(void) {
    hal_uart_init();
}

static void printf_test(void) {
    char* str = "printf() :: char pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    
    debug_printf("%s\n", "Hello printf()");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xFF, 0xFF);
}