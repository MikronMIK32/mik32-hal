#include "mik32_hal_timer32.h"

void HAL_Timer32_Init(TIMER32_HandleTypeDef *timer)
{
    if ((timer->Instance != TIMER32_0) && (timer->Instance != TIMER32_1) && (timer->Instance != TIMER32_2))
    {
        return;
    }
    HAL_Timer32_Top_Set(timer, timer->Top);
    HAL_Timer32_Prescaler_Set(timer, timer->Clock.Prescaler);
    HAL_Timer32_CountMode_Set(timer, timer->CountMode);
    HAL_Timer32_Source_Set(timer, timer->Clock.Source);
    HAL_Timer32_InterruptFlags_Clear(timer);
    HAL_Timer32_State_Set(timer, timer->State);
    HAL_Timer32_InterruptMask_Set(timer, timer->InterruptMask);
}

void HAL_Timer32_State_Set(TIMER32_HandleTypeDef *timer, HAL_TIMER32_StateTypeDef state)
{
    timer->State = state;
    if (state == TIMER32_STATE_DISABLE)
    {
        timer->Instance->Enable &= ~TIMER32_ENABLE_M;
    }
    else
    {
        timer->Instance->Enable |= TIMER32_ENABLE_M;
    }
}

void HAL_Timer32_Stop(TIMER32_HandleTypeDef *timer)
{
    timer->State = TIMER32_STATE_DISABLE;
    timer->Instance->Enable &= ~TIMER32_ENABLE_M;
}

void HAL_Timer32_Start(TIMER32_HandleTypeDef *timer)
{
    timer->State = TIMER32_STATE_ENABLE;
    timer->Instance->Enable |= TIMER32_ENABLE_M;
}

void HAL_Timer32_Top_Set(TIMER32_HandleTypeDef *timer, uint32_t top)
{
    timer->Top = top;
    timer->Instance->Top = top;
}

void HAL_Timer32_Prescaler_Set(TIMER32_HandleTypeDef *timer, uint32_t prescaler)
{
    timer->Clock.Prescaler = prescaler;
    timer->Instance->Prescaler = prescaler;
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

    uint32_t timer_cfg_reg = PM->TIMER_CFG & (~PM_TIMER_CFG_M(timer_cfg_mux_tim32_s));
    uint32_t timer_control_reg = timer->Instance->Control & (~TIMER32_CONTROL_CLOCK_M);

    /* В начале настройки переключается на hclk */
    timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_PRESCALER_M;

    switch (source)
    {
    // case TIMER32_SOURCE_PRESCALER:
    //     timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_PRESCALER_M;
    //     break;
    case TIMER32_SOURCE_TIM1_SYS_CLK:

        timer_cfg_reg |= TIMER32_TIM1_SYS_CLK << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM1_M;
        break;
    case TIMER32_SOURCE_TIM1_HCLK:
        timer_cfg_reg |= TIMER32_TIM1_HCLK << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM1_M;
        break;
    case TIMER32_SOURCE_TIM2_OSC32K:
        timer_cfg_reg |= TIMER32_TIM2_OSC32K << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM2_M;
        break;
    case TIMER32_SOURCE_TIM2_LSI32K:
        timer_cfg_reg |= TIMER32_TIM2_LSI32K << timer_cfg_mux_tim32_s;
        PM->TIMER_CFG = timer_cfg_reg;
        timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_TIM2_M;
        break;
    case TIMER32_SOURCE_TX_PAD:
        timer->Instance->Control = timer_control_reg | TIMER32_CONTROL_CLOCK_TX_PIN_M;
        break;

    default:
        break;
    }
}

void HAL_Timer32_InterruptMask_Set(TIMER32_HandleTypeDef *timer, uint32_t intMask)
{
    timer->InterruptMask = intMask;
    timer->Instance->IntMask = intMask;
}

void HAL_Timer32_CountMode_Set(TIMER32_HandleTypeDef *timer, uint8_t countMode)
{
    timer->CountMode = countMode;

    uint32_t timer32_control_temp = timer->Instance->Control;
    timer32_control_temp &= ~TIMER32_CONTROL_MODE_M;
    timer32_control_temp |= countMode << TIMER32_CONTROL_MODE_S;
    timer->Instance->Control = timer32_control_temp;
}

void HAL_Timer32_InterruptFlags_ClearMask(TIMER32_HandleTypeDef *timer, uint32_t clearMask)
{
    timer->Instance->IntClear = clearMask;
}

void HAL_Timer32_Channel_Init(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    timerChannel->Instance = (TIMER32_CHANNEL_TypeDef *)&(timerChannel->TimerInstance->Channels[timerChannel->ChannelIndex]);

    HAL_Timer32_Channel_PWM_Invert_Set(timerChannel, timerChannel->PWM_Invert);
    HAL_Timer32_Channel_Mode_Set(timerChannel, timerChannel->Mode);
    HAL_Timer32_Channel_CaptureEdge_Set(timerChannel, timerChannel->CaptureEdge);
    HAL_Timer32_Channel_OCR_Set(timerChannel, timerChannel->OCR);
    HAL_Timer32_Channel_ICR_Clear(timerChannel);
    HAL_Timer32_Channel_Noise_Set(timerChannel, timerChannel->Noise);
}

void HAL_Timer32_Channel_DeInit(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    HAL_Timer32_Channel_Disable(timerChannel);
    HAL_Timer32_Channel_CaptureEdge_Set(timerChannel, 0);
    HAL_Timer32_Channel_ICR_Clear(timerChannel);
    HAL_Timer32_Channel_Mode_Set(timerChannel, 0);
    HAL_Timer32_Channel_Noise_Set(timerChannel, 0);
    HAL_Timer32_Channel_OCR_Set(timerChannel, 0);
    HAL_Timer32_Channel_PWM_Invert_Set(timerChannel, 0);
}

void HAL_Timer32_Channel_Enable(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    timerChannel->State = TIMER32_STATE_ENABLE;
    timerChannel->Instance->Control |= TIMER32_CH_CONTROL_ENABLE_M;
}

void HAL_Timer32_Channel_Disable(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    timerChannel->State = TIMER32_STATE_DISABLE;
    timerChannel->Instance->Control &= ~TIMER32_CH_CONTROL_ENABLE_M;
}

void HAL_Timer32_Channel_PWM_Invert_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_PWMInvertTypeDef PWM_Invert)
{
    timerChannel->PWM_Invert = PWM_Invert;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->Control & (~TIMER32_CH_CONTROL_INVERTED_PWM_M);
    timerChannel->Instance->Control = timer32_channelcontrol_temp | (PWM_Invert << TIMER32_CH_CONTROL_INVERTED_PWM_S);
}

void HAL_Timer32_Channel_Mode_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_ModeTypeDef mode)
{
    timerChannel->Mode = mode;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->Control & (~TIMER32_CH_CONTROL_MODE_M);
    timerChannel->Instance->Control = timer32_channelcontrol_temp | (mode << TIMER32_CH_CONTROL_MODE_S);
}

void HAL_Timer32_Channel_CaptureEdge_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, HAL_TIMER32_CHANNEL_CaptureEdgeTypeDef captureEdge)
{
    timerChannel->CaptureEdge = captureEdge;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->Control & (~TIMER32_CH_CONTROL_CAPTURE_EDGE_M);
    timerChannel->Instance->Control = timer32_channelcontrol_temp | (captureEdge << TIMER32_CH_CONTROL_CAPTURE_EDGE_S);
}

void HAL_Timer32_Channel_OCR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t OCR)
{
    timerChannel->OCR = OCR;
    timerChannel->Instance->OCR = OCR;
}

void HAL_Timer32_Channel_ICR_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint32_t ICR)
{
    timerChannel->Instance->ICR = ICR;
}

void HAL_Timer32_Channel_ICR_Clear(TIMER32_CHANNEL_HandleTypeDef *timerChannel)
{
    timerChannel->Instance->ICR = 0;
}

void HAL_Timer32_Channel_Noise_Set(TIMER32_CHANNEL_HandleTypeDef *timerChannel, uint8_t noise)
{
    timerChannel->Noise = noise;

    uint32_t timer32_channelcontrol_temp = timerChannel->Instance->Control & (~TIMER32_CH_CONTROL_NOISE_M);
    timerChannel->Instance->Control = timer32_channelcontrol_temp | (noise << TIMER32_CH_CONTROL_NOISE_S);
}

