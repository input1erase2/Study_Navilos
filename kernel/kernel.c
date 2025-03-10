#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"

void kernel_start(void) {
    kernel_task_start();
}
void kernel_yield(void) {
    // Request scheduler to context switching.
    kernel_task_scheduler();
}