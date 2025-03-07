#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "task.h"

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

    taskId = kernel_task_create(user_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 0 creation failed\n");
    }

    taskId = kernel_task_create(user_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 1 creation failed\n");
    }

    taskId = kernel_task_create(user_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task 2 creation failed\n");
    }
}

void user_task0(void) {
    debug_printf("User Task #0\n");
    
    while (true);
}

void user_task1(void) {
    debug_printf("User Task #1\n");
    
    while (true);
}

void user_task2(void) {
    debug_printf("User Task #2\n");
    
    while (true);
}
