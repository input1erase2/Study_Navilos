#include "stdint.h"
#include "stdbool.h"
#include "HalTimer.h"

void delay(uint32_t ms) {
    uint32_t goal = hal_timer_get_1ms_counter() + ms;
    while (goal != hal_timer_get_1ms_counter());    // Overflow considered by using '!='
}

void memclr(void* dst, uint32_t count) {
    uint8_t* d = (uint8_t*)dst;
    while (count--) {
        *d++ = 0;
    }
}