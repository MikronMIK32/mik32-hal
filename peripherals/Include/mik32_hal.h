#ifndef MIK32_HAL
#define MIK32_HAL

#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"

#ifndef OSC_SYSTEM_VALUE
    #define OSC_SYSTEM_VALUE ((uint32_t)32000000U) // Значение частоты основного внешнего источника по умолчанию.
#endif

#ifndef OSC_CLOCK_VALUE
    #define OSC_CLOCK_VALUE ((uint32_t)32768U) // Значение частоты часового внешнего источника по умолчанию.
#endif

#ifndef HSI_VALUE
    #define HSI_VALUE ((uint32_t)32000000U) // Значение частоты основного внутреннего источника по умолчанию.
#endif

#ifndef LSI_VALUE
    #define LSI_VALUE ((uint32_t)32768U) // Значение частоты часового внутреннего источника по умолчанию.
#endif

void HAL_MspInit();
HAL_StatusTypeDef HAL_Init();
/* Функции программных задержек */
void HAL_ProgramDelayMs(uint32_t time_ms);
void HAL_ProgramDelayUs(uint32_t time_us);
/* Переопределяемые функции времени */
uint32_t HAL_Micros();
uint32_t HAL_Millis();
void HAL_DelayMs(uint32_t time_ms);
void HAL_DelayUs(uint32_t time_us);

#endif
