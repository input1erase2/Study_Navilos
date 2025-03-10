#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"
#include "memio.h"

void kernel_start(void) {
    kernel_task_start();
}
void kernel_yield(void) {
    // Request scheduler to context switching.
    kernel_task_scheduler();
}

void kernel_send_events(uint32_t eventList) {
    for (uint32_t i = 0; i < 32; ++i) {
        if ((eventList >> i) & 1) {
            KernelEventFlag_t sendingEvent = KernelEventFlag_Empty;
            sendingEvent = (KernelEventFlag_t)SET_BIT(sendingEvent, i);
            kernel_event_flag_set(sendingEvent);
        }
    }
}

KernelEventFlag_t kernel_wait_events(uint32_t eventList) {
    for (uint32_t i = 0; i < 32; ++i) {
        if ((eventList >> i) & 1) {
            KernelEventFlag_t waitingEvent = KernelEventFlag_Empty;
            waitingEvent = (KernelEventFlag_t)SET_BIT(waitingEvent, i);
            if (kernel_event_flag_check(waitingEvent)) {
                return waitingEvent;
            }
        }
    }
    return KernelEventFlag_Empty;
}

bool kernel_send_msg(KernelMsgQ_t q, void* data, uint32_t count) {

}

uint32_t kernel_recv_msg(KernelMsgQ_t q, void* data, uint32_t count) {

}
