#include "mik32_hal_timer16.h"

__attribute__((weak)) void HAL_TIMER16_MspInit(Timer16_HandleTypeDef* htimer16)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Вывод ШИМ не переводится в нужный режим */

    if (htimer16->Instance == TIMER16_0)
    {
        __HAL_PCC_TIMER16_0_CLK_ENABLE();
        
        if ((htimer16->Clock.Source == TIMER16_SOURCE_EXTERNAL_INPUT1) || (htimer16->CountMode == TIMER16_COUNTMODE_EXTERNAL))
        {
            GPIO_InitStruct.Pin = PORT0_5;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_5 | PORT0_6;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_7;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }
        
        
    }

    if (htimer16->Instance == TIMER16_1)
    {
        __HAL_PCC_TIMER16_1_CLK_ENABLE();
        
        if ((htimer16->Clock.Source == TIMER16_SOURCE_EXTERNAL_INPUT1) || (htimer16->CountMode == TIMER16_COUNTMODE_EXTERNAL))
        {
            GPIO_InitStruct.Pin = PORT0_8;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_8 | PORT0_9;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_10;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }
    }

    if (htimer16->Instance == TIMER16_2)
    {
        __HAL_PCC_TIMER16_2_CLK_ENABLE();

        if ((htimer16->Clock.Source == TIMER16_SOURCE_EXTERNAL_INPUT1) || (htimer16->CountMode == TIMER16_COUNTMODE_EXTERNAL))
        {
            GPIO_InitStruct.Pin = PORT0_11;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_11 | PORT0_12;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = PORT0_13;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }
    }
}

void HAL_Timer16_Disable(Timer16_HandleTypeDef *htimer16)
{
    htimer16->Instance->CR &= ~TIMER16_CR_ENABLE_M;
}

void HAL_Timer16_Enable(Timer16_HandleTypeDef *htimer16)
{
    htimer16->Instance->CR |= TIMER16_CR_ENABLE_M;
}

void HAL_Timer16_SetActiveEdge(Timer16_HandleTypeDef *htimer16, uint8_t ActiveEdge)
{
    htimer16->ActiveEdge = ActiveEdge;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_CKPOL_M;
    CFGRConfig |= ActiveEdge << TIMER16_CFGR_CKPOL_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_SetSourceClock(Timer16_HandleTypeDef *htimer16, uint8_t SourceClock)
{
    htimer16->Clock.Source = SourceClock;

    HAL_Timer16_Disable(htimer16);

    if(SourceClock == TIMER16_SOURCE_EXTERNAL_INPUT1)    /* Внешний источник Input1 */
    {
        htimer16->Instance->CFGR |= TIMER16_CFGR_CKSEL_M;  

        HAL_Timer16_SetActiveEdge(htimer16, htimer16->ActiveEdge);  /* Настройка активного фронта при тактировании от внешнего источника  */
    }
    else    /* Внутренний источник */
    {
        /* Настройка источника тактирования таймера в PM(Power Manager) */
        uint32_t CFGConfig = PM->TIMER_CFG;
        if(htimer16->Instance == TIMER16_0)
        {
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(MUX_TIM16_0);
            CFGConfig |= SourceClock << MUX_TIM16_0;
        }
        else if (htimer16->Instance == TIMER16_1)
        {
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(MUX_TIM16_1);
            CFGConfig |= SourceClock << MUX_TIM16_1;
        }
        else if (htimer16->Instance == TIMER16_2)
        {
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(MUX_TIM16_2);
            CFGConfig |= SourceClock << MUX_TIM16_2;
        }
        /* Установка выбранного источника тактирования таймера в PM */
        PM->TIMER_CFG = CFGConfig;
        
        htimer16->Instance->CFGR &= ~TIMER16_CFGR_CKSEL_M;  /* Внутренний источник */
    }

}

void HAL_Timer16_SetCountMode(Timer16_HandleTypeDef *htimer16, uint8_t CountMode)
{
    htimer16->CountMode = CountMode;

    HAL_Timer16_Disable(htimer16);
    
    switch (CountMode)
    {
    case TIMER16_COUNTMODE_INTERNAL:
        htimer16->Instance->CFGR &= ~TIMER16_CFGR_COUNTMODE_M;
        break;
    case TIMER16_COUNTMODE_EXTERNAL:
        htimer16->Instance->CFGR |= TIMER16_CFGR_COUNTMODE_M;
        break;
    }
        
}

void HAL_Timer16_ClockInit(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_SetSourceClock(htimer16, htimer16->Clock.Source);
    HAL_Timer16_SetCountMode(htimer16, htimer16->CountMode);
}

void HAL_Timer16_SetPreload(Timer16_HandleTypeDef *htimer16, uint8_t Preload)
{
    htimer16->Preload = Preload;

    HAL_Timer16_Disable(htimer16);

    switch (Preload)
    {
    case TIMER16_PRELOAD_AFTERWRITE:
        htimer16->Instance->CFGR &= ~TIMER16_CFGR_PRELOAD_M;
        break;
    case TIMER16_PRELOAD_ENDPERIOD:
        htimer16->Instance->CFGR |= TIMER16_CFGR_PRELOAD_M;
        break;
    }
}

void HAL_Timer16_WaitARROK(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_ARR_OK_M));
    HAL_Timer16_ClearInterruptFlag(htimer16, TIMER16_ARROK_IRQ);
    //while (htimer16->Instance->ISR & TIMER16_ISR_ARR_OK_M);
}

void HAL_Timer16_WaitCMPOK(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_CMP_OK_M));
    HAL_Timer16_ClearInterruptFlag(htimer16, TIMER16_CMPOK_IRQ);
    //while (htimer16->Instance->ISR & TIMER16_ISR_CMP_OK_M);
}

void HAL_Timer16_SetARR(Timer16_HandleTypeDef *htimer16, uint16_t Period)
{
    // htimer16->Period = Period;

    /* Выключение таймера для записи ARR */
    htimer16->Instance->ARR = Period;
    if (!(htimer16->Instance->IER & TIMER16_IER_ARROKIE_M))
    {
        HAL_Timer16_WaitARROK(htimer16);
    }
}

void HAL_Timer16_SetCMP(Timer16_HandleTypeDef *htimer16, uint16_t Compare)
{
    /* Выключение таймера для записи CMP */
    htimer16->Instance->CMP = Compare;

    if (!(htimer16->Instance->IER & TIMER16_IER_CMPOKIE_M))
    {
        HAL_Timer16_WaitCMPOK(htimer16);
    }  
}

void HAL_Timer16_SelectTrigger(Timer16_HandleTypeDef *htimer16, uint8_t TriggerSource)
{
    htimer16->Trigger.Source = TriggerSource;

    /* Выключение таймера для записи CFGR */
    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;

    CFGRConfig &= ~TIMER16_CFGR_TRIGSEL_M;
    CFGRConfig |= TriggerSource << TIMER16_CFGR_TRIGSEL_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_SetTriggerEdge(Timer16_HandleTypeDef *htimer16, uint8_t TriggerEdge)
{
    htimer16->Trigger.ActiveEdge = TriggerEdge;

    /* Выключение таймера для записи CFGR */
    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;

    CFGRConfig &= ~TIMER16_CFGR_TRIGEN_M;
    CFGRConfig |= TriggerEdge << TIMER16_CFGR_TRIGEN_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_SetTimeOut(Timer16_HandleTypeDef *htimer16, uint8_t TimeOut)
{
    htimer16->Trigger.TimeOut = TimeOut;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_TIMOUT_M;
    CFGRConfig |= TimeOut << TIMER16_CFGR_TIMOUT_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_SetFilterExternalClock(Timer16_HandleTypeDef *htimer16, uint8_t FilterExternalClock)
{
    htimer16->Filter.ExternalClock = FilterExternalClock;

    /* Выключение таймера для записи в CFGR */
    HAL_Timer16_Disable(htimer16);

    htimer16->Instance->CFGR &= ~TIMER16_CFGR_CKFLT_M;
    htimer16->Instance->CFGR |= FilterExternalClock << TIMER16_CFGR_CKFLT_S;
}

void HAL_Timer16_SetFilterTrigger(Timer16_HandleTypeDef *htimer16, uint8_t FilterTrigger)
{
    htimer16->Filter.Trigger = FilterTrigger;

    /* Выключение таймера для записи в CFGR */
    HAL_Timer16_Disable(htimer16);

    htimer16->Instance->CFGR &= ~TIMER16_CFGR_TRGFLT_M;
    htimer16->Instance->CFGR |= FilterTrigger << TIMER16_CFGR_TRGFLT_S;
}

void HAL_Timer16_SetEncoderMode(Timer16_HandleTypeDef *htimer16, uint8_t EncoderMode)
{
    htimer16->EncoderMode = EncoderMode;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_ENC_M;
    CFGRConfig |= EncoderMode << TIMER16_CFGR_ENC_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_WaveformPolarity(Timer16_HandleTypeDef *htimer16, HAL_Timer16_WaveformPolarityTypeDef WaveformPolarity)
{
    WaveformPolarity &= TIMER16_CFGR_WAVPOL_M;
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR = (htimer16->Instance->CFGR & (~TIMER16_CFGR_WAVPOL_M)) | WaveformPolarity;
}

void HAL_Timer16_SetPrescaler(Timer16_HandleTypeDef *htimer16, uint8_t Prescaler)
{
    htimer16->Clock.Prescaler = Prescaler;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_PRESC_M;
    CFGRConfig |= Prescaler << TIMER16_CFGR_PRESC_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

void HAL_Timer16_Init(Timer16_HandleTypeDef *htimer16)
{
    HAL_TIMER16_MspInit(htimer16);
    
    HAL_Timer16_Disable(htimer16);
    /* Настройка внутреннего/внешнего источника тактирования */
    HAL_Timer16_ClockInit(htimer16);

    HAL_Timer16_SetPrescaler(htimer16, htimer16->Clock.Prescaler);

    HAL_Timer16_SetFilterExternalClock(htimer16, htimer16->Filter.ExternalClock);
    HAL_Timer16_SetFilterTrigger(htimer16, htimer16->Filter.Trigger);

    /*********************************************************/
    HAL_Timer16_SetPreload(htimer16, htimer16->Preload);

    /* Настройка триггера */
    HAL_Timer16_SelectTrigger(htimer16, htimer16->Trigger.Source);
    HAL_Timer16_SetTriggerEdge(htimer16, htimer16->Trigger.ActiveEdge);
    HAL_Timer16_SetTimeOut(htimer16, htimer16->Trigger.TimeOut);
    /*********************************************************/

    // HAL_Timer16_SetEncoderMode(htimer16, htimer16->EncoderMode);

    HAL_Timer16_WaveformPolarity(htimer16, htimer16->Waveform.Polarity);

    // HAL_Timer16_Enable(htimer16);

    // /* Верхний предел счета */
    // HAL_Timer16_SetARR(htimer16, htimer16->Period);

}

uint16_t HAL_Timer16_GetCounterValue(Timer16_HandleTypeDef *htimer16)
{
    return htimer16->Instance->CNT;
}

uint8_t HAL_Timer16_CheckCMP(Timer16_HandleTypeDef *htimer16)
{
    if ((htimer16->Instance->ISR & TIMER16_ISR_CMP_MATCH_M) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void HAL_Timer16_WaitCMP(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_CMP_MATCH_M));
}

void HAL_Timer16_Counter_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Enable(htimer16);

    HAL_Timer16_SetARR(htimer16, Period);
    
    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

void HAL_Timer16_StartPWM(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_CONTINUOUS(htimer16);

}

void HAL_Timer16_StartOneShot(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_SINGLE(htimer16);

}

void HAL_Timer16_StartSetOnes(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR |= TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_SINGLE(htimer16);
}

void HAL_Timer16_Encoder_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_ENABLE);

    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_DOWNCF_M | TIMER16_ICR_UPCF_M;
    HAL_Timer16_SetARR(htimer16, Period);

    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

void HAL_Timer16_Encoder_Stop(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_DISABLE);
}

void HAL_Timer16_Stop(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);
}

void HAL_Timer16_Counter_Start_IT(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_ARRMCF_M;
    
    HAL_Timer16_SetARR(htimer16, Period);

    htimer16->Instance->IER |= TIMER16_IER_ARROKIE_M | TIMER16_IER_ARRMIE_M;

    
    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

void HAL_Timer16_StartPWM_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{


    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    

    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    htimer16->Instance->IER |= TIMER16_IER_ARROKIE_M | TIMER16_IER_CMPOKIE_M 
                            | TIMER16_IER_ARRMIE_M | TIMER16_IER_CMPMIE_M;

    if (htimer16->Trigger.ActiveEdge != TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE)
    {
        htimer16->Instance->IER |= TIMER16_IER_EXTTRIGIE_M;
    }

    __HAL_TIMER16_START_CONTINUOUS(htimer16);

}

void HAL_Timer16_StartOneShot_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    htimer16->Instance->IER |= TIMER16_IER_ARROKIE_M | TIMER16_IER_CMPOKIE_M 
                            | TIMER16_IER_ARRMIE_M | TIMER16_IER_CMPMIE_M;

    if (htimer16->Trigger.ActiveEdge != TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE)
    {
        htimer16->Instance->IER |= TIMER16_IER_EXTTRIGIE_M;
    }

    __HAL_TIMER16_START_SINGLE(htimer16);
}

void HAL_Timer16_StartSetOnes_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR |= TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    
    if(Period > Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    htimer16->Instance->IER |= TIMER16_IER_ARROKIE_M | TIMER16_IER_CMPOKIE_M 
                            | TIMER16_IER_ARRMIE_M | TIMER16_IER_CMPMIE_M;

    if (htimer16->Trigger.ActiveEdge != TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE)
    {
        htimer16->Instance->IER |= TIMER16_IER_EXTTRIGIE_M;
    }

    __HAL_TIMER16_START_SINGLE(htimer16);
}

void HAL_Timer16_Encoder_Start_IT(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_ENABLE);

    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_DOWNCF_M | TIMER16_ICR_UPCF_M;
    
    HAL_Timer16_SetARR(htimer16, Period);

    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->IER |= TIMER16_IER_DOWNIE_M | TIMER16_IER_UPIE_M;

    HAL_Timer16_Enable(htimer16);
    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

void HAL_Timer16_Encoder_Stop_IT(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_DISABLE);
    htimer16->Instance->IER &= ~(TIMER16_IER_DOWNIE_M | TIMER16_IER_UPIE_M);
}

void HAL_Timer16_Stop_IT(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);

    htimer16->Instance->IER &= ~(TIMER16_IER_ARROKIE_M | TIMER16_IER_CMPOKIE_M 
                            | TIMER16_IER_ARRMIE_M | TIMER16_IER_CMPMIE_M);

    if (htimer16->Trigger.ActiveEdge != TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE)
    {
        htimer16->Instance->IER &= ~TIMER16_IER_EXTTRIGIE_M;
    }

}

void HAL_Timer16_WaitTrigger(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_EXT_TRIG_M));
    HAL_Timer16_ClearInterruptFlag(htimer16, TIMER16_EXTTRIG_IRQ);
}

void HAL_Timer16_SetInterruptMask(Timer16_HandleTypeDef *htimer16, uint32_t InterruptMask)
{
    htimer16->Instance->IER = InterruptMask;

    htimer16->Interrupts.DOWN = (InterruptMask & TIMER16_IER_DOWNIE_M) >> TIMER16_IER_DOWNIE_S;
    htimer16->Interrupts.UP = (InterruptMask & TIMER16_IER_UPIE_M) >> TIMER16_IER_UPIE_S;
    htimer16->Interrupts.ARROK = (InterruptMask & TIMER16_IER_ARROKIE_M) >> TIMER16_IER_ARROKIE_S;
    htimer16->Interrupts.CMPOK = (InterruptMask & TIMER16_IER_CMPOKIE_M) >> TIMER16_IER_CMPOKIE_S;
    htimer16->Interrupts.EXTTRIG = (InterruptMask & TIMER16_IER_EXTTRIGIE_M) >> TIMER16_IER_EXTTRIGIE_S;
    htimer16->Interrupts.ARRM = (InterruptMask & TIMER16_IER_ARRMIE_M) >> TIMER16_IER_ARRMIE_S;
    htimer16->Interrupts.CMPM = (InterruptMask & TIMER16_IER_CMPMIE_M) >> TIMER16_IER_CMPMIE_S;
}

void HAL_Timer16_SetInterruptDOWN(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.DOWN = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_DOWNIE_M;
    config |= InterruptEnable << TIMER16_IER_DOWNIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptUP(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.UP = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_UPIE_M;
    config |= InterruptEnable << TIMER16_IER_UPIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptARROK(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.ARROK = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_ARROKIE_M;
    config |= InterruptEnable << TIMER16_IER_ARROKIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptCMPOK(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.CMPOK = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_CMPOKIE_M;
    config |= InterruptEnable << TIMER16_IER_CMPOKIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptEXTTRIG(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.EXTTRIG = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_EXTTRIGIE_M;
    config |= InterruptEnable << TIMER16_IER_EXTTRIGIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptARRM(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.ARRM = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_ARRMIE_M;
    config |= InterruptEnable << TIMER16_IER_ARRMIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_SetInterruptCMPM(Timer16_HandleTypeDef *htimer16, uint32_t InterruptEnable)
{
    htimer16->Interrupts.CMPM = InterruptEnable;

    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_CMPMIE_M;
    config |= InterruptEnable << TIMER16_IER_CMPMIE_S;
    htimer16->Instance->IER = config;
}

void HAL_Timer16_InterruptInit(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_SetInterruptDOWN(htimer16, htimer16->Interrupts.DOWN);
    HAL_Timer16_SetInterruptUP(htimer16, htimer16->Interrupts.UP);
    HAL_Timer16_SetInterruptARROK(htimer16, htimer16->Interrupts.ARROK);
    HAL_Timer16_SetInterruptCMPOK(htimer16, htimer16->Interrupts.CMPOK);
    HAL_Timer16_SetInterruptEXTTRIG(htimer16, htimer16->Interrupts.EXTTRIG);
    HAL_Timer16_SetInterruptARRM(htimer16, htimer16->Interrupts.ARRM);
    HAL_Timer16_SetInterruptCMPM(htimer16, htimer16->Interrupts.CMPM);
}


