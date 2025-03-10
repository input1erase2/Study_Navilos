#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "event.h"

static uint32_t eventFlag;

void kernel_event_flag_init(void) {
    eventFlag = 0;
}

void kernel_event_flag_set(KernelEventFlag_t event) {
    eventFlag |= (uint32_t)event;
}

void kernel_event_flag_clear(KernelEventFlag_t event) {
    eventFlag &= ~((uint32_t)event);
}

bool kernel_event_flag_check(KernelEventFlag_t event) {
    if (eventFlag & (uint32_t)event) {
        kernel_event_flag_clear(event);
        return true;
    }
    return false;
}
