#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"
#include "memio.h"

/* Task related functions ------------------- */
void kernel_start(void) {
    kernel_task_start();
}
void kernel_yield(void) {
    // Request scheduler to context switching.
    kernel_task_scheduler();
}

/* Event related functions ------------------- */
void kernel_send_events(uint32_t eventList) {
    KernelEventFlag_t sendingEvent = KernelEventFlag_Empty;
    for (uint32_t i = 0; i < 32; ++i) {
        if ((eventList >> i) & 1) {
            SET_BIT(sendingEvent, i);
            kernel_event_flag_set(sendingEvent);
        }
    }
}

KernelEventFlag_t kernel_wait_events(uint32_t eventList) {
    KernelEventFlag_t waitingEvent = KernelEventFlag_Empty;
    for (uint32_t i = 0; i < 32; ++i) {
        if ((eventList >> i) & 1) {
            SET_BIT(waitingEvent, i);
            if (kernel_event_flag_check(waitingEvent)) {
                return waitingEvent;
            }
        }
    }
    return KernelEventFlag_Empty;
}

/* Message related functions ------------------- */
bool kernel_send_msg(KernelMsgQ_t q, void* data, uint32_t count) {
    uint8_t* d = (uint8_t*) data;

    // If there is no space enough to store message,
    if (count > (MSG_Q_SIZE_BYTE - kernel_msgQ_count(q))) {
        return false;
    }
    for (uint32_t i = 0; i < count; ++i) {
        if (false == kernel_msgQ_enqueue(q,*d)) {
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

void kernel_flush_msg(KernelMsgQ_t q) {
    uint8_t d = 0;

    while (true) {
        // Dequeue every message from queue.
        if (false == kernel_msgQ_dequeue(q, &d)) {
            break;
        }
    }
}

/* Semaphore related functions --------------- */
void kernel_lock_sem(void) {
    // Keep waiting until semaphore is available.
    while (false == kernel_sem_test()) {
        kernel_yield();
    }
}

void kernel_unlock_sem(void) {
    kernel_sem_release();
}

/* Mutex related functions -------------------- */
void kernel_lock_mutex(void) {
    uint32_t current_task_id;
    while (true) {
        current_task_id = kernel_task_get_current_task_id();
        // trying mutex lock, mutex lock failed
        if (false == kernel_mutex_lock(current_task_id)) {
            kernel_yield();
        }
        // mutex lock sucessfully
        else {
            break;
        }
    }
}

void kernel_unlock_mutex(void) {
    uint32_t current_task_id = kernel_task_get_current_task_id();
    // trying to unlock mutex, but failed
    if (false == kernel_mutex_unlock(current_task_id)) {
        kernel_yield();
    }
}