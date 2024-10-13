#ifndef MIK32_HAL_SPIFI
#define MIK32_HAL_SPIFI

#include <stdbool.h>
#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <spifi.h>
#include <mik32_memory_map.h>

#define HAL_SPIFI_TIMEOUT 100000

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
    SPIFI_FIELDFORM_ALL_SERIAL = SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL,
    SPIFI_FIELDFORM_DATA_PARALLEL = SPIFI_CONFIG_CMD_FIELDFORM_DATA_PARALLEL,
    SPIFI_FIELDFORM_OPCODE_SERIAL = SPIFI_CONFIG_CMD_FIELDFORM_OPCODE_SERIAL,
    SPIFI_FIELDFORM_ALL_PARALLEL = SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL,
} HAL_SPIFI_FieldFormTypeDef;

typedef enum __HAL_SPIFI_FrameFormTypeDef
{
    SPIFI_FRAMEFORM_OPCODE = SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR,
    SPIFI_FRAMEFORM_OPCODE_1ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_1ADDR,
    SPIFI_FRAMEFORM_OPCODE_2ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_2ADDR,
    SPIFI_FRAMEFORM_OPCODE_3ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR,
    SPIFI_FRAMEFORM_OPCODE_4ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_4ADDR,
    SPIFI_FRAMEFORM_3ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_3ADDR,
    SPIFI_FRAMEFORM_4ADDR = SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_4ADDR,
} HAL_SPIFI_FrameFormTypeDef;

typedef enum __HAL_SPIFI_DirectionTypeDef
{
    SPIFI_DIRECTION_INPUT = SPIFI_CONFIG_CMD_DOUT_M ^ (1 << SPIFI_CONFIG_CMD_DOUT_S),
    SPIFI_DIRECTION_OUTPUT = SPIFI_CONFIG_CMD_DOUT_M
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

__attribute__((weak)) void HAL_SPIFI_MspInit();

static inline __attribute__((always_inline)) void HAL_SPIFI_MspInit_LL()
{
    PAD_CONFIG->PORT_2_CFG = (PAD_CONFIG->PORT_2_CFG & 0xF000) | 0x555;
    PAD_CONFIG->PORT_2_PUPD = (PAD_CONFIG->PORT_2_CFG & 0xF000) | 0x550;
    __HAL_PCC_SPIFI_CLK_ENABLE();
}

void HAL_SPIFI_MemoryMode_Init(SPIFI_MemoryModeConfig_HandleTypeDef *spifi);

HAL_StatusTypeDef HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t bufferSize,
    uint8_t *readBuffer,
    uint8_t *writeBuffer,
    uint32_t timeout);

HAL_StatusTypeDef HAL_SPIFI_SendCommand_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t bufferSize,
    uint8_t *readBuffer,
    uint8_t *writeBuffer,
    uint32_t interimData,
    uint32_t timeout);

bool HAL_SPIFI_IsMemoryModeEnabled(SPIFI_HandleTypeDef *spifi);

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsCommandCompleted(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) != 0;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitCommandProcessing(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (HAL_SPIFI_IsCommandCompleted(spifi))
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
        if ((spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) != 0)
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
        if ((spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) == 0)
        {
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitResetClear(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if ((spifi->Instance->STAT & SPIFI_CONFIG_STAT_RESET_M) == 0)
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

static inline __attribute__((always_inline)) void HAL_SPIFI_MemoryMode_Init_LL(SPIFI_HandleTypeDef *spifi, uint32_t ctrlReg, uint32_t climitReg, uint32_t mcmdReg)
{
    HAL_SPIFI_Reset(spifi);
    HAL_SPIFI_WaitResetClear(spifi, HAL_SPIFI_TIMEOUT);

    SPIFI_CONFIG->CLIMIT = climitReg;
    SPIFI_CONFIG->CTRL = ctrlReg;
    SPIFI_CONFIG->MCMD = mcmdReg;
}

#endif // MIK32_HAL_SPIFI
