#include "mik32_hal_timer32.h"

void HAL_Timer32_Stop(TIMER32_TypeDef *timer)
{
    timer->Enable = TIMER32_DISABLE_M;
    timer->IntClear = 0xFFFFFFFF;
}

void HAL_Timer32_Init(TIMER32_TypeDef *timer, uint32_t top, uint32_t prescaler, uint32_t control, uint32_t intmask)
{
    HAL_Timer32_Stop(timer);

    timer->Top = top;
    timer->Prescaler = prescaler;
    timer->Control = control;
    timer->IntMask = intmask;
}

void HAL_Timer32_DeInit(TIMER32_TypeDef *timer)
{
    // timer->Enable = TIMER32_RESET_VALUE_M | TIMER32_ENABLE_M;
    timer->IntMask = 0;
    timer->Prescaler = 0;
    timer->IntClear = 0xFFFFFFFF;
    timer->Enable = TIMER32_RESET_VALUE_M;
}

void HAL_Timer32_Start(TIMER32_TypeDef *timer)
{
    timer->Enable = TIMER32_ENABLE_M | TIMER32_RESET_VALUE_M;
}

void HAL_Timer32_Delay(TIMER32_TypeDef *timer, uint32_t delay_ms)
{
    HAL_Timer32_Init(timer, delay_ms * 32000, 0, TIMER32_CONTROL_MODE_UP_M, 1);
    HAL_Timer32_Start(timer);
    while ((timer->IntFlags && 1) == 0)
    {
    }
    HAL_Timer32_DeInit(timer);
}

void HAL_Timer32_DelayUs(TIMER32_TypeDef *timer, uint32_t delay_us)
{
    HAL_Timer32_Init(timer, delay_us * 32, 0, TIMER32_CONTROL_MODE_UP_M, 1);
    HAL_Timer32_Start(timer);
    while ((timer->IntFlags && 1) == 0)
    {
    }
    HAL_Timer32_DeInit(timer);
}
