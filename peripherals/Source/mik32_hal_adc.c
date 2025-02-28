#include "mik32_hal_adc.h"

__attribute__((weak)) void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_PCC_ANALOG_REGS_CLK_ENABLE();

    if ((hadc->Init.EXTClb == ADC_EXTCLB_ADCREF) && (hadc->Init.EXTRef == ADC_EXTREF_ON))
    {
#ifdef MIK32V0
        GPIO_InitStruct.Pin = GPIO_PIN_10;
#else  // MIK32V2
        GPIO_InitStruct.Pin = GPIO_PIN_11;
#endif // MIK32V0
    }

    GPIO_InitStruct.Mode = HAL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);

    switch (hadc->Init.Sel)
    {
    case ADC_CHANNEL0:
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL1:
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL2:
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL3:
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL4:
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL5:
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL6:
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL7:
        GPIO_InitStruct.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;
    }
}

void HAL_ADC_CLBEnable(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->REFV_CONFIG |= 1 << REF_CLB_EN_S;
}

void HAL_ADC_CLBDisable(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->REFV_CONFIG &= ~(1 << REF_CLB_EN_S);
}

void HAL_ADC_VCLBSet(ADC_HandleTypeDef *hadc, uint8_t v_coef)
{
    hadc->Instance->REFV_CONFIG &= ~(0xF << REF_CLB_VCOEF_S);
    hadc->Instance->REFV_CONFIG |= ((v_coef & 0xF) << REF_CLB_VCOEF_S);
}

void HAL_ADC_ICLBSet(ADC_HandleTypeDef *hadc, uint8_t i_coef)
{
    hadc->Instance->REFV_CONFIG &= ~(0xF << REF_CLB_ICOEF_S);
    hadc->Instance->REFV_CONFIG |= ((i_coef & 0xF) << REF_CLB_ICOEF_S);
}

void HAL_ADC_SAH_TIMESet(ADC_HandleTypeDef *hadc, uint8_t sah_time)
{
    hadc->Instance->ADC_CONFIG = (ANALOG_REG->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_READ_M)) |
                                 (sah_time << ADC_CONFIG_SAH_TIME_WRITE_S);
}

uint8_t HAL_ADC_SAH_TIMEGet(ADC_HandleTypeDef *hadc)
{
    return (hadc->Instance->ADC_CONFIG & ADC_CONFIG_SAH_TIME_READ_M) >> ADC_CONFIG_SAH_TIME_READ_S;
}

uint8_t HAL_ADC_CalculateSAH_TIME(uint32_t frequency)
{
    return frequency * 7 / 20000000 + 1;
}

void HAL_ADC_ResetEnable(ADC_HandleTypeDef *hadc)
{
#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG |= (1 << ADC_CONFIG_RN_S);
#else  // MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_READ_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_WRITE_M) |
                                 (1 << ADC_CONFIG_RN_S);
#endif // MIK32V0
}

void HAL_ADC_ResetDisable(ADC_HandleTypeDef *hadc)
{

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG &= ~(1 << ADC_CONFIG_RN_S);
#else  // MIK32V2
    hadc->Instance->ADC_CONFIG = ((hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_RN_M)) & (~ADC_CONFIG_SAH_TIME_READ_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_WRITE_M);
#endif // MIK32V0
}

void HAL_ADC_Disable(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->ADC_CONFIG &= ~(1 << ADC_CONFIG_EN_S);
    HAL_ADC_ResetDisable(hadc);
}

void HAL_ADC_Enable(ADC_HandleTypeDef *hadc)
{
#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG |= (1 << ADC_CONFIG_EN_S);
#else // MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_READ_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_WRITE_M) |
                                 (1 << ADC_CONFIG_EN_S);

#endif // MIK32V0
    HAL_ADC_ResetEnable(hadc);
}

void HAL_ADC_ChannelSet(ADC_HandleTypeDef *hadc)
{

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SEL_M)) | (hadc->Init.Sel << ADC_CONFIG_SEL_S); /* Настройка канала АЦП */
#else                                                                                                                       // MIK32V2
    hadc->Instance->ADC_CONFIG = ((hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_READ_M)) & (~ADC_CONFIG_SEL_M)) |
                                 (hadc->Init.Sel << ADC_CONFIG_SEL_S) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_WRITE_M);
#endif                                                                                                                      // MIK32V0
}

void HAL_ADC_Init(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_MspInit(hadc);
#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG = 0;
#else  // MIK32V2
    hadc->Instance->ADC_CONFIG = 0x0C00;
#endif // MIK32V0

    HAL_ADC_Enable(hadc);
    HAL_ADC_ChannelSet(hadc); /* Настройка канала АЦП. Перевод используемого вывода в аналоговый режим */

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG |= (hadc->Init.EXTRef << ADC_CONFIG_EXTEN_S) |    /* Настройка источника опорного напряжения */
                                  (hadc->Init.EXTClb << ADC_CONFIG_EXTPAD_EN_S); /* Выбор внешнего источника опорного напряжения */
#else                                                                            // MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_READ_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_WRITE_M) |
                                 (hadc->Init.EXTRef << ADC_CONFIG_EXTEN_S) |    /* Настройка источника опорного напряжения */
                                 (hadc->Init.EXTClb << ADC_CONFIG_EXTPAD_EN_S); /* Выбор внешнего источника опорного напряжения */
#endif                                                                           // MIK32V0

    /* Запуск одиночного измерения для переключения на выбранный канал */
    hadc->Instance->ADC_SINGLE = 1;
    HAL_ADC_WaitValid(hadc);
}

void HAL_ADC_Single(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->ADC_SINGLE = 1;
}

void HAL_ADC_ContinuousDisable(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->ADC_CONTINUOUS = 0;
}

void HAL_ADC_ContinuousEnable(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->ADC_CONTINUOUS = 1;
}

void HAL_ADC_WaitValid(ADC_HandleTypeDef *hadc)
{
    /* Ожидание актуальных данных - ADC_VALID = 1 */
    while (!(hadc->Instance->ADC_VALID))
        ;
}

uint16_t HAL_ADC_GetValue(ADC_HandleTypeDef *hadc)
{
    uint16_t value = hadc->Instance->ADC_VALUE;

    return value;
}

uint16_t HAL_ADC_WaitAndGetValue(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_WaitValid(hadc);

    uint16_t value = HAL_ADC_GetValue(hadc);

    return value;
}
