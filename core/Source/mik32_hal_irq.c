#include "mik32_hal_irq.h"

// #define MIK32_IRQ_DEBUG

void HAL_IRQ_EnableInterrupts()
{   
    set_csr(mstatus, MSTATUS_MIE);
    set_csr(mie, MIE_MEIE);
}

void HAL_IRQ_DisableInterrupts()
{    
    clear_csr(mie, MIE_MEIE);
}

void HAL_EPIC_MaskSet(uint32_t InterruptMask)
{
    EPIC->MASK_SET |= InterruptMask;
}

void HAL_EPIC_MaskClear(uint32_t InterruptMask)
{
    EPIC->MASK_CLEAR |= InterruptMask;
}

void HAL_EPIC_MaskLevelSet(uint32_t InterruptMask)
{
    EPIC->MASK_LEVEL_SET |= InterruptMask;
}

void HAL_EPIC_MaskLevelClear(uint32_t InterruptMask)
{
    EPIC->MASK_LEVEL_CLEAR |= InterruptMask;
}

void HAL_EPIC_Clear(uint32_t InterruptMask)
{
    EPIC->CLEAR = InterruptMask;
}

uint32_t HAL_EPIC_GetStatus()
{
    return EPIC->STATUS;
}

uint32_t HAL_EPIC_GetRawStatus()
{
    return EPIC->RAW_STATUS;
}

__attribute__ ((weak)) void Timer32_0_IRQHandler() {}
__attribute__ ((weak)) void UART_0_IRQHandler() {}
__attribute__ ((weak)) void UART_1_IRQHandler() {}
__attribute__ ((weak)) void SPI_0_IRQHandler() {}
__attribute__ ((weak)) void SPI_1_IRQHandler() {}
__attribute__ ((weak)) void GPIO_IRQHandler() {}
__attribute__ ((weak)) void I2C_0_IRQHandler() {}
__attribute__ ((weak)) void I2C_1_IRQHandler() {}
__attribute__ ((weak)) void WDT_IRQHandler() {}
__attribute__ ((weak)) void TIMER16_0_IRQHandler() {}
__attribute__ ((weak)) void TIMER16_1_IRQHandler() {}
__attribute__ ((weak)) void TIMER16_2_IRQHandler() {}
__attribute__ ((weak)) void TIMER32_1_IRQHandler() {}
__attribute__ ((weak)) void TIMER32_2_IRQHandler() {}
__attribute__ ((weak)) void SPIFI_IRQHandler() {}
__attribute__ ((weak)) void RTC_IRQHandler() {}
__attribute__ ((weak)) void EEPROM_IRQHandler() {}
__attribute__ ((weak)) void WDT_DOM3_IRQHandler() {}
__attribute__ ((weak)) void WDT_SPIFI_IRQHandler() {}
__attribute__ ((weak)) void WDT_EEPROM_IRQHandler() {}
__attribute__ ((weak)) void DMA_GLB_ERR_IRQHandler() {}
__attribute__ ((weak)) void DMA_CHANNELS_IRQHandler() {}
__attribute__ ((weak)) void FREQ_MON_IRQHandler() {}
__attribute__ ((weak)) void PVD_AVCC_UNDER_IRQHandler() {}
__attribute__ ((weak)) void PVD_AVCC_OVER_IRQHandler() {}
__attribute__ ((weak)) void PVD_VCC_UNDER_IRQHandler() {}
__attribute__ ((weak)) void PVD_VCC_OVER_IRQHandler() {}
__attribute__ ((weak)) void BATTERY_NON_GOOD_IRQHandler() {}
__attribute__ ((weak)) void BOR_IRQHandler() {}
__attribute__ ((weak)) void TSENS_IRQHandler() {}
__attribute__ ((weak)) void ADC_IRQHandler() {}


inline __attribute__((always_inline)) void Timer32_0_IT()
{
if (EPIC->RAW_STATUS & (1 << EPIC_TIMER32_0_INDEX)) 
{
    Timer32_0_IRQHandler();
}
}

inline __attribute__((always_inline)) void UART_0_IT()
{
if (EPIC->RAW_STATUS & (1 << EPIC_UART_0_INDEX)) 
{
    UART_0_IRQHandler();
}
}

inline __attribute__((always_inline)) void UART_1_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_UART_1_INDEX)) 
    {
        UART_1_IRQHandler();
    }
}

inline __attribute__((always_inline)) void SPI_0_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_SPI_0_INDEX)) 
    {
        SPI_0_IRQHandler();
    }
}

inline __attribute__((always_inline)) void SPI_1_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_SPI_1_INDEX)) 
    {
        SPI_1_IRQHandler();
    }
}

inline __attribute__((always_inline)) void GPIO_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_GPIO_IRQ_INDEX)) 
    {
        GPIO_IRQHandler();
    }
}

inline __attribute__((always_inline)) void I2C_0_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_I2C_0_INDEX)) 
    {
        I2C_0_IRQHandler();
    }
}

inline __attribute__((always_inline)) void I2C_1_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_I2C_1_INDEX)) 
    {
        I2C_1_IRQHandler();
    }
}

inline __attribute__((always_inline)) void WDT_IT()
{
    if (EPIC->STATUS & (1 << EPIC_WDT_INDEX)) 
    {
        WDT_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TIMER16_0_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TIMER16_0_INDEX)) 
    {
        TIMER16_0_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TIMER16_1_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TIMER16_1_INDEX)) 
    {
        TIMER16_1_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TIMER16_2_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TIMER16_2_INDEX)) 
    {
        TIMER16_2_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TIMER32_1_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TIMER32_1_INDEX)) 
    {
        TIMER32_1_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TIMER32_2_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TIMER32_2_INDEX)) 
    {
        TIMER32_2_IRQHandler();
    }
}

inline __attribute__((always_inline)) void SPIFI_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_SPIFI_INDEX)) 
    {
        SPIFI_IRQHandler();
    }
}

inline __attribute__((always_inline)) void RTC_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_RTC_INDEX)) 
    {
        RTC_IRQHandler();
    }
}

inline __attribute__((always_inline)) void EEPROM_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_EEPROM_INDEX)) 
    {
        EEPROM_IRQHandler();
    }
}

inline __attribute__((always_inline)) void WDT_DOM3_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_WDT_DOM3_INDEX)) 
    {
        WDT_DOM3_IRQHandler();
    }
}

inline __attribute__((always_inline)) void WDT_SPIFI_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_WDT_SPIFI_INDEX)) 
    {
        WDT_SPIFI_IRQHandler();
    }
}

inline __attribute__((always_inline)) void WDT_EEPROM_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_WDT_EEPROM_INDEX)) 
    {
        WDT_EEPROM_IRQHandler();
    }
}

inline __attribute__((always_inline)) void DMA_GLB_ERR_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_DMA_GLB_ERR_INDEX)) 
    {
        DMA_GLB_ERR_IRQHandler();
    }
}

inline __attribute__((always_inline)) void DMA_CHANNELS_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_DMA_CHANNELS_INDEX)) 
    {
        DMA_CHANNELS_IRQHandler();
    }
}

inline __attribute__((always_inline)) void FREQ_MON_IT()
{
    if (EPIC->STATUS & (1 << EPIC_FREQ_MON_INDEX)) 
    {
        FREQ_MON_IRQHandler();
    }
}

inline __attribute__((always_inline)) void PVD_AVCC_UNDER_IT()
{
    if (EPIC->STATUS & (1 << EPIC_PVD_AVCC_UNDER)) 
    {
        PVD_AVCC_UNDER_IRQHandler();
    }
}

inline __attribute__((always_inline)) void PVD_AVCC_OVER_IT()
{
    if (EPIC->STATUS & (1 << EPIC_PVD_AVCC_OVER)) 
    {
        PVD_AVCC_OVER_IRQHandler();
    }
}

inline __attribute__((always_inline)) void PVD_VCC_UNDER_IT()
{
    if (EPIC->STATUS & (1 << EPIC_PVD_VCC_UNDER)) 
    {
        PVD_VCC_UNDER_IRQHandler();
    }
}

inline __attribute__((always_inline)) void PVD_VCC_OVER_IT()
{
    if (EPIC->STATUS & (1 << EPIC_PVD_VCC_OVER)) 
    {
        PVD_VCC_OVER_IRQHandler();
    }
}

inline __attribute__((always_inline)) void BATTERY_NON_GOOD_IT()
{
    if (EPIC->STATUS & (1 << EPIC_BATTERY_NON_GOOD)) 
    {
        BATTERY_NON_GOOD_IRQHandler();
    }
}

inline __attribute__((always_inline)) void BOR_IT()
{
    if (EPIC->STATUS & (1 << EPIC_BOR_INDEX)) 
    {
        BOR_IRQHandler();
    }
}

inline __attribute__((always_inline)) void TSENS_IT()
{
    if (EPIC->RAW_STATUS & (1 << EPIC_TSENS_INDEX)) 
    {
        TSENS_IRQHandler();
    }
}

inline __attribute__((always_inline)) void ADC_IT()
{
    if (EPIC->STATUS & (1 << EPIC_ADC_INDEX)) 
    {
        ADC_IRQHandler();
    }
}







