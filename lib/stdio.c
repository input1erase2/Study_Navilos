#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

#define PRINTF_BUF_LEN 1024
static char printf_buf[PRINTF_BUF_LEN];     // 1KB

uint32_t putstr(const char* s) {
    uint32_t c = 0;
    while (*s) {
        hal_uart_put_char(*s++);
        c++;
    }
    return c;
}
uint32_t debug_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(printf_buf, fmt, args);
    va_end(args);
    return putstr(printf_buf);
}

uint32_t vsprintf(char* buf, const char* fmt, va_list arg) {
    uint32_t c = 0;
    char ch;
    char* str;
    uint32_t uint;
    uint32_t hex;

    for (uint32_t i = 0; fmt[i]; ++i) {
        if (fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
            case 'c':
                ch = (char)va_arg(arg, int32_t);
                buf[c++] = ch;
                break;
            case 's':
                str = (char*)va_arg(arg, char*);
                if (str == NULL) {
                    str = "(null)";
                }
                while (*str) {
                    buf[c++] = (*str++);
                }
                break;
            case 'u':
                uint = (uint32_t)va_arg(arg, uint32_t);
                c += utoa(&buf[c], uint, utoa_dec);
                break;
            case 'x':
                hex = (uint32_t)va_arg(arg, uint32_t);
                c += utoa(&buf[c], hex, utoa_hex);
                break;
            }
        }
        else {
            buf[c++] = fmt[i];
        }
    }
    buf[c] = '\0';      // [Important!] Make sure last char is null char.
    return c;
}

uint32_t utoa(char* buf, uint32_t val, utoa_t base) {
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[11];       // It is big enough for store 32-bit int

    do {
        uint32_t t = val % (uint32_t)base;
        if (t >= 10) t += 'A' - '0' - 10;
        tmp[idx] = (t + '0');
        val /= base;
        idx++;
    } while (val);

    // we should reverse tmp[]
    idx--;
    while (idx >= 0) {
        buf[c++] = tmp[idx];
        idx--;
    }
    return c;
}