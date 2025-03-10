#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "ARMv7AR.h"
#include "armcpu.h"
#include "task.h"

static KernelTCB_t taskList[MAX_TASK_NUM];
static KernelTCB_t* currentTCB;     // for Round-robin scheduling
static KernelTCB_t* nextTCB;        // for Round-robin scheduling
static uint32_t allocated_tcb_idx;
static uint32_t current_tcb_idx;    // for Round-robin scheduling

static KernelTCB_t* scheduler_roundrobin(void);
static KernelTCB_t* scheduler_priority(void);
static void save_context(void);
static void restore_context(void);

void kernel_task_init(void) {
    allocated_tcb_idx = 0;
    current_tcb_idx = 0;
    for (uint32_t i = 0; i < MAX_TASK_NUM; ++i) {
        taskList[i].stack_base = 
            (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        taskList[i].sp =
            (uint32_t)taskList[i].stack_base + USR_TASK_STACK_SIZE - 4; 
        taskList[i].sp -= sizeof(KernelTaskContext_t);


        KernelTaskContext_t* context = (KernelTaskContext_t *)taskList[i].sp;
        context->spsr = ARM_MODE_BIT_SYS;
        context->pc = 0;
    }
}
uint32_t kernel_task_create(KernelTaskFunc_t task) {
    KernelTCB_t* newTCB = &taskList[allocated_tcb_idx++];
    if (allocated_tcb_idx > MAX_TASK_NUM) {
        return NOT_ENOUGH_TASK_NUM;
    }

    KernelTaskContext_t* context = (KernelTaskContext_t *)newTCB->sp;
    context->pc = (uint32_t)task;

    return (allocated_tcb_idx - 1);
}
/* 
uint32_t kernel_task_create(KernelTaskFunc_t task, uint32_t priority) {
    KernelTCB_t* newTCB = &taskList[allocated_tcb_idx++];
    if (allocated_tcb_idx > MAX_TASK_NUM) {
        return NOT_ENOUGH_TASK_NUM;
    }
    newTCB->priority = priority;

    KernelTaskContext_t* context = (KernelTaskContext_t *)newTCB->sp;
    context->pc = (uint32_t)task;

    return (allocated_tcb_idx - 1);
}
 */
void kernel_task_scheduler(void) {
    currentTCB = &taskList[current_tcb_idx];
    debug_printf("current_tcb_idx = %u --> currentTCB.sp = 0x%x\n",
        current_tcb_idx, currentTCB->sp);
    nextTCB = scheduler_roundrobin();       // Select scheduler from here
    debug_printf("current_tcb_idx = %u --> currentTCB.sp = 0x%x\n",
        current_tcb_idx, nextTCB->sp);
    //disable_irq();
    kernel_task_context_switching();
    //enable_irq();
}
__attribute__((naked)) void kernel_task_context_switching(void) {
    __asm__ ("B save_context");
    __asm__ ("B restore_context");
}

static KernelTCB_t* scheduler_roundrobin(void) {
    current_tcb_idx++;
    current_tcb_idx %= allocated_tcb_idx;
    return &taskList[current_tcb_idx];
}

/* static KernelTCB_t* scheduler_priority(void) {
    for (uint32_t i = 0; i < allocated_tcb_idx; ++i) {
        KernelTCB_t* tempTCB = &taskList[i];
        if ((currentTCB != tempTCB) && 
            currentTCB->priority >= tempTCB->priority) {
            return tempTCB;
        }
    }
    return currentTCB;
}
 */
// This inline assembly function should co-worked with both structures
// KernelTCB_t and KernelTaskContext_t. Order should be followed.
static __attribute__((naked)) void save_context(void) {
    // save current task's context into the current task's stack
    __asm__("PUSH {lr}");
    __asm__("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("MRS r0, cpsr");
    __asm__("PUSH {r0}");
    // save current task's stack pointer into the current TCB
    __asm__("LDR r0, =currentTCB");
    __asm__("LDR r0, [r0]");
    __asm__("STMIA r0!, {sp}");
}
// This inline assembly function should co-worked with both structures
// KernelTCB_t and KernelTaskContext_t. Order should be followed.
static __attribute__((naked)) void restore_context(void) {
    // load next task's stack pointer in the next task's TCB
    __asm__("LDR r0, =nextTCB");
    __asm__("LDR r0, [r0]");
    __asm__("LDMIA r0!, {sp}");
    // load next task's context
    __asm__("POP {r0}");
    __asm__("MSR cpsr, r0");
    __asm__("POP {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("POP {pc}");
}

// This is for very first task when system begins.
void kernel_task_start(void) {
    nextTCB = &taskList[current_tcb_idx];
    restore_context();
}