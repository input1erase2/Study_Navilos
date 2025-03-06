#include "armcpu.h"

void enable_irq(void) {
    __asm("PUSH {R0, R1}");
    __asm("MRS R0, CPSR");
    __asm("BIC R1, R0, #0x80");     // Reset I-bit (IRQ enable)
    __asm("MSR CPSR, R1");
    __asm("POP {R0, R1}");
}

void disable_irq(void) {
    __asm("PUSH {R0, R1}");
    __asm("MRS R0, CPSR");
    __asm("ORR R1, R0, #0x80");     // Set I-bit (IRQ Disable)
    __asm("MSR CPSR, R1");
    __asm("POP {R0, R1}");
}

void enable_fiq(void) {
    __asm("PUSH {R0, R1}");
    __asm("MRS R1, CPSR");
    __asm("BIC R1, R0, #0x40");
    __asm("MSR CPSR, R1");
    __asm("POP {R0, R1}");
}

void disable_fiq(void) {
    __asm("PUSH {R0, R1}");
    __asm("MRS R1, CPSR");
    __asm("ORR R1, R0, #0x40");
    __asm("MSR CPSR, R1");
    __asm("POP {R0, R1}");
}
