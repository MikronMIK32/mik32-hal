#ifndef MIK32_HAL_GPIO
#define MIK32_HAL_GPIO

#include "mik32_hal_pad_config.h"
#include "mik32_hal_def.h"
#include "pad_config.h"
#include "gpio.h"
#include "gpio_irq.h"
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

typedef enum HAL_GPIO_Line
{
	GPIO_LINE_0 = 0, /**< Линия прерываний 0 */
	GPIO_LINE_1 = 1, /**< Линия прерываний 1 */
	GPIO_LINE_2 = 2, /**< Линия прерываний 2 */
	GPIO_LINE_3 = 3, /**< Линия прерываний 3 */
	GPIO_LINE_4 = 4, /**< Линия прерываний 4 */
	GPIO_LINE_5 = 5, /**< Линия прерываний 5 */
	GPIO_LINE_6 = 6, /**< Линия прерываний 6 */
	GPIO_LINE_7 = 7, /**< Линия прерываний 7 */
} HAL_GPIO_Line;

typedef enum HAL_GPIO_Line_Mux
{

	GPIO_MUX_LINE_0_PORT0_0 = 0,
	GPIO_MUX_LINE_0_PORT0_8 = 1,
	GPIO_MUX_LINE_0_PORT1_0 = 2,
	GPIO_MUX_LINE_0_PORT1_8 = 3,
	GPIO_MUX_LINE_0_PORT2_0 = 4,
	GPIO_MUX_LINE_0_PORT0_4 = 5,
	GPIO_MUX_LINE_0_PORT0_12 = 6,
	GPIO_MUX_LINE_0_PORT1_4 = 7,
	GPIO_MUX_LINE_0_PORT1_12 = 8,
	GPIO_MUX_LINE_0_PORT2_4 = 9,

	GPIO_MUX_LINE_1_PORT0_1 = 0,
	GPIO_MUX_LINE_1_PORT0_9 = 1,
	GPIO_MUX_LINE_1_PORT1_1 = 2,
	GPIO_MUX_LINE_1_PORT1_9 = 3,
	GPIO_MUX_LINE_1_PORT2_1 = 4,
	GPIO_MUX_LINE_1_PORT0_5 = 5,
	GPIO_MUX_LINE_1_PORT0_13 = 6,
	GPIO_MUX_LINE_1_PORT1_5 = 7,
	GPIO_MUX_LINE_1_PORT1_13 = 8,
	GPIO_MUX_LINE_1_PORT2_5 = 9,

	GPIO_MUX_LINE_2_PORT0_2 = 0,
	GPIO_MUX_LINE_2_PORT0_10 = 1,
	GPIO_MUX_LINE_2_PORT1_2 = 2,
	GPIO_MUX_LINE_2_PORT1_10 = 3,
	GPIO_MUX_LINE_2_PORT2_2 = 4,
	GPIO_MUX_LINE_2_PORT0_6 = 5,
	GPIO_MUX_LINE_2_PORT0_14 = 6,
	GPIO_MUX_LINE_2_PORT1_6 = 7,
	GPIO_MUX_LINE_2_PORT1_14 = 8,
	GPIO_MUX_LINE_2_PORT2_6 = 9,

	GPIO_MUX_LINE_3_PORT0_3 = 0,
	GPIO_MUX_LINE_3_PORT0_11 = 1,
	GPIO_MUX_LINE_3_PORT1_3 = 2,
	GPIO_MUX_LINE_3_PORT1_11 = 3,
	GPIO_MUX_LINE_3_PORT2_3 = 4,
	GPIO_MUX_LINE_3_PORT0_7 = 5,
	GPIO_MUX_LINE_3_PORT0_15 = 6,
	GPIO_MUX_LINE_3_PORT1_7 = 7,
	GPIO_MUX_LINE_3_PORT1_15 = 8,
	GPIO_MUX_LINE_3_PORT2_7 = 9,

	GPIO_MUX_LINE_4_PORT0_4 = 0,
	GPIO_MUX_LINE_4_PORT0_12 = 1,
	GPIO_MUX_LINE_4_PORT1_4 = 2,
	GPIO_MUX_LINE_4_PORT1_12 = 3,
	GPIO_MUX_LINE_4_PORT2_4 = 4,
	GPIO_MUX_LINE_4_PORT0_0 = 5,
	GPIO_MUX_LINE_4_PORT0_8 = 6,
	GPIO_MUX_LINE_4_PORT1_0 = 7,
	GPIO_MUX_LINE_4_PORT1_8 = 8,
	GPIO_MUX_LINE_4_PORT2_0 = 9,

	GPIO_MUX_LINE_5_PORT0_5 = 0,
	GPIO_MUX_LINE_5_PORT0_13 = 1,
	GPIO_MUX_LINE_5_PORT1_5 = 2,
	GPIO_MUX_LINE_5_PORT1_13 = 3,
	GPIO_MUX_LINE_5_PORT2_5 = 4,
	GPIO_MUX_LINE_5_PORT0_1 = 5,
	GPIO_MUX_LINE_5_PORT0_9 = 6,
	GPIO_MUX_LINE_5_PORT1_1 = 7,
	GPIO_MUX_LINE_5_PORT1_9 = 8,
	GPIO_MUX_LINE_5_PORT2_1 = 9,

	GPIO_MUX_LINE_6_PORT0_6 = 0,
	GPIO_MUX_LINE_6_PORT0_14 = 1,
	GPIO_MUX_LINE_6_PORT1_6 = 2,
	GPIO_MUX_LINE_6_PORT1_14 = 3,
	GPIO_MUX_LINE_6_PORT2_6 = 4,
	GPIO_MUX_LINE_6_PORT0_2 = 5,
	GPIO_MUX_LINE_6_PORT0_10 = 6,
	GPIO_MUX_LINE_6_PORT1_2 = 7,
	GPIO_MUX_LINE_6_PORT1_10 = 8,
	GPIO_MUX_LINE_6_PORT2_2 = 9,

	GPIO_MUX_LINE_7_PORT0_7 = 0,
	GPIO_MUX_LINE_7_PORT0_15 = 1,
	GPIO_MUX_LINE_7_PORT1_7 = 2,
	GPIO_MUX_LINE_7_PORT1_15 = 3,
	GPIO_MUX_LINE_7_PORT2_7 = 4,
	GPIO_MUX_LINE_7_PORT0_3 = 5,
	GPIO_MUX_LINE_7_PORT0_11 = 6,
	GPIO_MUX_LINE_7_PORT1_3 = 7,
	GPIO_MUX_LINE_7_PORT1_11 = 8,
	GPIO_MUX_LINE_7_PORT2_3 = 9,

	GPIO_MUX_LINE0 = 10
} HAL_GPIO_Line_Mux;

#define GPIO_MODE_BIT_LEVEL_S 0
#define GPIO_MODE_BIT_EDGE_S 1
#define GPIO_MODE_BIT_ANYEDGE_S 2

#define GPIO_MODE_BIT_LEVEL_M (1 << GPIO_MODE_BIT_LEVEL_S)
#define GPIO_MODE_BIT_EDGE_M (1 << GPIO_MODE_BIT_EDGE_S)
#define GPIO_MODE_BIT_ANYEDGE_M (1 << GPIO_MODE_BIT_ANYEDGE_S)

/** Режим прерывания линии GPIO
 * \note В режиме события (EVENT) функция инициализации не включает прерывания GPIO в EPIC
 */
typedef enum HAL_GPIO_InterruptMode
{
	GPIO_MODE_INT_LOW = 0,												   /**< Режим прерывания по низкому уровню на выводе */
	GPIO_MODE_INT_HIGH = GPIO_MODE_BIT_LEVEL_M,							   /**< Режим прерывания по высокому уровню на выводе */
	GPIO_MODE_INT_FALLING = GPIO_MODE_BIT_EDGE_M,						   /**< Режим прерывания по смене уровня на выводе с высокого на низкий */
	GPIO_MODE_INT_RISING = GPIO_MODE_BIT_LEVEL_M | GPIO_MODE_BIT_EDGE_M,   /**< Режим прерывания по смене уровня на выводе с низкого на высокий */
	GPIO_MODE_INT_CHANGE = GPIO_MODE_BIT_EDGE_M | GPIO_MODE_BIT_ANYEDGE_M, /**< Режим прерывания по изменению уровня на выводе */
} HAL_GPIO_InterruptMode;

/** Функция инициализации линии прерывания
 *
 * \param irq_line Номер линии прерывания
 * \param mux Настройка мультиплексора линии прерывания
 * \param mode Режим линии прерывания
 * \note Номер линии прерывания можно получить после настройки мультиплексора.
 * Введите в mux GPIO_MUX_GPIO_X_X, где X - номера порта и вывода,
 * и нажмите Ctrl+Пробел: появятся варианты констант для данного вывода,
 * далее достаточно выбрать константу для доступной линии
 */
HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line irq_line, HAL_GPIO_Line_Mux mux,
											 HAL_GPIO_InterruptMode mode);

/** Функция деинициализации линии прерывания, выключает линию и сбрасывает её настройки
 *
 * \param irq_line Номер линии прерывания
 */
void HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irq_line);

/** Функция получения состояния линии прерывания
 *
 * \param irq_line Номер линии прерывания
 * \return Возвращает true если сработало прерывание данной линии
 */
HAL_PinLevelTypeDef HAL_GPIO_LineInterruptState(HAL_GPIO_Line irq_line);

/** Функция чтения логического уровня вывода, подключенного к линии прерывания
 *
 * \param irq_line Номер линии прерывания
 * \return Логический уровень вывода
 */
HAL_PinLevelTypeDef HAL_GPIO_LinePinState(HAL_GPIO_Line irq_line);

/** Функция сброса регистра состояния прерываний.
 *  \note Когда срабатывает прерывание на одной из линии, в регистре INTERRUPT
 *  выставляется 1 в разряде, соответствующем линии прерывания.
 *  После обработки прерывания необходимо сбросить данный регистр
 *  в обработчике прерывания GPIO_IRQ_TRAP_HANDLER().
 *  Если после обработки прерывания регистр не был сброшен,
 *  обработчик будет вызван снова, программа будет бесконечно вызывать обработчик.
 */
void HAL_GPIO_ClearInterrupts();

#endif // MIK32_HAL_GPIO