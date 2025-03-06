#include "stdint.h"
#include "Uart.h"

volatile PL011_t* uart = (PL011_t*)UART_BASE_ADDRESS0;
