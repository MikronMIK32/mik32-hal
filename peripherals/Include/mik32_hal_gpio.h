#ifndef MIK32_HAL_GPIO
#define MIK32_HAL_GPIO

#include "mik32_hal_pad_config.h"
#include "mik32_hal_def.h"
#include "pad_config.h"
#include "gpio.h"
#include "mcu32_memory_map.h"

#define MODE_MASK 0b11

/* Направление вывода */
typedef enum HAL_PinDirectionTypeDef
{
	PIN_INPUT = 0,	/* Вход */
	PIN_OUTPUT = 1, /* Выход */
} HAL_PinDirectionTypeDef;

/* Логический уровень вывода */
typedef enum HAL_PinLevelTypeDef
{
	PIN_LOW = 0,  /* Низкий уровень */
	PIN_HIGH = 1, /* Высокий уровень */
} HAL_PinLevelTypeDef;

/* Вспомогательная функция инициализации выводов по заданным настройкам */
HAL_StatusTypeDef HAL_GPIO_PinFullInit(HAL_PinMapTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection,
                                   HAL_PinLevelTypeDef PinLevel, HAL_PinPullModeTypeDef PullMode,
                                   HAL_PinCurrentModeTypeDef Current);
HAL_StatusTypeDef HAL_GPIO_PinInputInit(HAL_PinMapTypeDef PinMask, HAL_PinPullModeTypeDef PullMode);
HAL_StatusTypeDef HAL_GPIO_PinOutputInit(HAL_PinMapTypeDef PinMask, HAL_PinLevelTypeDef PinLevel);

/* Задает направление выводов на одном порте */
HAL_StatusTypeDef HAL_GPIO_PinDirection(HAL_PinMapTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection);
/* Задает направление выводов на одном порте по порту и маске выводов */
HAL_StatusTypeDef HAL_GPIO_PortDirection(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection);

/* Задает логический уровень выбранных выводов на одном порте */
HAL_StatusTypeDef HAL_GPIO_PinWrite(HAL_PinMapTypeDef PinMask, HAL_PinLevelTypeDef PinLevel);
/* Задает логический уровень выбранных выводов на одном порте по порту и маске выводов */
HAL_StatusTypeDef HAL_GPIO_PortWrite(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinLevelTypeDef PinLevel);

/* Читает логический уровень вывода по аргументу */
HAL_PinLevelTypeDef HAL_GPIO_PinRead(HAL_PinMapTypeDef Pin);
/* Читает порт */
uint16_t HAL_GPIO_PortRead(HAL_PortTypeDef Port);

HAL_StatusTypeDef HAL_GPIO_PinToggle(HAL_PinMapTypeDef PinMask);
HAL_StatusTypeDef HAL_GPIO_PortToggle(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask);

#endif // MIK32_HAL_GPIO