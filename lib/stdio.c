#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

uint32_t putstr(const char* s) {
    uint32_t count = 0;
    while (*s) {
        hal_uart_put_char(*s++);
        count++;
    }
    return count;
}
