#ifndef HAL_INTERRUPT_H_
#define HAL_INTERRUPT_H_

#define INTERRUPT_HANDLER_NUM   255

typedef void (*InterHdlr_fptr)(void);

void hal_interrupt_init(void); 
void hal_interrupt_enable(uint32_t interruptNum);
void hal_interrupt_disable(uint32_t interruptNum);
void hal_interrupt_register_handler(uint32_t interruptNum, InterHdlr_fptr handler);
void hal_interrupt_run_handler(void);

#endif /* HAL_INTERRUPT_H_ */