#ifndef MIK32_HAL
#define MIK32_HAL

#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"

#ifndef OSC_SYSTEM_VALUE
    #define OSC_SYSTEM_VALUE ((uint32_t)32000000U) // Значение частоты основного внешнего источника по умолчанию.
#endif

#ifndef OSC_CLOCK_VALUE
    #define OSC_CLOCK_VALUE ((uint32_t)32000U) // Значение частоты часового внешнего источника по умолчанию.
#endif

#ifndef HSI_VALUE
    #define HSI_VALUE ((uint32_t)32000000U) // Значение частоты основного внутреннего источника по умолчанию.
#endif

#ifndef LSI_VALUE
    #define LSI_VALUE ((uint32_t)32000U) // Значение частоты часового внутреннего источника по умолчанию.
#endif

void HAL_MspInit();
HAL_StatusTypeDef HAL_Init();

#endif