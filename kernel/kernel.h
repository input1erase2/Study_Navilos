#ifndef KERNEL_KERNEL_H_
#define KERNEL_KERNEL_H_

#include "task.h"
#include "event.h"

void kernel_start(void);
void kernel_yield(void);

void kernel_send_events(uint32_t eventList);
KernelEventFlag_t kernel_wait_events(uint32_t eventList);

#endif