#include "mik32_hal_timer32.h"

__attribute__((weak)) void HAL_TIMER32_MspInit(TIMER32_HandleTypeDef* htimer32)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    

    if (htimer32->Instance == TIMER32_0)
    {
        __HAL_PCC_TIMER32_0_CLK_ENABLE();
    }

    if (htimer32->Instance == TIMER32_1)
    {
        __HAL_PCC_TIMER32_1_CLK_ENABLE();
        if (htimer32->Clock.Source == TIMER32_SOURCE_TX_PAD)
        {
            GPIO_InitStruct.Pin = PORT0_4;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }
    }

    if (htimer32->Instance == TIMER32_2)
    {
        __HAL_PCC_TIMER32_2_CLK_ENABLE();
        if (htimer32->Clock.Source == TIMER32_SOURCE_TX_PAD)
        {
            GPIO_InitStruct.Pin = PORT1_4;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        }
    }
}

__attribute__((weak)) void HAL_TIMER32_Channel_MspInit(TIMER32_CHANNEL_HandleTypeDef* timerChannel)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (timerChannel->TimerInstance == TIMER32_1)
    {
        GPIO_InitStruct.Pin = PORT0_0 | PORT0_1 | PORT0_2 | PORT0_3;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);

    }

    if (timerChannel->TimerInstance == TIMER32_2)
    {
        GPIO_InitStruct.Pin = PORT1_0 | PORT1_1 | PORT1_2 | PORT1_3;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
    }
    
}

HAL_StatusTypeDef HAL_Timer32_Init(TIMER32_HandleTypeDef *timer)
{
    if ((timer->Instance != TIMER32_0) && (timer->Instance != TIMER32_1) && (timer->Instance != TIMER32_2))
    {
        return HAL_ERROR;
    }

    HAL_TIMER32_MspInit(timer);

    HAL_Timer32_Top_Set(timer, timer->Top);
    HAL_Timer32_Prescaler_Set(timer, timer->Clock.Prescaler);
    HAL_Timer32_CountMode_Set(timer, timer->CountMode);
    HAL_Timer32_Source_Set(timer, timer->Clock.Source);
    HAL_Timer32_InterruptFlags_Clear(timer);
    HAL_Timer32_State_Set(timer, timer->State);
    HAL_Timer32_InterruptMask_Set(timer, 0);

    return HAL_OK;
}

void HAL_Timer32_State_Set(TIMER32_HandleTypeDef *timer, HAL_TIMER32_StateTypeDef state)
{
    timer->State = state;
    if (state == TIMER32_STATE_DISABLE)
    {
        timer->Instance->ENABLE &= ~TIMER32_ENABLE_TIM_EN_M;
    }
    else
    {
        timer->Instance->ENABLE |= TIMER32_ENABLE_TIM_EN_M;
    }
}

void HAL_Timer32_Top_Set(TIMER32_HandleTypeDef *timer, uint32_t top)
{
    timer->Top = top;
    timer->Instance->TOP = top;
}

void HAL_Timer32_Prescaler_Set(TIMER32_HandleTypeDef *timer, uint32_t prescaler)
{
    timer->Clock.Prescaler = prescaler;
    timer->Instance->PRESCALER = prescaler;
}

void HAL_Timer32_Source_Set(TIMER32_HandleTypeDef *timer, HAL_TIMER32_SourceTypeDef source)
{
    timer->Clock.Source = source;

    uint32_t timer_cfg_mux_tim32_s = 0;
    switch ((uint32_t)timer->Instance)
    {
    case (uint32_t)TIMER32_0:
        timer_cfg_mux_tim32_s = MUX_TIM32_0;
        break;
    case (uint32_t)TIMER32_1:
        timer_cfg_mux_tim32_s = MUX_TIM32_1;
        break;
    case (uint32_t)TIMER32_2:
        timer_cfg_mux_tim32_s = MUX_TIM32_2;
        break;

    default:
        break;
    }

    uint32_t timer_cfg_reg = PM->TIMER_CFG & (~PM_TIMER_CFG_MUX_TIMER_M(timer_cfg_mux_tim32_s));
    uint32_t timer_control_reg = timer->Instance->CONTROL & (~TIMER32_CONTROL_CLOCK_M);

    /* В начале настройки переключается на hclk */
    timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_PRESCALER_M;

    switch (source)
    {
    // case TIMER32_SOURCE_PRESCALER:
    //     timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_PRESCALER_M;
    //     break;
    case TIMER32_SOURCE_TIM1_SYS_CLK:

        timer_cfg_reg |= TIMER32_TIM1_SYS_CLK << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM1_M;
        break;
    case TIMER32_SOURCE_TIM1_HCLK:
        timer_cfg_reg |= TIMER32_TIM1_HCLK << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM1_M;
        break;
    case TIMER32_SOURCE_TIM2_OSC32K:
        timer_cfg_reg |= TIMER32_TIM2_OSC32K << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM2_M;
        break;
    case TIMER32_SOURCE_TIM2_LSI32K:
        timer_cfg_reg |= TIMER32_TIM2_LSI32K << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM2_M;
        break;
    case TIMER32_SOURCE_TX_PAD:
        timer->Instance->CONTROL = timer_control_reg | TIMER32_CONTROL_CLOCK_TX_PIN_M;
        break;

    default:
        break;
    }
}

void HAL_Timer32_InterruptMask_Set(TIMER32_HandleTypeDef *timer, uint32_t intMask)
{
    timer->InterruptMask |= intMask;
    timer->Instance->INT_MASK |= intMask;
}

void HAL_Timer32_InterruptMask_Clear(TIMER32_HandleTypeDef *timer, uint32_t intMask)
{
    timer->InterruptMask &= ~intMask;
    timer->Instance->INT_MASK &= ~intMask;
}

void HAL_Timer32_CountMode_Set(TIMER32_HandleTypeDef *timer, uint8_t countMode)
{
    timer->CountMode = countMode;

    uint32_t timer32_control_temp = timer->Instance->CONTROL;
    timer32_control_temp &= ~TIMER32_CONTROL_MODE_M;
    timer32_control_temp |= countMode << TIMER32_CONTROL_MODE_S;
    timer->Instance->CONTROL = timer32_control_temp;
}

void HAL_Timer32_InterruptFlags_ClearMask(TIMER32_HandleTypeDef *timer, uint32_t clearMask)
{
    timer->Instance->INT_CLEAR = clearMask;
}

HAL_StatusTypeDef HAL_Timer32_Channel_Init(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    HAL_TIMER32_Channel_MspInit(timerChannel);

    timerChannel->Instance = (TIMER32_CHANNEL_TypeDef *)&(timerChannel->TimerInstance->CHANNELS[timerChannel->ChannelIndex]);

    HAL_Timer32_Channel_PWM_Invert_Set(timerChannel, timerChannel->PWM_Invert);
    HAL_Timer32_Channel_Mode_Set(timerChannel, timerChannel->Mode);
    HAL_Timer32_Channel_CaptureEdge_Set(timerChannel, timerChannel->CaptureEdge);
    HAL_Timer32_Channel_OCR_Set(timerChannel, timerChannel->OCR);
    HAL_Timer32_Channel_ICR_Clear(timerChannel);
    HAL_Timer32_Channel_Noise_Set(timerChannel, timerChannel->Noise);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_DeInit(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    HAL_Timer32_Channel_Disable(timerChannel);
    HAL_Timer32_Channel_CaptureEdge_Set(timerChannel, 0);
    HAL_Timer32_Channel_ICR_Clear(timerChannel);
    HAL_Timer32_Channel_Mode_Set(timerChannel, 0);
    HAL_Timer32_Channel_Noise_Set(timerChannel, 0);
    HAL_Timer32_Channel_OCR_Set(timerChannel, 0);
    HAL_Timer32_Channel_PWM_Invert_Set(timerChannel, 0);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_Enable(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->State = TIMER32_STATE_ENABLE;
    timerChannel->Instance->CNTRL |= TIMER32_CH_CNTRL_ENABLE_M;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_Disable(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->State = TIMER32_STATE_DISABLE;
    timerChannel->Instance->CNTRL &= ~TIMER32_CH_CNTRL_ENABLE_M;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_PWM_Invert_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_PWMInvertTypeDef PWM_Invert)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->PWM_Invert = PWM_Invert;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->CNTRL & (~TIMER32_CH_CNTRL_INVERTED_PWM_M);
    timerChannel->Instance->CNTRL = timer32_channelcontrol_temp | (PWM_Invert << TIMER32_CH_CNTRL_INVERTED_PWM_S);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_Mode_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_ModeTypeDef mode)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->Mode = mode;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->CNTRL & (~TIMER32_CH_CNTRL_MODE_M);
    timerChannel->Instance->CNTRL = timer32_channelcontrol_temp | (mode << TIMER32_CH_CNTRL_MODE_S);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_CaptureEdge_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_CaptureEdgeTypeDef captureEdge)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->CaptureEdge = captureEdge;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->CNTRL & (~TIMER32_CH_CNTRL_CAPTURE_EDGE_M);
    timerChannel->Instance->CNTRL = timer32_channelcontrol_temp | (captureEdge << TIMER32_CH_CNTRL_CAPTURE_EDGE_S);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_OCR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t OCR)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->OCR = OCR;
    timerChannel->Instance->OCR = OCR;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_ICR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t ICR)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->Instance->ICR = ICR;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_ICR_Clear(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->Instance->ICR = 0;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_Timer32_Channel_Noise_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint8_t noise)
{
    if (timerChannel->TimerInstance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    timerChannel->Noise = noise;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->CNTRL & (~TIMER32_CH_CNTRL_NOISE_M);
    timerChannel->Instance->CNTRL = timer32_channelcontrol_temp | (noise << TIMER32_CH_CNTRL_NOISE_S);

    return HAL_OK;
}

void HAL_Timer32_Start(TIMER32_HandleTypeDef *timer)
{
    timer->State = TIMER32_STATE_ENABLE;
    timer->Instance->ENABLE |= TIMER32_ENABLE_TIM_EN_M;
}

void HAL_Timer32_Stop(TIMER32_HandleTypeDef *timer)
{
    timer->State = TIMER32_STATE_DISABLE;
    timer->Instance->ENABLE &= ~TIMER32_ENABLE_TIM_EN_M;
}

void HAL_Timer32_Base_Start_IT(TIMER32_HandleTypeDef *timer)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Set(timer, TIMER32_INT_OVERFLOW_M);

    HAL_Timer32_Start(timer);
}

void HAL_Timer32_Base_Stop_IT(TIMER32_HandleTypeDef *timer)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Clear(timer, TIMER32_INT_OVERFLOW_M);

    HAL_Timer32_Stop(timer);
}

HAL_StatusTypeDef HAL_Timer32_PWM_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timer->Instance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    if (timerChannel->Mode != TIMER32_CHANNEL_MODE_PWM)
    {
        return HAL_ERROR;
    }

    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Set(timer, TIMER32_INT_OVERFLOW_M);
    /* Включить канал */
    HAL_Timer32_Channel_Enable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Start(timer);

    return HAL_OK;
}

void HAL_Timer32_PWM_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Clear(timer, TIMER32_INT_OVERFLOW_M);
    /* Включить канал */
    HAL_Timer32_Channel_Disable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Stop(timer);
}

HAL_StatusTypeDef HAL_Timer32_Compare_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timer->Instance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    if (timerChannel->Mode != TIMER32_CHANNEL_MODE_COMPARE)
    {
        return HAL_ERROR;
    }

    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Set(timer, TIMER32_INT_OC_M(timerChannel->ChannelIndex));
    /* Включить канал */
    HAL_Timer32_Channel_Enable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Start(timer);

    return HAL_OK;
}

void HAL_Timer32_Compare_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Clear(timer, TIMER32_INT_OC_M(timerChannel->ChannelIndex));
    /* Включить канал */
    HAL_Timer32_Channel_Disable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Stop(timer);
}

HAL_StatusTypeDef HAL_Timer32_Capture_Start_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    if (timer->Instance == TIMER32_0)
    {
        return HAL_ERROR;
    }

    if (timerChannel->Mode != TIMER32_CHANNEL_MODE_CAPTURE)
    {
        return HAL_ERROR;
    }

    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Set(timer, TIMER32_INT_IC_M(timerChannel->ChannelIndex));
    /* Включить канал */
    HAL_Timer32_Channel_Enable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Start(timer);

    return HAL_OK;
}

void HAL_Timer32_Capture_Stop_IT(TIMER32_HandleTypeDef *timer, TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Clear(timer, TIMER32_INT_IC_M(timerChannel->ChannelIndex));
    /* Включить канал */
    HAL_Timer32_Channel_Disable(timerChannel);
    /* Запустить счет */
    HAL_Timer32_Stop(timer);
}

void HAL_Timer32_Start_IT(TIMER32_HandleTypeDef *timer, uint32_t intMask)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Set(timer, intMask);
    /* Запустить счет */
    HAL_Timer32_Start(timer);
}

void HAL_Timer32_Stop_IT(TIMER32_HandleTypeDef *timer, uint32_t intMask)
{
    /* Включить прерывание по переполнению */
    HAL_Timer32_InterruptMask_Clear(timer, intMask);
    /* Запустить счет */
    HAL_Timer32_Stop(timer);
}