#include "mik32_hal_pcc.h"
#include "mik32_hal.h"

void HAL_PCC_OscEnable(uint32_t Oscillator)
{
    switch (Oscillator)
    {
    case PCC_OSCILLATORTYPE_HSI32M:
        WU->CLOCKS_SYS &= ~(1 << WU_CLOCKS_SYS_HSI32M_EN_S); // Включить HSI32M
        break;
    case PCC_OSCILLATORTYPE_OSC32M:
        WU->CLOCKS_SYS &= ~(1 << WU_CLOCKS_SYS_OSC32M_EN_S); // Включить OSC32M
        break;
    case PCC_OSCILLATORTYPE_LSI32K:
        WU->CLOCKS_BU &= ~(1 << WU_CLOCKS_BU_LSI32K_EN_S); // Включить LSI32K
        break;
    case PCC_OSCILLATORTYPE_OSC32K:
        WU->CLOCKS_BU &= ~(1 << WU_CLOCKS_BU_OSC32K_EN_S); // Включить OSC32K
        break;
    }
}

void HAL_PCC_OscDisable(uint32_t Oscillator)
{
    switch (Oscillator)
    {
    case PCC_OSCILLATORTYPE_HSI32M:
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_HSI32M_EN_S); // ВЫключить HSI32M
        break;
    case PCC_OSCILLATORTYPE_OSC32M:
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_OSC32M_EN_S); // Выключить OSC32M
        break;
    case PCC_OSCILLATORTYPE_LSI32K:
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_LSI32K_EN_S); // Выключить LSI32K
        break;
    case PCC_OSCILLATORTYPE_OSC32K:
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_OSC32K_EN_S); // Выключить OSC32K
        break;
    }
}

HAL_StatusTypeDef HAL_PCC_FreqMonRefSet(HAL_PCC_FreqMonitorSourceTypeDef Force32KClk)
{
    uint32_t clockswitch_timeout = 0;
    uint32_t ref_clk_m = 0;
    uint32_t ref_clk_flag_m = 0;

    switch (Force32KClk)
    {
    case PCC_FREQ_MONITOR_SOURCE_AUTO:
        WU->CLOCKS_SYS &= ~WU_CLOCKS_SYS_FORCE_32K_CLK_M;
        return HAL_OK;
    case PCC_FREQ_MONITOR_SOURCE_OSC32K:
        ref_clk_m = WU_CLOCKS_SYS_FORCE_32K_CLK_OSC32K_M;
        ref_clk_flag_m = PM_FREQ_STATUS_OSC32K_M;
        break;
    case PCC_FREQ_MONITOR_SOURCE_LSI32K:
        ref_clk_m = WU_CLOCKS_SYS_FORCE_32K_CLK_LSI32K_M;
        ref_clk_flag_m = PM_FREQ_STATUS_LSI32K_M;
        break;
    default:
        return HAL_ERROR;
    }

    while (!(PM->FREQ_STATUS & ref_clk_flag_m))
    {
        clockswitch_timeout++;
        if (clockswitch_timeout >= CLOCKSWITCH_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    WU->CLOCKS_SYS = (WU->CLOCKS_SYS & ~(WU_CLOCKS_SYS_FORCE_32K_CLK_M)) | ref_clk_m;
    for (volatile int i = 0; i < 100; i++)
        ;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_PCC_SetOscSystem(uint32_t OscillatorSystem, HAL_PCC_ForceOscSysTypeDef ForceOscSys)
{
    uint32_t clockswitch_timeout = 0;
    uint32_t sys_clk_m = 0;
    uint32_t osc_clk_flag_m = 0;

    /* Настройка источника тактирования системы */
    switch (OscillatorSystem)
    {
    case PCC_OSCILLATORTYPE_HSI32M:
        sys_clk_m = PM_AHB_CLK_MUX_HSI32M_M;
        osc_clk_flag_m = PM_FREQ_STATUS_HSI32M_M;
        break;
    case PCC_OSCILLATORTYPE_OSC32M:
        sys_clk_m = PM_AHB_CLK_MUX_OSC32M_M;
        osc_clk_flag_m = PM_FREQ_STATUS_OSC32M_M;
        break;
    case PCC_OSCILLATORTYPE_LSI32K:
        sys_clk_m = PM_AHB_CLK_MUX_LSI32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_LSI32K_M;
        break;
    case PCC_OSCILLATORTYPE_OSC32K:
        sys_clk_m = PM_AHB_CLK_MUX_OSC32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_OSC32K_M;
        break;
    default:
        return HAL_ERROR;
    }

    while (!(PM->FREQ_STATUS & osc_clk_flag_m))
    {
        clockswitch_timeout++;
        if (clockswitch_timeout >= CLOCKSWITCH_TIMEOUT_VALUE)
        {
            PM->AHB_CLK_MUX = sys_clk_m | PM_AHB_FORCE_MUX_UNFIXED;
            return HAL_TIMEOUT;
        }
    }

    PM->AHB_CLK_MUX = sys_clk_m | (ForceOscSys << PM_AHB_FORCE_MUX_S);

    for (volatile int i = 0; i < 100; i++)
        ;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_PCC_RTCClock(HAL_PCC_RTCClockSourceTypeDef Oscillator)
{
    uint32_t clockswitch_timeout = 0;
    uint32_t rtc_clk_m = 0;
    uint32_t osc_clk_flag_m = 0;

    switch (Oscillator)
    {
    case PCC_RTC_CLOCK_SOURCE_AUTO:
        WU->CLOCKS_BU &= ~WU_CLOCKS_BU_RTC_CLK_MUX_M;
        return HAL_OK;
    case PCC_RTC_CLOCK_SOURCE_LSI32K:
        rtc_clk_m = WU_CLOCKS_BU_RTC_CLK_MUX_LSI32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_LSI32K_M;
        break;
    case PCC_RTC_CLOCK_SOURCE_OSC32K:
        rtc_clk_m = WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_OSC32K_M;
        break;
    default:
        return HAL_ERROR;
    }

    while (!(PM->FREQ_STATUS & osc_clk_flag_m))
    {
        clockswitch_timeout++;
        if (clockswitch_timeout >= CLOCKSWITCH_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    /* Выбор источника тактирования RTC */
    WU->CLOCKS_BU = (WU->CLOCKS_BU & ~WU_CLOCKS_BU_RTC_CLK_MUX_M) | rtc_clk_m;
    WU->RTC_CONRTOL = WU_RTC_CONTROL_RESET_CLEAR_M;

    for (volatile int i = 0; i < 100; i++)
        ;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_PCC_CPURTCClock(HAL_PCC_CPURTCClockSourceTypeDef Oscillator)
{
    uint32_t clockswitch_timeout = 0;
    uint32_t rtc_clk_m = 0;
    uint32_t osc_clk_flag_m = 0;

    switch (Oscillator)
    {
    case PCC_CPU_RTC_CLOCK_SOURCE_LSI32K:
        rtc_clk_m = PM_CPU_RTC_CLK_MUX_LSI32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_LSI32K_M;
        break;
    case PCC_CPU_RTC_CLOCK_SOURCE_OSC32K:
        rtc_clk_m = PM_CPU_RTC_CLK_MUX_OSC32K_M;
        osc_clk_flag_m = PM_FREQ_STATUS_OSC32K_M;
        break;
    default:
        return HAL_ERROR;
    }

    while (!(PM->FREQ_STATUS & osc_clk_flag_m))
    {
        clockswitch_timeout++;
        if (clockswitch_timeout >= CLOCKSWITCH_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }
    
    PM->CPU_RTC_CLK_MUX = rtc_clk_m;

    for (volatile int i = 0; i < 100; i++)
        ;

    return HAL_OK;
}

void HAL_PCC_DividerAHB(uint32_t DividerAHB)
{
    PM->DIV_AHB = DividerAHB;
}

void HAL_PCC_DividerAPB_M(uint32_t DividerAPB_M)
{
    PM->DIV_APB_M = DividerAPB_M;
}

void HAL_PCC_DividerAPB_P(uint32_t DividerAPB_P)
{
    PM->DIV_APB_P = DividerAPB_P;
}

PCC_ConfigErrorsTypeDef HAL_PCC_Config(PCC_InitTypeDef *PCC_Init)
{
    PCC_ConfigErrorsTypeDef errors = {HAL_OK};

    /* Включить все источники тактирования */
    WU->CLOCKS_SYS &= ~(0b11 << WU_CLOCKS_SYS_OSC32M_EN_S); // Включить OSC32M и HSI32M
    WU->CLOCKS_BU &= ~(0b11 << WU_CLOCKS_BU_OSC32K_EN_S);   // Включить OSC32K и LSI32K

    WU->CLOCKS_SYS = WU_CLOCKS_SYS_ADJ_HSI32M(PCC_Init->HSI32MCalibrationValue); // Поправочный коэффициент HSI32M
    WU->CLOCKS_BU = WU_CLOCKS_BU_ADJ_LSI32K(PCC_Init->LSI32KCalibrationValue);   // Поправочный коэффициент LSI32K

    /* Опорный источник для монитора частоты */
    errors.FreqMonRef = HAL_PCC_FreqMonRefSet(PCC_Init->FreqMon.Force32KClk);
    
    /* Настройка источника тактирования системы */
    errors.SetOscSystem = HAL_PCC_SetOscSystem(PCC_Init->FreqMon.OscillatorSystem, PCC_Init->FreqMon.ForceOscSys);

    /* Делители частоты */
    HAL_PCC_DividerAHB(PCC_Init->AHBDivider);
    HAL_PCC_DividerAPB_M(PCC_Init->APBMDivider);
    HAL_PCC_DividerAPB_P(PCC_Init->APBPDivider);

    /* Выбор источника тактирования RTC */
    errors.RTCClock = HAL_PCC_RTCClock(PCC_Init->RTCClockSelection);
    
    /* Выбор источника тактирования RTC в составе ядра*/
    errors.CPURTCClock = HAL_PCC_CPURTCClock(PCC_Init->RTCClockCPUSelection);



    /* Отключение неиспользуемых источников тактирования */
    /* Источники 32МГц */
    /* Внутренний */
    if (!(PCC_Init->OscillatorEnable & PCC_OSCILLATORTYPE_HSI32M))
    {
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_HSI32M_EN_S); // Выключить HSI32M
    }

    // /* Внешний */
    if (!(PCC_Init->OscillatorEnable & PCC_OSCILLATORTYPE_OSC32M))
    {
        WU->CLOCKS_SYS |= (1 << WU_CLOCKS_SYS_OSC32M_EN_S); // Выключить OSC32M
    }

    /* Источники 32кГц */
    /* Внутренний  */
    if (!(PCC_Init->OscillatorEnable & PCC_OSCILLATORTYPE_LSI32K))
    {
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_LSI32K_EN_S); // Выключить LSI32K
    }

    /* Внешний */
    if (!(PCC_Init->OscillatorEnable & PCC_OSCILLATORTYPE_OSC32K))
    {
        WU->CLOCKS_BU |= (1 << WU_CLOCKS_BU_OSC32K_EN_S); // Выключить OSC32K
    }

    return errors;
}

void HAL_PCC_ClockConfig(PCC_PeriphCLKInitTypeDef *PeriphClkInit)
{
    /* Управление тактированием устройств на шине AHB */
    PM->CLK_AHB_CLEAR = ~PeriphClkInit->PMClockAHB; // Выключение тактирования необходимых блоков
    PM->CLK_AHB_SET = PeriphClkInit->PMClockAHB;    // включение тактирования необходимых блоков

    /* Управление тактированием устройств на шине APB_M */
    PM->CLK_APB_M_CLEAR = ~PeriphClkInit->PMClockAPB_M; // Выключение тактирования необходимых блоков
    PM->CLK_APB_M_SET = PeriphClkInit->PMClockAPB_M;    // включение тактирования необходимых блоков

    /* Управление тактированием устройств на шине APB_P */
    PM->CLK_APB_P_CLEAR = ~PeriphClkInit->PMClockAPB_P; // Выключение тактирования необходимых блоков
    PM->CLK_APB_P_SET = PeriphClkInit->PMClockAPB_P;    // включение тактирования необходимых блоков
}

void HAL_PCC_ClockSet(uint32_t Periphery)
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

void HAL_PCC_ClockClear(uint32_t Periphery)
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

uint32_t HAL_PCC_GetSysClockFreq()
{
    uint32_t system_clock = 0;

    switch (PM->AHB_CLK_MUX & PM_AHB_CLK_MUX_M)
    {
    case PM_AHB_CLK_MUX_OSC32M_M:
        system_clock = OSC_SYSTEM_VALUE;
        break;
    case PM_AHB_CLK_MUX_OSC32K_M:
        system_clock = OSC_CLOCK_VALUE;
        break;
    case PM_AHB_CLK_MUX_HSI32M_M:
        system_clock = HSI_VALUE;
        break;
    case PM_AHB_CLK_MUX_LSI32K_M:
        system_clock = LSI_VALUE;
        break;
    }

    return system_clock;
}