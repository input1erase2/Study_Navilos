#include "stdbool.h"
#include "stdint.h"
#include "HalInterrupt.h"

// Exception vector table handlers

// __attribute__((interrupt (""))) corrects LR (return addr) automatically.
__attribute__ ((interrupt ("IRQ"))) void irq_handler(void) {
    hal_interrupt_run_handler();
}

__attribute__ ((interrupt ("FIQ"))) void fiq_handler(void) {
    // DUMMY HANDLER for now
    while (true);
}