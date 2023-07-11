#ifndef MIK32_HAL_TIMER32
#define MIK32_HAL_TIMER32

#include <mcu32_memory_map.h>
#include <timer32.h>

#define TIMER32_INTERRUPT_CLEAR_MASK 0x3FF

typedef enum
{
    TIMER32_SOURCE_PRESCALER = 0b00,
    TIMER32_SOURCE_TIM1 = 0b01,
    TIMER32_SOURCE_TX_PAD = 0b10,
    TIMER32_SOURCE_TIM2 = 0b11
} HAL_TIMER32_SourceTypeDef;

typedef struct
{
    HAL_TIMER32_SourceTypeDef Source;

    uint32_t Prescaler;

} Timer32_ClockConfigTypeDef;

typedef enum
{
    TIMER32_COUNTMODE_FORWARD = 0b00,
    TIMER32_COUNTMODE_REVERSE = 0b01,
    TIMER32_COUNTMODE_BIDIRECTIONAL = 0b10
} HAL_TIMER32_CountModeTypeDef;

typedef enum
{
    TIMER32_STATE_DISABLE = 0b00,
    TIMER32_STATE_ENABLE = 0b01
} HAL_TIMER32_StateTypeDef;

typedef struct
{
    uint32_t Top;

    HAL_TIMER32_StateTypeDef State;

    TIMER32_TypeDef *Instance;

    Timer32_ClockConfigTypeDef Clock;

    uint32_t InterruptMask;

    HAL_TIMER32_CountModeTypeDef CountMode;

} Timer32_HandleTypeDef;

typedef struct
{
    TIMER32_CHANNEL_TypeDef *Instance;

    uint8_t Enable;

    uint8_t PWM_Invert;

    uint8_t Mode;

    uint8_t Edge;

    uint32_t OCR;

    uint8_t Noise;

} Timer32_Channel_HandleTypeDef;

void HAL_Timer32_Init(Timer32_HandleTypeDef *timer);

void HAL_Timer32_State_Set(Timer32_HandleTypeDef *timer, HAL_TIMER32_StateTypeDef state);

void HAL_Timer32_Stop(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Start(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Top_Set(Timer32_HandleTypeDef *timer, uint32_t top);

void HAL_Timer32_Clock_Set(Timer32_HandleTypeDef *timer, Timer32_ClockConfigTypeDef clockConfig);

void HAL_Timer32_InterruptMask_Set(Timer32_HandleTypeDef *timer, uint32_t intMask);

void HAL_Timer32_CountMode_Set(Timer32_HandleTypeDef *timer, uint8_t countMode);

uint32_t HAL_Timer32_Value_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Value_Clear(Timer32_HandleTypeDef *timer);

uint32_t HAL_Timer32_InterruptFlags_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_InterruptFlags_ClearMask(Timer32_HandleTypeDef *timer, uint32_t clearMask);

void HAL_Timer32_InterruptFlags_Clear(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Channel_Init(Timer32_Channel_HandleTypeDef *timer);

void HAL_Timer32_Channel_PWM_Invert_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t PWM_Invert);

void HAL_Timer32_Channel_Mode_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t mode);

void HAL_Timer32_Channel_Edge_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t edge);

void HAL_Timer32_Channel_OCR_Set(Timer32_Channel_HandleTypeDef *timer, uint32_t OCR);

void HAL_Timer32_Channel_ICR_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint32_t ICR);

void HAL_Timer32_Channel_ICR_Clear(Timer32_Channel_HandleTypeDef *timerChannel);

void HAL_Timer32_Channel_ICR_Get(Timer32_Channel_HandleTypeDef *timerChannel);

void HAL_Timer32_Channel_Noise_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t noise);

#endif // MIK32_HAL_TIMER32
