#ifndef MIK32_HAL_SPIFI
#define MIK32_HAL_SPIFI

#include <stdbool.h>
#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <spifi.h>
#include <mcu32_memory_map.h>

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

typedef enum __HAL_SPIFI_ClockEdgeTypeDef
{
    SPIFI_CLOCK_EDGE_RISING = 0,
    SPIFI_CLOCK_EDGE_FALLING = 1,
} HAL_SPIFI_ClockEdgeTypeDef;

typedef enum __HAL_SPIFI_FeedbackClockTypeDef
{
    SPIFI_FEEDBACK_CLOCK_INTERNAL = 0,
    SPIFI_FEEDBACK_CLOCK_EXTERNAL = 1,
} HAL_SPIFI_FeedbackClockTypeDef;

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
    uint32_t interimData;
    uint8_t interimLength;
    HAL_SPIFI_FieldFormTypeDef fieldForm;
    HAL_SPIFI_FrameFormTypeDef frameForm;
    uint8_t opCode;

} SPIFI_MemoryCommandTypeDef;

typedef struct __SPIFI_CommandTypeDef
{
    HAL_SPIFI_DirectionTypeDef direction;
    uint32_t interimData;
    uint8_t interimLength;
    HAL_SPIFI_FieldFormTypeDef fieldForm;
    HAL_SPIFI_FrameFormTypeDef frameForm;
    uint8_t opCode;

} SPIFI_CommandTypeDef;

typedef struct __SPIFI_InitTypeDef
{
    uint32_t divider;
    HAL_SPIFI_DualModeEnableTypeDef dualModeEnable;
    HAL_SPIFI_Mode3EnableTypeDef mode3Enable;
    HAL_SPIFI_ClockEdgeTypeDef clockEdge;
    HAL_SPIFI_FeedbackClockTypeDef feedbackClock;

    uint32_t timeout;
    uint32_t CS_Delay;
    
    HAL_SPIFI_InterruptEnableTypeDef interruptEnable;
    bool DMAEnable;

    HAL_SPIFI_CacheEnableTypeDef cacheEnable;
    HAL_SPIFI_DataCacheEnableTypeDef dataCacheEnable;
    HAL_SPIFI_PrefetchEnableTypeDef prefetchEnable;
    uint32_t cacheLimit;

} SPIFI_InitTypeDef;

typedef struct __SPIFI_HandleTypeDef
{
    SPIFI_CONFIG_TypeDef *Instance;

} SPIFI_HandleTypeDef;

__attribute__((weak)) void HAL_SPIFI_MspInit();

static inline __attribute__((always_inline)) void HAL_SPIFI_MspInit_LL()
{
    PAD_CONFIG->PORT_2_CFG = (PAD_CONFIG->PORT_2_CFG & 0xF000) | 0x555;
    PAD_CONFIG->PORT_2_PUPD = (PAD_CONFIG->PORT_2_CFG & 0xF000) | 0x550;
    __HAL_PCC_SPIFI_CLK_ENABLE();
}

void HAL_SPIFI_MemoryMode_Init(SPIFI_HandleTypeDef *spifi, SPIFI_InitTypeDef *init, SPIFI_MemoryCommandTypeDef *mcmd);
void HAL_SPIFI_Init(SPIFI_HandleTypeDef *spifi, SPIFI_InitTypeDef *init);
void HAL_SPIFI_Init_LL(SPIFI_HandleTypeDef *spifi, uint32_t ctrlReg, uint32_t climitReg);

void HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address);

HAL_StatusTypeDef HAL_SPIFI_Transmit(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data);

void HAL_SPIFI_SendCommand_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint32_t interimData);

HAL_StatusTypeDef HAL_SPIFI_Transmit_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdReg,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data,
    uint32_t interimData);

HAL_StatusTypeDef HAL_SPIFI_Receive_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data,
    uint32_t interimData);

HAL_StatusTypeDef HAL_SPIFI_ReceiveWords_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t dataLength,
    uint32_t *data,
    uint32_t interimData);

HAL_StatusTypeDef HAL_SPIFI_WaitCommandComplete_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmd,
    uint32_t timeout);

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsMemoryModeReady(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_MCINIT_M) != 0;
}

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsCommandProcessing(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_CMD_M) != 0;
}

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsReset(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_RESET_M) != 0;
}

static inline __attribute__((always_inline)) void HAL_SPIFI_Reset(SPIFI_HandleTypeDef *spifi)
{
    spifi->Instance->STAT = SPIFI_CONFIG_STAT_RESET_M;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitResetClear(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (HAL_SPIFI_IsReset(spifi) == false)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

static inline __attribute__((always_inline)) bool HAL_SPIFI_IsInterruptRequest(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_INTRQ_M) != 0;
}

static inline __attribute__((always_inline)) void HAL_SPIFI_ClearInterruptRequest(SPIFI_HandleTypeDef *spifi)
{
    spifi->Instance->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
}

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_WaitInterruptRequest(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (HAL_SPIFI_IsInterruptRequest(spifi))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

static inline __attribute__((always_inline)) bool HAL_SPIFI_Version(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_RESET_M) != 0;
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
