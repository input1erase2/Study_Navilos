#ifndef KERNEL_KERNEL_H_
#define KERNEL_KERNEL_H_

#include "task.h"
#include "event.h"
#include "msg.h"
#include "synch.h"

// task.h
void kernel_start(void);
void kernel_yield(void);
// event.h
void kernel_send_events(uint32_t eventList);
KernelEventFlag_t kernel_wait_events(uint32_t eventList);
// msg.h
bool kernel_send_msg(KernelMsgQ_t q, void* data, uint32_t count);
uint32_t kernel_recv_msg(KernelMsgQ_t q, void* data, uint32_t count);
// synch.h
void kernel_lock_sem(void);
void kernel_unlock_sem(void);
void kernel_lock_mutex(void);
void kernel_unlock_mutex(void);

#endif