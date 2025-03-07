#include "stdint.h"
#include "Uart.h"
#include "Interrupt.h"
#include "Timer.h"

volatile PL011_t* uart = (PL011_t*)UART_BASE_ADDRESS0;
volatile GicCpu_t* gicCpu = (GicCpu_t*)GIC_CPU_BASE;
volatile GicDist_t* gicDist = (GicDist_t*)GIC_DIST_BASE;
volatile Timer_t* timer = (Timer_t*)TIMER_CPU_BASE;