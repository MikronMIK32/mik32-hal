#ifndef MIK32_HAL_SPIFI
#define MIK32_HAL_SPIFI

#include <stdbool.h>
#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <spifi.h>
#include <mcu32_memory_map.h>

typedef enum __HAL_SPIFI_CacheEnableTypeDef
{
    SPIFI_CACHE_DISABLE = 0,
    SPIFI_CACHE_ENABLE = 1,
} HAL_SPIFI_CacheEnableTypeDef;

typedef enum __HAL_SPIFI_DataCacheEnableTypeDef
{
    SPIFI_DATA_CACHE_ENABLE = 0,
    SPIFI_DATA_CACHE_DISABLE = 1,
} HAL_SPIFI_DataCacheEnableTypeDef;

typedef enum __HAL_SPIFI_InterruptEnableTypeDef
{
    SPIFI_INTERRUPT_DISABLE = 0,
    SPIFI_INTERRUPT_ENABLE = 1,
} HAL_SPIFI_InterruptEnableTypeDef;

typedef enum __HAL_SPIFI_Mode3EnableTypeDef
{
    SPIFI_MODE3_DISABLE = 0,
    SPIFI_MODE3_ENABLE = 1,
} HAL_SPIFI_Mode3EnableTypeDef;

typedef enum __HAL_SPIFI_PrefetchEnableTypeDef
{
    SPIFI_PREFETCH_ENABLE = 0,
    SPIFI_PREFETCH_DISABLE = 1,
} HAL_SPIFI_PrefetchEnableTypeDef;

typedef enum __HAL_SPIFI_DualModeEnableTypeDef
{
    SPIFI_DUAL_MODE_ENABLE = 0,
    SPIFI_DUAL_MODE_DISABLE = 1,
} HAL_SPIFI_DualModeEnableTypeDef;

typedef enum __HAL_SPIFI_DMAEnableTypeDef
{
    SPIFI_DMA_ENABLE = 0,
    SPIFI_DMA_DISABLE = 1,
} HAL_SPIFI_DMAEnableTypeDef;

typedef enum __HAL_SPIFI_FieldFormTypeDef
{
    SPIFI_FIELDFORM_ALL_SERIAL = 0,
    SPIFI_FIELDFORM_DATA_PARALLEL = 1,
    SPIFI_FIELDFORM_COMMAND_SERIAL = 2,
    SPIFI_FIELDFORM_ALL_PARALLEL = 3
} HAL_SPIFI_FieldFormTypeDef;

typedef enum __HAL_SPIFI_FrameFormTypeDef
{
    SPIFI_FRAMEFORM_OPCODE = 1,
    SPIFI_FRAMEFORM_OPCODE_1ADDR = 2,
    SPIFI_FRAMEFORM_OPCODE_2ADDR = 3,
    SPIFI_FRAMEFORM_OPCODE_3ADDR = 4,
    SPIFI_FRAMEFORM_OPCODE_4ADDR = 5,
    SPIFI_FRAMEFORM_3ADDR = 6,
    SPIFI_FRAMEFORM_4ADDR = 7
} HAL_SPIFI_FrameFormTypeDef;

typedef enum __HAL_SPIFI_DirectionTypeDef
{
    SPIFI_DIRECTION_INPUT = 0,
    SPIFI_DIRECTION_OUTPUT = 1
} HAL_SPIFI_DirectionTypeDef;

typedef struct __SPIFI_MemoryCommandTypeDef
{
    uint32_t InterimData;

    uint8_t InterimLength;

    HAL_SPIFI_FieldFormTypeDef FieldForm;

    HAL_SPIFI_FrameFormTypeDef FrameForm;

    uint8_t OpCode;

} SPIFI_MemoryCommandTypeDef;

typedef struct __SPIFI_MemoryModeConfig_HandleTypeDef
{

    SPIFI_CONFIG_TypeDef *Instance;

    HAL_SPIFI_CacheEnableTypeDef CacheEnable;

    uint32_t CacheLimit;

    SPIFI_MemoryCommandTypeDef Command;

} SPIFI_MemoryModeConfig_HandleTypeDef;

typedef struct __SPIFI_HandleTypeDef
{

    SPIFI_CONFIG_TypeDef *Instance;

    uint16_t timeout;

    uint8_t CS_High;

    HAL_SPIFI_CacheEnableTypeDef cacheEnabled;

    HAL_SPIFI_DataCacheEnableTypeDef dataCacheEnabled;

    HAL_SPIFI_InterruptEnableTypeDef interruptEnabled;

    HAL_SPIFI_Mode3EnableTypeDef mode3Enabled;

    uint8_t divider;

    HAL_SPIFI_PrefetchEnableTypeDef prefetchEnabled;

    HAL_SPIFI_DualModeEnableTypeDef dualModeEnabled;

    bool DMAEnabled;

} SPIFI_HandleTypeDef;

typedef struct __SPIFI_CommandTypeDef
{
    HAL_SPIFI_DirectionTypeDef Direction;

    uint32_t InterimData;

    uint8_t InterimLength;

    HAL_SPIFI_FieldFormTypeDef FieldForm;

    HAL_SPIFI_FrameFormTypeDef FrameForm;

    uint8_t OpCode;

} SPIFI_CommandTypeDef;

__attribute__((weak)) void HAL_SPIFI_MspInit(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_MemoryMode_Init(SPIFI_MemoryModeConfig_HandleTypeDef *spifi);

void HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *dataBytes);

bool HAL_SPIFI_IsMemoryModeEnabled(SPIFI_HandleTypeDef *spifi);

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsCommandProcessing(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_CMD_M) != 0;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitCommandProcessing(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if (!HAL_SPIFI_IsCommandProcessing(spifi))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

void HAL_SPIFI_Reset(SPIFI_HandleTypeDef *spifi);

bool HAL_SPIFI_IsReady(SPIFI_HandleTypeDef *spifi);

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitInterruptRequest(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if((spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) != 0)
        {
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitInterruptClear(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if((spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) == 0)
        {
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsInterruptRequest(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) != 0;
}

#endif // MIK32_HAL_SPIFI
