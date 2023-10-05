#include "mik32_hal.h"


__attribute__((weak)) void HAL_MspInit()
{
    __HAL_PCC_PAD_CONFIG_CLK_ENABLE();   
    __HAL_PCC_EPIC_CLK_ENABLE();
}

HAL_StatusTypeDef HAL_Init()
{
	HAL_MspInit();

	return HAL_OK;
}