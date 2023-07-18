#ifndef MIK32_HAL_SPIFI
#define MIK32_HAL_SPIFI

#include <mcu32_memory_map.h>
#include <spifi.h>

typedef enum
{
    SPIFI_CACHE_DISABLE = 0,
    SPIFI_CACHE_ENABLE = 1
} HAL_SPIFI_CacheEnableTypeDef;

typedef enum
{
    SPIFI_FIELDFORM_ALL_SERIAL = 0,
    SPIFI_FIELDFORM_DATA_PARALLEL = 1,
    SPIFI_FIELDFORM_COMMAND_SERIAL = 2,
    SPIFI_FIELDFORM_ALL_PARALLEL = 3
} HAL_SPIFI_FieldFormTypeDef;

typedef enum
{
    SPIFI_FRAMEFORM_OPCODE = 1,
    SPIFI_FRAMEFORM_OPCODE_1ADDR = 2,
    SPIFI_FRAMEFORM_OPCODE_2ADDR = 3,
    SPIFI_FRAMEFORM_OPCODE_3ADDR = 4,
    SPIFI_FRAMEFORM_OPCODE_4ADDR = 5,
    SPIFI_FRAMEFORM_3ADDR = 6,
    SPIFI_FRAMEFORM_4ADDR = 7
} HAL_SPIFI_FrameFormTypeDef;

typedef enum
{
    SPIFI_DIRECTION_INPUT = 0,
    SPIFI_DIRECTION_OUTPUT = 1
} HAL_SPIFI_DirectionTypeDef;

typedef struct
{
    uint32_t InterimData;
    
    uint8_t InterimLength;

    HAL_SPIFI_FieldFormTypeDef FieldForm;

    HAL_SPIFI_FrameFormTypeDef FrameForm;

    uint8_t OpCode;

} SPIFI_MemoryCommandTypeDef;

typedef struct
{

    SPIFI_CONFIG_TypeDef *Instance;

    HAL_SPIFI_CacheEnableTypeDef CacheEnable;

    uint32_t CacheLimit;

    SPIFI_MemoryCommandTypeDef Command;

} SPIFI_MemoryModeConfig_HandleTypeDef;

typedef struct
{

    SPIFI_CONFIG_TypeDef *Instance;

} SPIFI_HandleTypeDef;

typedef struct
{
    HAL_SPIFI_DirectionTypeDef Direction;

    uint8_t InterimLength;

    HAL_SPIFI_FieldFormTypeDef FieldForm;

    HAL_SPIFI_FrameFormTypeDef FrameForm;

    uint8_t OpCode;

} SPIFI_CommandTypeDef;

void HAL_SPIFI_MemoryMode_Init(SPIFI_MemoryModeConfig_HandleTypeDef *spifi);

void HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *dataBytes);

#endif // MIK32_HAL_SPIFI
