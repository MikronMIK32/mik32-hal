#ifndef MCU_TIMER_H_INCLUDED
#define MCU_TIMER_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/timer32.h"

#include <stdbool.h>

#include "mcu_core.h"

// HAL
#define FREQ_BY_MS (FREQ/1000)
#define FREQ_BY_QS (FREQ/10000000)

#define DELAY_TIMER (TIMER32_1)

void delay(uint32_t periodMs);
void delayQs(uint32_t periodQs);
void initTimer(TIMER32_TypeDef* timer, uint32_t top);
void startTimer(TIMER32_TypeDef* timer);
void startTimerIT(TIMER32_TypeDef* timer);

// END HAL

#endif
