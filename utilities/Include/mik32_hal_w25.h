#ifndef MIK32_HAL_W25
#define MIK32_HAL_W25

#include "mik32_hal_spifi.h"

typedef enum __HAL_W25_SREGTypeDef
{
    W25_SREG1 = 1,
    W25_SREG2 = 2,
} HAL_W25_SREGTypeDef;

void HAL_W25_WriteEnable(SPIFI_HandleTypeDef *spifi);

uint8_t HAL_W25_ReadSREG(SPIFI_HandleTypeDef *spifi, HAL_W25_SREGTypeDef SREG);

void HAL_W25_WriteSREG(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2);

void HAL_W25_WriteSREG_Volatile(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2);

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_W25_WaitBusy(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if((HAL_W25_ReadSREG(spifi, W25_SREG1) & 1) == 0)
        {
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

void HAL_W25_PageProgram(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t data);

#endif // MIK32_HAL_W25
