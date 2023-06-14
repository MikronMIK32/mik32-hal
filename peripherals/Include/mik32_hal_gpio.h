#ifndef MIK32_HAL_GPIO
#define MIK32_HAL_GPIO

#include "mik32_hal_def.h"
#include "pad_config.h"
#include "gpio.h"
#include "gpio_irq.h"
#include "mcu32_memory_map.h"

#define MODE_MASK 0b11

/* Направление вывода */
typedef enum HAL_PinDirectionTypeDef
{
	GPIO_PIN_INPUT = 0,	/**< Вход */
	GPIO_PIN_OUTPUT = 1, /**< Выход */
} HAL_PinDirectionTypeDef;

/* Логический уровень вывода */
typedef enum HAL_PinLevelTypeDef
{
	GPIO_PIN_LOW = 0,  /**< Низкий уровень */
	GPIO_PIN_HIGH = 1, /**< Высокий уровень */
} HAL_PinLevelTypeDef;


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

#define GPIO_IRQ_LINE_S 4

typedef enum HAL_GPIO_Line
{
	GPIO_LINE_0 = 0 << GPIO_IRQ_LINE_S, /**< Линия прерываний 0 */
	GPIO_LINE_1 = 1 << GPIO_IRQ_LINE_S, /**< Линия прерываний 1 */
	GPIO_LINE_2 = 2 << GPIO_IRQ_LINE_S, /**< Линия прерываний 2 */
	GPIO_LINE_3 = 3 << GPIO_IRQ_LINE_S, /**< Линия прерываний 3 */
	GPIO_LINE_4 = 4 << GPIO_IRQ_LINE_S, /**< Линия прерываний 4 */
	GPIO_LINE_5 = 5 << GPIO_IRQ_LINE_S, /**< Линия прерываний 5 */
	GPIO_LINE_6 = 6 << GPIO_IRQ_LINE_S, /**< Линия прерываний 6 */
	GPIO_LINE_7 = 7 << GPIO_IRQ_LINE_S, /**< Линия прерываний 7 */
} HAL_GPIO_Line;


/**
 * @brief Interrupt line configuration
 * 
 * @note use form 0b0LLLMMMM, where L - line number bits, M - mux configuration bits
 */
typedef enum HAL_GPIO_Line_Config
{
	GPIO_MUX_LINE_0_PORT0_0  = 0 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_8  = 1 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_0  = 2 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_8  = 3 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT2_0  = 4 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_4  = 5 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_12 = 6 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_4  = 7 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_12 = 8 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT2_4  = 9 | GPIO_LINE_0,

	GPIO_MUX_PORT0_0_LINE_0  = 0 | GPIO_LINE_0,
	GPIO_MUX_PORT0_8_LINE_0  = 1 | GPIO_LINE_0,
	GPIO_MUX_PORT1_0_LINE_0  = 2 | GPIO_LINE_0,
	GPIO_MUX_PORT1_8_LINE_0  = 3 | GPIO_LINE_0,
	GPIO_MUX_PORT2_0_LINE_0  = 4 | GPIO_LINE_0,
	GPIO_MUX_PORT0_4_LINE_0  = 5 | GPIO_LINE_0,
	GPIO_MUX_PORT0_12_LINE_0 = 6 | GPIO_LINE_0,
	GPIO_MUX_PORT1_4_LINE_0  = 7 | GPIO_LINE_0,
	GPIO_MUX_PORT1_12_LINE_0 = 8 | GPIO_LINE_0,
	GPIO_MUX_PORT2_4_LINE_0  = 9 | GPIO_LINE_0,


	GPIO_MUX_LINE_1_PORT0_1  = 0 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_9  = 1 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_1  = 2 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_9  = 3 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT2_1  = 4 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_5  = 5 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_13 = 6 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_5  = 7 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_13 = 8 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT2_5  = 9 | GPIO_LINE_1,

	GPIO_MUX_PORT0_1_LINE_1  = 0 | GPIO_LINE_1,
	GPIO_MUX_PORT0_9_LINE_1  = 1 | GPIO_LINE_1,
	GPIO_MUX_PORT1_1_LINE_1  = 2 | GPIO_LINE_1,
	GPIO_MUX_PORT1_9_LINE_1  = 3 | GPIO_LINE_1,
	GPIO_MUX_PORT2_1_LINE_1  = 4 | GPIO_LINE_1,
	GPIO_MUX_PORT0_5_LINE_1  = 5 | GPIO_LINE_1,
	GPIO_MUX_PORT0_13_LINE_1 = 6 | GPIO_LINE_1,
	GPIO_MUX_PORT1_5_LINE_1  = 7 | GPIO_LINE_1,
	GPIO_MUX_PORT1_13_LINE_1 = 8 | GPIO_LINE_1,
	GPIO_MUX_PORT2_5_LINE_1  = 9 | GPIO_LINE_1,


	GPIO_MUX_LINE_2_PORT0_2  = 0 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_10 = 1 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_2  = 2 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_10 = 3 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT2_2  = 4 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_6  = 5 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_14 = 6 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_6  = 7 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_14 = 8 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT2_6  = 9 | GPIO_LINE_2,

	GPIO_MUX_PORT0_2_LINE_2  = 0 | GPIO_LINE_2,
	GPIO_MUX_PORT0_10_LINE_2 = 1 | GPIO_LINE_2,
	GPIO_MUX_PORT1_2_LINE_2  = 2 | GPIO_LINE_2,
	GPIO_MUX_PORT1_10_LINE_2 = 3 | GPIO_LINE_2,
	GPIO_MUX_PORT2_2_LINE_2  = 4 | GPIO_LINE_2,
	GPIO_MUX_PORT0_6_LINE_2  = 5 | GPIO_LINE_2,
	GPIO_MUX_PORT0_14_LINE_2 = 6 | GPIO_LINE_2,
	GPIO_MUX_PORT1_6_LINE_2  = 7 | GPIO_LINE_2,
	GPIO_MUX_PORT1_14_LINE_2 = 8 | GPIO_LINE_2,
	GPIO_MUX_PORT2_6_LINE_2  = 9 | GPIO_LINE_2,


	GPIO_MUX_LINE_3_PORT0_3  = 0 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_11 = 1 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_3  = 2 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_11 = 3 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT2_3  = 4 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_7  = 5 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_15 = 6 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_7  = 7 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_15 = 8 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT2_7  = 9 | GPIO_LINE_3,

	GPIO_MUX_PORT0_3_LINE_3  = 0 | GPIO_LINE_3,
	GPIO_MUX_PORT0_11_LINE_3 = 1 | GPIO_LINE_3,
	GPIO_MUX_PORT1_3_LINE_3  = 2 | GPIO_LINE_3,
	GPIO_MUX_PORT1_11_LINE_3 = 3 | GPIO_LINE_3,
	GPIO_MUX_PORT2_3_LINE_3  = 4 | GPIO_LINE_3,
	GPIO_MUX_PORT0_7_LINE_3  = 5 | GPIO_LINE_3,
	GPIO_MUX_PORT0_15_LINE_3 = 6 | GPIO_LINE_3,
	GPIO_MUX_PORT1_7_LINE_3  = 7 | GPIO_LINE_3,
	GPIO_MUX_PORT1_15_LINE_3 = 8 | GPIO_LINE_3,
	GPIO_MUX_PORT2_7_LINE_3  = 9 | GPIO_LINE_3,


	GPIO_MUX_LINE_4_PORT0_4  = 0 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_12 = 1 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_4  = 2 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_12 = 3 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT2_4  = 4 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_0  = 5 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_8  = 6 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_0  = 7 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_8  = 8 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT2_0  = 9 | GPIO_LINE_4,

	GPIO_MUX_PORT0_4_LINE_4  = 0 | GPIO_LINE_4,
	GPIO_MUX_PORT0_12_LINE_4 = 1 | GPIO_LINE_4,
	GPIO_MUX_PORT1_4_LINE_4  = 2 | GPIO_LINE_4,
	GPIO_MUX_PORT1_12_LINE_4 = 3 | GPIO_LINE_4,
	GPIO_MUX_PORT2_4_LINE_4  = 4 | GPIO_LINE_4,
	GPIO_MUX_PORT0_0_LINE_4  = 5 | GPIO_LINE_4,
	GPIO_MUX_PORT0_8_LINE_4  = 6 | GPIO_LINE_4,
	GPIO_MUX_PORT1_0_LINE_4  = 7 | GPIO_LINE_4,
	GPIO_MUX_PORT1_8_LINE_4  = 8 | GPIO_LINE_4,
	GPIO_MUX_PORT2_0_LINE_4  = 9 | GPIO_LINE_4,


	GPIO_MUX_LINE_5_PORT0_5  = 0 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_13 = 1 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_5  = 2 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_13 = 3 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT2_5  = 4 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_1  = 5 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_9  = 6 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_1  = 7 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_9  = 8 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT2_1  = 9 | GPIO_LINE_5,

	GPIO_MUX_PORT0_5_LINE_5  = 0 | GPIO_LINE_5,
	GPIO_MUX_PORT0_13_LINE_5 = 1 | GPIO_LINE_5,
	GPIO_MUX_PORT1_5_LINE_5  = 2 | GPIO_LINE_5,
	GPIO_MUX_PORT1_13_LINE_5 = 3 | GPIO_LINE_5,
	GPIO_MUX_PORT2_5_LINE_5  = 4 | GPIO_LINE_5,
	GPIO_MUX_PORT0_1_LINE_5  = 5 | GPIO_LINE_5,
	GPIO_MUX_PORT0_9_LINE_5  = 6 | GPIO_LINE_5,
	GPIO_MUX_PORT1_1_LINE_5  = 7 | GPIO_LINE_5,
	GPIO_MUX_PORT1_9_LINE_5  = 8 | GPIO_LINE_5,
	GPIO_MUX_PORT2_1_LINE_5  = 9 | GPIO_LINE_5,


	GPIO_MUX_LINE_6_PORT0_6  = 0 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_14 = 1 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_6  = 2 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_14 = 3 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT2_6  = 4 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_2  = 5 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_10 = 6 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_2  = 7 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_10 = 8 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT2_2  = 9 | GPIO_LINE_6,

	GPIO_MUX_PORT0_6_LINE_6  = 0 | GPIO_LINE_6,
	GPIO_MUX_PORT0_14_LINE_6 = 1 | GPIO_LINE_6,
	GPIO_MUX_PORT1_6_LINE_6  = 2 | GPIO_LINE_6,
	GPIO_MUX_PORT1_14_LINE_6 = 3 | GPIO_LINE_6,
	GPIO_MUX_PORT2_6_LINE_6  = 4 | GPIO_LINE_6,
	GPIO_MUX_PORT0_2_LINE_6  = 5 | GPIO_LINE_6,
	GPIO_MUX_PORT0_10_LINE_6 = 6 | GPIO_LINE_6,
	GPIO_MUX_PORT1_2_LINE_6  = 7 | GPIO_LINE_6,
	GPIO_MUX_PORT1_10_LINE_6 = 8 | GPIO_LINE_6,
	GPIO_MUX_PORT2_2_LINE_6  = 9 | GPIO_LINE_6,


	GPIO_MUX_LINE_7_PORT0_7  = 0 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_15 = 1 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_7  = 2 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_15 = 3 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT2_7  = 4 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_3  = 5 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_11 = 6 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_3  = 7 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_11 = 8 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT2_3  = 9 | GPIO_LINE_7,

	GPIO_MUX_PORT0_7_LINE_7  = 0 | GPIO_LINE_7,
	GPIO_MUX_PORT0_15_LINE_7 = 1 | GPIO_LINE_7,
	GPIO_MUX_PORT1_7_LINE_7  = 2 | GPIO_LINE_7,
	GPIO_MUX_PORT1_15_LINE_7 = 3 | GPIO_LINE_7,
	GPIO_MUX_PORT2_7_LINE_7  = 4 | GPIO_LINE_7,
	GPIO_MUX_PORT0_3_LINE_7  = 5 | GPIO_LINE_7,
	GPIO_MUX_PORT0_11_LINE_7 = 6 | GPIO_LINE_7,
	GPIO_MUX_PORT1_3_LINE_7  = 7 | GPIO_LINE_7,
	GPIO_MUX_PORT1_11_LINE_7 = 8 | GPIO_LINE_7,
	GPIO_MUX_PORT2_3_LINE_7  = 9 | GPIO_LINE_7,

} HAL_GPIO_Line_Config;

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
	GPIO_INT_MODE_LOW = 0,												   /**< Режим прерывания по низкому уровню на выводе */
	GPIO_INT_MODE_HIGH = GPIO_MODE_BIT_LEVEL_M,							   /**< Режим прерывания по высокому уровню на выводе */
	GPIO_INT_MODE_FALLING = GPIO_MODE_BIT_EDGE_M,						   /**< Режим прерывания по смене уровня на выводе с высокого на низкий */
	GPIO_INT_MODE_RISING = GPIO_MODE_BIT_LEVEL_M | GPIO_MODE_BIT_EDGE_M,   /**< Режим прерывания по смене уровня на выводе с низкого на высокий */
	GPIO_INT_MODE_CHANGE = GPIO_MODE_BIT_EDGE_M | GPIO_MODE_BIT_ANYEDGE_M, /**< Режим прерывания по изменению уровня на выводе */
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
HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line_Config mux,
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