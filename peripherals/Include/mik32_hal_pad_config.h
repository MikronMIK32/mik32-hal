#ifndef MIK32_HAL_PAD_CONFIG
#define MIK32_HAL_PAD_CONFIG

#include "mik32_hal_def.h"
#include "pad_config.h"
#include "mcu32_memory_map.h"


#define MODE_MASK 	0b11

/** 
 * @enum Mode
 * 
 */
typedef enum
{
	PIN_MODE0 = 0b00,	/**< Последовательный интерфейс */
	PIN_MODE1 = 0b01,	/**< GPIO - порт общего назначения */
	PIN_MODE2 = 0b10,   /**< Последовательный интерфейс или таймер */
	PIN_MODE3 = 0b11	/**< Аналоговый сигнал */
} HAL_PinModeTypeDef;

/* Подтяжка вывода */
typedef enum
{
	PIN_PULL_NONE = 0b00,   /**< Регистры подтяжки не подключаются */
	PIN_PULL_UP = 0b01,		/**< Подключен регистр подтяжки 50КОм к питанию */
	PIN_PULL_DOWN = 0b10	/**< Подключен регистр подтяжки 50КОм к земле */
} HAL_PinPullModeTypeDef;

/* Нагрузочная способность вывода */
typedef enum
{
	PIN_CURRENT_2 = 0b00,   /* Максимальный выходной ток 2мА */
	PIN_CURRENT_4 = 0b01,	/* Максимальный выходной ток 4мА */
	PIN_CURRENT_8 = 0b10 	/* Максимальный выходной ток 8мА */
} HAL_PinCurrentModeTypeDef;


/** 
 * @brief Задает режим выводов на одном порте
 * @param PinMask Выбор вывода для изменения режима, принимает аргумент вида PORTX_X, например PORT0_0
 * @param PinMode Выбор режима, возможные значения PIN_MODE0, PIN_MODE1, PIN_MODE2, PIN_MODE3
 */
HAL_StatusTypeDef HAL_PadConfig_PinMode(HAL_PinMapTypeDef PinMask, HAL_PinModeTypeDef PinMode);

/** 
 * @brief Задает режим выводов на одном порте
 * @param Port Выбор порта для изменения режима по маске, возможные значения PORT0, PORT1, PORT2
 * @param PinMask Выбор выводов по маске, например PIN0 | PIN1
 * @param PinMode Выбор режима, возможные значения PIN_MODE0, PIN_MODE1, PIN_MODE2, PIN_MODE3
 */
HAL_StatusTypeDef HAL_PadConfig_PortMode(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinModeTypeDef PinMode);
/* Задать режим подтяжки выводов на одном порте */
HAL_StatusTypeDef HAL_PadConfig_PinPull(HAL_PinMapTypeDef PinMask, HAL_PinPullModeTypeDef PullMode);
/* Задать режим подтяжки выводов на одном порте */
HAL_StatusTypeDef HAL_PadConfig_PortPull(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinPullModeTypeDef PullMode);
/* Задать нагрузочную способностью выводов на одном порте */
HAL_StatusTypeDef HAL_PadConfig_PinCurrent(HAL_PinMapTypeDef PinMask, HAL_PinCurrentModeTypeDef Current);
/* Задать нагрузочную способностью выводов на одном порте */
HAL_StatusTypeDef HAL_PadConfig_PortCurrent(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinCurrentModeTypeDef Current);


#endif