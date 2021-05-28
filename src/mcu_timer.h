#ifndef MCU_TIMER_H_INCLUDED
#define MCU_TIMER_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/timer32.h"

#include <stdbool.h>

#include "../../mcu32-hal-main/src/mcu_core.h"

// HAL
#define FREQ_BY_MS (FREQ/1000)

#define DELAY_TIMER (TIMER32_1)

void delay(uint32_t periodMs);
void Timer_Init(TIMER32_TypeDef* timer, uint32_t top);
void Timer_Start(TIMER32_TypeDef* timer);
void Timer_StartIT(TIMER32_TypeDef* timer);

// END HAL

#endif
