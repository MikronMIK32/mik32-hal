#include "mik32_hal_wdt.h"

uint16_t TimeOut = 20;
uint16_t WDT_prescale[] = {1, 2, 4, 16, 64, 256, 1024, 4096};

/**
  * @brief  Инициализация WDT MSP.
  * @param  hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
  * 
  *                 информацию о конфигурации для модуля WDT.
  */
__attribute__((weak)) void HAL_WDT_MspInit(WDT_HandleTypeDef *hwdt)
{
    __HAL_PCC_WDT_CLK_ENABLE();
}

/**  
 * @brief Расчет начального значения отсчета таймера соответствующее времени до перезагрузки @ref WDT_InitTypeDef::ReloadMs "WDT_HandleTypeDef.init.ReloadMs".
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @return Структура с рассчитанным начальным значением @ref WDT_ClockTypeDef.tick и код подобранного делителя частоты @ref WDT_ClockTypeDef.divIndex.
 * Значение 0 в @ref WDT_ClockTypeDef.tick означает ошибку при расчете.
 */
WDT_ClockTypeDef HAL_WDT_MillisInClock(WDT_HandleTypeDef *hwdt)
{
    uint32_t wdtClock = 0;

    if (hwdt->Init.Clock < HAL_WDT_OSC32K)
    {
        if (hwdt->Init.ReloadMs > WDT_MAX_VALUE_CLOCK32M)
        {
            return (WDT_ClockTypeDef){.tick = 0, .divIndex = 0};
        }

        wdtClock = WDT_CLOCK32M_VALUE;
    }
    else
    {
        if (hwdt->Init.ReloadMs > WDT_MAX_VALUE_CLOCK32K)
        {
            return (WDT_ClockTypeDef){.tick = 0, .divIndex = 0};
        }

        wdtClock = WDT_CLOCK32K_VALUE;
    }

    uint32_t tick = 0;
    int divIndex = 0;
    int countDiv = sizeof(WDT_prescale) / sizeof(*WDT_prescale);
    for (divIndex = 0; divIndex < countDiv; divIndex++)
    {
        tick = ((wdtClock / (WDT_prescale[divIndex])) * hwdt->Init.ReloadMs) / 1000;

        if ((tick <= 4095) && (tick != 0))
        {
            tick = 4095 - tick;
            break;
        }
        else if (divIndex == (countDiv - 1))
        {
            return (WDT_ClockTypeDef){.tick = 0, .divIndex = 0}; /* Не удалось подобрать делитель для заданного времени отсчета WDT */
        }
    }

    return (WDT_ClockTypeDef){.tick = tick, .divIndex = divIndex};
}

/**
 * @brief Инициализировать WDT в соответствии с настройками в @ref WDT_HandleTypeDef.
 * 
 * Функция включает тактирование таймера, задает источник тактирование сторожевого таймера WDT, настраивает делитель частоты
 * таймера и начальное значение отсчета в соответствии с временем указанным в @ref WDT_InitTypeDef::ReloadMs "WDT_HandleTypeDef.init.ReloadMs".
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_WDT_Init(WDT_HandleTypeDef *hwdt, uint32_t timeout)
{
    if (hwdt->Instance != WDT)
    {
        return HAL_ERROR;
    }

    HAL_WDT_MspInit(hwdt);


    if (HAL_WDT_Stop(hwdt, timeout) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }

    PM->WDT_CLK_MUX = hwdt->Init.Clock;

    WDT_ClockTypeDef wdtClock = HAL_WDT_MillisInClock(hwdt);

    if (wdtClock.tick == 0)
    {
        return HAL_ERROR;
    }

    uint32_t conValue = (wdtClock.divIndex << WDT_CON_PRESCALE_S) | WDT_CON_PRELOAD(wdtClock.tick);
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->CON = conValue;

    return HAL_OK;
}

/**
 * @brief Перезагрузка значения сторожевого таймера.
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @param timeout продолжительность тайм-аута.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_WDT_Refresh(WDT_HandleTypeDef *hwdt, uint32_t timeout)
{
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->KEY = WDT_KEY_START;

    while (timeout--)
    {
        if (!(hwdt->Instance->STA & WDT_STA_LOADING_M))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}


/**
 * @brief Запустить таймер WDT.
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @param timeout продолжительность тайм-аута.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_WDT_Start(WDT_HandleTypeDef *hwdt, uint32_t timeout)
{
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->KEY = WDT_KEY_START;

    while (timeout--)
    {
        if (hwdt->Instance->STA & WDT_STA_ENABLED_M)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

/**
 * @brief Остановить таймер.
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @param timeout продолжительность тайм-аута.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_WDT_Stop(WDT_HandleTypeDef *hwdt, uint32_t timeout)
{
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->KEY = WDT_KEY_STOP;

    while (timeout--)
    {
        if (!(hwdt->Instance->STA & WDT_STA_ENABLED_M))
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

/**
 * @brief Задать делитель частоты WDT.
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @param prescale делитель частоты WDT.
 */
void HAL_WDT_SetPrescale(WDT_HandleTypeDef *hwdt, HAL_WDT_Prescale prescale)
{
    uint32_t conValue = (hwdt->Instance->CON & (~WDT_CON_PRESCALE_M)) | ((prescale << WDT_CON_PRESCALE_S) & WDT_CON_PRESCALE_M);
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->CON = conValue;
}

/**
 * @brief Задать начальное значение таймера при запуске или перезапуске.
 * @param hwdt указатель на структуру WDT_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля WDT.
 * @param preload начальное значение таймера при запуске или перезапуске.
 */
void HAL_WDT_SetPreload(WDT_HandleTypeDef *hwdt, HAL_WDT_Prescale preload)
{
    uint32_t conValue = (hwdt->Instance->CON & (~WDT_CON_PRELOAD_M)) | WDT_CON_PRELOAD(preload);
    hwdt->Instance->KEY = WDT_KEY_UNLOCK;
    hwdt->Instance->CON = conValue;
}