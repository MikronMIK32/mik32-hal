#ifndef MIK32_HAL_WDT
#define MIK32_HAL_WDT

#include "mik32_hal_pcc.h"
#include "mik32_memory_map.h"
#include "wdt.h"
#include "wdt_bus.h"
#include "power_manager.h"
#include "mik32_hal_def.h"
#include "stddef.h"

#define WDT_CLOCK32K_VALUE 32768        /**< Частота осцилляторов OSC32K или LSI32K. */
#define WDT_CLOCK32M_VALUE 32000000     /**< Частота осцилляторов OSC32M или HSI32M. */
#define WDT_MAX_VALUE_CLOCK32K 511875   /**< Максимально возможный интервал времени в миллисекундах при тактировании от OSC32K или LSI32K. */
#define WDT_MAX_VALUE_CLOCK32M 524      /**< Максимально возможный интервал времени в миллисекундах при тактировании от OSC32M или HSI32M. */
#define WDT_TIMEOUT_DEFAULT 10000       /**< Значение задержки по умолчанию. */

/** @brief  Проверить установлен ли указанный флаг SPI или нет.
  * @param  __HANDLE__ Указатель WDT Handle.
  * @param  __FLAG__ Флаг для проверки.
  *         Этот параметр должен быть одним из следующих значений:
  *            - @ref WDT_STA_ENABLED_M:    флаг активности таймера
  *            - @ref WDT_STA_LOADING_M:    флаг перезагрузки значения
  *            - @ref WDT_STA_RST_FLAG_M:   флаг генерации сброса сторожевым таймером. Сбрасывается в 0 только при снятии и последующей подаче питания.
  * 
  * @return Состояние флага @p __FLAG__ (TRUE или FALSE).
  */
#define __HAL_WDT_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->STA) & (__FLAG__)) == (__FLAG__))

/**
 * @brief Задать источник тактирования сторожевого таймера.
 * @param __WDT_CLOCK__ Bсточник тактирования сторожевого таймера.
 *          Этот параметр должен быть одним из значений перечисления @ref HAL_WDT_Clocks.
 */
#define __HAL_WDT_SET_CLOCK(__WDT_CLOCK__) (PM->WDT_CLK_MUX = (__WDT_CLOCK__) & PM_WDT_CLK_MUX_M)

/**
 * @brief Перечисление делителей входной частоты сторожевого таймера.
 * 
 */
typedef enum __HAL_WDT_Prescale
{
    HAL_WDT_PRESCALE_1 = 0,         /**< Делитель входной частоты сторожевого таймера 1. */    
    HAL_WDT_PRESCALE_2 = 1,         /**< Делитель входной частоты сторожевого таймера 2. */    
    HAL_WDT_PRESCALE_4 = 2,         /**< Делитель входной частоты сторожевого таймера 4. */    
    HAL_WDT_PRESCALE_16 = 3,        /**< Делитель входной частоты сторожевого таймера 16. */        
    HAL_WDT_PRESCALE_64 = 4,        /**< Делитель входной частоты сторожевого таймера 64. */        
    HAL_WDT_PRESCALE_256 = 5,       /**< Делитель входной частоты сторожевого таймера 256. */        
    HAL_WDT_PRESCALE_1024 = 6,      /**< Делитель входной частоты сторожевого таймера 1024. */        
    HAL_WDT_PRESCALE_4096 = 7       /**< Делитель входной частоты сторожевого таймера 4096. */        
} HAL_WDT_Prescale;

/**
 * @brief Перечисление источников тактирования сторожевого таймера.
 */
typedef enum __HAL_WDT_Clocks
{
    HAL_WDT_OSC32M,     /**< Внешний 32МГц. */  
    HAL_WDT_HSI32M,     /**< Внутренний 32МГц. */  
    HAL_WDT_OSC32K,     /**< Внешний 32кГц. */  
    HAL_WDT_LSI32K      /**< Внутренний 32кГц. */ 
} HAL_WDT_Clocks;

/**
 * @brief Определение структуры конфигурации WDT.
 */
typedef struct __WDT_InitTypeDef
{
    HAL_WDT_Clocks Clock; /**< Источник тактирования WDT. */

    /**
     * @brief Время до перезагрузки контроллера в миллисекундах.
     * 
     * Значение не должно превышать 524 мс при тактировании от источника 32 Мгц и 
     * 511875 мс при тактировании от источника 32 кГц.
     */
    uint32_t ReloadMs; 

} WDT_InitTypeDef;

/**
 * @brief Определение структуры WDT Handle.
 */
typedef struct __WDT_HandleTypeDef
{
    WDT_TypeDef *Instance;  /**< Адрес регистров WDT. */

    WDT_InitTypeDef Init;   /**< Параметры WDT. */

} WDT_HandleTypeDef;

/**
 * @brief Возвращаемая структура для функции @ref HAL_WDT_MillisInClock.
 */
typedef struct __WDT_ClockTypeDef
{
    uint32_t tick;  /**< Начальное значение таймера при запуске или перезапуске для отсчета WDT_InitTypeDef::ReloadMs "WDT_HandleTypeDef.Init.ReloadMs" миллисекунд. */

    int divIndex;   /**< Значение делителя частоты таймера для отсчета @ref WDT_InitTypeDef::ReloadMs "WDT_HandleTypeDef.Init.ReloadMs" миллисекунд. */

} WDT_ClockTypeDef;


void HAL_WDT_MspInit(WDT_HandleTypeDef* hwdt);
HAL_StatusTypeDef HAL_WDT_Init(WDT_HandleTypeDef *hwdt, uint32_t timeout);
HAL_StatusTypeDef HAL_WDT_Refresh(WDT_HandleTypeDef *hwdt, uint32_t timeout);
HAL_StatusTypeDef HAL_WDT_Start(WDT_HandleTypeDef *hwdt, uint32_t timeout);
HAL_StatusTypeDef HAL_WDT_Stop(WDT_HandleTypeDef *hwdt, uint32_t timeout);
void HAL_WDT_SetPrescale(WDT_HandleTypeDef *hwdt, HAL_WDT_Prescale prescale);
void HAL_WDT_SetPreload(WDT_HandleTypeDef *hwdt, HAL_WDT_Prescale preload);

#endif // MIK_HAL_WDT