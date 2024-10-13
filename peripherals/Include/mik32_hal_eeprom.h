#ifndef MIK32_HAL_EEPROM_H_INCLUDED
#define MIK32_HAL_EEPROM_H_INCLUDED

#include "mik32_hal_def.h"
#include <mik32_memory_map.h>
#include <eeprom.h>
#include <power_manager.h>

#ifndef HAL_EEPROM_TIMEOUT
#define HAL_EEPROM_TIMEOUT 10000
#endif

typedef enum
{
    HAL_EEPROM_OP_READ = 0b00,
    HAL_EEPROM_OP_CLEAR = 0b01,
    HAL_EEPROM_OP_PROGRAM = 0b10,
} HAL_EEPROM_OperationTypeDef;

typedef enum
{
    HAL_EEPROM_WRITE_SINGLE = 0b00,
    HAL_EEPROM_WRITE_EVEN = 0b01,
    HAL_EEPROM_WRITE_ODD = 0b10,
    HAL_EEPROM_WRITE_ALL = 0b11,
} HAL_EEPROM_WriteBehaviourTypeDef;

typedef enum
{
    HAL_EEPROM_MODE_TWO_STAGE = 0b00,
    HAL_EEPROM_MODE_THREE_STAGE = 0b01,
} HAL_EEPROM_ModeTypeDef;

typedef enum
{
    HAL_EEPROM_ECC_ENABLE = 0b00,
    HAL_EEPROM_ECC_DISABLE = 0b01,
} HAL_EEPROM_ErrorCorrectionTypeDef;

typedef enum
{
    HAL_EEPROM_SERR_DISABLE = 0b00,
    HAL_EEPROM_SERR_ENABLE = 0b01,
} HAL_EEPROM_EnableInterruptTypeDef;

typedef struct
{
    uint8_t N_LD;
    uint8_t N_R_1;
    uint8_t N_R_2;
    uint32_t N_EP_1;
    uint16_t N_EP_2;
} HAL_EEPROM_TimingsTypeDef;

typedef struct
{
    EEPROM_REGS_TypeDef *Instance;
    HAL_EEPROM_TimingsTypeDef Timings;
    HAL_EEPROM_ModeTypeDef Mode;
    HAL_EEPROM_ErrorCorrectionTypeDef ErrorCorrection;
    HAL_EEPROM_EnableInterruptTypeDef EnableInterrupt;
} HAL_EEPROM_HandleTypeDef;

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_EEPROM_WaitBusy(HAL_EEPROM_HandleTypeDef *eeprom, uint32_t timeout)
{
    while (timeout)
    {
        timeout--;
        if (!(eeprom->Instance->EESTA & EEPROM_EESTA_BSY_M))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

#define HAL_EEPROM_INTERRUPT_GET(eeprom) ((*eeprom).Instance->EESTA & EEPROM_EESTA_SERR_M)
#define HAL_EEPROM_INTERRUPT_FLAG_CLEAR(eeprom) (*eeprom).Instance->EESTA &= ~EEPROM_EESTA_SERR_M;

HAL_StatusTypeDef HAL_EEPROM_Init(HAL_EEPROM_HandleTypeDef *eeprom);

HAL_StatusTypeDef HAL_EEPROM_Erase(
    HAL_EEPROM_HandleTypeDef *eeprom,
    uint16_t address,
    uint8_t erasedWordsCount,
    HAL_EEPROM_WriteBehaviourTypeDef erasedPages,
    uint32_t timeout);

HAL_StatusTypeDef HAL_EEPROM_Write(
    HAL_EEPROM_HandleTypeDef *eeprom,
    uint16_t address,
    uint32_t *data,
    uint8_t length,
    HAL_EEPROM_WriteBehaviourTypeDef writedPages,
    uint32_t timeout);

HAL_StatusTypeDef HAL_EEPROM_Read(HAL_EEPROM_HandleTypeDef *eeprom, uint16_t address, uint32_t *data, uint8_t length, uint32_t timeout);

void HAL_EEPROM_SetMode(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_ModeTypeDef mode);

void HAL_EEPROM_SetErrorCorrection(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_ErrorCorrectionTypeDef errorCorrection);

void HAL_EEPROM_SetInterrupt(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_EnableInterruptTypeDef enableInterrupt);

void HAL_EEPROM_CalculateTimings(HAL_EEPROM_HandleTypeDef *eeprom, int32_t frequency);

HAL_StatusTypeDef HAL_EEPROM_GetECC(HAL_EEPROM_HandleTypeDef *eeprom, uint16_t address, uint8_t *buf_value_ecc, uint32_t timeout);

#endif // MIK32_HAL_EEPROM_H_INCLUDED