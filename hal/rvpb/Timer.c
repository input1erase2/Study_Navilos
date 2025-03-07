#include "stdint.h"
#include "HalTimer.h"
#include "Timer.h"
#include "HalInterrupt.h"

extern volatile Timer_t* timer;

static void interrupt_handler(void);
static uint32_t internal_1ms_counter;

void hal_timer_init(void) {
    // Interface reset procedure from datasheet [section 2.2.3]
    timer->timerxcontrol.bits.TimerEn = 0;
    timer->timerxcontrol.bits.TimerMode = 0;
    timer->timerxcontrol.bits.OneShot = 0;
    timer->timerxcontrol.bits.TimerSize = 0;
    timer->timerxcontrol.bits.TimerPre = 0;
    timer->timerxcontrol.bits.IntEnable = 1;
    timer->timerxload = 0;
    timer->timerxvalue = 0xFFFFFFFF;

    // Set periodic mode
    timer->timerxcontrol.bits.TimerMode = TIMER_PERIODIC;
    timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    timer->timerxcontrol.bits.TimerPre = 0;     // 0 stages of prescale, clock is divided by 1
    timer->timerxcontrol.bits.OneShot = 0;      // Wrapping mode
    timer->timerxcontrol.bits.IntEnable = 1;    // Interrupt enabled
    
    uint32_t interval_1ms = TIMER_1MHZ_INTERVAL / 1000;

    timer->timerxload = interval_1ms;           // Set deadline to 1ms interval value
    timer->timerxcontrol.bits.TimerEn = 1;      // Timer ON
    internal_1ms_counter = 0;
    // Register timer interrupt handler
    hal_interrupt_enable(TIMER_INTERRUPT);
    hal_interrupt_register_handler(TIMER_INTERRUPT, interrupt_handler);
}

uint32_t hal_timer_get_1ms_counter(void) {
    return internal_1ms_counter;
}

static void interrupt_handler(void) {
    internal_1ms_counter++;
    timer->timerxintclr = 1;        // Clear interrupt
}
