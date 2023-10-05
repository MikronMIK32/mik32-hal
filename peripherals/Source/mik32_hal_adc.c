#include "mik32_hal_adc.h"

__attribute__((weak)) void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_PCC_ANALOG_REGS_CLK_ENABLE();
    
    if ((hadc->Init.EXTClb == ADC_EXTCLB_ADCREF) && (hadc->Init.EXTRef == ADC_EXTREF_ON))
    {
        #ifdef MIK32V0
        GPIO_InitStruct.Pin = PORT1_10;
        #endif

        #ifdef MIK32V2
        GPIO_InitStruct.Pin = PORT1_11;
        #endif
    }
    
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);

    switch (hadc->Init.Sel)
    {
    case ADC_CHANNEL0:
        GPIO_InitStruct.Pin = PORT1_5;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL1:
        GPIO_InitStruct.Pin = PORT1_7;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL2:
        GPIO_InitStruct.Pin = PORT0_2;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL3:
        GPIO_InitStruct.Pin = PORT0_4;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL4:
        GPIO_InitStruct.Pin = PORT0_7;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL5:
        GPIO_InitStruct.Pin = PORT0_9;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL6:
        GPIO_InitStruct.Pin = PORT0_11;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        break;

    case ADC_CHANNEL7:
        GPIO_InitStruct.Pin = PORT0_13;
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

void HAL_ADC_ResetEnable(ADC_HandleTypeDef *hadc)
{
#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG |= (1 << ADC_CONFIG_RESETN_S);
#endif
#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_M) |
                                 (1 << ADC_CONFIG_RESETN_S);
#endif
}

void HAL_ADC_ResetDisable(ADC_HandleTypeDef *hadc)
{

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG &= ~(1 << ADC_CONFIG_RESETN_S);
#endif

#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = ((hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_RESETN_M)) & (~ADC_CONFIG_SAH_TIME_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_M);
#endif
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
#endif
#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_M) |
                                 (1 << ADC_CONFIG_EN_S);

#endif
    HAL_ADC_ResetEnable(hadc);
}

void HAL_ADC_ChannelSet(ADC_HandleTypeDef *hadc)
{

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SEL_M)) | (hadc->Init.Sel << ADC_CONFIG_SEL_S); /* Настройка канала АЦП */
#endif

#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = ((hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_M)) & (~ADC_CONFIG_SEL_M)) |
                                 (hadc->Init.Sel << ADC_CONFIG_SEL_S) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_M);
#endif
}

void HAL_ADC_Init(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_MspInit(hadc);
#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG = 0;
#endif

#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = 0x3C00;
#endif

    HAL_ADC_Enable(hadc);

    HAL_ADC_ChannelSet(hadc); /* Настройка канала АЦП. Перевод используемого вывода в аналоговый режим */
    if ((hadc->Init.EXTRef == ADC_EXTREF_ON) || (hadc->Init.EXTClb == ADC_EXTCLB_ADCREF))
    {
        PAD_CONFIG->PORT_1_CFG |= (ADC_PORT_AS_FUNC3 << 2 * ADC_REF_PORT_1_10);
    }

#ifdef MIK32V0
    hadc->Instance->ADC_CONFIG |= (hadc->Init.EXTRef << ADC_CONFIG_EXTREF_S) |   /* Настройка источника опорного напряжения */
                                  (hadc->Init.EXTClb << ADC_CONFIG_EXTPAD_EN_S); /* Выбор внешнего источника опорного напряжения */
#endif

#ifdef MIK32V2
    hadc->Instance->ADC_CONFIG = (hadc->Instance->ADC_CONFIG & (~ADC_CONFIG_SAH_TIME_M)) |
                                 ((hadc->Instance->ADC_CONFIG >> 1) & ADC_CONFIG_SAH_TIME_M) |
                                 (hadc->Init.EXTRef << ADC_CONFIG_EXTREF_S) |   /* Настройка источника опорного напряжения */
                                 (hadc->Init.EXTClb << ADC_CONFIG_EXTPAD_EN_S); /* Выбор внешнего источника опорного напряжения */
#endif
}

void HAL_ADC_Single(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->ADC_SINGLE = 1;
}

void HAL_ADC_ContinuousDisabled(ADC_HandleTypeDef *hadc)
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
