#ifndef MIK32V0
#include "mik32_hal_tsens.h"

/**
 * @brief  Функция включения тактирования аналогового блока (включающего термосенсор).
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 */
__attribute__((weak)) void HAL_TSENS_MspInit(TSENS_HandleTypeDef *htsens)
{
    __HAL_PCC_ANALOG_REGS_CLK_ENABLE();
}


/**
 * @brief Функция начальной инициализации термосенсора.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_TSENS_Init(TSENS_HandleTypeDef *htsens)
{
    HAL_StatusTypeDef errorCode = HAL_OK;
    HAL_TSENS_MspInit(htsens);
    /* Включение, тактирование температурного сенсора, отключение режима сброса. */
    ANALOG_REG->TSENS_CFG = (1<<TSENS_CFG_NRST_S)|
							(1<<TSENS_CFG_NPD_CLK_S)|
							(1<<TSENS_CFG_NPD_S);
    
    // Установка источника тактирования термосенсора.
    if ((errorCode = HAL_TSENS_ClockSource(htsens, htsens->Clock)) != HAL_OK)
    {
        return errorCode;
    }
    
    // Подбор делителя для заданной частоты термосенсора.
    if ((errorCode = HAL_TSENS_Clock(htsens, htsens->Frequency)) != HAL_OK)
    {
        return errorCode;
    }
    
    return errorCode;
}


/**
 * @brief Функция установки источника тактирования термосенсора.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param clk_source источник тактирования термосенсора.
 *                      - @ref HAL_TSENS_SYS_CLK        - системная тактовая частота
 *                      - @ref HAL_TSENS_HCLK           - частота шины АНВ
 *                      - @ref HAL_TSENS_OSC32M         - внешний генератор 32МГц
 *                      - @ref HAL_TSENS_HSI32M         - внутренний генератор 32МГц
 *                      - @ref HAL_TSENS_OSC32K         - внешний генератор 32кГц
 *                      - @ref HAL_TSENS_LSI32K         - внутренний генератор 32кГц
 * @return Статус HAL.
 *      HAL_OK, если источник тактирования установлен успешно
 *      HAL_ERROR, если входной параметр некорректен.
 */
HAL_StatusTypeDef HAL_TSENS_ClockSource(TSENS_HandleTypeDef *htsens, HAL_TSENS_ClockTypeDef clk_source)
{
    switch (clk_source)
    {
    case HAL_TSENS_SYS_CLK:
    case HAL_TSENS_HCLK:   
    case HAL_TSENS_OSC32M: 
    case HAL_TSENS_HSI32M: 
    case HAL_TSENS_OSC32K: 
    case HAL_TSENS_LSI32K: 
        break;
    default:
        return HAL_ERROR;
    }
    
    htsens->Clock = clk_source;
    ANALOG_REG->TSENS_CFG = (ANALOG_REG->TSENS_CFG & (~TSENS_CFG_CLK_MUX_M)) | (clk_source << TSENS_CFG_CLK_MUX_S);
    return HAL_OK;
}

/**
 * @brief Функция установки делителя частоты для термосенсора.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param clk_div делитель частоты термосенсора.
 *                  Этот параметр может принимать значения от 0 до 1023.
 * @return Статус HAL.
 *      HAL_OK, если делитель частоты установлен успешно;
 *      HAL_ERROR, если делитель частоты слишком мал или слишком велик для получения частоты термосенсора 0 - 100 кГц.
 */
HAL_StatusTypeDef HAL_TSENS_ClockDivider(TSENS_HandleTypeDef *htsens, uint16_t clk_div)
{
    if (clk_div >= 1024) return HAL_ERROR;

    clk_div &= 0x3FF;
    uint32_t divider;
    uint32_t systemFreq = HAL_PCC_GetSysClockFreq();
    uint32_t tsensFreq = 0;
    switch (htsens->Clock)
    {
        case HAL_TSENS_SYS_CLK:
            divider = (clk_div + 1) << 1;
            tsensFreq = systemFreq / divider;
            if ((tsensFreq > 100000UL)||(tsensFreq == 0)) return HAL_ERROR;
            break;
        case HAL_TSENS_HCLK:
            divider = ((uint32_t)PM->DIV_AHB + 1) * ((clk_div + 1) << 1);
            tsensFreq = systemFreq / divider;
            if ((tsensFreq > 100000UL)||(tsensFreq == 0)) return HAL_ERROR;
            break;
        case HAL_TSENS_OSC32M:
            if (clk_div < (OSC_SYSTEM_VALUE/200000 - 1)) return HAL_ERROR;
            tsensFreq = OSC_SYSTEM_VALUE/((1 + clk_div) << 1);
            break;
        case HAL_TSENS_HSI32M:
            if (clk_div < (HSI_VALUE/200000 - 1)) return HAL_ERROR;
            tsensFreq = HSI_VALUE/((1 + clk_div) << 1);
            break;
        case HAL_TSENS_OSC32K:
        case HAL_TSENS_LSI32K:
            tsensFreq = OSC_CLOCK_VALUE/((1 + clk_div) << 1);
            break;
        default:
            return HAL_ERROR;
    }
    ANALOG_REG->TSENS_CFG = (ANALOG_REG->TSENS_CFG & (~TSENS_CFG_DIV_M)) | (clk_div << TSENS_CFG_DIV_S);
    htsens->Frequency = tsensFreq;
    return HAL_OK;
}

/**
 * @brief  Функция установки тактовой частоты для термосенсора.
 * 
 * Функция подбирает делитель для получения наиболее близкой частоты термосенсора к заданной.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param f_enter тактовая частота термосенсора.
 * @return Статус HAL.
 *     HAL_OK, если частота установлена успешно
 *     HAL_ERROR, если введенная частота не попадает в область допустимых значений частот 0 - 100 кГц или если невозможно подобрать делитель.
 */
HAL_StatusTypeDef HAL_TSENS_Clock(TSENS_HandleTypeDef *htsens, uint32_t f_enter)
{
    if ((f_enter > 100000UL)||(f_enter == 0)) return HAL_ERROR;
    uint32_t f_real;
    uint32_t systemFreq = HAL_PCC_GetSysClockFreq();
    switch (htsens->Clock)
    {
        case HAL_TSENS_SYS_CLK:
            f_real = systemFreq; /* Примечание: надо проверить какой источник и входимость в диапазон */
            break;
        case HAL_TSENS_HCLK:
            f_real = systemFreq / (PM->DIV_AHB + 1); /* Примечание: надо проверить какой источник и входимость в диапазон */
            break;
        case HAL_TSENS_OSC32M:
            f_real = OSC_SYSTEM_VALUE;
            break;
        case HAL_TSENS_HSI32M:
            f_real = HSI_VALUE;
            break;
        case HAL_TSENS_OSC32K:
            f_real = OSC_CLOCK_VALUE;
            break;
        case HAL_TSENS_LSI32K:
            f_real = LSI_VALUE;
            break;
        default:
            return HAL_ERROR;
    }

    uint32_t divider = (f_real / f_enter) >> 1;
    if (divider == 0) return HAL_ERROR;

    uint32_t pre_result = f_real / (divider << 1);
    while ((pre_result > 100000) && (divider <= 0x400))
    {   
        divider += 1;
        if (divider > 0x400)
        {
            return HAL_ERROR;
        }
        pre_result = f_real / (divider << 1);
    }
    
    divider -= 1;

    /* Запись данных */
    ANALOG_REG->TSENS_CFG = (ANALOG_REG->TSENS_CFG & (~TSENS_CFG_DIV_M)) | (divider<<TSENS_CFG_DIV_S);
    htsens->Frequency = f_real / ((1 + divider) << 1);
    return HAL_OK;
}

/**
 * @brief  Функция установки сырого значения нижнего порога температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param low_tres сырое значение нижнего порога температуры.
 *                  Этот параметр может принимать значение от 225 до 603.
 * @return Статус HAL.
 *     HAL_OK, если нижнее пороговое значение установлено успешно;
 *     HAL_ERROR, если значение low_tres не попадает в диапазон [225, 603].
 */
HAL_StatusTypeDef HAL_TSENS_SetLowThresholdRaw(TSENS_HandleTypeDef *htsens, uint16_t low_tres)
{
    if (low_tres > 603 || low_tres < 225)
    {
        return HAL_ERROR;
    }
    
    low_tres &= 0x3FF;
    htsens->Instance->TSENS_THRESHOLD = (htsens->Instance->TSENS_THRESHOLD & (~TSENS_TRESHOLD_LOW_M)) | ((uint32_t)low_tres << TSENS_TRESHOLD_LOW_S);

    return HAL_OK;
}

/**
 * @brief  Функция установки сырого значения верхнего порога температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param hi_tres сырое значение верхнего порога температуры.
 *                  Этот параметр может принимать значение от 225 до 603.
 * @return Статус HAL.
 *     HAL_OK, если верхнее пороговое значение установлено успешно;
 *     HAL_ERROR, если значение hi_tres не попадает в диапазон [225, 603].
 */
HAL_StatusTypeDef HAL_TSENS_SetHiThresholdRaw(TSENS_HandleTypeDef *htsens, uint16_t hi_tres)
{
    if (hi_tres > 603 || hi_tres < 225)
    {
        return HAL_ERROR;
    }
    hi_tres &= 0x3FF;
    htsens->Instance->TSENS_THRESHOLD = (htsens->Instance->TSENS_THRESHOLD & (~TSENS_TRESHOLD_HI_M)) | ((uint32_t)hi_tres << TSENS_TRESHOLD_HI_S);

    return HAL_OK;
}

/**
 * @brief  Функция установки значения нижнего порога температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param low_temp_border значение нижнего порога температуры в градусах Цельсия.
 *                          Этот параметр может принимать значение от -40 до 125.
 * @return Статус HAL.
 *     HAL_OK, если нижнее пороговое значение установлено успешно;
 *     HAL_ERROR, если рассчитанное значение нижнего порога не попадает в диапазон [225, 603].
 */
HAL_StatusTypeDef HAL_TSENS_SetLowThreshold(TSENS_HandleTypeDef *htsens, int low_temp_border)
{
    return HAL_TSENS_SetLowThresholdRaw(htsens, TSENS_CELSIUS_TO_VALUE(low_temp_border));
}

/**
 * @brief  Функция установки значения верхнего порога температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param hi_temp_border значение верхнего порога температуры в градусах Цельсия.
 *                          Этот параметр может принимать значение от -40 до 125.
 * @return Статус HAL.
 *     HAL_OK, если верхнее пороговое значение установлено успешно;
 *     HAL_ERROR, если рассчитанное значение верхнего порога не попадает в диапазон [225, 603].
 */
HAL_StatusTypeDef HAL_TSENS_SetHiThreshold(TSENS_HandleTypeDef *htsens, int hi_temp_border)
{
    return HAL_TSENS_SetHiThresholdRaw(htsens, TSENS_CELSIUS_TO_VALUE(hi_temp_border));
}

/**
 * @brief  Функция чтения и перевода в градусы Цельсия данных термосенсора.
 * 
 * Функция используется для получения значения температуры в непрерывном режиме измерения (Continuous).
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @return температура в градусах Цельсия, увеличенная в 100 раз.
 */
uint32_t HAL_TSENS_GetTemperature(TSENS_HandleTypeDef *htsens)
{
   return TSENS_VALUE_TO_CELSIUS(__HAL_TSENS_READ_MEASUREMENT(htsens));
}

/**
 * @brief Запустить непрерывное измерение температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 */
void HAL_TSENS_ContinuousStart(TSENS_HandleTypeDef *htsens)
{
    __HAL_TSENS_RELEASE_RESET(htsens);
    __HAL_TSENS_CONTINUOUS_START(htsens);
}

/**
 * @brief Запустить одиночное измерение температуры.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 * @param timeout значение таймаута. Если значение 0, то ожидание флага конца преобразования никогда не прервется. 
 * @return Структура со статусом HAL и измеренным значением температуры.
 *      @ref TSENS_ValueTypeDef.statusHAL статус HAL
 *      @ref TSENS_ValueTypeDef.value значение температуры в градусах Цельсия. Значение в 100 раз больше.
 */
TSENS_ValueTypeDef HAL_TSENS_SingleStart(TSENS_HandleTypeDef *htsens, uint32_t timeout)
{
    uint32_t timeoutCounter = timeout;

    __HAL_TSENS_RELEASE_RESET(htsens);
    __HAL_TSENS_SINGLE_START(htsens);
    while (!__HAL_TSENS_GET_EOC(htsens))
    {
        if ((timeoutCounter-- == 0) && (timeout != 0))
        {
            return (TSENS_ValueTypeDef) {.statusHAL = HAL_TIMEOUT, .value = 0};
        }
    }

    uint32_t rawValue = __HAL_TSENS_READ_MEASUREMENT(htsens);
    
   return (TSENS_ValueTypeDef) {.statusHAL = HAL_OK, .value = TSENS_VALUE_TO_CELSIUS(rawValue)};
}

/**
 * @brief Запустить непрерывное измерение температуры с использованием прерываний.
 * 
 * Включаются все прерывания датчика температуры (окончание преобразование, выход измеренного значения температуры за пороговые значения).
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 */
void HAL_TSENS_ContinuousStart_IT(TSENS_HandleTypeDef *htsens)
{
    __HAL_TSENS_IRQ_CLEAR(htsens, TSENS_CLEAR_IRQ_LOW_CLEAR_M | TSENS_CLEAR_IRQ_HI_CLEAR_M | TSENS_CLEAR_IRQ_EOC_CLEAR_M);
    __HAL_TSENS_IRQ_ENABLE(htsens, TSENS_IRQ_LOW_MASK_M | TSENS_IRQ_HI_MASK_M | TSENS_IRQ_EOC_MASK_M);
    __HAL_TSENS_RELEASE_RESET(htsens);
    __HAL_TSENS_CONTINUOUS_START(htsens);
}

/**
 * @brief Запустить одиночное измерение температуры с использованием прерываний.
 * 
 * Включаются все прерывания датчика температуры (окончание преобразование, выход измеренного значения температуры за пороговые значения).
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 */
void HAL_TSENS_SingleStart_IT(TSENS_HandleTypeDef *htsens)
{
    __HAL_TSENS_IRQ_CLEAR(htsens, TSENS_CLEAR_IRQ_LOW_CLEAR_M | TSENS_CLEAR_IRQ_HI_CLEAR_M | TSENS_CLEAR_IRQ_EOC_CLEAR_M);
    __HAL_TSENS_IRQ_ENABLE(htsens, TSENS_IRQ_LOW_MASK_M | TSENS_IRQ_HI_MASK_M | TSENS_IRQ_EOC_MASK_M);
    __HAL_TSENS_RELEASE_RESET(htsens);
    __HAL_TSENS_SINGLE_START(htsens);
}

/**
 * @brief Прервать измерение температуры и отключить прерывания.
 * 
 * Функция останавливает измерения, выключает все прерывания термосенсора и очищает флаги прерываний.
 * @param htsens указатель на структуру TSENS_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля TSENS.
 */
void HAL_TSENS_Stop_IT(TSENS_HandleTypeDef *htsens)
{
    __HAL_TSENS_FORCE_RESET(htsens);
    __HAL_TSENS_IRQ_DISABLE(htsens, TSENS_IRQ_LOW_MASK_M | TSENS_IRQ_HI_MASK_M | TSENS_IRQ_EOC_MASK_M);
    __HAL_TSENS_IRQ_CLEAR(htsens, TSENS_CLEAR_IRQ_LOW_CLEAR_M | TSENS_CLEAR_IRQ_HI_CLEAR_M | TSENS_CLEAR_IRQ_EOC_CLEAR_M);
}

#endif // MIK32V0