#ifndef MIK32_HAL_TIMER32
#define MIK32_HAL_TIMER32

#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <timer32.h>
#include <power_manager.h>
#include "mik32_hal_def.h"
#include <mcu32_memory_map.h>

#define TIMER32_TIMEOUT 10000000

#define TIMER32_INTERRUPT_CLEAR_MASK 0x3FF

/* Сдвиг настройки тактирования таймера в регистре Timer_CFG */
#define MUX_TIM32_0 0
#define MUX_TIM32_1 3
#define MUX_TIM32_2 6

/* Значение в регистре Timer_CFG при тактировании таймера от TIM1 или TIM2 */
#define TIMER32_TIM1_SYS_CLK 0b00
#define TIMER32_TIM1_HCLK 0b01
#define TIMER32_TIM2_OSC32K 0b00
#define TIMER32_TIM2_LSI32K 0b10

typedef enum
{
    TIMER32_SOURCE_PRESCALER,    /* Выход с делителя - частота шины AHB (hclk) */
    TIMER32_SOURCE_TIM1_SYS_CLK, /* Системная частота */
    TIMER32_SOURCE_TIM1_HCLK,    /* частота шины AHB */
    TIMER32_SOURCE_TIM2_OSC32K,  /* Внешний источник 32кГц */
    TIMER32_SOURCE_TIM2_LSI32K,  /* Внутренний источник 32кГц */
    TIMER32_SOURCE_TX_PAD,       /* Внешний вывод TX_PAD */
} HAL_TIMER32_SourceTypeDef;

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

typedef enum
{
    TIMER32_CHANNEL_STATE_DISABLE,
    TIMER32_CHANNEL_STATE_ENABLE,
} HAL_TIMER32_CHANNEL_StateTypeDef;

typedef enum
{
    TIMER32_CHANNEL_NON_INVERTED_PWM = 0,
    TIMER32_CHANNEL_INVERTED_PWM = 1
} HAL_TIMER32_CHANNEL_PWMInvertTypeDef;

typedef enum
{
    TIMER32_CHANNEL_MODE_COMPARE = 0b01, /* Режим сравнения */
    TIMER32_CHANNEL_MODE_CAPTURE = 0b10, /* Режим захвата */
    TIMER32_CHANNEL_MODE_PWM = 0b11,     /* ШИМ */
} HAL_TIMER32_CHANNEL_ModeTypeDef;

typedef enum
{
    TIMER32_CHANNEL_CAPTUREEDGE_RISING = 0,
    TIMER32_CHANNEL_CAPTUREEDGE_FALLING = 1
} HAL_TIMER32_CHANNEL_CaptureEdgeTypeDef;

typedef enum
{
    TIMER32_CHANNEL_FILTER_OFF = 0,
    TIMER32_CHANNEL_FILTER_ON = 1
} HAL_TIMER32_CHANNEL_NoiseTypeDef;

typedef enum
{
    TIMER32_CHANNEL_0 = 0,
    TIMER32_CHANNEL_1 = 1,
    TIMER32_CHANNEL_2 = 2,
    TIMER32_CHANNEL_3 = 3
} HAL_TIMER32_CHANNEL_IndexTypeDef;

typedef struct
{
    /* Источник тактирования таймера для счета*/
    HAL_TIMER32_SourceTypeDef Source;

    /* Делитель частоты. Частота делится на Prescaler + 1 */
    uint32_t Prescaler;

} TIMER32_ClockConfigTypeDef;

typedef struct
{
    TIMER32_TypeDef *Instance;

    /* Настройки тактирования */
    TIMER32_ClockConfigTypeDef Clock;

    /* Режим счета */
    HAL_TIMER32_CountModeTypeDef CountMode;

    /* Максимальное значение счета */
    uint32_t Top;

    HAL_TIMER32_StateTypeDef State;

    uint32_t InterruptMask;

} TIMER32_HandleTypeDef;

typedef struct
{
    TIMER32_TypeDef *TimerInstance;

    TIMER32_CHANNEL_TypeDef *Instance;

    HAL_TIMER32_CHANNEL_IndexTypeDef ChannelIndex;

    /* Инверсия ГИМ */
    HAL_TIMER32_CHANNEL_PWMInvertTypeDef PWM_Invert;

    /* Режим канала */
    HAL_TIMER32_CHANNEL_ModeTypeDef Mode;

    /* Активный фронт в режиме захвата*/
    HAL_TIMER32_CHANNEL_CaptureEdgeTypeDef CaptureEdge;

    /* Значение сравнения канала */
    uint32_t OCR;

    /* Фильтр */
    HAL_TIMER32_CHANNEL_NoiseTypeDef Noise;

    uint8_t State;

} TIMER32_CHANNEL_HandleTypeDef;


#define HAL_TIMER32_VALUE_GET(timer_instance) (*timer_instance).Instance->VALUE;
#define HAL_TIMER32_INTERRUPTFLAGS_CLEAR(timer_instance) (*timer_instance).Instance->INT_CLEAR = TIMER32_INTERRUPT_CLEAR_MASK;
#define HAL_TIMER32_VALUE_CLEAR(timer_instance) (*timer_instance).Instance->ENABLE |= TIMER32_ENABLE_TIM_CLR_M;

static inline __attribute__((always_inline)) uint32_t HAL_Timer32_Channel_ICR_Get(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    return timerChannel->Instance->ICR;
}
static inline __attribute__((always_inline)) uint32_t HAL_Timer32_InterruptFlags_Get(TIMER32_HandleTypeDef *timer)
{
    return timer->Instance->INT_FLAGS;
}
static inline __attribute__((always_inline)) uint32_t HAL_Timer32_Value_Get(TIMER32_HandleTypeDef *timer)
{
    return timer->Instance->VALUE;
}
static inline __attribute__((always_inline)) void HAL_Timer32_InterruptFlags_Clear(TIMER32_HandleTypeDef *timer)
{
    timer->Instance->INT_CLEAR = TIMER32_INTERRUPT_CLEAR_MASK;
}
static inline __attribute__((always_inline)) void HAL_Timer32_Value_Clear(TIMER32_HandleTypeDef *timer)
{
    timer->Instance->ENABLE |= TIMER32_ENABLE_TIM_CLR_M;
}
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_Timer32_Channel_WaitFlagCapture(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t timeout)
{
    while (timeout)
    {
        timeout--;
        if (timerChannel->TimerInstance->INT_FLAGS & TIMER32_INT_IC_M(timerChannel->ChannelIndex))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_Timer32_Channel_WaitFlagCompare(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t timeout)
{
    while (timeout)
    {
        timeout--;
        if (timerChannel->TimerInstance->INT_FLAGS & TIMER32_INT_OC_M(timerChannel->ChannelIndex))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_Timer32_WaitFlagOverflow(TIMER32_HandleTypeDef *timer, uint32_t timeout)
{
    while (timeout)
    {
        timeout--;
        if (timer->Instance->INT_FLAGS & TIMER32_INT_OVERFLOW_M)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_Timer32_WaitFlagUnderflow(TIMER32_HandleTypeDef *timer, uint32_t timeout)
{
    while (timeout)
    {
        timeout--;
        if (timer->Instance->INT_FLAGS & TIMER32_INT_UNDERFLOW_M)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

void HAL_TIMER32_MspInit(TIMER32_HandleTypeDef* htimer32);
void HAL_TIMER32_Channel_MspInit(TIMER32_CHANNEL_HandleTypeDef* timerChannel);
HAL_StatusTypeDef HAL_Timer32_Init(TIMER32_HandleTypeDef *timer);
void HAL_Timer32_State_Set(TIMER32_HandleTypeDef *timer, HAL_TIMER32_StateTypeDef state);
void HAL_Timer32_Top_Set(TIMER32_HandleTypeDef *timer, uint32_t top);
void HAL_Timer32_Prescaler_Set(TIMER32_HandleTypeDef *timer, uint32_t prescaler);
void HAL_Timer32_Source_Set(TIMER32_HandleTypeDef *timer, HAL_TIMER32_SourceTypeDef source);
void HAL_Timer32_InterruptMask_Set(TIMER32_HandleTypeDef *timer, uint32_t intMask);
void HAL_Timer32_InterruptMask_Clear(TIMER32_HandleTypeDef *timer, uint32_t intMask);
void HAL_Timer32_CountMode_Set(TIMER32_HandleTypeDef *timer, uint8_t countMode);
void HAL_Timer32_InterruptFlags_ClearMask(TIMER32_HandleTypeDef *timer, uint32_t clearMask);
HAL_StatusTypeDef HAL_Timer32_Channel_Init(TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Channel_DeInit(TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Channel_Enable(TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Channel_Disable(TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Channel_PWM_Invert_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_PWMInvertTypeDef PWM_Invert);
HAL_StatusTypeDef HAL_Timer32_Channel_Mode_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_ModeTypeDef mode);
HAL_StatusTypeDef HAL_Timer32_Channel_CaptureEdge_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_CaptureEdgeTypeDef captureEdge);
HAL_StatusTypeDef HAL_Timer32_Channel_OCR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t OCR);
HAL_StatusTypeDef HAL_Timer32_Channel_ICR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t ICR);
HAL_StatusTypeDef HAL_Timer32_Channel_ICR_Clear(TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Channel_Noise_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint8_t noise);
void HAL_Timer32_Start(TIMER32_HandleTypeDef *timer);
void HAL_Timer32_Stop(TIMER32_HandleTypeDef *timer);
void HAL_Timer32_Base_Start_IT(TIMER32_HandleTypeDef *timer);
void HAL_Timer32_Base_Stop_IT(TIMER32_HandleTypeDef *timer);
HAL_StatusTypeDef HAL_Timer32_PWM_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
void HAL_Timer32_PWM_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Compare_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
void HAL_Timer32_Compare_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
HAL_StatusTypeDef HAL_Timer32_Capture_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
void HAL_Timer32_Capture_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel);
void HAL_Timer32_Start_IT(TIMER32_HandleTypeDef *timer, uint32_t intMask);
void HAL_Timer32_Stop_IT(TIMER32_HandleTypeDef *timer, uint32_t intMask);

#endif // MIK32_HAL_TIMER32
