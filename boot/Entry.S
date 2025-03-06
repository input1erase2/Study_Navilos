#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
    .code 32

    .global vector_start
    .global vector_end

    vector_start :
        LDR     PC, reset_handler_addr
        LDR     PC, undef_handler_addr
        LDR     PC, svc_handler_addr
        LDR     PC, prfch_handler_addr
        LDR     PC, data_handler_addr
        B       .
        LDR     PC, irq_handler_addr
        LDR     PC, fiq_handler_addr

        reset_handler_addr :    .word   reset_handler
        undef_handler_addr :    .word   dummy_handler
        svc_handler_addr :      .word   dummy_handler
        prfch_handler_addr :    .word   dummy_handler
        data_handler_addr :     .word   dummy_handler
        irq_handler_addr :      .word   irq_handler
        fiq_handler_addr :      .word   fiq_handler
    vector_end :

    reset_handler :
        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_SVC   @ change to SVC mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =SVC_STACK_TOP          @ change SP
        
        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_IRQ   @ change to IRQ mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =IRQ_STACK_TOP          @ change SP

        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_FIQ   @ change to FIQ mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =FIQ_STACK_TOP          @ change SP

        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_ABT   @ change to ABT mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =ABT_STACK_TOP          @ change SP

        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_UND   @ change to UND mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =UND_STACK_TOP          @ change SP

        MRS     R0, CPSR                    @ R0 <- CPSR
        BIC     R1, R0, #0x1F               @ clear mode bits [5:0]
        ORR     R1, R1, #ARM_MODE_BIT_SYS   @ change to SYS mode
        MSR     CPSR, R1                    @ apply modified CPSR, update
        LDR     SP, =USRSYS_STACK_TOP       @ change SP

        BL      main
    
    dummy_handler :
        B       .

.end
