#include "mik32_hal_scr1_timer.h"

struct __HAL_Time_SCR1TIM_Handler
{
    uint32_t presc;     // AHB prescaler
    uint32_t pt;        // Timer divider
    uint32_t clock_freq; // Clock frequency
} HAL_Time_SCR1TIM_Handler;

/**
 * @brief Инициализация таймера SCR1 для работы в качестве системных часов.
 * После инициализации системного таймера не рекомендуется изменять делитель тактовой частоты AHB.
 * Если делитель такта был изменен или микроконтроллер переключился на другой источник тактирования, необходимо
 * переинициализаровать таймер. При этом прежнее значение системного времени потеряется
 * 
 * Время переполнения системных часов зависит от частоты тактирования. Минимальное время переполнения - 4295c.
*/
void HAL_Time_SCR1TIM_Init()
{
    /* Setting dividers */
    HAL_Time_SCR1TIM_Handler.presc = (PM->DIV_AHB+1);
    HAL_Time_SCR1TIM_Handler.clock_freq = HAL_PCC_GetSysClockFreq();
    if (HAL_Time_SCR1TIM_Handler.clock_freq % (HAL_Time_SCR1TIM_Handler.presc * 1000000UL) != 0)
        HAL_Time_SCR1TIM_Handler.pt = 1;
    else
    {
        uint32_t pt_raw = HAL_Time_SCR1TIM_Handler.clock_freq / (HAL_Time_SCR1TIM_Handler.presc * 1000000UL);
        if (pt_raw < 2) HAL_Time_SCR1TIM_Handler.pt = 1;
        else HAL_Time_SCR1TIM_Handler.pt = pt_raw;
    }
    __HAL_SCR1_TIMER_SET_DIVIDER(HAL_Time_SCR1TIM_Handler.pt-1);
    /* Timer enable */
    __HAL_SCR1_TIMER_ENABLE();
    /* Clear the timer */
    __HAL_SCR1_TIMER_SET_TIME(0);
}

/**
 * @brief Системное время в микросекундах, используется таймер SCR1 в качестве системных часов
*/
uint32_t HAL_Time_SCR1TIM_Micros()
{
    return (uint32_t)(__HAL_SCR1_TIMER_GET_TIME() * (1000000UL * HAL_Time_SCR1TIM_Handler.presc *
        HAL_Time_SCR1TIM_Handler.pt) / HAL_Time_SCR1TIM_Handler.clock_freq);
}

/**
 * @brief Системное время в миллисекундах, используется таймер SCR1 в качестве системных часов
*/
uint32_t HAL_Time_SCR1TIM_Millis()
{
    return (uint32_t)(__HAL_SCR1_TIMER_GET_TIME() * (1000UL * HAL_Time_SCR1TIM_Handler.presc *
        HAL_Time_SCR1TIM_Handler.pt) / HAL_Time_SCR1TIM_Handler.clock_freq);
}

/**
 * @brief Функция задержки в микросекундах, используется таймер SCR1 в качестве системных часов
*/
void HAL_Time_SCR1TIM_DelayUs(uint32_t time_us)
{
    uint32_t time_metka = HAL_Time_SCR1TIM_Micros();
    while (HAL_Time_SCR1TIM_Micros() - time_metka < time_us);
}

/**
 * @brief Функция задержки в миллисекундах, используется таймер SCR1 в качестве системных часов
*/
void HAL_Time_SCR1TIM_DelayMs(uint32_t time_ms)
{
    uint32_t time_metka = HAL_Time_SCR1TIM_Millis();
    while (HAL_Time_SCR1TIM_Millis() - time_metka < time_ms);
}
