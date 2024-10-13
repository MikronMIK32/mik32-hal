#ifndef MIK32_HAL_PCC
#define MIK32_HAL_PCC

#include "wakeup.h"
#include "power_manager.h"
#include "mik32_hal_def.h"
#include "mik32_memory_map.h"
#include "mik32_hal.h"

#define CLOCKSWITCH_TIMEOUT_VALUE 500000 /**< Стандартная задержка для ожидания детектирования источника тактирования, на который происходит переключение */

/**
 * @brief Источники тактирования.
 *
 * Используется как маска для включения источников частоты и 
 * для назначения приоритетного источника тактирования системы в мониторе частоты.
 */
typedef enum __HAL_PCC_OscillatorTypeTypeDef
{
    PCC_OSCILLATORTYPE_NONE     = 0b0000,       /**< Автоматический выбор источника тактирования RTC */
    PCC_OSCILLATORTYPE_HSI32M   = 0b0001,       /**< Внутренний источник тактирования 32МГц */ 
    PCC_OSCILLATORTYPE_OSC32M   = 0b0010,       /**< Внешний источник тактирования 32МГц */    
    PCC_OSCILLATORTYPE_LSI32K   = 0b0100,       /**< Внутренний источник тактирования 32КГц */ 
    PCC_OSCILLATORTYPE_OSC32K   = 0b1000,       /**< Внешний источник тактирования 32КГц */   
    PCC_OSCILLATORTYPE_ALL      = 0b1111        /**< Все источники */    
} HAL_PCC_OscillatorTypeTypeDef;

/**
 * @brief Принудительный выбор источника тактирования системы.
 * 
 * Используется для включения или отключения автоматического переключения источника тактирования системы.
 */
typedef enum __HAL_PCC_ForceOscSysTypeDef
{
	PCC_FORCE_OSC_SYS_UNFIXED = 0,   /**< Источник тактирования системы не выбирается принудительно */
	PCC_FORCE_OSC_SYS_FIXED = 1      /**< Источник тактирования системы выбирается принудительно */
} HAL_PCC_ForceOscSysTypeDef;

/**
 * @brief Опорный источник тактирования монитора частоты.
 * 
 * Используется для назначения приоритетного опорного источника тактирования монитора частоты.
 */
typedef enum __HAL_PCC_FreqMonitorSourceTypeDef
{
	PCC_FREQ_MONITOR_SOURCE_AUTO    = 0b00,     /**< Опорный источник частоты монитора частоты выбирается автоматически */
    PCC_FREQ_MONITOR_SOURCE_LSI32K  = 0b01,     /**< Опорный источник частоты монитора частоты принудительно выбран как LSI32K */
	PCC_FREQ_MONITOR_SOURCE_OSC32K  = 0b10      /**< Опорный источник частоты монитора частоты принудительно выбран как OSC32K */
} HAL_PCC_FreqMonitorSourceTypeDef;

/**
 * @brief Источники тактирования RTC.
 * 
 * Используются для назначения приоритетного источника тактирования модуля RTC.
 */
typedef enum __HAL_PCC_RTCClockSourceTypeDef
{
	PCC_RTC_CLOCK_SOURCE_AUTO    = 0b00,    /**< Источник тактирования RTC выбирается автоматически. Если присутствуют оба тактовых сигнала 32K, то выбирается LSI32K */
    PCC_RTC_CLOCK_SOURCE_LSI32K  = 0b01,    /**< Приоритетный источник тактирования RTC - LSI32K */
	PCC_RTC_CLOCK_SOURCE_OSC32K  = 0b10     /**< Приоритетный источник тактирования RTC - OSC32K */
} HAL_PCC_RTCClockSourceTypeDef;

/**
 * @brief Источники тактирования RTC в составе ядра.
 * 
 * Используются для назначения приоритетного источника тактирования RTC в составе ядра.
 */
typedef enum __HAL_PCC_CPURTCClockSourceTypeDef
{
	PCC_CPU_RTC_CLOCK_SOURCE_OSC32K  = 0,    /**< Источник тактирования RTC в составе ядра - OSC32K */
	PCC_CPU_RTC_CLOCK_SOURCE_LSI32K  = 1     /**< Источник тактирования RTC в составе ядра - LSI32K */
} HAL_PCC_CPURTCClockSourceTypeDef;


/** 
 * @name Шина AHB
 * @brief Макросы для включения и отключения тактирования периферии на шине AHB
    @{
*/

#define __HAL_PCC_CPU_CLK_ENABLE()      (PM->CLK_AHB_SET = PM_CLOCK_AHB_CPU_M)       /**< Включить  тактирование CPU */
#define __HAL_PCC_CPU_CLK_DISABLE()     (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_CPU_M)       /**< Выключить тактирование CPU */

#define __HAL_PCC_EEPROM_CLK_ENABLE()   (PM->CLK_AHB_SET = PM_CLOCK_AHB_EEPROM_M)    /**< Включить  тактирование EEPROM */
#define __HAL_PCC_EEPROM_CLK_DISABLE()  (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_EEPROM_M)    /**< Выключить тактирование EEPROM */

#define __HAL_PCC_RAM_CLK_ENABLE()      (PM->CLK_AHB_SET = PM_CLOCK_AHB_RAM_M)       /**< Включить  тактирование RAM */ 
#define __HAL_PCC_RAM_CLK_DISABLE()     (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_RAM_M)       /**< Выключить тактирование SPIFI */

#define __HAL_PCC_SPIFI_CLK_ENABLE()    (PM->CLK_AHB_SET = PM_CLOCK_AHB_SPIFI_M)     /**< Включить  тактирование SPIFI */
#define __HAL_PCC_SPIFI_CLK_DISABLE()   (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_SPIFI_M)     /**< Выключить тактирование SPIFI */

#define __HAL_PCC_TCB_CLK_ENABLE()      (PM->CLK_AHB_SET = PM_CLOCK_AHB_TCB_M)       /**< Включить  тактирование блока TCB */
#define __HAL_PCC_TCB_CLK_DISABLE()     (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_TCB_M)       /**< Выключить тактирование блока TCB */

#define __HAL_PCC_DMA_CLK_ENABLE()      (PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M)       /**< Включить  тактирование DMA */
#define __HAL_PCC_DMA_CLK_DISABLE()     (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_DMA_M)       /**< Выключить тактирование DMA */

#define __HAL_PCC_CRYPTO_CLK_ENABLE()   (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRYPTO_M)    /**< Включить  тактирование крипто-блока */
#define __HAL_PCC_CRYPTO_CLK_DISABLE()  (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_CRYPTO_M)    /**< Выключить тактирование крипто-блока */

#define __HAL_PCC_CRC32_CLK_ENABLE()    (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRC32_M)      /**< Включить  тактирование CRC */
#define __HAL_PCC_CRC32_CLK_DISABLE()   (PM->CLK_AHB_CLEAR = PM_CLOCK_AHB_CRC32_M)      /**< Выключить тактирование CRC */

/** @} */ 

/** 
 * @name Шина APB_M
 * @brief Макросы для включения и отключения тактирования периферии на шине APB_M
    @{
*/

#define __HAL_PCC_PM_CLK_ENABLE()                   (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PM_M)                   /**< Включить  тактирование Power manager */
#define __HAL_PCC_PM_CLK_DISABLE()                  (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PM_M)                 /**< Выключить тактирование Power manager */

#define __HAL_PCC_EPIC_CLK_ENABLE()                 (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_EPIC_M)                 /**< Включить  тактирование контроллера прерываний */
#define __HAL_PCC_EPIC_CLK_DISABLE()                (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_EPIC_M)               /**< Выключить тактирование контроллера прерываний */

#define __HAL_PCC_TIMER32_0_CLK_ENABLE()            (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M)            /**< Включить  тактирование TIMER32_0 */
#define __HAL_PCC_TIMER32_0_CLK_DISABLE()           (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M)          /**< Выключить тактирование TIMER32_0 */

#define __HAL_PCC_PAD_CONFIG_CLK_ENABLE()           (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PAD_CONFIG_M)           /**< Включить  тактирование контроллера выводов */
#define __HAL_PCC_PAD_CONFIG_CLK_DISABLE()          (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PAD_CONFIG_M)         /**< Выключить тактирование контроллера выводов */

#define __HAL_PCC_WDT_BUS_CLK_ENABLE()              (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_WDT_BUS_M)              /**< Включить  тактирование сторожевого таймера шины */
#define __HAL_PCC_WDT_BUS_CLK_DISABLE()             (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_WDT_BUS_M)            /**< Выключить тактирование сторожевого таймера шины */

#define __HAL_PCC_OTP_CONTROLLER_CLK_ENABLE()       (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_OTP_CONTROLLER_M)       /**< Включить  тактирование OTP */
#define __HAL_PCC_OTP_CONTROLLER_CLK_DISABLE()      (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_OTP_CONTROLLER_M)     /**< Выключить тактирование OTP */

#define __HAL_PCC_PVD_CONTROL_CLK_ENABLE()          (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PVD_CONTROL_M)          /**< Включить  тактирование Монитора напряжения */
#define __HAL_PCC_PVD_CONTROL_CLK_DISABLE()         (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PVD_CONTROL_M)        /**< Выключить тактирование Монитора напряжения */

#define __HAL_PCC_WU_CLK_ENABLE()                   (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_WU_M)                   /**< Включить  тактирование Wake up */
#define __HAL_PCC_WU_CLK_DISABLE()                  (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_WU_M)                 /**< Выключить тактирование Wake up */

#define __HAL_PCC_RTC_CLK_ENABLE()                  (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_RTC_M)                  /**< Включить  тактирование RTC */
#define __HAL_PCC_RTC_CLK_DISABLE()                 (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_RTC_M)                /**< Выключить тактирование RTC */

/** @} */ 


/** 
 * @name Шина APB_P
 * @brief Макросы для включения и отключения тактирования периферии на шине APB_P
    @{
*/

#define __HAL_PCC_WDT_CLK_ENABLE()              (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_WDT_M)              /**< Включить  тактирование сторожевого таймера */
#define __HAL_PCC_WDT_CLK_DISABLE()             (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_WDT_M)            /**< Выключить тактирование сторожевого таймера */

#define __HAL_PCC_UART_0_CLK_ENABLE()           (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_UART_0_M)           /**< Включить  тактирование UART_0 */
#define __HAL_PCC_UART_0_CLK_DISABLE()          (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_UART_0_M)         /**< Выключить тактирование UART_0 */

#define __HAL_PCC_UART_1_CLK_ENABLE()           (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_UART_1_M)           /**< Включить  тактирование UART_1 */
#define __HAL_PCC_UART_1_CLK_DISABLE()          (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_UART_1_M)         /**< Выключить тактирование UART_1 */

#define __HAL_PCC_TIMER16_0_CLK_ENABLE()        (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M)        /**< Включить  тактирование Timer16_0 */
#define __HAL_PCC_TIMER16_0_CLK_DISABLE()       (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_0_M)      /**< Выключить тактирование Timer16_0 */

#define __HAL_PCC_TIMER16_1_CLK_ENABLE()        (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M)        /**< Включить  тактирование Timer16_1 */
#define __HAL_PCC_TIMER16_1_CLK_DISABLE()       (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_1_M)      /**< Выключить тактирование Timer16_1 */

#define __HAL_PCC_TIMER16_2_CLK_ENABLE()        (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M)        /**< Включить  тактирование Timer16_2 */
#define __HAL_PCC_TIMER16_2_CLK_DISABLE()       (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_2_M)      /**< Выключить тактирование Timer16_2 */

#define __HAL_PCC_TIMER32_1_CLK_ENABLE()        (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M)        /**< Включить  тактирование Timer32_1 */
#define __HAL_PCC_TIMER32_1_CLK_DISABLE()       (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M)      /**< Выключить тактирование Timer32_1 */

#define __HAL_PCC_TIMER32_2_CLK_ENABLE()        (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M)        /**< Включить  тактирование Timer32_2 */
#define __HAL_PCC_TIMER32_2_CLK_DISABLE()       (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M)      /**< Выключить тактирование Timer32_2 */

#define __HAL_PCC_SPI_0_CLK_ENABLE()            (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_0_M)            /**< Включить  тактирование SPI_0 */
#define __HAL_PCC_SPI_0_CLK_DISABLE()           (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_SPI_0_M)          /**< Выключить тактирование SPI_0 */

#define __HAL_PCC_SPI_1_CLK_ENABLE()            (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M)            /**< Включить  тактирование SPI_1 */
#define __HAL_PCC_SPI_1_CLK_DISABLE()           (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_SPI_1_M)          /**< Выключить тактирование SPI_1 */

#define __HAL_PCC_I2C_0_CLK_ENABLE()            (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_I2C_0_M)            /**< Включить  тактирование I2C_0 */
#define __HAL_PCC_I2C_0_CLK_DISABLE()           (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_I2C_0_M)          /**< Выключить тактирование I2C_0 */

#define __HAL_PCC_I2C_1_CLK_ENABLE()            (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_I2C_1_M)            /**< Включить  тактирование I2C_1 */
#define __HAL_PCC_I2C_1_CLK_DISABLE()           (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_I2C_1_M)          /**< Выключить тактирование I2C_1 */

#define __HAL_PCC_GPIO_0_CLK_ENABLE()           (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_0_M)           /**< Включить  тактирование GPIO_0 */
#define __HAL_PCC_GPIO_0_CLK_DISABLE()          (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_0_M)         /**< Выключить тактирование GPIO_0 */

#define __HAL_PCC_GPIO_1_CLK_ENABLE()           (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_1_M)           /**< Включить  тактирование GPIO_1 */
#define __HAL_PCC_GPIO_1_CLK_DISABLE()          (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_1_M)         /**< Выключить тактирование GPIO_1 */

#define __HAL_PCC_GPIO_2_CLK_ENABLE()           (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_2_M)           /**< Включить  тактирование GPIO_2 */
#define __HAL_PCC_GPIO_2_CLK_DISABLE()          (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_2_M)         /**< Выключить тактирование GPIO_2 */

#define __HAL_PCC_ANALOG_REGS_CLK_ENABLE()      (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_ANALOG_REGS_M)      /**< Включить  тактирование аналоговых блоков */
#define __HAL_PCC_ANALOG_REGS_CLK_DISABLE()     (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_ANALOG_REGS_M)    /**< Выключить тактирование аналоговых блоков */

#define __HAL_PCC_GPIO_IRQ_CLK_ENABLE()         (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_IRQ_M)         /**< Включить  тактирование схемы формирования прерываний GPIO */
#define __HAL_PCC_GPIO_IRQ_CLK_DISABLE()        (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_IRQ_M)       /**< Выключить тактирование схемы формирования прерываний GPIO */

/** @} */

/**
 * @brief Структура с состояниями ошибок.
 * 
 * Используется только как возвращаемое значение функции HAL_PCC_Config(PCC_InitTypeDef *PCC_Init).
 */
typedef struct __PCC_ConfigErrorsTypeDef
{
    HAL_StatusTypeDef FreqMonRef;       /**< Ошибка, возвращаемая функцией HAL_PCC_FreqMonRefSet(HAL_PCC_FreqMonitorSourceTypeDef Force32KClk) */

    HAL_StatusTypeDef SetOscSystem;     /**< Ошибка, возвращаемая функцией HAL_PCC_SetOscSystem(uint32_t OscillatorSystem, HAL_PCC_ForceOscSysTypeDef ForceOscSys) */

    HAL_StatusTypeDef RTCClock;         /**< Ошибка, возвращаемая функцией HAL_PCC_RTCClock(HAL_PCC_RTCClockSourceTypeDef Oscillator) */

    HAL_StatusTypeDef CPURTCClock;      /**< Ошибка, возвращаемая функцией HAL_PCC_CPURTCClock(HAL_PCC_CPURTCClockSourceTypeDef Oscillator) */

} PCC_ConfigErrorsTypeDef;


/**
 * @brief Настройки монитора частоты
 */
typedef struct __PCC_FreqMonTypeDef
{
    uint8_t OscillatorSystem;                       /**<  Источник тактирования системы. Может принимать одно из значений HAL_PCC_OscillatorTypeTypeDef. */   

    HAL_PCC_ForceOscSysTypeDef ForceOscSys;         /**< Разрешение принудительного выбора источника тактирования системы. Может принимать одно из значений HAL_PCC_ForceOscSysTypeDef. */

    HAL_PCC_FreqMonitorSourceTypeDef Force32KClk;   /**< Опорный источник монитора частоты. Может принимать одно из значений HAL_PCC_FreqMonitorSourceTypeDef. */

} PCC_FreqMonTypeDef;

/**
 * @brief Настройки тактирования и монитора частоты
 */
typedef struct __PCC_InitTypeDef
{
    uint8_t OscillatorEnable;                               /**< Осциллятор, который должен быть включен. Этот параметр может быть маской из значений HAL_PCC_OscillatorTypeTypeDef. */

    PCC_FreqMonTypeDef FreqMon;                             /**< Настройки монитора частоты. */

    uint32_t AHBDivider;                                     /**< Делитель частоты AHB. Этот параметр должен быть числом между Min = 0 и Max = 255 */

    uint32_t APBMDivider;                                    /**< Делитель частоты APB_M. Этот параметр должен быть числом между Min = 0 и Max = 255 */

    uint32_t APBPDivider;                                    /**< Делитель частоты APB_P. Этот параметр должен быть числом между Min = 0 и Max = 255 */
    
    uint8_t HSI32MCalibrationValue;                         /**< Поправочный коэффициент HSI32M. Этот параметр должен быть числом между Min = 0 и Max = 255 */

    uint8_t LSI32KCalibrationValue;                         /**< Поправочный коэффициент LSI32K. Этот параметр должен быть числом между Min = 0 и Max = 255 */

    HAL_PCC_RTCClockSourceTypeDef RTCClockSelection;        /**<  Источник тактирования RTC. Может принимать одно из значений HAL_PCC_RTCClockSourceTypeDef. */

    HAL_PCC_CPURTCClockSourceTypeDef RTCClockCPUSelection;  /**<  Источник тактирования RTC в составе ядра. Может принимать одно из значений HAL_PCC_CPURTCClockSourceTypeDef. */

} PCC_InitTypeDef;



void HAL_PCC_OscEnable(HAL_PCC_OscillatorTypeTypeDef Oscillator);
void HAL_PCC_OscDisable(uint32_t Oscillator);
HAL_StatusTypeDef HAL_PCC_FreqMonRefSet(HAL_PCC_FreqMonitorSourceTypeDef Force32KClk);
HAL_StatusTypeDef HAL_PCC_SetOscSystem(uint32_t OscillatorSystem, HAL_PCC_ForceOscSysTypeDef ForceOscSys);
HAL_StatusTypeDef HAL_PCC_RTCClock(HAL_PCC_RTCClockSourceTypeDef Oscillator);
HAL_StatusTypeDef HAL_PCC_CPURTCClock(HAL_PCC_CPURTCClockSourceTypeDef Oscillator);
void HAL_PCC_DividerAHB(uint32_t DividerAHB);
void HAL_PCC_DividerAPB_M(uint32_t DividerAPB_M);
void HAL_PCC_DividerAPB_P(uint32_t DividerAPB_P);
PCC_ConfigErrorsTypeDef HAL_PCC_Config(PCC_InitTypeDef *PCC_Init);
uint32_t HAL_PCC_GetSysClockFreq();


#endif