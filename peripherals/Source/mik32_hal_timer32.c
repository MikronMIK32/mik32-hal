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

void HAL_Timer32_DeInit(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Enable(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Disable(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Stop(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Start(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Top_Set(Timer32_HandleTypeDef *timer, uint32_t top);

void HAL_Timer32_Clock_Set(Timer32_HandleTypeDef *timer, Timer32_ClockConfigTypeDef clockConfig);

void HAL_Timer32_InterruptMask_Set(Timer32_HandleTypeDef *timer, uint32_t intMask);

void HAL_Timer32_CountMode_Set(Timer32_HandleTypeDef *timer, uint8_t countMode);

uint32_t HAL_Timer32_Value_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Value_Clean(Timer32_HandleTypeDef *timer);

uint32_t HAL_Timer32_InterruptFlags_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_InterruptFlags_Clean(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Channel_Init(Timer32_Channel_HandleTypeDef *timer)
{
    HAL_Timer32_Channel_DeInit(timer);

    HAL_Timer32_Channel_PWM_Invert_Set(timer, timer->PWM_Invert);
    HAL_Timer32_Channel_Mode_Set(timer, timer->Mode);
    HAL_Timer32_Channel_Edge_Set(timer, timer->Edge);
    HAL_Timer32_Channel_OCR_Set(timer, timer->OCR);
    HAL_Timer32_Channel_ICR_Set(timer, timer->ICR);
    HAL_Timer32_Channel_Noise_Set(timer, timer->Noise);
}

void HAL_Timer32_Channel_DeInit(Timer32_Channel_HandleTypeDef *timer);

void HAL_Timer32_Channel_PWM_Invert_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t PWM_Invert);

void HAL_Timer32_Channel_Mode_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t mode);

void HAL_Timer32_Channel_Edge_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t edge);

void HAL_Timer32_Channel_OCR_Set(Timer32_Channel_HandleTypeDef *timer, uint32_t OCR);

void HAL_Timer32_Channel_ICR_Set(Timer32_Channel_HandleTypeDef *timer, uint32_t ICR);

void HAL_Timer32_Channel_Noise_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t noise);
