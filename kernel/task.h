#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "MemoryMap.h"      // for using task stack size

#define NOT_ENOUGH_TASK_NUM     0xFFFFFFFF
#define USR_TASK_STACK_SIZE     0x100000
#define MAX_TASK_NUM            (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)

typedef struct KernelTaskContext_t {
    uint32_t spsr;
    uint32_t r0_r12[13];
    uint32_t lr;            // But every task runs inf loop, so LR is no need in this proj.
    uint32_t pc;
} KernelTaskContext_t;

typedef struct KernelTCB_t {
    uint32_t sp;
    uint8_t* stack_base;
    uint32_t priority;
} KernelTCB_t;

typedef void (*KernelTaskFunc_t)(void);

void kernel_task_init(void);
void kernel_task_start(void);
uint32_t kernel_task_create(KernelTaskFunc_t task, uint32_t priority);
void kernel_task_scheduler(void);
void kernel_task_context_switching(void);

#endif /* KERNEL_TASK_H_ */