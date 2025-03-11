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
    uint8_t* d = (uint8_t*) data;
    for (uint32_t i = 0; i < count; ++i) {
        /* Enqueue the message to the message box.
            If queue is full or something wrong, we should roll-back. */
        if (false == kernel_msgQ_enqueue(q,*d)) {
            for (uint32_t j = 0; j < i; ++j) {
                uint8_t rollback;
                kernel_msgQ_dequeue(q, &rollback);
            }
            return false;
        }
        d++;
    }
    return true;
}

uint32_t kernel_recv_msg(KernelMsgQ_t q, void* data, uint32_t count) {
    uint8_t* d = (uint8_t*)data;
    for (uint32_t i = 0; i < count; ++i) {
        /* Read the message from the message box.
            If the queue is empty or something wrong, we should stop. */
        if (false == kernel_msgQ_dequeue(q, d)) {
            return i;   // return how many byte we have read for next request.
        }
        d++;
    }
    return count;
}
