#ifndef HAL_UART_H_
#define HAL_UART_H_

void hal_uart_init(void);
void hal_uart_put_char(uint8_t ch);
uint8_t hal_uart_get_char(void);

#endif /* HAL_UART_H_ */