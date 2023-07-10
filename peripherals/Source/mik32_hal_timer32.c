#include "mik32_hal_timer32.h"

void HAL_Timer32_Init(Timer32_HandleTypeDef *timer)
{
    HAL_Timer32_DeInit(timer);

    HAL_Timer32_Top_Set(timer, timer->Top);
    HAL_Timer32_Clock_Set(timer, timer->Clock);
    HAL_Timer32_InterruptMask_Set(timer, timer->InterruptMask);
    HAL_Timer32_CountMode_Set(timer, timer->CountMode);
    HAL_Timer32_InterruptFlags_Clean(timer);
}

void HAL_Timer32_DeInit(Timer32_HandleTypeDef *timer)
{
    HAL_Timer32_InterruptMask_Set(timer, 0);
    Timer32_ClockConfigTypeDef default_clock_config = {0, 0};
    HAL_Timer32_Clock_Set(timer, default_clock_config);
    HAL_Timer32_InterruptFlags_Clean(timer);
    HAL_Timer32_CountMode_Set(timer, 0);
    HAL_Timer32_Top_Set(timer, 0);

    HAL_Timer32_Stop(timer);
    HAL_Timer32_Value_Clean(timer);
}

void HAL_Timer32_Stop(Timer32_HandleTypeDef *timer)
{
    timer->Enable = 0;
    timer->Instance->Enable &= ~TIMER32_ENABLE_M;
}

void HAL_Timer32_Start(Timer32_HandleTypeDef *timer)
{
    timer->Enable = 1;
    timer->Instance->Enable |= TIMER32_ENABLE_M;
}

void HAL_Timer32_Top_Set(Timer32_HandleTypeDef *timer, uint32_t top)
{
    timer->Top = top;
    timer->Instance->Top = top;
}

void HAL_Timer32_Clock_Set(Timer32_HandleTypeDef *timer, Timer32_ClockConfigTypeDef clockConfig)
{
    timer->Clock = clockConfig;
    timer->Instance->Prescaler = clockConfig.Prescaler;
    timer->Instance->Control = clockConfig.Source << TIMER32_CONTROL_CLOCK_S;
}

void HAL_Timer32_InterruptMask_Set(Timer32_HandleTypeDef *timer, uint32_t intMask)
{
    timer->InterruptMask = intMask;
    timer->Instance->IntMask = intMask;
}

void HAL_Timer32_CountMode_Set(Timer32_HandleTypeDef *timer, uint8_t countMode)
{
    timer->CountMode = countMode;
    timer->Instance->Control = countMode << TIMER32_CONTROL_MODE_S;
}

uint32_t HAL_Timer32_Value_Get(Timer32_HandleTypeDef *timer)
{
    return timer->Instance->Value;
}

void HAL_Timer32_Value_Clean(Timer32_HandleTypeDef *timer)
{
    timer->Instance->Enable = (timer->Instance->Enable & TIMER32_ENABLE_M) | TIMER32_RESET_VALUE_M;
}

uint32_t HAL_Timer32_InterruptFlags_Get(Timer32_HandleTypeDef *timer)
{
    return timer->Instance->IntFlags;
}

void HAL_Timer32_InterruptFlags_Clean(Timer32_HandleTypeDef *timer)
{
    timer->Instance->IntClear = 0xFFFFFFFF;
}

void HAL_Timer32_Channel_Init(Timer32_Channel_HandleTypeDef *timerChannel)
{
    HAL_Timer32_Channel_DeInit(timerChannel);

    HAL_Timer32_Channel_PWM_Invert_Set(timerChannel, timerChannel->PWM_Invert);
    HAL_Timer32_Channel_Mode_Set(timerChannel, timerChannel->Mode);
    HAL_Timer32_Channel_Edge_Set(timerChannel, timerChannel->Edge);
    HAL_Timer32_Channel_OCR_Set(timerChannel, timerChannel->OCR);
    HAL_Timer32_Channel_ICR_Clear(timerChannel);
    HAL_Timer32_Channel_Noise_Set(timerChannel, timerChannel->Noise);
}

void HAL_Timer32_Channel_DeInit(Timer32_Channel_HandleTypeDef *timerChannel)
{

}

void HAL_Timer32_Channel_Enable(Timer32_Channel_HandleTypeDef *timerChannel)
{
    timerChannel->Enable = 1;
    timerChannel->Instance->Control |= TIMER32_CH_CONTROL_ENABLE_M;
}

void HAL_Timer32_Channel_Disable(Timer32_Channel_HandleTypeDef *timerChannel)
{
    timerChannel->Enable = 0;
    timerChannel->Instance->Control &= ~TIMER32_CH_CONTROL_ENABLE_M;
}

void HAL_Timer32_Channel_PWM_Invert_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint8_t PWM_Invert)
{
    timerChannel->PWM_Invert = PWM_Invert;
    timerChannel->Instance->Control = PWM_Invert << TIMER32_CH_CONTROL_INVERTED_PWM_S;
}

void HAL_Timer32_Channel_Mode_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint8_t mode)
{
    timerChannel->Mode = mode;
    timerChannel->Instance->Control = mode << TIMER32_CH_CONTROL_MODE_S;
}

void HAL_Timer32_Channel_Edge_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint8_t edge)
{
    timerChannel->Edge = edge;
    timerChannel->Instance->Control = edge << TIMER32_CH_CONTROL_CAPTURE_EDGE_S;
}

void HAL_Timer32_Channel_OCR_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint32_t OCR)
{
    timerChannel->OCR = OCR;
    timerChannel->Instance->OCR = OCR;
}

void HAL_Timer32_Channel_ICR_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint32_t ICR)
{
    timerChannel->Instance->ICR = ICR;
}

void HAL_Timer32_Channel_ICR_Clear(Timer32_Channel_HandleTypeDef *timerChannel)
{
    timerChannel->Instance->ICR = 0;
}

void HAL_Timer32_Channel_ICR_Get(Timer32_Channel_HandleTypeDef *timerChannel)
{
    return timerChannel->Instance->ICR;
}

void HAL_Timer32_Channel_Noise_Set(Timer32_Channel_HandleTypeDef *timerChannel, uint8_t noise)
{
    timerChannel->Noise = noise;
    timerChannel->Instance->Control = noise << TIMER32_CH_CONTROL_NOISE_S;
}
