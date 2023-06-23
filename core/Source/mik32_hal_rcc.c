#include "mik32_hal_rcc.h"


void HAL_RCC_OscEnable(uint32_t Oscillator)
{
    switch (Oscillator)
    {
    case RCC_OSCILLATORTYPE_HSI32M:
        WU->CLOCKS_SYS &= ~(1 << WU_CLOCKS_SYS_HSI32M_PD_S); //Включить HSI32M
        break;
    case RCC_OSCILLATORTYPE_OSC32M:
        WU->CLOCKS_SYS &= ~(1 << WU_CLOCKS_SYS_OSC32M_PD_S); // Включить OSC32M
        break;
    case RCC_OSCILLATORTYPE_LSI32K:
        WU->CLOCKS_BU &= ~(1 << WU_CLOCKS_BU_LSI32K_PD_S); // Включить LSI32K
        break;
    case RCC_OSCILLATORTYPE_OSC32K:
        WU->CLOCKS_BU &= ~(1 << WU_CLOCKS_BU_OSC32K_PD_S); // Включить OSC32K
        break;
    }
}

void HAL_RCC_OscDisable(uint32_t Oscillator)
{
    switch (Oscillator)
    {
    case RCC_OSCILLATORTYPE_HSI32M:
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_HSI32M_PD_S); //ВЫключить HSI32M
        break;
    case RCC_OSCILLATORTYPE_OSC32M:
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_OSC32M_PD_S); // Выключить OSC32M
        break;
    case RCC_OSCILLATORTYPE_LSI32K:
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_LSI32K_PD_S); // Выключить LSI32K
        break;
    case RCC_OSCILLATORTYPE_OSC32K:
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_OSC32K_PD_S); // Выключить OSC32K
        break;
    }
}

void HAL_RCC_SetOscSystem(uint32_t OscillatorSystem)
{
    /* Настройка источника тактирования системы */
    switch (OscillatorSystem)
    {
    case RCC_OSCILLATORTYPE_HSI32M:
        PM->AHB_CLK_MUX = PM_AHB_CLK_MUX_HSI32M_M;
        break;
    case RCC_OSCILLATORTYPE_OSC32M:
        PM->AHB_CLK_MUX = PM_AHB_CLK_MUX_OSC32M_M;
        break;
    case RCC_OSCILLATORTYPE_LSI32K:
        PM->AHB_CLK_MUX = PM_AHB_CLK_MUX_LSI32K_M;
        break;
    case RCC_OSCILLATORTYPE_OSC32K:
        PM->AHB_CLK_MUX = PM_AHB_CLK_MUX_OSC32K_M;
        break;
    }
}

void HAL_RCC_DividerAHB(uint32_t DividerAHB)
{
    PM->DIV_AHB = DividerAHB;
}

void HAL_RCC_DividerAPB_M(uint32_t DividerAPB_M)
{
    PM->DIV_APB_M = DividerAPB_M;
}

void HAL_RCC_DividerAPB_P(uint32_t DividerAPB_P)
{
    PM->DIV_APB_P = DividerAPB_P;
}  

void HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInit)
{

    /* Включить все источники тактирования */
    WU->CLOCKS_SYS &= ~(0b11 << WU_CLOCKS_SYS_OSC32M_PD_S); // Включить OSC32M и HSI32M
    WU->CLOCKS_BU &= ~(0b11 << WU_CLOCKS_BU_OSC32K_PD_S); // Включить OSC32K и LSI32K

    WU->CLOCKS_SYS = WU_CLOCKS_SYS_ADJ_HSI32M(RCC_OscInit->HSI32MCalibrationValue); // Поправочный коэффициент HSI32M
    WU->CLOCKS_BU = WU_CLOCKS_BU_ADJ_LSI32K(RCC_OscInit->LSI32KCalibrationValue); // Поправочный коэффициент LSI32K

    /* Настройка источника тактирования системы */
    HAL_RCC_SetOscSystem(RCC_OscInit->OscillatorSystem);

    /* Делители частоты */
    HAL_RCC_DividerAHB(RCC_OscInit->AHBDivider);
    HAL_RCC_DividerAPB_M(RCC_OscInit->APBMDivider);
    HAL_RCC_DividerAPB_P(RCC_OscInit->APBPDivider);

    /* Выбор источника тактирования RTC */
    HAL_RCC_RTCClock(RCC_OscInit->RTCClockSelection);

    /* Выбор источника тактирования RTC в составе ядра*/
    HAL_RCC_CPURTCClock(RCC_OscInit->RTCClockCPUSelection);

    /* Отключение неиспользуемых источников тактирования */
    /* Источники 32МГц */
    /* Внутренний */
    if(!(RCC_OscInit->OscillatorEnable & RCC_OSCILLATORTYPE_HSI32M))
    {
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_HSI32M_PD_S); //Выключить HSI32M
    }

    // /* Внешний */
    if(!(RCC_OscInit->OscillatorEnable & RCC_OSCILLATORTYPE_OSC32M))
    {
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_OSC32M_PD_S); //Выключить OSC32M
    }

    /* Источники 32кГц */
    /* Внутренний  */
    if(!(RCC_OscInit->OscillatorEnable & RCC_OSCILLATORTYPE_LSI32K))
    {
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_LSI32K_PD_S); // Выключить LSI32K
    }

    /* Внешний */
    if(!(RCC_OscInit->OscillatorEnable & RCC_OSCILLATORTYPE_OSC32K))
    {
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_OSC32K_PD_S); // Выключить OSC32K
    }

}

void HAL_RCC_ClockConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit)
{
    /* Управление тактированием устройств на шине AHB */
    PM->CLK_AHB_CLEAR = ~PeriphClkInit->PMClockAHB; // Выключение тактирования необходимых блоков
    PM->CLK_AHB_SET = PeriphClkInit->PMClockAHB; // включение тактирования необходимых блоков

    /* Управление тактированием устройств на шине APB_M */
    PM->CLK_APB_M_CLEAR = ~PeriphClkInit->PMClockAPB_M; // Выключение тактирования необходимых блоков
    PM->CLK_APB_M_SET = PeriphClkInit->PMClockAPB_M; // включение тактирования необходимых блоков

    /* Управление тактированием устройств на шине APB_P */
    PM->CLK_APB_P_CLEAR = ~PeriphClkInit->PMClockAPB_P; // Выключение тактирования необходимых блоков
    PM->CLK_APB_P_SET = PeriphClkInit->PMClockAPB_P; // включение тактирования необходимых блоков
}

void HAL_RCC_ClockSet(uint32_t Periphery)
{
    uint32_t mask = Periphery & HAL_CLOCK_MASK;
    Periphery &= ~HAL_CLOCK_MASK; 
    switch (mask)
    {
    case HAL_CLOCK_AHB_MASK:
        PM->CLK_AHB_SET |= Periphery; // включение тактирования необходимых блоков
        break;
    case HAL_CLOCK_APB_M_MASK:
        PM->CLK_APB_M_SET |= Periphery; // включение тактирования необходимых блоков
        break;
    case HAL_CLOCK_APB_P_MASK:
        PM->CLK_APB_P_SET |= Periphery; // включение тактирования необходимых блоков
        break;
    }
}

void HAL_RCC_ClockClear(uint32_t Periphery)
{
    uint32_t mask = Periphery & HAL_CLOCK_MASK;
    Periphery &= ~HAL_CLOCK_MASK;
    switch (mask)
    {
    case HAL_CLOCK_AHB_MASK:
        PM->CLK_AHB_CLEAR |= Periphery; // включение тактирования необходимых блоков
        break;
    case HAL_CLOCK_APB_M_MASK:
        PM->CLK_APB_M_CLEAR |= Periphery; // включение тактирования необходимых блоков
        break;
    case HAL_CLOCK_APB_P_MASK:
        PM->CLK_APB_P_CLEAR |= Periphery; // включение тактирования необходимых блоков
        break;
    }
}

void HAL_RCC_RTCClock(uint32_t Oscillator)
{
    if ((Oscillator != RCC_RTCCLKSOURCE_LSI32K) && (Oscillator != RCC_RTCCLKSOURCE_OSC32K))
    {
        return;
    }

    /* Выбор источника тактирования RTC */
    uint32_t ClockBUConfig = WU->CLOCKS_BU;
    ClockBUConfig &= ~WU_CLOCKS_BU_RTC_CLK_MUX_M;
    ClockBUConfig |= Oscillator << WU_CLOCKS_BU_RTC_CLK_MUX_S;
    WU->CLOCKS_BU = ClockBUConfig;

    WU->RTC_CONRTOL = WU_RTC_RESET_CLEAR_M;

}

void HAL_RCC_CPURTCClock(uint32_t Oscillator)
{
    /* Выбор источника тактирования RTC в составе ядра*/
    switch (Oscillator)
    {
    case RCC_RTCCLKCPUSOURCE_LSI32K:
        PM->CPU_RTC_CLK_MUX = PM_CPU_RTC_CLK_MUX_LSI32K;
        break;
    
    case RCC_RTCCLKCPUSOURCE_OSC32K:
        PM->CPU_RTC_CLK_MUX = PM_CPU_RTC_CLK_MUX_OSC32K;
        break;
    }
}
