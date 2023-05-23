#ifndef MIK32_HAL_IRQ
#define MIK32_HAL_IRQ

#include "epic.h"
#include "csr.h"
#include "scr1_csr_encoding.h"
#include "mcu32_memory_map.h"


/* Title: Макросы */

/*
 * Defines: Маска линии прерывания 
 *
 * 
 * HAL_EPIC_TIMER32_0_MASK       - Маска для линии прерывания Timer32_0 
 * HAL_EPIC_UART_0_MASK          - Маска для линии прерывания USART_0 
 * HAL_EPIC_UART_1_MASK          - Маска для линии прерывания USART_1 
 * HAL_EPIC_SPI_0_MASK           - Маска для линии прерывания SPI_0 
 * HAL_EPIC_SPI_1_MASK           - Маска для линии прерывания SPI_1 
 * HAL_EPIC_GPIO_IRQ_MASK        - Маска для линии прерывания GPIO 
 * HAL_EPIC_I2C_0_MASK           - Маска для линии прерывания I2C_0 
 * HAL_EPIC_I2C_1_MASK           - Маска для линии прерывания I2C_1 
 * HAL_EPIC_WDT_MASK             - Маска для линии прерывания Сторожевой таймер 
 * HAL_EPIC_TIMER16_0_MASK       - Маска для линии прерывания Timer16_0 
 * HAL_EPIC_TIMER16_1_MASK       - Маска для линии прерывания Timer16_1 
 * HAL_EPIC_TIMER16_2_MASK       - Маска для линии прерывания Timer16_2 
 * HAL_EPIC_TIMER32_1_MASK       - Маска для линии прерывания Timer32_1 
 * HAL_EPIC_TIMER32_2_MASK       - Маска для линии прерывания Timer32_2 
 * HAL_EPIC_SPIFI_MASK           - Маска для линии прерывания SPIFI 
 * HAL_EPIC_RTC_MASK             - Маска для линии прерывания RTC 
 * HAL_EPIC_EEPROM_MASK          - Маска для линии прерывания EEPROM 
 * HAL_EPIC_WDT_DOM3_MASK        - Маска для линии прерывания Сторожевой таймер шины (периферийные устройства) 
 * HAL_EPIC_WDT_SPIFI_MASK       - Маска для линии прерывания Сторожевой таймер шины (SPIFI) 
 * HAL_EPIC_WDT_EEPROM_MASK      - Маска для линии прерывания Сторожевой таймер шины (EEPROM) 
 * HAL_EPIC_DMA_GLB_ERR_MASK     - Маска для линии прерывания ПДП глобальное прерывание 
 * HAL_EPIC_DMA_CHANNELS_MASK    - Маска для линии прерывания ПДП локальное прерывание  
 * HAL_EPIC_FREQ_MON_MASK        - Маска для линии прерывания Монитор частоты  
 * HAL_EPIC_PVD_AVCC_UNDER       - Маска для линии прерывания Монитор напряжения AVCC (ниже порога) 
 * HAL_EPIC_PVD_AVCC_OVER        - Маска для линии прерывания Монитор напряжения AVCC (выше порога)  
 * HAL_EPIC_PVD_VCC_UNDER        - Маска для линии прерывания Монитор напряжения VCC (ниже порога)  
 * HAL_EPIC_PVD_VCC_OVER         - Маска для линии прерывания Монитор напряжения VCC (выше порога)  
 * HAL_EPIC_BATTERY_NON_GOOD     - Маска для линии прерывания Недостаточное напряжение батареи 
 * HAL_EPIC_BOR_MASK             - Маска для линии прерывания BrouwnOut детектор  
 * HAL_EPIC_TSENS_MASK           - Маска для линии прерывания Монитор температуры 
 * HAL_EPIC_ADC_MASK             - Маска для линии прерывания АЦП 
 *
 */
#define HAL_EPIC_TIMER32_0_MASK         ( 1 << EPIC_TIMER32_0_INDEX )
#define HAL_EPIC_UART_0_MASK            ( 1 << EPIC_UART_0_INDEX )
#define HAL_EPIC_UART_1_MASK            ( 1 << EPIC_UART_1_INDEX )
#define HAL_EPIC_SPI_0_MASK             ( 1 << EPIC_SPI_0_INDEX )
#define HAL_EPIC_SPI_1_MASK             ( 1 << EPIC_SPI_1_INDEX )
#define HAL_EPIC_GPIO_IRQ_MASK          ( 1 << EPIC_GPIO_IRQ_INDEX )
#define HAL_EPIC_I2C_0_MASK             ( 1 << EPIC_I2C_0_INDEX )
#define HAL_EPIC_I2C_1_MASK             ( 1 << EPIC_I2C_1_INDEX )
#define HAL_EPIC_WDT_MASK               ( 1 << EPIC_WDT_INDEX )
#define HAL_EPIC_TIMER16_0_MASK         ( 1 << EPIC_TIMER16_0_INDEX )
#define HAL_EPIC_TIMER16_1_MASK         ( 1 << EPIC_TIMER16_1_INDEX )
#define HAL_EPIC_TIMER16_2_MASK         ( 1 << EPIC_TIMER16_2_INDEX )
#define HAL_EPIC_TIMER32_1_MASK         ( 1 << EPIC_TIMER32_1_INDEX )
#define HAL_EPIC_TIMER32_2_MASK         ( 1 << EPIC_TIMER32_2_INDEX )
#define HAL_EPIC_SPIFI_MASK             ( 1 << EPIC_SPIFI_INDEX )
#define HAL_EPIC_RTC_MASK               ( 1 << EPIC_RTC_INDEX )
#define HAL_EPIC_EEPROM_MASK            ( 1 << EPIC_EEPROM_INDEX )
#define HAL_EPIC_WDT_DOM3_MASK          ( 1 << EPIC_WDT_DOM3_INDEX )
#define HAL_EPIC_WDT_SPIFI_MASK         ( 1 << EPIC_WDT_SPIFI_INDEX )
#define HAL_EPIC_WDT_EEPROM_MASK        ( 1 << EPIC_WDT_EEPROM_INDEX )
#define HAL_EPIC_DMA_GLB_ERR_MASK       ( 1 << EPIC_DMA_GLB_ERR_INDEX )
#define HAL_EPIC_DMA_CHANNELS_MASK      ( 1 << EPIC_DMA_CHANNELS_INDEX )
#define HAL_EPIC_FREQ_MON_MASK          ( 1 << EPIC_FREQ_MON_INDEX )
#define HAL_EPIC_PVD_AVCC_UNDER         ( 1 << EPIC_PVD_AVCC_UNDER )
#define HAL_EPIC_PVD_AVCC_OVER          ( 1 << EPIC_PVD_AVCC_OVER )
#define HAL_EPIC_PVD_VCC_UNDER          ( 1 << EPIC_PVD_VCC_UNDER )
#define HAL_EPIC_PVD_VCC_OVER           ( 1 << EPIC_PVD_VCC_OVER )
#define HAL_EPIC_BATTERY_NON_GOOD       ( 1 << EPIC_BATTERY_NON_GOOD )
#define HAL_EPIC_BOR_MASK               ( 1 << EPIC_BOR_INDEX )
#define HAL_EPIC_TSENS_MASK             ( 1 << EPIC_TSENS_INDEX )
#define HAL_EPIC_ADC_MASK               ( 1 << EPIC_ADC_INDEX )


/* Title: Функции */

/*
 * functions: Обработчики прерываний 
 *
 * 
 * Timer32_0_IRQHandler           - Функция обработчик прерывания Timer32_0
 * UART_0_IRQHandler              - Функция обработчик прерывания USART_0
 * UART_1_IRQHandler              - Функция обработчик прерывания USART_1
 * SPI_0_IRQHandler               - Функция обработчик прерывания SPI_0
 * SPI_1_IRQHandler               - Функция обработчик прерывания SPI_1
 * GPIO_IRQHandler                - Функция обработчик прерывания GPIO
 * I2C_0_IRQHandler               - Функция обработчик прерывания I2C_0
 * I2C_1_IRQHandler               - Функция обработчик прерывания I2C_1
 * WDT_IRQHandler                 - Функция обработчик прерывания Сторожевой таймер
 * TIMER16_0_IRQHandler           - Функция обработчик прерывания Timer16_0
 * TIMER16_1_IRQHandler           - Функция обработчик прерывания Timer16_1
 * TIMER16_2_IRQHandler           - Функция обработчик прерывания Timer16_2
 * TIMER32_1_IRQHandler           - Функция обработчик прерывания Timer32_1
 * TIMER32_2_IRQHandler           - Функция обработчик прерывания Timer32_2
 * SPIFI_IRQHandler               - Функция обработчик прерывания SPIFI
 * RTC_IRQHandler                 - Функция обработчик прерывания RTC
 * EEPROM_IRQHandler              - Функция обработчик прерывания EEPROM
 * WDT_DOM3_IRQHandler            - Функция обработчик прерывания Сторожевой таймер шины (периферийные устройства)
 * WDT_SPIFI_IRQHandler           - Функция обработчик прерывания Сторожевой таймер шины (SPIFI)
 * WDT_EEPROM_IRQHandler          - Функция обработчик прерывания Сторожевой таймер шины (EEPROM)
 * DMA_GLB_ERR_IRQHandler         - Функция обработчик прерывания ПДП глобальное прерывание
 * DMA_CHANNELS_IRQHandler        - Функция обработчик прерывания ПДП локальное прерывание 
 * FREQ_MON_IRQHandler            - Функция обработчик прерывания Монитор частоты 
 * PVD_AVCC_UNDER_IRQHandler      - Функция обработчик прерывания Монитор напряжения AVCC (ниже порога)
 * PVD_AVCC_OVER_IRQHandler       - Функция обработчик прерывания Монитор напряжения AVCC (выше порога) 
 * PVD_VCC_UNDER_IRQHandler       - Функция обработчик прерывания Монитор напряжения VCC (ниже порога) 
 * PVD_VCC_OVER_IRQHandler        - Функция обработчик прерывания Монитор напряжения VCC (выше порога) 
 * BATTERY_NON_GOOD_IRQHandler    - Функция обработчик прерывания Недостаточное напряжение батареи
 * BOR_IRQHandler                 - Функция обработчик прерывания BrouwnOut детектор 
 * TSENS_IRQHandler               - Функция обработчик прерывания Монитор температуры
 * ADC_IRQHandler                 - Функция обработчик прерывания АЦП
 *
 */
extern void Timer32_0_IRQHandler();
extern void UART_0_IRQHandler();
extern void UART_1_IRQHandler();
extern void SPI_0_IRQHandler();
extern void SPI_1_IRQHandler();
extern void GPIO_IRQHandler();
extern void I2C_0_IRQHandler();
extern void I2C_1_IRQHandler();
extern void WDT_IRQHandler();
extern void TIMER16_0_IRQHandler();
extern void TIMER16_1_IRQHandler();
extern void TIMER16_2_IRQHandler();
extern void TIMER32_1_IRQHandler();
extern void TIMER32_2_IRQHandler();
extern void SPIFI_IRQHandler();
extern void RTC_IRQHandler();
extern void EEPROM_IRQHandler();
extern void WDT_DOM3_IRQHandler();
extern void WDT_SPIFI_IRQHandler();
extern void WDT_EEPROM_IRQHandler();
extern void DMA_GLB_ERR_IRQHandler();
extern void DMA_CHANNELS_IRQHandler();
extern void FREQ_MON_IRQHandler();
extern void PVD_AVCC_UNDER_IRQHandler();
extern void PVD_AVCC_OVER_IRQHandler();
extern void PVD_VCC_UNDER_IRQHandler();
extern void PVD_VCC_OVER_IRQHandler();
extern void BATTERY_NON_GOOD_IRQHandler();
extern void BOR_IRQHandler();
extern void TSENS_IRQHandler();
extern void ADC_IRQHandler();

/*
 * Function: HAL_IRQ_EnableInterrupts
 * Разрешить глобальные прерывания.
 * 
 * Разрешается машинное программное прерывание.
 *
 * Returns:
 * void.
 */
void HAL_IRQ_EnableInterrupts();

/*
 * Function: HAL_IRQ_DisableInterrupts
 * Запретить машинное программное прерывание.
 * 
 * Глобальные прерывания не запрещаются.
 *
 * Returns:
 * void.
 */
void HAL_IRQ_DisableInterrupts();
/* Прерывание по фронту */

/*
 * Function: HAL_EPIC_MaskSet
 * Задать маску разрешенных линий прерываний по фронту
 *
 * Parameters:
 * InterruptMask - Маска разрешенных линий прерываний
 *
 * Returns:
 * void.
 */
void HAL_EPIC_MaskSet(uint32_t InterruptMask);

/*
 * Function: HAL_EPIC_MaskClear
 * Сбросить маску разрешенный линий прерываний по фронту
 *
 * Parameters:
 * InterruptMask - Маска сбрасываемых линий прерываний
 *
 * Returns:
 * void.
 */
void HAL_EPIC_MaskClear(uint32_t InterruptMask);

/*
 * Function: HAL_EPIC_MaskSet
 * Задать маску разрешенных линий прерываний по уровню
 *
 * Parameters:
 * InterruptMask - Маска разрешенных линий прерываний
 *
 * Returns:
 * void.
 */
void HAL_EPIC_MaskLevelSet(uint32_t InterruptMask);

/*
 * Function: HAL_EPIC_MaskClear
 * Сбросить маску разрешенный линий прерываний по уровню
 *
 * Parameters:
 * InterruptMask - Маска сбрасываемых линий прерываний
 *
 * Returns:
 * void.
 */
void HAL_EPIC_MaskLevelClear(uint32_t InterruptMask);

/*
 * Function: HAL_EPIC_Clear
 * Сбросить прерывания
 *
 * Parameters:
 * InterruptMask - Маска сбрасываемых линий прерываний
 *
 * Returns:
 * void.
 */
void HAL_EPIC_Clear(uint32_t InterruptMask);

/*
 * Function: HAL_EPIC_GetStatus
 * Получить текущий статус прерываний в соответствии с маской разрешенных прерываний
 *
 * Returns:
 * (uint32_t ) - Текущий статус прерываний в соответствии с маской разрешенных прерываний
 */
uint32_t HAL_EPIC_GetStatus();

/*
 * Function: HAL_EPIC_GetRawStatus
 * Получить текущий статус прерываний
 * 
 * В данном статусе маска разрешенных прерываний не учитывается
 *
 * Returns:
 * (uint32_t ) - текущий статус прерываний
 */
uint32_t HAL_EPIC_GetRawStatus();


void Timer32_0_IT();
void UART_0_IT();
void UART_1_IT();
void SPI_0_IT();
void SPI_1_IT();
void GPIO_IT();
void I2C_0_IT();
void I2C_1_IT();
void WDT_IT();
void TIMER16_0_IT();
void TIMER16_1_IT();
void TIMER16_2_IT();
void TIMER32_1_IT();
void TIMER32_2_IT();
void SPIFI_IT();
void RTC_IT();
void EEPROM_IT();
void WDT_DOM3_IT();
void WDT_SPIFI_IT();
void WDT_EEPROM_IT();
void DMA_GLB_ERR_IT();
void DMA_CHANNELS_IT();
void FREQ_MON_IT();
void PVD_AVCC_UNDER_IT();
void PVD_AVCC_OVER_IT();
void PVD_VCC_UNDER_IT();
void PVD_VCC_OVER_IT();
void BATTERY_NON_GOOD_IT();
void BOR_IT();
void TSENS_IT();
void ADC_IT();


#endif