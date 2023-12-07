#ifndef MIK32_HAL_PCC
#define MIK32_HAL_PCC

#include "wakeup.h"
#include "power_manager.h"
#include "mik32_hal_def.h"
#include "mcu32_memory_map.h"

#define CLOCKSWITCH_TIMEOUT_VALUE 500000

/* Источники тактирования */   
typedef enum __HAL_PCC_OscillatorTypeTypeDef
{
    PCC_OSCILLATORTYPE_NONE     = 0b0000,       /* Автоматический выбор источника тактирования RTC */
    PCC_OSCILLATORTYPE_HSI32M   = 0b0001,       /* Внутренний источник тактирования 32МГц */ 
    PCC_OSCILLATORTYPE_OSC32M   = 0b0010,       /* Внешний источник тактирования 32МГц */    
    PCC_OSCILLATORTYPE_LSI32K   = 0b0100,       /* Внутренний источник тактирования 32КГц */ 
    PCC_OSCILLATORTYPE_OSC32K   = 0b1000,       /* Внешний источник тактирования 32КГц */   
    PCC_OSCILLATORTYPE_ALL      = 0b1111       /* Все источники */    
} HAL_PCC_OscillatorTypeTypeDef;

typedef enum __HAL_PCC_RTCClockSourceTypeDef
{
	PCC_RTC_CLOCK_SOURCE_AUTO    = 0b00,    /* Источник тактирования RTC выбирается автоматически. Если присутствуют оба тактовых сигнала 32K, то выбирается LSI32K */
    PCC_RTC_CLOCK_SOURCE_LSI32K  = 0b01,    /* Приоритетный источник тактирования RTC - LSI32K */
	PCC_RTC_CLOCK_SOURCE_OSC32K  = 0b10     /* Приоритетный источник тактирования RTC - OSC32K */
} HAL_PCC_RTCClockSourceTypeDef;

typedef enum HAL_PCC_CPURTCClockSourceTypeDef
{
	PCC_CPU_RTC_CLOCK_SOURCE_OSC32K  = 0,    /* Источник тактирования RTC в составе ядра - OSC32K */
	PCC_CPU_RTC_CLOCK_SOURCE_LSI32K  = 1     /* Источник тактирования RTC в составе ядра - LSI32K */
} HAL_PCC_CPURTCClockSourceTypeDef;

typedef enum __HAL_PCC_FreqMonitorSourceTypeDef
{
	PCC_FREQ_MONITOR_SOURCE_AUTO    = 0b00,     /* Опорный источник частоты монитора частоты выбирается автоматически */
    PCC_FREQ_MONITOR_SOURCE_LSI32K  = 0b01,     /* Опорный источник частоты монитора частоты принудительно выбран как LSI32K */
	PCC_FREQ_MONITOR_SOURCE_OSC32K  = 0b10      /* Опорный источник частоты монитора частоты принудительно выбран как OSC32K */
} HAL_PCC_FreqMonitorSourceTypeDef;

typedef enum __HAL_PCC_ForceOscSysTypeDef
{
	PCC_FORCE_OSC_SYS_UNFIXED = 0,   /* Источник тактирования системы не выбирается принудительно */
	PCC_FORCE_OSC_SYS_FIXED = 1     /* Источник тактирования системы выбирается принудительно */
} HAL_PCC_ForceOscSysTypeDef;

/* Источники тактирования RTC */
#define PCC_RTCCLKSOURCE_NO_CLK            0b00          /* Нет источника */
#define PCC_RTCCLKSOURCE_LSI32K            0b01          /* LSI32K - источник тактирования RTC */
#define PCC_RTCCLKSOURCE_OSC32K            0b10          /* OSC32K - источник тактирования RTC */

/* Источник тактирования RTC в составе ядра*/
#define PCC_RTCCLKCPUSOURCE_NO_CLK         0b00          /* Нет источника */
#define PCC_RTCCLKCPUSOURCE_LSI32K         0b01          /* LSI32K - источник тактирования RTC */
#define PCC_RTCCLKCPUSOURCE_OSC32K         0b10          /* OSC32K - источник тактирования RTC */

#define PMCLOCKAHB_DEFAULT                 0x1F          /* Ядро; RAM; EEPROm; SPIFI; DMA */
#define PMCLOCKAPB_M_DEFAULT               0x89          /* Блок управления питанием (PM - Power Monitor); Контроллер выводов; RTC батарейного домена*/
#define PMCLOCKAPB_P_DEFAULT               0x00

#define PM_CPU_RTC_CLK_MUX_OSC32K          0x00          /* OSC32K -источник тактирования RTC в составе ядра*/
#define PM_CPU_RTC_CLK_MUX_LSI32K          0x01          /* LSI32K -источник тактирования RTC в составе ядра*/

/* Тактирование различных модулей. На 30-м и 31-м бите стоит номер, который соответствует шине AHB, APB_M и APB_P */
#define HAL_CLOCK_MASK              (0b11 << 30)   
/* AHB. 31-й бит = 0, 30-й бит = 0 */
#define HAL_CLOCK_AHB_MASK              (0b00 << 30)                                      /* Идентификатор AHB шины */
#define HAL_CLOCK_AHB_CPU                   (PM_CLOCK_AHB_CPU_M         | HAL_CLOCK_AHB_MASK)    /* Тактирование ядра */
#define HAL_CLOCK_AHB_EEPROM                (PM_CLOCK_AHB_EEPROM_M      | HAL_CLOCK_AHB_MASK)    /* Тактирование EEPROM */
#define HAL_CLOCK_AHB_RAM                   (PM_CLOCK_AHB_RAM_M         | HAL_CLOCK_AHB_MASK)    /* Тактирование RAM */
#define HAL_CLOCK_AHB_SPIFI                 (PM_CLOCK_AHB_SPIFI_M       | HAL_CLOCK_AHB_MASK)    /* Тактирование SPIFI */
#define HAL_CLOCK_AHB_TCB                   (PM_CLOCK_AHB_TCB_M         | HAL_CLOCK_AHB_MASK)    /* Тактирование TCB */
#define HAL_CLOCK_AHB_DMA                   (PM_CLOCK_AHB_DMA_M         | HAL_CLOCK_AHB_MASK)    /* Тактирование DMA */
#define HAL_CLOCK_AHB_CRYPTO                (PM_CLOCK_AHB_CRYPTO_M      | HAL_CLOCK_AHB_MASK)    /* Тактирование ускорителя криптографии */
#define HAL_CLOCK_AHB_CRC32                 (PM_CLOCK_AHB_CRC32_M       | HAL_CLOCK_AHB_MASK)    /* Тактирование CRC32 */
/* APB_M. 31-й бит = 0, 30-й бит = 1 */ 
#define HAL_CLOCK_APB_M_MASK            (0b01 << 30)                                             /* Идентификатор APB_M шины */
#define HAL_CLOCK_APB_M_PM                    (PM_CLOCK_APB_M_PM_M                  | HAL_CLOCK_APB_M_MASK) /* Тактирование блока управления питанием (PM - Power Monitor) */
#define HAL_CLOCK_APB_M_EPIC                  (PM_CLOCK_APB_M_EPIC_M                | HAL_CLOCK_APB_M_MASK) /* Тактирование контроллера прерываний EPIC */
#define HAL_CLOCK_APB_M_TIMER32_0             (PM_CLOCK_APB_M_TIMER32_0_M           | HAL_CLOCK_APB_M_MASK) /* Тактирование Timer32_0 */
#define HAL_CLOCK_APB_M_PAD_CONFIG            (PM_CLOCK_APB_M_PAD_CONFIG_M          | HAL_CLOCK_APB_M_MASK) /* Тактирование контроллера выводов */
#define HAL_CLOCK_APB_M_WDT_BUS               (PM_CLOCK_APB_M_WDT_BUS_M             | HAL_CLOCK_APB_M_MASK) /* Тактирование сторожевого таймера шины */
#define HAL_CLOCK_APB_M_OTP_CONTROLLER        (PM_CLOCK_APB_M_OTP_CONTROLLER_M      | HAL_CLOCK_APB_M_MASK) /* Тактирование OTP */
#define HAL_CLOCK_APB_M_PVD_CONTROL           (PM_CLOCK_APB_M_PVD_CONTROL_M         | HAL_CLOCK_APB_M_MASK) /* Тактирование монитора питания системного домена */
#define HAL_CLOCK_APB_M_WU                    (PM_CLOCK_APB_M_WU_M                  | HAL_CLOCK_APB_M_MASK) /* Тактирование блока WU батарейного домена */
#define HAL_CLOCK_APB_M_RTC                   (PM_CLOCK_APB_M_RTC_M                 | HAL_CLOCK_APB_M_MASK) /* Тактирование RTC батарейного домена */
/* APB_P. 31-й бит = 1, 30-й бит = 0 */ 
#define HAL_CLOCK_APB_P_MASK            (0b10 << 30)                                        /* Идентификатор APB_P шины */
#define HAL_CLOCK_APB_P_WDT                   (PM_CLOCK_APB_P_WDT_M             | HAL_CLOCK_APB_P_MASK) /* Тактирование сторожевого таймера */
#define HAL_CLOCK_APB_P_UART_0                (PM_CLOCK_APB_P_UART_0_M          | HAL_CLOCK_APB_P_MASK) /*Тактирование UART_0 */
#define HAL_CLOCK_APB_P_UART_1                (PM_CLOCK_APB_P_UART_1_M          | HAL_CLOCK_APB_P_MASK) /*Тактирование UART_1 */
#define HAL_CLOCK_APB_P_TIMER16_0             (PM_CLOCK_APB_P_TIMER16_0_M       | HAL_CLOCK_APB_P_MASK) /*Тактирование Timer16_0 */
#define HAL_CLOCK_APB_P_TIMER16_1             (PM_CLOCK_APB_P_TIMER16_1_M       | HAL_CLOCK_APB_P_MASK) /*Тактирование Timer16_1 */
#define HAL_CLOCK_APB_P_TIMER16_2             (PM_CLOCK_APB_P_TIMER16_2_M       | HAL_CLOCK_APB_P_MASK) /*Тактирование Timer 16_2 */
#define HAL_CLOCK_APB_P_TIMER32_1             (PM_CLOCK_APB_P_TIMER32_1_M       | HAL_CLOCK_APB_P_MASK) /*Тактирование Timer32_1 */ 
#define HAL_CLOCK_APB_P_TIMER32_2             (PM_CLOCK_APB_P_TIMER32_2_M       | HAL_CLOCK_APB_P_MASK) /*Тактирование Timer32_2 */
#define HAL_CLOCK_APB_P_SPI_0                 (PM_CLOCK_APB_P_SPI_0_M           | HAL_CLOCK_APB_P_MASK) /*Тактирование SPI_0 */
#define HAL_CLOCK_APB_P_SPI_1                 (PM_CLOCK_APB_P_SPI_1_M           | HAL_CLOCK_APB_P_MASK) /*Тактирование SPI_1 */
#define HAL_CLOCK_APB_P_I2C_0                 (PM_CLOCK_APB_P_I2C_0_M           | HAL_CLOCK_APB_P_MASK) /*Тактирование I2C_0 */
#define HAL_CLOCK_APB_P_I2C_1                 (PM_CLOCK_APB_P_I2C_1_M           | HAL_CLOCK_APB_P_MASK) /*Тактирование I2C_1 */
#define HAL_CLOCK_APB_P_GPIO_0                (PM_CLOCK_APB_P_GPIO_0_M          | HAL_CLOCK_APB_P_MASK) /*Тактирование GPIO_0 */
#define HAL_CLOCK_APB_P_GPIO_1                (PM_CLOCK_APB_P_GPIO_1_M          | HAL_CLOCK_APB_P_MASK) /*Тактирование GPIO_1 */
#define HAL_CLOCK_APB_P_GPIO_2                (PM_CLOCK_APB_P_GPIO_2_M          | HAL_CLOCK_APB_P_MASK) /*Тактирование GPIO_2 */
#define HAL_CLOCK_APB_P_ANALOG_REGS           (PM_CLOCK_APB_P_ANALOG_REGS_M     | HAL_CLOCK_APB_P_MASK) /*Тактирование регистров аналоговых блоков */
#define HAL_CLOCK_APB_P_GPIO_IRQ              (PM_CLOCK_APB_P_GPIO_IRQ_M        | HAL_CLOCK_APB_P_MASK) /*Тактирование регистров аналоговых блоков */



/* AHB */
#define __HAL_PCC_CPU_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CPU_M)
#define __HAL_PCC_CPU_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CPU_M)

#define __HAL_PCC_EEPROM_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_EEPROM_M)
#define __HAL_PCC_EEPROM_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_EEPROM_M)

#define __HAL_PCC_RAM_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_RAM_M)
#define __HAL_PCC_RAM_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_RAM_M)

#define __HAL_PCC_SPIFI_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_SPIFI_M)
#define __HAL_PCC_SPIFI_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_SPIFI_M)

#define __HAL_PCC_TCB_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_TCB_M)
#define __HAL_PCC_TCB_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_TCB_M)

#define __HAL_PCC_DMA_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M)
#define __HAL_PCC_DMA_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M)

#define __HAL_PCC_CRYPTO_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRYPTO_M)
#define __HAL_PCC_CRYPTO_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRYPTO_M)

#define __HAL_PCC_CRC32_CLK_ENABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRC32_M)
#define __HAL_PCC_CRC32_CLK_DISABLE() (PM->CLK_AHB_SET = PM_CLOCK_AHB_CRC32_M)

/* APB_M */
#define __HAL_PCC_PM_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PM_M)
#define __HAL_PCC_PM_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PM_M)

#define __HAL_PCC_EPIC_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_EPIC_M)
#define __HAL_PCC_EPIC_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_EPIC_M)

#define __HAL_PCC_TIMER32_0_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M)
#define __HAL_PCC_TIMER32_0_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M)

#define __HAL_PCC_PAD_CONFIG_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PAD_CONFIG_M)
#define __HAL_PCC_PAD_CONFIG_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PAD_CONFIG_M)

#define __HAL_PCC_WDT_BUS_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_WDT_BUS_M)
#define __HAL_PCC_WDT_BUS_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_WDT_BUS_M)

#define __HAL_PCC_OTP_CONTROLLER_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_OTP_CONTROLLER_M)
#define __HAL_PCC_OTP_CONTROLLER_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_OTP_CONTROLLER_M)

#define __HAL_PCC_PVD_CONTROL_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PVD_CONTROL_M)
#define __HAL_PCC_PVD_CONTROL_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_PVD_CONTROL_M)

#define __HAL_PCC_WU_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_WU_M)
#define __HAL_PCC_WU_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_WU_M)

#define __HAL_PCC_RTC_CLK_ENABLE() (PM->CLK_APB_M_SET = PM_CLOCK_APB_M_RTC_M)
#define __HAL_PCC_RTC_CLK_DISABLE() (PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_RTC_M)

/* APB_P */
#define __HAL_PCC_WDT_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_WDT_M)
#define __HAL_PCC_WDT_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_WDT_M)

#define __HAL_PCC_UART_0_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_UART_0_M)
#define __HAL_PCC_UART_0_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_UART_0_M)

#define __HAL_PCC_UART_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_UART_1_M)
#define __HAL_PCC_UART_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_UART_1_M)

#define __HAL_PCC_TIMER16_0_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M)
#define __HAL_PCC_TIMER16_0_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_0_M)

#define __HAL_PCC_TIMER16_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M)
#define __HAL_PCC_TIMER16_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_1_M)

#define __HAL_PCC_TIMER16_2_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M)
#define __HAL_PCC_TIMER16_2_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_2_M)

#define __HAL_PCC_TIMER32_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M)
#define __HAL_PCC_TIMER32_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M)

#define __HAL_PCC_TIMER32_2_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M)
#define __HAL_PCC_TIMER32_2_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M)

#define __HAL_PCC_SPI_0_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_0_M)
#define __HAL_PCC_SPI_0_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_SPI_0_M)

#define __HAL_PCC_SPI_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M)
#define __HAL_PCC_SPI_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_SPI_1_M)

#define __HAL_PCC_I2C_0_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_I2C_0_M)
#define __HAL_PCC_I2C_0_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_I2C_0_M)

#define __HAL_PCC_I2C_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_I2C_1_M)
#define __HAL_PCC_I2C_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_I2C_1_M)

#define __HAL_PCC_GPIO_0_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_0_M)
#define __HAL_PCC_GPIO_0_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_0_M)

#define __HAL_PCC_GPIO_1_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_1_M)
#define __HAL_PCC_GPIO_1_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_1_M)

#define __HAL_PCC_GPIO_2_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_2_M)
#define __HAL_PCC_GPIO_2_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_2_M)

#define __HAL_PCC_ANALOG_REGS_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_ANALOG_REGS_M)
#define __HAL_PCC_ANALOG_REGS_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_ANALOG_REGS_M)

#define __HAL_PCC_GPIO_IRQ_CLK_ENABLE() (PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_IRQ_M)
#define __HAL_PCC_GPIO_IRQ_CLK_DISABLE() (PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_GPIO_IRQ_M)


typedef struct __PCC_ConfigErrorsTypeDef
{
    HAL_StatusTypeDef FreqMonRef;

    HAL_StatusTypeDef SetOscSystem;

    HAL_StatusTypeDef RTCClock;

    HAL_StatusTypeDef CPURTCClock;

} PCC_ConfigErrorsTypeDef;


typedef struct __PCC_FreqMonTypeDef
{
    uint8_t OscillatorSystem;              /*  Источник тактирования системы */   

    HAL_PCC_ForceOscSysTypeDef ForceOscSys; /* Разрешение принудительного выбора источника тактирования системы */

    HAL_PCC_FreqMonitorSourceTypeDef Force32KClk; /* Опорный источник монитора частоты */

} PCC_FreqMonTypeDef;

typedef struct __PCC_InitTypeDef
{
    uint8_t OscillatorEnable;           /*  Осциллятор, который должен быть включен.
                                            Этот параметр может быть PCC_OSCILLATORTYPE_NONE,  PCC_OSCILLATORTYPE_HSI32M,
                                            PCC_OSCILLATORTYPE_LSI32K, PCC_OSCILLATORTYPE_OSC32K  */

    PCC_FreqMonTypeDef FreqMon;         /* Настройки монитора частоты */

    uint8_t AHBDivider;                 /*  Делитель частоты AHB.
                                            Этот параметр должен быть числом между Min = 0 и Max = 255 */

    uint8_t APBMDivider;                /*  Делитель частоты APB_M.
                                            Этот параметр должен быть числом между Min = 0 и Max = 255 */

    uint8_t APBPDivider;                /*  Делитель частоты APB_P.
                                            Этот параметр должен быть числом между Min = 0 и Max = 255 */
    
    uint8_t HSI32MCalibrationValue;     /*  Поправочный коэффициент HSI32M.
                                            Этот параметр должен быть числом между Min = 0 и Max = 63 */

    uint8_t LSI32KCalibrationValue;     /*  Поправочный коэффициент LSI32K.
                                            Этот параметр должен быть числом между Min = 0 и Max = 15 */

    HAL_PCC_RTCClockSourceTypeDef RTCClockSelection;         /*  Источник тактирования RTC. */

    HAL_PCC_CPURTCClockSourceTypeDef RTCClockCPUSelection;      /*  Источник тактирования RTC в составе ядра. */

} PCC_InitTypeDef;


typedef struct __PCC_PeriphCLKInitTypeDef
{
    uint32_t PMClockAHB;                /*  Выбор тактирования устройств на шине AHB. */

    uint32_t PMClockAPB_M;              /*  Выбор тактирования устройств на шине APB_M. */

    uint32_t PMClockAPB_P;              /*  Выбор тактирования устройств на шине APB_P. */

} PCC_PeriphCLKInitTypeDef;


void HAL_PCC_OscEnable(uint32_t Oscillator);
void HAL_PCC_OscDisable(uint32_t Oscillator);
HAL_StatusTypeDef HAL_PCC_FreqMonRefSet(HAL_PCC_FreqMonitorSourceTypeDef Force32KClk);
HAL_StatusTypeDef HAL_PCC_SetOscSystem(uint32_t OscillatorSystem, HAL_PCC_ForceOscSysTypeDef ForceOscSys);
HAL_StatusTypeDef HAL_PCC_RTCClock(HAL_PCC_RTCClockSourceTypeDef Oscillator);
HAL_StatusTypeDef HAL_PCC_CPURTCClock(HAL_PCC_CPURTCClockSourceTypeDef Oscillator);
void HAL_PCC_DividerAHB(uint32_t DividerAHB);
void HAL_PCC_DividerAPB_M(uint32_t DividerAPB_M);
void HAL_PCC_DividerAPB_P(uint32_t DividerAPB_P);
PCC_ConfigErrorsTypeDef HAL_PCC_Config(PCC_InitTypeDef *PCC_Init);
void HAL_PCC_ClockConfig(PCC_PeriphCLKInitTypeDef *PeriphClkInit);
void HAL_PCC_ClockSet(uint32_t Periphery);
void HAL_PCC_ClockClear(uint32_t Periphery);
uint32_t HAL_PCC_GetSysClockFreq();


#endif