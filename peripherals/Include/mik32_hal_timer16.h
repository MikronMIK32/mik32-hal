#ifndef MIK32_HAL_TIMER16
#define MIK32_HAL_TIMER16

#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "power_manager.h"
#include "timer16.h"
#include "pad_config.h"
#include "stdbool.h"
#include "mik32_hal_irq.h"
#include "mik32_memory_map.h"


#define TIMER16_FLAG_DOWN       TIMER16_ISR_DOWN_M      /**< Флаг изменения направления счетчика с вверх на вниз. */
#define TIMER16_FLAG_UP         TIMER16_ISR_UP_M        /**< Флаг изменения направления счетчика с вниз на вверх. */
#define TIMER16_FLAG_ARROK      TIMER16_ISR_ARROK_M     /**< Флаг обновления регистра автозагрузки. */
#define TIMER16_FLAG_CMPOK      TIMER16_ISR_CMPOK_M     /**< Флаг обновления регистра сравнения. */
#define TIMER16_FLAG_EXTTRIG    TIMER16_ISR_EXTTRIG_M   /**< Флаг события фронта внешнего триггера. */
#define TIMER16_FLAG_ARRM       TIMER16_ISR_ARRM_M      /**< Флаг соответствия значения счетчика со значением автозагрузки. */
#define TIMER16_FLAG_CMPM       TIMER16_ISR_CMPM_M      /**< Флаг совпадения значения счетчика со значением сравнения. */

   
/**
 * @brief Источник тактирования.
 */
typedef enum __HAL_Timer16_SourceTypeDef
{
    TIMER16_SOURCE_INTERNAL_SYSTEM  =   0x0,     /**< Тактирование от системной частоты (sys_clk). */
    TIMER16_SOURCE_INTERNAL_AHB     =   0x1,     /**< Тактирование от частоты шины AHB. */
    TIMER16_SOURCE_INTERNAL_OSC32M  =   0x2,     /**< Тактирование от частоты внешнего осциллятора OSC32М. */ 
    TIMER16_SOURCE_INTERNAL_HSI32M  =   0x3,     /**< Тактирование от частоты встроенного осциллятора HSI32M. */
    TIMER16_SOURCE_INTERNAL_OSC32K  =   0x4,     /**< Тактирование от частоты внешнего осциллятора OSC32К. */
    TIMER16_SOURCE_INTERNAL_LSI32K  =   0x5,     /**< Тактирование от частоты встроенного осциллятора LSI32K. */
    TIMER16_SOURCE_EXTERNAL_INPUT1  =   0x6      /**< Тактирование от внешнего вывода Input1. */
} HAL_Timer16_SourceTypeDef;

/**
 * @brief Делитель частоты.
 */
typedef enum __HAL_Timer16_PrescalerTypeDef
{
    TIMER16_PRESCALER_1    =    0b000,       /**< Делитель частоты 1. */
    TIMER16_PRESCALER_2    =    0b001,       /**< Делитель частоты 2. */
    TIMER16_PRESCALER_4    =    0b010,       /**< Делитель частоты 4. */
    TIMER16_PRESCALER_8    =    0b011,       /**< Делитель частоты 8. */
    TIMER16_PRESCALER_16   =    0b100,       /**< Делитель частоты 16. */
    TIMER16_PRESCALER_32   =    0b101,       /**< Делитель частоты 32. */
    TIMER16_PRESCALER_64   =    0b110,       /**< Делитель частоты 64. */
    TIMER16_PRESCALER_128  =    0b111        /**< Делитель частоты 128. */
} HAL_Timer16_PrescalerTypeDef;

/**
 * @brief Источник тактового сигнала таймера для синхронизации счетчика.
 */
typedef enum __HAL_Timer16_CountModeTypeDef
{
    TIMER16_COUNTMODE_INTERNAL = 0,    /**< Счетчик увеличивается после каждого внутреннего тактового импульса. */
    TIMER16_COUNTMODE_EXTERNAL = 1     /**< Счетчик увеличивается после каждого действительного тактового импульса на внешнем выводе input1. */
} HAL_Timer16_CountModeTypeDef;

/**
 * @brief Полярность синхронизации.
 * Выбор активного фронта при тактировании от внешнего источника.
 * 
 */
typedef enum __HAL_Timer16_ActiveEdgeTypeDef
{
    TIMER16_ACTIVEEDGE_RISING   =  0b00,      /**< Нарастающий фронт является активным фронтом, используемым для подсчета. */
    TIMER16_ACTIVEEDGE_FOLLING  =  0b01,      /**< Спадающий фронт является активным фронтом, используемым для подсчета. */
    TIMER16_ACTIVEEDGE_BOTH     =  0b10       /**< Оба фронта являются активными фронтами. */
} HAL_Timer16_ActiveEdgeTypeDef;

/* Title: Источник триггера */

typedef enum __HAL_Timer16_TriggerTypeDef
{
    TIMER16_TRIGGER_TIM0_GPIO0_7    =   0b000,      /**< GPIO0_7 источник триггера для Timer16_0. */
    TIMER16_TRIGGER_TIM0_GPIO0_4    =   0b001,      /**< GPIO0_4 источник триггера для Timer16_0. */
    TIMER16_TRIGGER_TIM0_GPIO0_15   =   0b010,      /**< GPIO0_15 источник триггера для Timer16_0. */
    TIMER16_TRIGGER_TIM0_GPIO0_14   =   0b011,      /**< GPIO0_14 источник триггера для Timer16_0. */

    TIMER16_TRIGGER_TIM1_GPIO1_9    =   0b000,      /**< GPIO1_9 источник триггера для Timer16_1. */
    TIMER16_TRIGGER_TIM1_GPIO1_8    =   0b001,      /**< GPIO1_8 источник триггера для Timer16_1. */
    TIMER16_TRIGGER_TIM1_GPIO1_7    =   0b010,      /**< GPIO1_7 источник триггера для Timer16_1. */
    TIMER16_TRIGGER_TIM1_GPIO1_6    =   0b011,      /**< GPIO1_6 источник триггера для Timer16_1. */

    TIMER16_TRIGGER_TIM2_GPIO2_3    =   0b000,      /**< GPIO2_3 источник триггера для Timer16_2. */
    TIMER16_TRIGGER_TIM2_GPIO2_2    =   0b001,      /**< GPIO2_2 источник триггера для Timer16_2. */
    TIMER16_TRIGGER_TIM2_GPIO2_1    =   0b010,      /**< GPIO2_1 источник триггера для Timer16_2. */
    TIMER16_TRIGGER_TIM2_GPIO2_0    =   0b011,      /**< GPIO2_0 источник триггера для Timer16_2. */

    TIMER16_TRIGGER_TEMPERATURE     =   0b100,       /**< Источник триггера - термосенсор. */
    TIMER16_TRIGGER_ADC             =   0b101,       /**< Источник триггера - окончание преобразования АЦП. */
    TIMER16_TRIGGER_RTC             =   0b110,       /**< Источник триггера - прерывание RTC. */
    TIMER16_TRIGGER_AlARM           =   0b111        /**< Источник триггера - Будильник. */
} HAL_Timer16_TriggerTypeDef;

/**
 * @brief Полярность синхронизации.
 * Выбор активного фронта при тактировании от внешнего источника.
 */
typedef enum __HAL_Timer16_Trigger_ActiveEdgeTypeDef
{
    TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE =   0b00,     /**< Программный триггер. Запускается при установленном CNTSTRT или SNGSTRT. */
    TIMER16_TRIGGER_ACTIVEEDGE_RISING   =   0b01,     /**< Нарастающий фронт является активным фронтом. */
    TIMER16_TRIGGER_ACTIVEEDGE_FOLLING  =   0b10,     /**< Спадающий фронт является активным фронтом. */
    TIMER16_TRIGGER_ACTIVEEDGE_BOTH     =   0b11      /**< Оба фронта являются активными фронтами. */
} HAL_Timer16_Trigger_ActiveEdgeTypeDef;

/**
 * @brief Функция тайм-аут.
 */
typedef enum __HAL_Timer16_TimeoutTypeDef
{
    TIMER16_TIMEOUT_DISABLE =   0,     /**< Триггерное событие, поступающее, когда таймер уже запущен, будет проигнорировано. */
    TIMER16_TIMEOUT_ENABLE  =   1      /**< Триггерное событие, поступающее, когда таймер уже запущен, сбросит и перезапустит счетчик. */
} HAL_Timer16_TimeoutTypeDef;

/**
 * @brief Режим обновления регистров ARR и CMP.
 */
typedef enum __HAL_Timer16_PreloadTypeDef
{
    TIMER16_PRELOAD_AFTERWRITE = 0,     /**< Регистры обновляются после каждого доступа к записи на шине APB. */
    TIMER16_PRELOAD_ENDPERIOD  = 1     /**< Регистры обновляются в конце текущего периода Timer16. */
} HAL_Timer16_PreloadTypeDef;

/**
 * @brief Фильтр для внешнего тактового генератора/триггера.
 */
typedef enum __HAL_Timer16_FilterTypeDef
{
    TIMER16_FILTER_NONE   = 0b00,   /**< Фильтр отключен. */
    TIMER16_FILTER_2CLOCK = 0b01,   /**< Фильтр на 2 такта. */
    TIMER16_FILTER_4CLOCK = 0b10,   /**< Фильтр на 4 такта. */
    TIMER16_FILTER_8CLOCK = 0b11    /**< Фильтр на 8 тактов. */
} HAL_Timer16_FilterTypeDef;

/**
 * @brief Режим энкодера.
 */
typedef enum __HAL_Timer16_EncoderTypeDef
{
    TIMER16_ENCODER_DISABLE  =  0,      /**< Режим энкодера выключен. */
    TIMER16_ENCODER_ENABLE   =  1       /**< Режим энкодера включен. */
} HAL_Timer16_EncoderTypeDef;

/**
 * @brief Генерация волновой формы.
 */
typedef enum __HAL_Timer16_WaveformGenTypeDef
{
	TIMER16_WAVEFORM_GENERATION_DISABLE = 0,    /**< Выключить генерацию волновой формы. */ 
	TIMER16_WAVEFORM_GENERATION_ENABLE = 1    /**< Включить генерацию волновой формы. */
} HAL_Timer16_WaveformGenTypeDef;

/**
 * @brief Полярность волновой формы.
 */
typedef enum __HAL_Timer16_WaveformPolarityTypeDef
{
	TIMER16_WAVEFORM_POLARITY_NONINVERTED = 0,                      /**< Неинвертированная полярность волновой формы.  */
	TIMER16_WAVEFORM_POLARITY_INVERTED = TIMER16_CFGR_WAVPOL_M      /**< Инвертированная полярность волновой формы. */ 
} HAL_Timer16_WaveformPolarityTypeDef;


/**
 * @brief Настройки источника тактирования.
 */
typedef struct __Timer16_ClockConfigTypeDef
{
    uint8_t Source;         /**< Источник тактирования. */
    uint8_t Prescaler;      /**< Делитель частоты. */

} Timer16_ClockConfigTypeDef;

/**
 * @brief Настройки генерации волновой формы.
 */
typedef struct __Timer16_WaveformConfigTypeDef
{
    HAL_Timer16_WaveformGenTypeDef Enable;          /**< Включить или выключить генерацию волновой формы. */
    HAL_Timer16_WaveformPolarityTypeDef Polarity;   /**< Полярность выходного сигнала. */

} Timer16_WaveformConfigTypeDef;

/**
 * @brief Настройки триггера.
 */
typedef struct __Timer16_TriggerConfigTypeDef
{
    uint8_t Source;         /**< Источник триггера. */
    uint8_t ActiveEdge;     /**< Активный фронт. */
    uint8_t TimeOut;        /**< Функция тайм-аут. */

} Timer16_TriggerConfigTypeDef;


/**
 * @brief Настройки фильтров.
 */
typedef struct __Timer16_FilterConfigTypeDef
{
    uint8_t ExternalClock;      /**< Фильтр для внешнего тактового генератора. */
    uint8_t Trigger;            /**< фильтр для триггера. */

} Timer16_FilterConfigTypeDef;


/**
 * @brief Настройки Timer16
 */
typedef struct __Timer16_HandleTypeDef
{
    TIMER16_TypeDef *Instance;                  /**< Базовый адрес регистров Timer16. #TIMER16_0, #TIMER16_1, #TIMER16_2. */
    Timer16_ClockConfigTypeDef Clock;           /**< Настройки тактирования. */
    uint8_t CountMode;                          /**< Источник синхронизации. */
    uint8_t ActiveEdge;                         /**< Активный фронт. */
    uint8_t Preload;                            /**< Режим записи в ARR и CMP. */
    Timer16_TriggerConfigTypeDef Trigger;       /**< Настройки триггера. */
    Timer16_FilterConfigTypeDef Filter;         /**< Настройки фильтра. */
    Timer16_WaveformConfigTypeDef Waveform;     /**< Настройки генерации волновой формы. */
    uint8_t EncoderMode;                        /**< Включить или выключить режим энкодера. */
    
} Timer16_HandleTypeDef;

/**
 * @brief Запустить таймер16 в продолжительном режиме.
 */
#define __HAL_TIMER16_START_CONTINUOUS(__HANDLE__)  ((__HANDLE__)->Instance->CR |=  TIMER16_CR_CNTSTRT_M)

/**
 * @brief Запустить таймер16 в однократном режиме.
 */
#define __HAL_TIMER16_START_SINGLE(__HANDLE__)      ((__HANDLE__)->Instance->CR |=  TIMER16_CR_SNGSTRT_M)

/**
  * @brief  Проверка установлен ли флаг Timer16.
  * @param  __HANDLE__ Указатель на структуру с настройками Timer16.
  * @param  __FLAG__ Флаг для проверки.
  *             Этот параметр может принимать одно из значений:
  *             @arg TIMER16_FLAG_DOWN    : Флаг изменения направления счетчика с вверх на вниз. 
  *             @arg TIMER16_FLAG_UP      : Флаг изменения направления счетчика с вниз на вверх.
  *             @arg TIMER16_FLAG_ARROK   : Флаг обновления регистра автозагрузки.
  *             @arg TIMER16_FLAG_CMPOK   : Флаг обновления регистра сравнения.
  *             @arg TIMER16_FLAG_EXTTRIG : Флаг события фронта внешнего триггера.
  *             @arg TIMER16_FLAG_ARRM    : Флаг соответствия значения счетчика со значением автозагрузки.
  *             @arg TIMER16_FLAG_CMPM    : Флаг совпадения значения счетчика со значением сравнения.
  * @retval Состояние указанного флага (установлен или сброшен).
  */
#define __HAL_TIMER16_GET_FLAG(__HANDLE__, __FLAG__)          (((__HANDLE__)->Instance->ISR &(__FLAG__)) == (__FLAG__))

/**
  * @brief  Проверка установлен ли флаг Timer16.
  * @param  __HANDLE__ Указатель на структуру с настройками Timer16.
  * @param  __FLAG__ Флаг для проверки с учетом маски разрешенных прерываний.
  *             Этот параметр может принимать одно из значений:
  *             @arg TIMER16_FLAG_DOWN    : Флаг изменения направления счетчика с вверх на вниз. 
  *             @arg TIMER16_FLAG_UP      : Флаг изменения направления счетчика с вниз на вверх.
  *             @arg TIMER16_FLAG_ARROK   : Флаг обновления регистра автозагрузки.
  *             @arg TIMER16_FLAG_CMPOK   : Флаг обновления регистра сравнения.
  *             @arg TIMER16_FLAG_EXTTRIG : Флаг события фронта внешнего триггера.
  *             @arg TIMER16_FLAG_ARRM    : Флаг соответствия значения счетчика со значением автозагрузки.
  *             @arg TIMER16_FLAG_CMPM    : Флаг совпадения значения счетчика со значением сравнения.
  * @retval Состояние указанного флага (установлен или сброшен).
  */
#define __HAL_TIMER16_GET_FLAG_IT(__HANDLE__, __FLAG__)          (( ((__HANDLE__)->Instance->ISR) & ((__HANDLE__)->Instance->IER) & (__FLAG__) ) == (__FLAG__))

/**
  * @brief  Сбросить флаг Timer16.
  * @param  __HANDLE__ Указатель на структуру с настройками Timer16.
  * @param  __FLAG__ Флаг для очистки.
  *             Этот параметр может принимать одно из значений:
  *             @arg TIMER16_FLAG_DOWN    : Флаг изменения направления счетчика с вверх на вниз. 
  *             @arg TIMER16_FLAG_UP      : Флаг изменения направления счетчика с вниз на вверх.
  *             @arg TIMER16_FLAG_ARROK   : Флаг обновления регистра автозагрузки.
  *             @arg TIMER16_FLAG_CMPOK   : Флаг обновления регистра сравнения.
  *             @arg TIMER16_FLAG_EXTTRIG : Флаг события фронта внешнего триггера.
  *             @arg TIMER16_FLAG_ARRM    : Флаг соответствия значения счетчика со значением автозагрузки.
  *             @arg TIMER16_FLAG_CMPM    : Флаг совпадения значения счетчика со значением сравнения.
  */
#define __HAL_TIMER16_CLEAR_FLAG(__HANDLE__, __FLAG__)        ((__HANDLE__)->Instance->ICR  = (__FLAG__))

void HAL_TIMER16_MspInit(Timer16_HandleTypeDef* htimer16);
void HAL_Timer16_Disable(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_Enable(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetActiveEdge(Timer16_HandleTypeDef *htimer16, uint8_t ActiveEdge);
void HAL_Timer16_SetSourceClock(Timer16_HandleTypeDef *htimer16, uint8_t SourceClock);
void HAL_Timer16_SetCountMode(Timer16_HandleTypeDef *htimer16, uint8_t CountMode);
void HAL_Timer16_ClockInit(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetPreload(Timer16_HandleTypeDef *htimer16, uint8_t Preload);
void HAL_Timer16_WaitARROK(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_WaitCMPOK(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetARR(Timer16_HandleTypeDef *htimer16, uint16_t Period);
void HAL_Timer16_SetCMP(Timer16_HandleTypeDef *htimer16, uint16_t Compare);
void HAL_Timer16_SelectTrigger(Timer16_HandleTypeDef *htimer16, uint8_t TriggerSource);
void HAL_Timer16_SetTriggerEdge(Timer16_HandleTypeDef *htimer16, uint8_t TriggerEdge);
void HAL_Timer16_SetTimeOut(Timer16_HandleTypeDef *htimer16, uint8_t TimeOut);
void HAL_Timer16_SetFilterExternalClock(Timer16_HandleTypeDef *htimer16, uint8_t FilterExternalClock);
void HAL_Timer16_SetFilterTrigger(Timer16_HandleTypeDef *htimer16, uint8_t FilterTrigger);
void HAL_Timer16_SetEncoderMode(Timer16_HandleTypeDef *htimer16, uint8_t EncoderMode);
void HAL_Timer16_WaveformPolarity(Timer16_HandleTypeDef *htimer16, HAL_Timer16_WaveformPolarityTypeDef WaveformPolarity);
void HAL_Timer16_SetPrescaler(Timer16_HandleTypeDef *htimer16, uint8_t Prescaler);
void HAL_Timer16_Init(Timer16_HandleTypeDef *htimer16);
uint16_t HAL_Timer16_GetCounterValue(Timer16_HandleTypeDef *htimer16);
uint8_t HAL_Timer16_CheckCMP(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_WaitCMP(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_Counter_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period);
void HAL_Timer16_StartPWM(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_StartOneShot(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_StartSetOnes(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_Encoder_Start(Timer16_HandleTypeDef *htimer16, uint32_t Period);
void HAL_Timer16_Encoder_Stop(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_Stop(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_Counter_Start_IT(Timer16_HandleTypeDef *htimer16, uint32_t Period);
void HAL_Timer16_StartPWM_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_StartOneShot_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_StartSetOnes_IT(Timer16_HandleTypeDef *htimer16, uint16_t Period, uint16_t Compare);
void HAL_Timer16_Encoder_Start_IT(Timer16_HandleTypeDef *htimer16, uint32_t Period);
void HAL_Timer16_Encoder_Stop_IT(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_Stop_IT(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_WaitTrigger(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptMask(Timer16_HandleTypeDef *htimer16, uint32_t InterruptMask);
void HAL_Timer16_SetInterruptDOWN(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptUP(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptARROK(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptCMPOK(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptEXTTRIG(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptARRM(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_SetInterruptCMPM(Timer16_HandleTypeDef *htimer16);
void HAL_Timer16_InterruptInit(Timer16_HandleTypeDef *htimer16);
/* Функции системного времени */
void HAL_Time_TIM16_InterruptHandler();
void HAL_Time_TIM16_Init(TIMER16_TypeDef* timer);
// uint32_t HAL_Time_TIM16_GetTick();
uint32_t HAL_Time_TIM16_Micros();
uint32_t HAL_Time_TIM16_Millis();
void HAL_Time_TIM16_DelayUs(uint32_t time_us);
void HAL_Time_TIM16_DelayMs(uint32_t time_ms);

#endif
