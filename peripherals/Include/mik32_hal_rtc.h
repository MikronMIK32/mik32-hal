#ifndef MIK32_HAL_RTC
#define MIK32_HAL_RTC

#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "rtc.h"
#include "mcu32_memory_map.h"

/* Title: Макросы */

/*
 * Defines: Месяц 
 *
 * 
 * RTC_MONTH_JANUARY    - Январь
 * RTC_MONTH_FEBRUARY   - Февраль
 * RTC_MONTH_MARCH      - Март
 * RTC_MONTH_APRIL      - Апрель
 * RTC_MONTH_MAY        - Май
 * RTC_MONTH_JUNE       - Июнь
 * RTC_MONTH_JULY       - Июль
 * RTC_MONTH_AUGUST     - Август
 * RTC_MONTH_SEPTEMBER  - Сентябрь
 * RTC_MONTH_OCTOBER    - Октябрь
 * RTC_MONTH_NOVEMBER   - Ноябрь
 * RTC_MONTH_DECEMBER   - Декабрь
 *
 */
/* Месяц */
#define RTC_MONTH_JANUARY              1
#define RTC_MONTH_FEBRUARY             2
#define RTC_MONTH_MARCH                3
#define RTC_MONTH_APRIL                4
#define RTC_MONTH_MAY                  5
#define RTC_MONTH_JUNE                 6
#define RTC_MONTH_JULY                 7
#define RTC_MONTH_AUGUST               8
#define RTC_MONTH_SEPTEMBER            9
#define RTC_MONTH_OCTOBER              10
#define RTC_MONTH_NOVEMBER             11
#define RTC_MONTH_DECEMBER             12

/* Title: Макросы */

/*
 * Defines: День недели 
 *
 * 
 * RTC_WEEKDAY_MONDAY    - Понедельник
 * RTC_WEEKDAY_TUESDAY   - Вторник
 * RTC_WEEKDAY_WEDNESDAY - Среда
 * RTC_WEEKDAY_THURSDAY  - Четверг
 * RTC_WEEKDAY_FRIDAY    - Пятница
 * RTC_WEEKDAY_SATURDAY  - Суббота
 * RTC_WEEKDAY_SUNDAY    - Воскресенье
 *
 */
/* День недели */
#define RTC_WEEKDAY_MONDAY             1
#define RTC_WEEKDAY_TUESDAY            2
#define RTC_WEEKDAY_WEDNESDAY          3
#define RTC_WEEKDAY_THURSDAY           4
#define RTC_WEEKDAY_FRIDAY             5
#define RTC_WEEKDAY_SATURDAY           6
#define RTC_WEEKDAY_SUNDAY             7

/*
 * Defines: Прерывание будильника Alarm 
 *
 * 
 * RTC_ALARM_IRQ_DISABLE - Запретить прерывание Alarm
 * RTC_ALARM_IRQ_ENABLE  - Разрешить прерывание Alarm
 *
 */
#define RTC_ALARM_IRQ_DISABLE         0   /* Запретить прерывание Alarm */
#define RTC_ALARM_IRQ_ENABLE          1   /* Разрешить прерывание Alarm */

/* Title: Структуры */

typedef struct
{
  uint8_t Alarm; /* Разрешение прерывания при наличии установленного бита ALRM */

} RTC_IRQnTypeDef;

/*
 * Struct: RTC_HandleTypeDef
 * Настройки инициализации RTC
 * 
 */
typedef struct
{
  /*
  * Variable: Instance
  * Базовый адрес регистров RTC
  *
  */
  RTC_TypeDef                 *Instance;

  RTC_IRQnTypeDef             Interrupts; /* Прерывания RTC */

} RTC_HandleTypeDef;

/*
 * Struct: RTC_HandleTypeDef
 * Время и день недели
 * 
 */
typedef struct
{
/*
 * Variable: Dow
 * День недели
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <RTC_WEEKDAY_MONDAY>
 * - <RTC_WEEKDAY_TUESDAY>
 * - <RTC_WEEKDAY_WEDNESDAY>
 * - <RTC_WEEKDAY_THURSDAY>
 * - <RTC_WEEKDAY_FRIDAY>
 * - <RTC_WEEKDAY_SATURDAY>
 * - <RTC_WEEKDAY_SUNDAY>
 *
 */
  uint8_t Dow;             /* Параметр RTC. День недели.
                              Этот параметр должен быть числом между Min = 1 и Max = 7 */

/*
 * Variable: Hours
 * Часы
 * 
 * Этот параметр должен быть числом в диапазоне от 0 до 23.
 *
 */
  uint8_t Hours;

  /*
  * Variable: Minutes
  * Минуты
  * 
  * Этот параметр должен быть числом в диапазоне от 0 до 59.
  *
  */
  uint8_t Minutes;

  /*
  * Variable: Seconds
  * Секунды
  * 
  * Этот параметр должен быть числом в диапазоне от 0 до 59.
  *
  */
  uint8_t Seconds;

} RTC_TimeTypeDef;

/*
 * Struct: RTC_DateTypeDef
 * Дата
 * 
 */
typedef struct
{
  /*
  * Variable: Century
  * Секунды
  * 
  * Этот параметр должен быть числом в диапазоне от 0 до 99.
  *
  */
  uint8_t Century;

  /*
  * Variable: Day
  * Число
  * 
  * Этот параметр должен быть числом в диапазоне от 1 до 31.
  *
  */
  uint8_t Day;

  /*
  * Variable: Day
  * Месяц
  * 
  * Этот параметр должен быть числом в диапазоне от 1 до 12.
  *
  */
  uint8_t Month;

  /*
  * Variable: Day
  * Год
  * 
  * Этот параметр должен быть числом в диапазоне от 0 до 99.
  *
  */
  uint8_t Year;

} RTC_DateTypeDef;

/*
 * Struct: RTC_DateTypeDef
 * Дата и время будильника
 * 
 */
typedef struct
{
  /*
  * Variable: AlarmTime
  * Время и день недели будильника
  *
  */
  RTC_TimeTypeDef AlarmTime;

  /*
  * Variable: AlarmDate
  * Дата будильника
  *
  */
  RTC_DateTypeDef AlarmDate;

  /*
  * Variable: MaskAlarmTime
  * Маска сравнений будильника по времени
  *
  */
  uint32_t MaskAlarmTime;

  /*
  * Variable: MaskAlarmDate
  * Маска сравнений будильника по дате
  *
  */
  uint32_t MaskAlarmDate;

} RTC_AlarmTypeDef;

/* Title: Функции */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc);

/*  
 * Function: HAL_RTC_WaitFlag
 * 
 * Ожидать после записи в регистр RTC установку Flag в 0
 * 
 * Сигнал проведения синхронизации между тактовыми доменами. После записи в любой регистр и пока данный флаг 
 * читается равным 1, запрещено выполнять любую новую запись.
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 * 
 */
void HAL_RTC_WaitFlag(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_Disable
 * 
 * Выключить модуль RTC
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_Disable(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_Disable
 * 
 * Включить модуль RTC
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_Enable(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_SetTime
 * 
 * Установить временя и день недели RTC в соответствии с параметрами sTime
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * sTime - Указатель на структуру с настройками времени и дня недели.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime);

/*  
 * Function: HAL_RTC_SetDate
 * 
 * Установить дату в соответствии с параметрами sDate
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * sDate - Указатель на структуру с настройками даты.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate);

/*  
 * Function: HAL_RTC_Alarm_SetTime
 * 
 * Установить время будильника в соответствии с параметрами sAlarm
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * sAlarm - Указатель на структуру с настройками времени и дня недели будильника.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_Alarm_SetTime(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm);

/*  
 * Function: HAL_RTC_Alarm_SetDate
 * 
 * Установить время будильника в соответствии с параметрами sAlarm
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * sAlarm - Указатель на структуру с настройками даты будильника.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_Alarm_SetDate(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm);

/*  
 * Function: HAL_RTC_SetAlarm
 * 
 * Установить дату и время будильника в соответствии с параметрами sAlarm
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * sAlarm - Указатель на структуру с настройками даты и времени будильника.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm);

/*  
 * Function: HAL_RTC_AlarmDisable
 * 
 * Запретить все сравнения будильника
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_AlarmDisable(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_AlarmDisable
 * 
 * Очистить флаг будильника Alrm
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 */
void HAL_RTC_ClearAlrmFlag(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_GetAlrmFlag
 * 
 * Получить значение флага будильника Alrm
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * void.
 */
int HAL_RTC_GetAlrmFlag(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_GetDate
 * 
 * Получить текущую дату RTC
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * (RTC_DateTypeDef ) - Структура с текущей датой RTC
 */
RTC_DateTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc);

/*  
 * Function: HAL_RTC_GetTime
 * 
 * Получить текущее время RTC
 * 
 * Parameters:
 * 
 * hrtc - Указатель на структуру с настройками RTC.
 * 
 * Returns:
 * (RTC_DateTypeDef ) - Структура с текущей датой RTC
 */
RTC_TimeTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc);

/*
 * Function: HAL_RTC_SetInterruptAlarm
 * Задать разрешение на включение прерывания Alarm в соответствии с InterruptEnable
 *
 * Parameters:
 * hrtc - Указатель на структуру с настройками RTC.
 * InterruptEnable - Разрешение прерывания Alarm.
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <RTC_ALARM_IRQ_DISABLE>
 * - <RTC_ALARM_IRQ_ENABLE>
 *
 * Returns:
 * void.
 */
void HAL_RTC_SetInterruptAlarm(RTC_HandleTypeDef *hrtc, uint32_t InterruptEnable);

/*
 * Function: HAL_RTC_InterruptInit
 * Инициализировать прерывания RTC в соответствии с настройками hrtc
 *
 * Parameters:
 * hrtc - Указатель на структуру с настройками RTC.
 *
 * Returns:
 * void.
 */
void HAL_RTC_InterruptInit(RTC_HandleTypeDef *hrtc);

/*
 * Function: HAL_RTC_GetINTE
 * Получить текущее значение бита разрешения прерывания Alarm - INTE
 *
 * Parameters:
 * hrtc - Указатель на структуру с настройками RTC.
 *
 * Returns:
 * (int ) - Текущее значение бита разрешения прерывания Alarm - INTE
 *
 */
int HAL_RTC_GetINTE(RTC_HandleTypeDef *hrtc);

#endif