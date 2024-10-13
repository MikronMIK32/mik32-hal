#ifndef MIK32_HAL_SCR1_TIMER
#define MIK32_HAL_SCR1_TIMER

#include "mik32_memory_map.h"
#include "power_manager.h"
#include "inttypes.h"
#include "mik32_hal_pcc.h"

#include "csr.h"
#include "scr1_csr_encoding.h"
#include "scr1_timer.h"



/**
 * Разрешить работу системного таймера.
 */
#define __HAL_SCR1_TIMER_ENABLE()   (SCR1_TIMER->TIMER_CTRL |= SCR1_TIMER_CTRL_ENABLE_M)

/**
 * Запретить работу системного таймера.
 */
#define __HAL_SCR1_TIMER_DISABLE()  (SCR1_TIMER->TIMER_CTRL &= ~SCR1_TIMER_CTRL_ENABLE_M)

/**
 * Задать делитель частоты для системного таймера.
 * Делитель частоты таймера рассчитывается как @p __DIV__ + 1.
 * Значение параметра @p __DIV__ может быть от 0 до 1023.
 */
#define __HAL_SCR1_TIMER_SET_DIVIDER(__DIV__)   (SCR1_TIMER->TIMER_DIV = (__DIV__) & 0x3FF)

/**
 * Задать значение счетчика системного таймера.
 * Значение параметра @p __V__ может быть 64-битным числом.
 */
#define __HAL_SCR1_TIMER_SET_TIME(__V__)                               \
    ({                                                                 \
        SCR1_TIMER->MTIMEH = 0;                                        \
        SCR1_TIMER->MTIME = (uint32_t)(__V__);                         \
        SCR1_TIMER->MTIMEH = (uint32_t)((uint64_t)(__V__) >> 32);      \
    })

/**
 * Получить значение счетчика системного таймера.
 * Возвращаемое значение имеет тип uint64_t.
 */
#define __HAL_SCR1_TIMER_GET_TIME()   (((uint64_t)(SCR1_TIMER->MTIMEH) << 32) | (SCR1_TIMER->MTIME))

/**
 * Задать значение сравнения системного таймера.
 * Значение параметра @p __CMP__ может быть 64-битным числом.
 * Может использоваться для сброса прерывания по сравнению.
 */
#define __HAL_SCR1_TIMER_SET_CMP(__CMP__)                                   \
    ({                                                                      \
        SCR1_TIMER->MTIMECMPH = -1;                                         \
        SCR1_TIMER->MTIMECMP = (uint32_t)(__CMP__);                         \
        SCR1_TIMER->MTIMECMPH = (uint32_t)((uint64_t)(__CMP__) >> 32);      \
    })                                                                          

/**
 * Получить значение сравнения системного таймера.
 * Возвращаемое значение имеет тип uint64_t.
 */
#define __HAL_SCR1_TIMER_GET_CMP()   (((uint64_t)(SCR1_TIMER->MTIMECMPH) << 32) | (SCR1_TIMER->MTIMECMP))

/**
 * Разрешить глобальные прерывания и прерывание по сравнению системного таймера.
 */
#define __HAL_SCR1_TIMER_IRQ_ENABLE()       \
    ({                                      \
        set_csr(mstatus, MSTATUS_MIE);      \
        set_csr(mie, MIE_MTIE);             \
    })

/**
 * Запретить прерывание по сравнению системного таймера.
 */
#define __HAL_SCR1_TIMER_IRQ_DISABLE()  (clear_csr(mie, MIE_MTIE))
    
    

/* Перечисление источников тактирования системного таймера. */
typedef enum __HAL_SCR1_TIMER_ModeTypeDef
{
    /**
     * @brief Тактирование от ядра.
     */
    HAL_SCR1_TIMER_CLKSRC_INTERNAL = SCR1_TIMER_CTRL_CLKSRC_INTERNAL_M,
    
    /**
     * @brief Тактирование от внешнего RTC
     */
    HAL_SCR1_TIMER_CLKSRC_EXTERNAL_RTC = SCR1_TIMER_CTRL_CLKSRC_RTC_M
} HAL_SCR1_TIMER_ModeTypeDef;



/**
 * @brief Функция установки источника тактирования системного таймера.
 * @param clockSource Источник тактирования системного таймера.
 *                  Этот параметр должен быть одним из следующих значений:
 *                      - @ref HAL_SCR1_TIMER_CLKSRC_INTERNAL - тактирование от ядра
 *                      - @ref HAL_SCR1_TIMER_CLKSRC_EXTERNAL_RTC - тактирование от внешнего RTC
 */
static inline __attribute__((always_inline)) void HAL_SCR1_Timer_SetClockSource(HAL_SCR1_TIMER_ModeTypeDef clockSource)
{
    switch (clockSource)
    {
    case HAL_SCR1_TIMER_CLKSRC_INTERNAL:
        SCR1_TIMER->TIMER_CTRL &= ~SCR1_TIMER_CTRL_CLKSRC_M;
        break;

    case HAL_SCR1_TIMER_CLKSRC_EXTERNAL_RTC:
        SCR1_TIMER->TIMER_CTRL |= SCR1_TIMER_CTRL_CLKSRC_M;
        break;

    default:
        break;
    }
}


/**
 * @brief Функция сброса системного таймера.
 * 
 * Во время сброса таймер останавливается, частота ядра выбирается как источник частоты.
 * Значение счетчика таймера TIME становится 0, а значение сравнение CMP 0xFFFFFFFFFFFFFFFF.
 */
static inline __attribute__((always_inline)) void HAL_SCR1_Timer_Reset()
{
    SCR1_TIMER->TIMER_CTRL = 0;
    SCR1_TIMER->TIMER_DIV = 0;

    SCR1_TIMER->MTIME = 0;
    SCR1_TIMER->MTIMEH = 0;

    SCR1_TIMER->MTIMECMP = -1;
    SCR1_TIMER->MTIMECMPH = -1;
}

/**
 * @brief Функция инициализации системного таймера.
 * @param clockSource источник тактирования системного таймера.
 *                  Этот параметр должен быть одним из следующих значений:
 *                      - @ref HAL_SCR1_TIMER_CLKSRC_INTERNAL - тактирование от ядра
 *                      - @ref HAL_SCR1_TIMER_CLKSRC_EXTERNAL_RTC - тактирование от внешнего RTC
 * @param divider делитель частоты системного таймера. 
 *                  Делитель частоты таймера рассчитывается как @p divider + 1.
 *                  Значение параметра @p divider может быть от 0 до 1023.
 */
static inline __attribute__((always_inline)) void HAL_SCR1_Timer_Init(HAL_SCR1_TIMER_ModeTypeDef clockSource, uint32_t divider)
{
    __HAL_SCR1_TIMER_DISABLE();
    HAL_SCR1_Timer_SetClockSource(clockSource);
    __HAL_SCR1_TIMER_SET_DIVIDER(divider);

    __HAL_SCR1_TIMER_SET_TIME(0);
    __HAL_SCR1_TIMER_SET_CMP(-1);
    __HAL_SCR1_TIMER_ENABLE();
}


void HAL_Time_SCR1TIM_Init();
uint32_t HAL_Time_SCR1TIM_Micros();
uint32_t HAL_Time_SCR1TIM_Millis();
void HAL_Time_SCR1TIM_DelayUs(uint32_t time_us);
void HAL_Time_SCR1TIM_DelayMs(uint32_t time_ms);


#endif // MIK32_HAL_SCR1_TIMER



/*
 * uint32_t TIMER_CTRL;
 * uint32_t TIMER_DIV;
 * uint32_t MTIME;
 * uint32_t MTIMEH;    
 * uint32_t MTIMECMP;
 * uint32_t MTIMECMPH;
 */

/*
Регистр TIMER_CTRL
------------------------------------------------------------------------------------------------
Обозначение     |   Биты    |   По умолчанию    |   Описание                |   Доступ
------------------------------------------------------------------------------------------------
ENABLE          |   0       |   1               |   Включение таймера       |   RW
------------------------------------------------------------------------------------------------
CLKSRC          |   1       |   0               |   Источник тактирования   |   RW
                |           |                   |   0 - Тактирования ядра   |
                |           |                   |   1 - Внешний RTC         |  
------------------------------------------------------------------------------------------------
Reserved        |   31:2    |                                               |   RZ
------------------------------------------------------------------------------------------------


Регистр TIMER_DIV
------------------------------------------------------------------------------------------------
Обозначение     |   Биты    |   По умолчанию    |   Описание                |   Доступ
------------------------------------------------------------------------------------------------
DIV             |   9:0     |   0               |   Делитель                |   RW
                                                |   Счет идет каждые DIV+1  |
                                                |   такта частоты           |
------------------------------------------------------------------------------------------------
Reserved        |   31:10   |   X                                           |   RZ
------------------------------------------------------------------------------------------------


По умолчанию, MTIME и MTIMEH равны нулю после перезагрузки ядра (которая также
начинает подсчет). Другим вариантом начала счета для MTIME/MTIMEH является запись некоторого значения в
MTIME/MTIMEH.

MTIME - Счетчик таймера. 64-битне число. MTIMEH - старшее слово, MTIME - младшее слово.
MTIMECMP - Регистр сравнения. 64-битное число. MTIMECMPH - старшее слово, MTIMECMP - младшее слово.

Прерывание машинного таймера срабатывает, когда mtime содержит значение, большее или равное mtimecmp, 
обрабатывая значения как целые числа без знака. Прерывание остается в состоянии ожидания до тех пор, пока mtimecmp не станет больше
mtime (обычно в результате записи mtimecmp). Прерывание будет выполнено только в том случае, если прерывания
разрешены, а в регистре mie установлен бит MTIE.

*/
