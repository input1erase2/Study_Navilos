#include "stdint.h"
#include "Interrupt.h"
#include "HalInterrupt.h"
#include "armcpu.h"
#include "memio.h"

extern volatile GicCpu_t* gicCpu;
extern volatile GicDist_t* gicDist;

static InterHdlr_fptr handlers[INTERRUPT_HANDLER_NUM];

void hal_interrupt_init(void) {
    gicCpu->cpucontrol.bits.Enable = 1;
    // GIC_PRIORITY_MASK_NONE = Priority 0x0 to 0xE are not masked
    gicCpu->prioritymask.bits.Prioritymask = GIC_PRIORITY_MASK_NONE;
    gicDist->distributorctrl.bits.Enable = 1;
    for (uint32_t i = 0; i < INTERRUPT_HANDLER_NUM; ++i) {
        // Initialize every handler to nullptr
        handlers[i] = NULL;
    }
    enable_irq();
}

void hal_interrupt_enable(uint32_t interruptNum) {
    // Check param interrupt number is in legal range.
    if ((interruptNum < GIC_IRQ_START) || (GIC_IRQ_END < interruptNum)) {
        return;
    }
    uint32_t bitNum = interruptNum - GIC_IRQ_START;
    if (bitNum >= GIC_IRQ_START) {
        // Interrupt Number is higher than 64, so it should go to 'setenable2'
        bitNum -= GIC_IRQ_START;
        SET_BIT(gicDist->setenable2, bitNum);
    } else {
        SET_BIT(gicDist->setenable1, bitNum);
    }
}

void hal_interrupt_disable(uint32_t interruptNum) {
    // Check param interrupt number is in legal range.
    if ((interruptNum < GIC_IRQ_START) || (GIC_IRQ_END < interruptNum)) {
        return;
    }
    uint32_t bitNum = interruptNum - GIC_IRQ_START;
    if (bitNum >= GIC_IRQ_START) {
        // Interrupt Number is higher than 64, so it should go to 'setenable2'
        bitNum -= GIC_IRQ_START;
        CLEAR_BIT(gicDist->setenable2, bitNum);
    } else {
        CLEAR_BIT(gicDist->setenable1, bitNum);
    }
}

void hal_interrupt_register_handler(uint32_t interruptNum, InterHdlr_fptr handler) {
    handlers[interruptNum] = handler;
}

void hal_interrupt_run_handler(void) {
    uint32_t interruptNum = gicCpu->interruptack.bits.InterruptID;  // read ACK
    if (handlers[interruptNum] != NULL) handlers[interruptNum]();   // call
    gicCpu->endofinterrupt.bits.InterruptID = interruptNum;         // end
}