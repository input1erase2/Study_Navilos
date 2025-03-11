#include "stdint.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

#include "task.h"
#include "kernel.h"

#define LOCAL_BUF_LEN   100

static void hw_init(void);
static void printf_test(void);
static void timer_test(void);
static void kernel_init(void);
void user_task0(void);
void user_task1(void);
void user_task2(void);

void main(void) {
    hw_init();
    
    uint32_t i = 50;
    while (i--) {
        hal_uart_put_char('C');
    }
    hal_uart_put_char('\n');
    putstr("Hello, world!\n");

    printf_test();
    timer_test();
    kernel_init();
    while (true);
}

static void hw_init(void) {
    hal_interrupt_init();
    hal_uart_init();
    hal_timer_init();
}

static void printf_test(void) {
    char* str = "printf() :: char pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    
    // Test 1. print string literal, %s
    debug_printf("%s\n", "Hello printf()");
    // Test 2. print char pointer, %s
    debug_printf("output string pointer: %s\n", str);
    // Test 3. print null pointer and unsigned int (const)
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    // Test 4. print unsigned int (variable)
    debug_printf("%u = 5\n", i);
    // Test 5. print number in decimal and hex
    debug_printf("dec=%u hex=%x\n", 0xFF, 0xFF);
}

static void timer_test(void) {
    for (uint32_t i = 0; i < 5; ++i) {
        debug_printf("Current timer count = %u\n", hal_timer_get_1ms_counter());
        delay(1000);    // 1000ms = 1s
    }
}
static void kernel_init(void) {
    uint32_t taskId;

    kernel_task_init();
    kernel_event_flag_init();
    kernel_msgQ_init();
    
    taskId = kernel_task_create(user_task0);
    // taskId = kernel_task_create(user_task0, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 0 creation failed\n");
    }
    // debug_printf("[>><<] Task 0 created successfully\n");

    taskId = kernel_task_create(user_task1);
    // taskId = kernel_task_create(user_task1, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 1 creation failed\n");
    }
    // debug_printf("[>><<] Task 1 created successfully\n");

    taskId = kernel_task_create(user_task2);
    // taskId = kernel_task_create(user_task2, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 2 creation failed\n");
    }
    // debug_printf("[>><<] Task 2 created successfully\n");

    kernel_start();
}

void user_task0(void) {
    uint32_t local = 0;
    debug_printf("[User Task #0] >> SP = 0x%x\n", &local);

    uint8_t cmdBuf[LOCAL_BUF_LEN];        // local buffer
    uint32_t cmdBufIdx = 0;     // local buffer's idx = message length
    uint8_t uartch = 0;

    while (true) {
        KernelEventFlag_t handleEvent = kernel_wait_events(
            KernelEventFlag_UartIn |
            KernelEventFlag_CmdOut
        );
        switch(handleEvent) {
        case KernelEventFlag_UartIn:
            // Task 0 <--- MessageQueue 0 <--- uartch(1-byte)
            kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);
            // Task 1 <--- MessageQueue 1 <--- Msg + length <--- 'Enter(\r\n)'
            if (uartch == '\r') {
                cmdBuf[cmdBufIdx] = '\0';
                // Exception handling --- Inf loop until message successfully send.
                while (true) {
                    // Step 1. Send length of message
                    if (false == kernel_send_msg(KernelMsgQ_Task1, &cmdBufIdx, 1)) {
                        kernel_yield();
                    }
                    // Step 2. Send message
                    else if (false == kernel_send_msg(KernelMsgQ_Task1, cmdBuf, cmdBufIdx)) {
                        uint8_t rollback;
                        kernel_recv_msg(KernelMsgQ_Task1, &rollback, 1);
                        kernel_yield();
                    }
                    else {
                        // Step 3. Send event
                        kernel_send_events(KernelEventFlag_CmdIn);
                        break;
                    }
                }
                cmdBufIdx = 0; 
            }
            else {
                cmdBuf[cmdBufIdx++] = uartch;
                cmdBufIdx %= LOCAL_BUF_LEN;
            }
            // debug_printf("\nTask 0 Event Received --- <UART IN> event\n");
            break;
        case KernelEventFlag_CmdOut:
            debug_printf("\nTask 0 Event Received --- <CMD OUT> event\n");
            break;
        default:
            break;
        }
        kernel_yield();
    }
}

void user_task1(void) {
    uint32_t local = 0;
    debug_printf("[User Task #1] >> SP = 0x%x\n", &local);

    uint8_t cmdlen = 0;     // message length
    uint8_t cmd[LOCAL_BUF_LEN];       // local buffer

    while (true) {
        KernelEventFlag_t handleEvent = kernel_wait_events(KernelEventFlag_CmdIn);
        switch(handleEvent) {
        case KernelEventFlag_CmdIn:
            memclr(cmd, LOCAL_BUF_LEN);
            kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
            kernel_recv_msg(KernelMsgQ_Task1, &cmd, cmdlen);
            debug_printf("\n[TASK 1] Message received: %s\n", cmd);
            break;
        default:
            break;
        }
        kernel_yield();
    }
}

void user_task2(void) {
    uint32_t local = 0;
    debug_printf("[User Task #2] >> SP = 0x%x\n", &local);
    while (true) {
        kernel_yield();
    }
}
