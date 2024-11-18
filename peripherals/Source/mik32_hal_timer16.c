#include "mik32_hal_timer16.h"

/**
 * @brief Настройка режимов выводов и тактирования Timer16.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
__attribute__((weak)) void HAL_TIMER16_MspInit(Timer16_HandleTypeDef* htimer16)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Вывод ШИМ не переводится в нужный режим */

    if (htimer16->Instance == TIMER16_0)
    {
        __HAL_PCC_TIMER16_0_CLK_ENABLE();
        
        if ((htimer16->Clock.Source == TIMER16_SOURCE_EXTERNAL_INPUT1) || (htimer16->CountMode == TIMER16_COUNTMODE_EXTERNAL))
        {
            GPIO_InitStruct.Pin = GPIO_PIN_5;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_7;
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
            GPIO_InitStruct.Pin = GPIO_PIN_8;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_10;
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
            GPIO_InitStruct.Pin = GPIO_PIN_11;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->EncoderMode == TIMER16_ENCODER_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }

        if (htimer16->Waveform.Enable == TIMER16_WAVEFORM_GENERATION_ENABLE)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_13;
            GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        }
    }
}

/**
 * @brief Выключить таймер.
 * Может использоваться для отключения таймера или при записи в регистр CFGR.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_Disable(Timer16_HandleTypeDef *htimer16)
{
    htimer16->Instance->CR &= ~TIMER16_CR_ENABLE_M;
}

/**
 * @brief Включить таймер
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_Enable(Timer16_HandleTypeDef *htimer16)
{
    htimer16->Instance->CR |= TIMER16_CR_ENABLE_M;
}

/**
 * @brief Установить активный фронт для подсчёта или задать подрежим энкодера.
 * Используется при тактировании Timer16 от внешнего источника тактового сигнала на выводе Input1.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param ActiveEdge Активный фронт. Возможные значения ActiveEdge:
 * - #TIMER16_ACTIVEEDGE_RISING Нарастающий фронт является активным. Подрежим энкодера 1;
 * - #TIMER16_ACTIVEEDGE_FOLLING Спадающий фронт является активным. Подрежим энкодера 2;
 * - #TIMER16_ACTIVEEDGE_BOTH Оба фронта являются активными фронтами. Подрежим энкодера 3.
 * 
 * @note Если оба фронта сконфигурированы как активные, необходимо также обеспечить внутренний тактовый сигнал. 
 * В этом случае частота внутреннего тактового сигнала должна быть как минимум в четыре раза выше частоты внешнего тактового сигнала.
 * При тактировании от Input1 ( HAL_Timer16_SetSourceClock ) счетчик Timer16 может обновляться либо по нарастающему, 
 * либо по спадающему фронту тактового сигнала lnput1, но не по двум (нарастающему и спадающему фронту) одновременно.
 * 
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 * 
 * 
 */
void HAL_Timer16_SetActiveEdge(Timer16_HandleTypeDef *htimer16, uint8_t ActiveEdge)
{
    htimer16->ActiveEdge = ActiveEdge;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_CKPOL_M;
    CFGRConfig |= ActiveEdge << TIMER16_CFGR_CKPOL_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

/**
 * @brief Выбрать источник тактирования, который будет использовать Timer16.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param SourceClock Источник тактирования.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 * 
 */
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
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(PM_TIMER_CFG_MUX_TIMER16_0_S);
            CFGConfig |= SourceClock << PM_TIMER_CFG_MUX_TIMER16_0_S;
        }
        else if (htimer16->Instance == TIMER16_1)
        {
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(PM_TIMER_CFG_MUX_TIMER16_1_S);
            CFGConfig |= SourceClock << PM_TIMER_CFG_MUX_TIMER16_1_S;
        }
        else if (htimer16->Instance == TIMER16_2)
        {
            CFGConfig &= ~PM_TIMER_CFG_MUX_TIMER_M(PM_TIMER_CFG_MUX_TIMER16_2_S);
            CFGConfig |= SourceClock << PM_TIMER_CFG_MUX_TIMER16_2_S;
        }
        /* Установка выбранного источника тактирования таймера в PM */
        PM->TIMER_CFG = CFGConfig;
        
        htimer16->Instance->CFGR &= ~TIMER16_CFGR_CKSEL_M;  /* Внутренний источник */
    }

}

/**
 * @brief Выбрать источник тактового сигнала для синхронизации счетчика Timer16.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param CountMode Источник тактового сигнала для синхронизации счетчика.
 * @note При тактировании от Input1 Timer16 не нуждается во внутреннем источнике тактового сигнала 
 * (за исключением случаев, когда включены фильтры glitch). Сигнал, подаваемый на lnput1 Timer16, используется в качестве 
 * системного тактового генератора для Timer16. Эта конфигурация подходит для режимов работы, в которых не включен встроенный генератор. 
 * При такой конфигурации счетчик Timer16 может обновляться либо по нарастающему, либо по спадающему фронту тактового сигнала lnput1, 
 * но не по двум (нарастающему и спадающему фронту) одновременно.
 * 
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 * 
 */
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

/**
 * @brief Инициализация тактирования в соответствии с параметрами #Timer16_HandleTypeDef *htimer16.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
void HAL_Timer16_ClockInit(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_SetSourceClock(htimer16, htimer16->Clock.Source);
    HAL_Timer16_SetCountMode(htimer16, htimer16->CountMode);
}

/**
 * @brief Задать режим обновления регистров ARR - значение автоматической перезагрузки и CMP - значение сравнения.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Preload Режим записи в ARR и CMP.
 * 
 * @warning Для записи в ARR и CMP таймер должен быть включен. При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 *
 */
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

/**
 * @brief Ожидание флага ARROK, 
 * Установка флага ARROK означает успешную запись в регистр ARR - значение автоматической перезагрузки.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_WaitARROK(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_ARROK_M));
    __HAL_TIMER16_CLEAR_FLAG(htimer16, TIMER16_FLAG_ARROK);
}

/**
 * @brief Ожидание флага CMPOK.
 * Установка флага CMPOK означает успешную запись в регистр CMP - значение сравнения.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_WaitCMPOK(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_CMPOK_M));
    __HAL_TIMER16_CLEAR_FLAG(htimer16, TIMER16_FLAG_CMPOK);
}

/**
 * @brief Задать значение автоматической перезагрузки (ARR).
 * Используется для установки верхнего предела, до которого будет считать счетчик Timer16.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period 16 битное число в пределах от 0 до 65535.
 * 
 * @warning Значение ARR должно быть всегда больше значения CMP.
 */
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

/**
 * @brief Задать значение сравнения (CMP).
 * Используется для сравнения текущего значения счетчика в регистре CNT со значением в регистре CMP. 
 * При совпадении CNT и CMP устанавливается флаг CMPM.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Compare 16 битное число в пределах от 0 до 65534.
 * 
 * @warning В режиме генерации волновой формы при совпадении значений регистров CMP и CNT сигнал на выводе output меняет свое состояние.
 * Значение ARR должно быть всегда больше значения CMP.
 */
void HAL_Timer16_SetCMP(Timer16_HandleTypeDef *htimer16, uint16_t Compare)
{
    /* Выключение таймера для записи CMP */
    htimer16->Instance->CMP = Compare;

    if (!(htimer16->Instance->IER & TIMER16_IER_CMPOKIE_M))
    {
        HAL_Timer16_WaitCMPOK(htimer16);
    }  
}

/**
 * @brief Выбрать источник триггера.
 * Счетчик Timer16 может быть запущен либо программно, либо после обнаружения активного 
 * фронта импульса на одном из 8 триггерных входов.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param TriggerSource Источник триггера.
 * 
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
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

/**
 * @brief Выбрать разрешение работы и активный фронт триггера.
 * Счетчик Timer16 может быть запущен либо программно, 
 * либо после обнаружения активного фронта импульса на одном из 8 триггерных входов.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param TriggerEdge Активный фронт триггера.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
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

/**
 * @brief Включить или выключить функцию time-out.
 * С помощью включенной функции time-out активный фронт триггера 
 * может перезапустить отсчет таймера. Иначе повторное срабатывание 
 * триггера во время счета будет проигнорировано. Может быть реализована функция time-out 
 * с низким энергопотреблением. Значение time-out соответствует значению сравнения - CMP. 
 * Если в течение ожидаемого периода времени триггер не срабатывает, 
 * MCU пробуждается по событию совпадения сравнения.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param TimeOut Режим time-out.
 * 
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
void HAL_Timer16_SetTimeOut(Timer16_HandleTypeDef *htimer16, uint8_t TimeOut)
{
    htimer16->Trigger.TimeOut = TimeOut;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_TIMOUT_M;
    CFGRConfig |= TimeOut << TIMER16_CFGR_TIMOUT_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

/**
 * @brief Задать чувствительность фильтра для внешнего тактового генератора.
 * Прежде чем активировать цифровые фильтры, на Timer16 сначала должен быть подан 
 * внутренний источник синхронизации. В случае отсутствия внутреннего тактового сигнала 
 * цифровой фильтр должен быть деактивирован.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param FilterExternalClock Чувствительность фильтра.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 * 
 */
void HAL_Timer16_SetFilterExternalClock(Timer16_HandleTypeDef *htimer16, uint8_t FilterExternalClock)
{
    htimer16->Filter.ExternalClock = FilterExternalClock;

    /* Выключение таймера для записи в CFGR */
    HAL_Timer16_Disable(htimer16);

    htimer16->Instance->CFGR &= ~TIMER16_CFGR_CKFLT_M;
    htimer16->Instance->CFGR |= FilterExternalClock << TIMER16_CFGR_CKFLT_S;
}

/**
 * @brief Задать чувствительность фильтра для триггера.
 * Прежде чем активировать цифровые фильтры, на LPTIM сначала должен быть подан внутренний 
 * источник синхронизации. В случае отсутствия внутреннего тактового сигнала цифровой фильтр 
 * должен быть деактивирован.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param FilterTrigger Чувствительность фильтра.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
void HAL_Timer16_SetFilterTrigger(Timer16_HandleTypeDef *htimer16, uint8_t FilterTrigger)
{
    htimer16->Filter.Trigger = FilterTrigger;

    /* Выключение таймера для записи в CFGR */
    HAL_Timer16_Disable(htimer16);

    htimer16->Instance->CFGR &= ~TIMER16_CFGR_TRGFLT_M;
    htimer16->Instance->CFGR |= FilterTrigger << TIMER16_CFGR_TRGFLT_S;
}

/**
 * @brief Включить или выключить режим энкодера.
 * Режим энкодера доступен только в том случае, если Timer16 работает от внутреннего источника синхронизации. 
 * Частота сигналов на обоих входах lnput1 и lnput2 не должна превышать частоту внутреннего тактового генератора Timer16 , деленную на 4. 
 * Кроме того, коэффициент деления предделителя должен быть равен его начальному значению, которое равно 1. 
 * Это обязательно для обеспечения нормальной работы Timer16.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param EncoderMode Режим энкодера.
 * @warning При использовании данной функции таймер выключается. Это необходимо для записи в регистр CFGR.
 */
void HAL_Timer16_SetEncoderMode(Timer16_HandleTypeDef *htimer16, uint8_t EncoderMode)
{
    htimer16->EncoderMode = EncoderMode;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_ENC_M;
    CFGRConfig |= EncoderMode << TIMER16_CFGR_ENC_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

/**
 * @brief Задать полярность формы волны на выводе Output.
 * Используется при генерации волновой формы для настройки полярности выходного сигнала.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param WaveformPolarity Полярность выходного сигнала.
 */
void HAL_Timer16_WaveformPolarity(Timer16_HandleTypeDef *htimer16, HAL_Timer16_WaveformPolarityTypeDef WaveformPolarity)
{
    WaveformPolarity &= TIMER16_CFGR_WAVPOL_M;
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR = (htimer16->Instance->CFGR & (~TIMER16_CFGR_WAVPOL_M)) | WaveformPolarity;
}

/**
 * @brief Установить делитель частоты.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Prescaler Делитель частоты.
 */
void HAL_Timer16_SetPrescaler(Timer16_HandleTypeDef *htimer16, uint8_t Prescaler)
{
    htimer16->Clock.Prescaler = Prescaler;

    HAL_Timer16_Disable(htimer16);

    uint32_t CFGRConfig = htimer16->Instance->CFGR;
    CFGRConfig &= ~TIMER16_CFGR_PRESC_M;
    CFGRConfig |= Prescaler << TIMER16_CFGR_PRESC_S;

    htimer16->Instance->CFGR = CFGRConfig;
}

/**
 * @brief Инициализировать Timer16 в соответствии с настройками #Timer16_HandleTypeDef *htimer16.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
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

/**
 * @brief Получить текущее значение счетчика из регистра CNT.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @return Текущее значение счетчика таймера16 (CNT).
 */
uint16_t HAL_Timer16_GetCounterValue(Timer16_HandleTypeDef *htimer16)
{
    return htimer16->Instance->CNT;
}

/**
 * @brief Проверить состояние флага сравнения CMPM.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @return Текущее состояние флага CMPM.
 */
uint8_t HAL_Timer16_CheckCMP(Timer16_HandleTypeDef *htimer16)
{
    if ((htimer16->Instance->ISR & TIMER16_ISR_CMPM_M) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief Ожидать когда счетчик достигнет значения сравнения CMP.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_WaitCMP(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_CMPM_M));
}

/**
 * @brief Запустить таймер в продолжительном режиме.
 * Счетчик будет считать от 0 до значения Period, а затем начнет сначала.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Значение автоматической перезагрузки.
 */
void HAL_Timer16_Counter_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Enable(htimer16);

    HAL_Timer16_SetARR(htimer16, Period);
    
    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

/**
 * @brief Запустить таймер с ШИМ сигналом.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Период ШИМ сигнала. Число от 0 до 65535.
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartPWM(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period >= Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_CONTINUOUS(htimer16);

}

/**
 * @brief Запустить таймер в одноимпульсном режиме.
 * Счетчик будет считать от 0 до значения в регистре ARR. 
 * При достижении значения CMP сигнал на выводе Output сменит свое состояние. 
 * При достижении значения ARR сигнал на выводе Output вернется в исходное состояние.
 *
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Период ШИМ сигнала. Число от 0 до 65535;
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartOneShot(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period >= Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_SINGLE(htimer16);

}

/**
 * @brief Запустить таймер в однократном режиме.
 * При достижении значения CMP сигнал на выводе Output сменит свое состояние.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Период ШИМ сигнала. Число от 0 до 65535.
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartSetOnes(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR |= TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);
    
    if(Period >= Compare)
    {
        HAL_Timer16_SetCMP(htimer16, Compare);
        HAL_Timer16_SetARR(htimer16, Period);
    }

    __HAL_TIMER16_START_SINGLE(htimer16);
}

/**
 * @brief Запустить timer16 в режиме энкодера.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 */
void HAL_Timer16_Encoder_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_ENABLE);

    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_DOWNCF_M | TIMER16_ICR_UPCF_M;
    HAL_Timer16_SetARR(htimer16, Period);

    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

/**
 * @brief Выключить таймер и режим энкодера.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_Encoder_Stop(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);
    HAL_Timer16_SetEncoderMode(htimer16, TIMER16_ENCODER_DISABLE);
}

/**
 * @brief Выключить таймер.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_Stop(Timer16_HandleTypeDef *htimer16)
{
    HAL_Timer16_Disable(htimer16);
}

/**
 * @brief Запустить таймер в продолжительном режиме с использованием прерываний.
 * Используются прерывания успешной записи в регистр автозагрузки ARR (ARROK) 
 * и достижение верхнего значения (ARRM).
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 */
void HAL_Timer16_Counter_Start_IT(Timer16_HandleTypeDef *htimer16, uint32_t Period)
{
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_ARRMCF_M;
    
    HAL_Timer16_SetARR(htimer16, Period);

    htimer16->Instance->IER |= TIMER16_IER_ARROKIE_M | TIMER16_IER_ARRMIE_M;

    __HAL_TIMER16_START_CONTINUOUS(htimer16);
}

/**
 * @brief Запустить ШИМ с использованием прерываний.
 * Используются следующие прерывания:
 * - Обновление регистра автозагрузки успешно завершено (ARROK);
 * - Обновление регистра сравнения успешно завершено (CMPOK);
 * - Значение регистра счета CNT достигло значения регистра ARR (ARRM);
 * - Значение регистра счета CNT достигло значения регистра CMP (CMPM);
 * - На выбранном входе внешнего триггера возник достоверный фронт импульса. (EXTTRIG)
 * Если триггер игнорируется, так как таймер уже запущен, то этот флаг не устанавливается.
 *
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartPWM_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{


    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    

    
    if(Period >= Compare)
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

/**
 * @brief Запустить таймер в одноимпульсном режиме с использованием прерываний.
 * Используются следующие прерывания:
 * - Обновление регистра автозагрузки успешно завершено (ARROK);
 * - Обновление регистра сравнения успешно завершено (CMPOK);
 * - Значение регистра счета CNT достигло значения регистра ARR (ARRM);
 * - Значение регистра счета CNT достигло значения регистра CMP (CMPM);
 * - На выбранном входе внешнего триггера возник достоверный фронт импульса. (EXTTRIG)
 * Если триггер игнорируется, так как таймер уже запущен, то этот флаг не устанавливается.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartOneShot_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR &= ~TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    
    if(Period >= Compare)
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

/**
 * @brief Запустить таймер в однократном режиме с использованием прерываний.
 * Используются следующие прерывания:
 * - Обновление регистра автозагрузки успешно завершено (ARROK);
 * - Обновление регистра сравнения успешно завершено (CMPOK);
 * - Значение регистра счета CNT достигло значения регистра ARR (ARRM);
 * - Значение регистра счета CNT достигло значения регистра CMP (CMPM);
 * - На выбранном входе внешнего триггера возник достоверный фронт импульса. (EXTTRIG)
 * Если триггер игнорируется, так как таймер уже запущен, то этот флаг не устанавливается.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 * @param Compare Значение при достижении которого сигнал на выводе Output сменит свое состояние. 
 * Число от 0 до 65534. Данное число всегда должно быть меньше значения Period.
 */
void HAL_Timer16_StartSetOnes_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare)
{
    HAL_Timer16_Disable(htimer16);
    htimer16->Instance->CFGR |= TIMER16_CFGR_WAVE_M;
    HAL_Timer16_Enable(htimer16);

    htimer16->Instance->ICR = TIMER16_ICR_ARROKCF_M | TIMER16_ICR_CMPOKCF_M | TIMER16_ICR_ARRMCF_M 
                            | TIMER16_ICR_CMPMCF_M | TIMER16_ICR_EXTTRIGCF_M;
    
    if(Period >= Compare)
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

/**
 * @brief Запустить таймер в режиме энкодера с использованием прерываний.
 * Используются следующие прерывания:
 * - Изменение направления счетчика c вверх на вниз (DOWN);
 * - Изменение направления счетчика с вниз на вверх (UP).
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param Period Верхнее значение счета.
 */
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

/**
 * @brief Выключить таймер, режим энкодера и прерывания.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
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

/**
 * @brief Ожидать флаг триггера - EXTTRIG.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_WaitTrigger(Timer16_HandleTypeDef *htimer16)
{
    while (!(htimer16->Instance->ISR & TIMER16_ISR_EXTTRIG_M));
    __HAL_TIMER16_CLEAR_FLAG(htimer16, TIMER16_FLAG_EXTTRIG);
}

/**
 * @brief Установить маску прерываний в регистр IER.
 * @param htimer16 Указатель на структуру с настройками Timer16.
 * @param InterruptMask Маска перываний.
 */
void HAL_Timer16_SetInterruptMask(Timer16_HandleTypeDef *htimer16, uint32_t InterruptMask)
{
    htimer16->Instance->IER = InterruptMask;
}

/**
 * @brief Разрешить прерывание DOWN.
 * Используется в режиме энкодера для формирования прерывания при смене направления с вверх на вниз.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptDOWN(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_DOWNIE_M;
    config |= TIMER16_IER_DOWNIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание UP.
 * Используется в режиме энкодера для формирования прерывания при смене направления с вверх на вниз.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptUP(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_UPIE_M;
    config |= TIMER16_IER_UPIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание ARROK.
 * Используется для формирования прерывания при успешной записи в регистр ARR.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptARROK(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_ARROKIE_M;
    config |= TIMER16_IER_ARROKIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание CMPOK.
 * Используется для формирования прерывания при успешной записи в регистр CMP.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptCMPOK(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_CMPOKIE_M;
    config |= TIMER16_IER_CMPOKIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание EXTTRIG.
 * Используется для формирования прерывания при возникновении достоверного фронта импульса на выбранном входе внешнего триггера (EXTTRIG). 
 * Если триггер игнорируется, так как таймер уже запущен, то этот флаг не устанавливается.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptEXTTRIG(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_EXTTRIGIE_M;
    config |= TIMER16_IER_EXTTRIGIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание ARRM.
 * Используется для формирования прерывания при совпадении счетчика со значенеим ARR.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptARRM(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_ARRMIE_M;
    config |= TIMER16_IER_ARRMIE_M;
    htimer16->Instance->IER = config;
}

/**
 * @brief Разрешить прерывание CMPM.
 * Используется для формирования прерывания при совпадении счетчика со значенеим CMP.
 * 
 * @param htimer16 Указатель на структуру с настройками Timer16.
 */
void HAL_Timer16_SetInterruptCMPM(Timer16_HandleTypeDef *htimer16)
{
    uint32_t config = htimer16->Instance->IER;
    config &= ~TIMER16_IER_CMPMIE_M;
    config |= TIMER16_IER_CMPMIE_M;
    htimer16->Instance->IER = config;
}


/* Функции системного времени */


/* Структура, необходимая для работы функций системных часов на 16-р таймере */
struct {
    Timer16_HandleTypeDef tim16;
    /* Timer prescaler */
    uint32_t pt;
    /* Time in ticks, updated by interrupts */
    volatile uint32_t ticks;
    /* Clock frequency */
    uint32_t clock_freq;
} HAL_Time_TIM16_Handler;


void HAL_Time_TIM16_InterruptHandler()
{
    uint32_t condition;
    switch ((uint32_t)HAL_Time_TIM16_Handler.tim16.Instance)
    {
        case (uint32_t)TIMER16_0: condition = EPIC_CHECK_TIMER16_0(); break;
        case (uint32_t)TIMER16_1: condition = EPIC_CHECK_TIMER16_1(); break;
        case (uint32_t)TIMER16_2: condition = EPIC_CHECK_TIMER16_2(); break;
        default: condition = 0;
    }
    if (!condition) return;
    if(__HAL_TIMER16_GET_FLAG_IT(&(HAL_Time_TIM16_Handler.tim16), TIMER16_FLAG_ARRM))
    {
        HAL_Time_TIM16_Handler.ticks += 0x00010000UL;
    }
    __HAL_TIMER16_CLEAR_FLAG(&(HAL_Time_TIM16_Handler.tim16), 0xFFFFFFFF);
}


/**
 * @brief Инициализация 16-р таймера для работы в качестве системных часов.
 * После инициализации системного таймера не рекомендуется изменять делители тактовой частоты AHB, APB_P и APB_M
 * Если делители такта были изменены или микроконтроллер переключился на другой источник тактирования, необходимо
 * переинициализаровать таймер. При этом прежнее значение системного времени потеряется.
 * 
 * Время переполнения системных часов зависит от частоты тактирования. Минимальное время переполнения - 4295с.
 * 
 * @warning При работе 16-р таймера в качестве системных часов используются прерывания по переполнению. Не рекомендуется
 * запрещать глобальные прерывания.
 * 
 * @param timer  TIMER16_0, TIMER16_1 или TIMER16_2
*/
void HAL_Time_TIM16_Init(TIMER16_TypeDef* timer)
{
    HAL_Time_TIM16_Handler.tim16.Instance = timer;
    HAL_Time_TIM16_Handler.tim16.Clock.Source = TIMER16_SOURCE_INTERNAL_SYSTEM;
    HAL_Time_TIM16_Handler.tim16.CountMode = TIMER16_COUNTMODE_INTERNAL;
    HAL_Time_TIM16_Handler.tim16.ActiveEdge = TIMER16_ACTIVEEDGE_RISING;
    HAL_Time_TIM16_Handler.tim16.Preload = TIMER16_PRELOAD_AFTERWRITE;
    HAL_Time_TIM16_Handler.tim16.Trigger.Source = TIMER16_TRIGGER_TIM1_GPIO1_9; 
    HAL_Time_TIM16_Handler.tim16.Trigger.ActiveEdge = TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE;
    HAL_Time_TIM16_Handler.tim16.Trigger.TimeOut = TIMER16_TIMEOUT_DISABLE;
    HAL_Time_TIM16_Handler.tim16.Filter.ExternalClock = TIMER16_FILTER_NONE;
    HAL_Time_TIM16_Handler.tim16.Filter.Trigger = TIMER16_FILTER_NONE;
    HAL_Time_TIM16_Handler.tim16.EncoderMode = TIMER16_ENCODER_DISABLE;
    HAL_Time_TIM16_Handler.tim16.Waveform.Enable = TIMER16_WAVEFORM_GENERATION_DISABLE;
    HAL_Time_TIM16_Handler.tim16.Waveform.Polarity = TIMER16_WAVEFORM_POLARITY_NONINVERTED;
    /* Calculate prescaler values */
    HAL_Time_TIM16_Handler.clock_freq = HAL_PCC_GetSysClockFreq();
    if (HAL_Time_TIM16_Handler.clock_freq % 1000000UL != 0)
    {
        HAL_Time_TIM16_Handler.pt = 1;
        HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_1;
    }
    else
    {
        uint32_t pt_raw = HAL_Time_TIM16_Handler.clock_freq / 1000000UL;
        switch (pt_raw)
        {
            case 1: HAL_Time_TIM16_Handler.pt = 1; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_1; break;
            case 2: HAL_Time_TIM16_Handler.pt = 2; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_2; break;
            case 4: HAL_Time_TIM16_Handler.pt = 4; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_4; break;
            case 8: HAL_Time_TIM16_Handler.pt = 8; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_8; break;
            case 16: HAL_Time_TIM16_Handler.pt = 16; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_16; break;
            case 32: HAL_Time_TIM16_Handler.pt = 32; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_32; break;
            case 64: HAL_Time_TIM16_Handler.pt = 64; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_64; break;
            case 128: HAL_Time_TIM16_Handler.pt = 128; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_128; break;
            default: HAL_Time_TIM16_Handler.pt = 1; HAL_Time_TIM16_Handler.tim16.Clock.Prescaler = TIMER16_PRESCALER_1; break;
        }
    }
    HAL_Timer16_Init(&(HAL_Time_TIM16_Handler.tim16));
    /* Reset the timer */
    HAL_Time_TIM16_Handler.tim16.Instance->CNT = 0;
    HAL_Time_TIM16_Handler.ticks = 0;
    /* Set interrupt mask */
    switch ((uint32_t)timer)
    {
        case (uint32_t)TIMER16_0: HAL_EPIC_MaskLevelSet(HAL_EPIC_TIMER16_0_MASK); break;
        case (uint32_t)TIMER16_1: HAL_EPIC_MaskLevelSet(HAL_EPIC_TIMER16_1_MASK); break;
        case (uint32_t)TIMER16_2: HAL_EPIC_MaskLevelSet(HAL_EPIC_TIMER16_2_MASK); break;
    }
    HAL_IRQ_EnableInterrupts();
    HAL_Timer16_Counter_Start_IT(&(HAL_Time_TIM16_Handler.tim16), 0xFFFF);
}

// uint32_t HAL_Time_TIM16_GetTick()
// {
//     return HAL_Time_TIM16_Handler.ticks + time_timer16.Instance->CNT;
// }

/**
 * @brief Системное время в микросекундах, используется 16-р таймер в качестве системных часов
*/
uint32_t HAL_Time_TIM16_Micros()
{
    //HAL_IRQ_DisableInterrupts();
    uint64_t time_raw = HAL_Time_TIM16_Handler.ticks + HAL_Time_TIM16_Handler.tim16.Instance->CNT;
    return (uint32_t)(time_raw * (1000000UL * HAL_Time_TIM16_Handler.pt) /
        HAL_Time_TIM16_Handler.clock_freq);
}

/**
 * @brief Системное время в миллисекундах, используется 16-р таймер в качестве системных часов
*/
uint32_t HAL_Time_TIM16_Millis()
{
    uint64_t time_raw = HAL_Time_TIM16_Handler.ticks + HAL_Time_TIM16_Handler.tim16.Instance->CNT;
    return (uint32_t)(time_raw * (1000UL * HAL_Time_TIM16_Handler.pt) /
        HAL_Time_TIM16_Handler.clock_freq);
}

/**
 * @brief Функция задержки в микросекундах, используется 16-р таймер в качестве системных часов
 * @warning При работе 16-р таймера в качестве системных часов используются прерывания по
 * переполнению. Если запретить глобальные прерывания, данная функция может вызвать зависание
 * микроконтроллера.
*/
void HAL_Time_TIM16_DelayUs(uint32_t time_us)
{
    uint32_t time_metka = HAL_Time_TIM16_Micros();
    while ((HAL_Time_TIM16_Micros() - time_metka) < time_us);
}

/**
 * @brief Функция задержки в миллисекундах, используется 16-р таймер в качестве системных часов
 * @warning При работе 16-р таймера в качестве системных часов используются прерывания по
 * переполнению. Если запретить глобальные прерывания, данная функция может вызвать зависание
 * микроконтроллера.
*/
void HAL_Time_TIM16_DelayMs(uint32_t time_ms)
{
    uint32_t time_metka = HAL_Time_TIM16_Millis();
    while ((HAL_Time_TIM16_Millis() - time_metka) < time_ms);
}
