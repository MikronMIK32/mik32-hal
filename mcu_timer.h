#ifndef MCU_TIMER_H_INCLUDED
#define MCU_TIMER_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/timer32.h"

#include <stdbool.h>

// HAL
#define FREQ 10000000
#define FREQ_BY_MS (FREQ/1000)

void delay(uint32_t periodMs);

// END HAL

#endif
