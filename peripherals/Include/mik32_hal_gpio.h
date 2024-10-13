#ifndef MIK32_HAL_GPIO_NEW
#define MIK32_HAL_GPIO_NEW

#include "mik32_hal_def.h"
#include "pad_config.h"
#include "gpio.h"
#include "gpio_irq.h"
#include "mik32_memory_map.h"
#include "mik32_hal_pcc.h"

#define GPIO_MODE_BIT_LEVEL_S 0	  /**< Позиция бита для режима LEVEL_SET (тип прерывания по уровню низкий/высокий или по событию фронт/спад) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_MODE_BIT_EDGE_S 1	  /**< Позиция бита для режима EDGE (тип прерывания уровень или событие) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_MODE_BIT_ANYEDGE_S 2 /**< Позиция бита для режима ANYEDGE (режим прерывания по любому событию фронт/спад) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_MODE_BIT_LEVEL_M (1 << GPIO_MODE_BIT_LEVEL_S)		 /**< Маска для режима LEVEL_SET (тип прерывания по уровню низкий/высокий или по событию фронт/спад) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_MODE_BIT_EDGE_M (1 << GPIO_MODE_BIT_EDGE_S)		 /**< Маска для режима EDGE (тип прерывания уровень или событие) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_MODE_BIT_ANYEDGE_M (1 << GPIO_MODE_BIT_ANYEDGE_S)	 /**< Маска для режима ANYEDGE (режим прерывания по любому событию фронт/спад) в перечислении @ref HAL_GPIO_InterruptMode. */
#define GPIO_IRQ_LINE_S 4 /**< Позиция номера канала в перечислении @ref HAL_GPIO_Line @ref и @ref HAL_GPIO_Line_Config. */

#define __LOW GPIO_PIN_LOW					/**< Псевдоним @ref GPIO_PIN_LOW. Низкий выходной уровень GPIO. */
#define __HIGH GPIO_PIN_HIGH				/**< Псевдоним @ref GPIO_PIN_HIGH. Высокий выходной уровень GPIO. */
#define __INPUT HAL_GPIO_MODE_GPIO_INPUT	/**< Псевдоним @ref HAL_GPIO_MODE_GPIO_INPUT. Режим вывода - GPIO. Вход. */
#define __OUTPUT HAL_GPIO_MODE_GPIO_OUTPUT	/**< Псевдоним @ref HAL_GPIO_MODE_GPIO_OUTPUT. Режим вывода - GPIO. Выход. */				
#define __PULL_NONE HAL_GPIO_PULL_NONE		/**< Псевдоним @ref HAL_GPIO_PULL_NONE. Подтяжка не подключаются. */			
#define __PULL_UP HAL_GPIO_PULL_UP			/**< Псевдоним @ref HAL_GPIO_PULL_UP. Подтяжка к питанию. */		
#define __PULL_DOWN HAL_GPIO_PULL_DOWN		/**< Псевдоним @ref HAL_GPIO_PULL_DOWN. Подтяжка к земле. */			
#define __DS_2MA HAL_GPIO_DS_2MA			/**< Псевдоним @ref HAL_GPIO_DS_2MA. Режим драйвера 2мА. */		
#define __DS_4MA HAL_GPIO_DS_4MA			/**< Псевдоним @ref HAL_GPIO_DS_4MA. Режим драйвера 4мА. */		
#define __DS_8MA HAL_GPIO_DS_8MA			/**< Псевдоним @ref HAL_GPIO_DS_8MA. Режим драйвера 8мА. */		

/**
 * @brief Маска выводов.
 */
typedef enum __HAL_PinsTypeDef
{
	GPIO_PIN_0 = (1 << 0),	 /**< Выбран пин 0. */
	GPIO_PIN_1 = (1 << 1),	 /**< Выбран пин 1. */
	GPIO_PIN_2 = (1 << 2),	 /**< Выбран пин 2. */
	GPIO_PIN_3 = (1 << 3),	 /**< Выбран пин 3. */
	GPIO_PIN_4 = (1 << 4),	 /**< Выбран пин 4. */
	GPIO_PIN_5 = (1 << 5),	 /**< Выбран пин 5. */
	GPIO_PIN_6 = (1 << 6),	 /**< Выбран пин 6. */
	GPIO_PIN_7 = (1 << 7),	 /**< Выбран пин 7. */
	GPIO_PIN_8 = (1 << 8),	 /**< Выбран пин 8. */
	GPIO_PIN_9 = (1 << 9),	 /**< Выбран пин 9. */
	GPIO_PIN_10 = (1 << 10), /**< Выбран пин 10. */
	GPIO_PIN_11 = (1 << 11), /**< Выбран пин 11. */
	GPIO_PIN_12 = (1 << 12), /**< Выбран пин 12. */
	GPIO_PIN_13 = (1 << 13), /**< Выбран пин 13. */
	GPIO_PIN_14 = (1 << 14), /**< Выбран пин 14. */
	GPIO_PIN_15 = (1 << 15), /**< Выбран пин 15. */
	GPIO_PIN_ALL = 0xFFFF	 /**< Выбраны все пины порта. */

} HAL_PinsTypeDef;

/**
 * @brief Перечисление состояний выходного уровня GPIO.
 */
typedef enum __GPIO_PinState
{
	GPIO_PIN_LOW = 0,	/**< Низкий выходной уровень GPIO. */
	GPIO_PIN_HIGH = 1	/**< Высокий выходной уровень GPIO.*/
} GPIO_PinState;

/**
 * @brief Перечисление режимов вывода.
 */
typedef enum __HAL_GPIO_ModeTypeDef
{
#ifdef MIK32V0
	HAL_GPIO_MODE_GPIO_INPUT = 0b101,  /**< Режим вывода - GPIO. Вход. */
	HAL_GPIO_MODE_GPIO_OUTPUT = 0b001, /**< Режим вывода - GPIO. Выход. */
	HAL_GPIO_MODE_SERIAL = 0b00,	   /**< Режим вывода - последовательный интерфейс. */
#else // MIK32V2
	HAL_GPIO_MODE_GPIO_INPUT = 0b100,  /**< Режим вывода - GPIO. Вход. */
	HAL_GPIO_MODE_GPIO_OUTPUT = 0b000, /**< Режим вывода - GPIO. Выход. */
	HAL_GPIO_MODE_SERIAL = 0b01,	   /**< Режим вывода - последовательный интерфейс. */
#endif // MIK32V0

	HAL_GPIO_MODE_TIMER_SERIAL = 0b10, /**< Режим вывода - последовательный интерфейс или таймер. */
	HAL_GPIO_MODE_ANALOG = 0b11		   /**< Аналоговый сигнал. */
} HAL_GPIO_ModeTypeDef;

/**
 * @brief Перечисление режимов подтяжки.
 */
typedef enum __HAL_GPIO_PullTypeDef
{
	HAL_GPIO_PULL_NONE = 0b00, /**< Подтяжка не подключаются. */
	HAL_GPIO_PULL_UP = 0b01,   /**< Подтяжка к питанию. */
	HAL_GPIO_PULL_DOWN = 0b10  /**< Подтяжка к земле. */
} HAL_GPIO_PullTypeDef;

/**
 * @brief Перечисление режимов нагрузочной способности. 
 */
typedef enum __HAL_GPIO_DSTypeDef
{
	HAL_GPIO_DS_2MA = 0b00, /**< Режим драйвера 2мА. */
	HAL_GPIO_DS_4MA = 0b01, /**< Режим драйвера 4мА. */
	HAL_GPIO_DS_8MA = 0b10	/**< Режим драйвера 8мА. */
} HAL_GPIO_DSTypeDef;

/**
 * @brief Настройки для конфигурации выводов.
 */
typedef struct __GPIO_InitTypeDef
{
    /**
     * @brief Маска выводов, к которым применяется инициализация..
     *
     * Этот параметр является маской из значений @ref HAL_PinsTypeDef.
     */
	HAL_PinsTypeDef Pin;

	/**
     * @brief Режим выводов или направление GPIO.
     *
     * Этот параметр должен быть одним из значений:
     *      - @ref HAL_GPIO_MODE_GPIO_INPUT  - режим вывода - GPIO. Вход
     *      - @ref HAL_GPIO_MODE_GPIO_OUTPUT - режим вывода - GPIO. Выход
	 * 		- @ref HAL_GPIO_MODE_SERIAL - режим вывода - последовательный интерфейс
	 * 		- @ref HAL_GPIO_MODE_TIMER_SERIAL - режим вывода - последовательный интерфейс или таймер
	 * 		- @ref HAL_GPIO_MODE_ANALOG - режим вывода аналоговый сигнал
     */
	
	/**
	 * @brief Режим выводов или направление GPIO.
	 * 	
	 * Этот параметр должен быть одним из значений:
     *      - @ref HAL_GPIO_MODE_GPIO_INPUT  - режим вывода GPIO. Направление вывода как вход;
     *      - @ref HAL_GPIO_MODE_GPIO_OUTPUT - режим вывода GPIO. Направление вывода как выход;
	 * 		- @ref HAL_GPIO_MODE_SERIAL - режим вывода последовательный интерфейс;
	 * 		- @ref HAL_GPIO_MODE_TIMER_SERIAL - режим вывода последовательный интерфейс или таймер;
	 * 		- @ref HAL_GPIO_MODE_ANALOG - режим вывода аналоговый сигнал.
	 */
	HAL_GPIO_ModeTypeDef Mode;

	/**
	 * @brief Режим подтяжки.
	 * 
	 * Этот параметр должен быть одним из значений:
	 * 		- @ref HAL_GPIO_PULL_NONE подтяжка не подключаются
	 *		- @ref HAL_GPIO_PULL_UP подтяжка к питанию
	 *		- @ref HAL_GPIO_PULL_DOWN подтяжка к земле
	 */
	HAL_GPIO_PullTypeDef Pull;

	/**
	 * @brief Нагрузочная способность выводов.
	 * 
	 * Этот параметр должен быть одним из значений:
	 * 		- @ref HAL_GPIO_DS_2MA режим драйвера 2мА
	 * 		- @ref HAL_GPIO_DS_4MA режим драйвера 4мА
	 * 		- @ref HAL_GPIO_DS_8MA режим драйвера 8мА
	 */
	HAL_GPIO_DSTypeDef DS;
} GPIO_InitTypeDef;

/**
 * @brief Перечисление масок для линий внешних прерываний.
 */
typedef enum __HAL_GPIO_Line
{
	GPIO_LINE_0 = 0 << GPIO_IRQ_LINE_S, /**< Линия прерываний 0.*/
	GPIO_LINE_1 = 1 << GPIO_IRQ_LINE_S, /**< Линия прерываний 1.*/
	GPIO_LINE_2 = 2 << GPIO_IRQ_LINE_S, /**< Линия прерываний 2.*/
	GPIO_LINE_3 = 3 << GPIO_IRQ_LINE_S, /**< Линия прерываний 3.*/
	GPIO_LINE_4 = 4 << GPIO_IRQ_LINE_S, /**< Линия прерываний 4.*/
	GPIO_LINE_5 = 5 << GPIO_IRQ_LINE_S, /**< Линия прерываний 5.*/
	GPIO_LINE_6 = 6 << GPIO_IRQ_LINE_S, /**< Линия прерываний 6.*/
	GPIO_LINE_7 = 7 << GPIO_IRQ_LINE_S, /**< Линия прерываний 7.*/
} HAL_GPIO_Line;

/**
 * @brief Конфигурация линии прерывания.
 *
 * @note значение представляется в виде 0b0LLLMMMM, где L - биты номера линии, M - биты выбранного входа мультиплексора линии.
 */
typedef enum __HAL_GPIO_Line_Config
{
	GPIO_MUX_LINE_0_PORT0_0 = 0 | GPIO_LINE_0,	/**< PORT0_0  как вход для линии прерывания 0. */
	GPIO_MUX_LINE_0_PORT0_8 = 1 | GPIO_LINE_0,	/**< PORT0_8  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT1_0 = 2 | GPIO_LINE_0,	/**< PORT1_0  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT1_8 = 3 | GPIO_LINE_0,	/**< PORT1_8  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT2_0 = 4 | GPIO_LINE_0,	/**< PORT2_0  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT0_4 = 5 | GPIO_LINE_0,	/**< PORT0_4  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT0_12 = 6 | GPIO_LINE_0,	/**< PORT0_12 как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT1_4 = 7 | GPIO_LINE_0,	/**< PORT1_4  как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT1_12 = 8 | GPIO_LINE_0,	/**< PORT1_12 как вход для линии прерывания 0. */				
	GPIO_MUX_LINE_0_PORT2_4 = 9 | GPIO_LINE_0,	/**< PORT2_4  как вход для линии прерывания 0. */				

	GPIO_MUX_PORT0_0_LINE_0 = 0 | GPIO_LINE_0,	/**< PORT0_0  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT0_8_LINE_0 = 1 | GPIO_LINE_0,	/**< PORT0_8  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT1_0_LINE_0 = 2 | GPIO_LINE_0,	/**< PORT1_0  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT1_8_LINE_0 = 3 | GPIO_LINE_0,	/**< PORT1_8  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT2_0_LINE_0 = 4 | GPIO_LINE_0,	/**< PORT2_0  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT0_4_LINE_0 = 5 | GPIO_LINE_0,	/**< PORT0_4  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT0_12_LINE_0 = 6 | GPIO_LINE_0,	/**< PORT0_12 как вход для линии прерывания 0. */			
	GPIO_MUX_PORT1_4_LINE_0 = 7 | GPIO_LINE_0,	/**< PORT1_4  как вход для линии прерывания 0. */			
	GPIO_MUX_PORT1_12_LINE_0 = 8 | GPIO_LINE_0,	/**< PORT1_12 как вход для линии прерывания 0. */			
	GPIO_MUX_PORT2_4_LINE_0 = 9 | GPIO_LINE_0,	/**< PORT2_4  как вход для линии прерывания 0. */			

	GPIO_MUX_LINE_1_PORT0_1 = 0 | GPIO_LINE_1,	/**< PORT0_1  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT0_9 = 1 | GPIO_LINE_1,	/**< PORT0_9  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT1_1 = 2 | GPIO_LINE_1,	/**< PORT1_1  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT1_9 = 3 | GPIO_LINE_1,	/**< PORT1_9  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT2_1 = 4 | GPIO_LINE_1,	/**< PORT2_1  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT0_5 = 5 | GPIO_LINE_1,	/**< PORT0_5  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT0_13 = 6 | GPIO_LINE_1,	/**< PORT0_13 как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT1_5 = 7 | GPIO_LINE_1,	/**< PORT1_5  как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT1_13 = 8 | GPIO_LINE_1,	/**< PORT1_13 как вход для линии прерывания 1. */				
	GPIO_MUX_LINE_1_PORT2_5 = 9 | GPIO_LINE_1,	/**< PORT2_5  как вход для линии прерывания 1. */				

	GPIO_MUX_PORT0_1_LINE_1 = 0 | GPIO_LINE_1,	/**< PORT0_1  как вход для линии прерывания 1. */
	GPIO_MUX_PORT0_9_LINE_1 = 1 | GPIO_LINE_1,	/**< PORT0_9  как вход для линии прерывания 1. */
	GPIO_MUX_PORT1_1_LINE_1 = 2 | GPIO_LINE_1,	/**< PORT1_1  как вход для линии прерывания 1. */
	GPIO_MUX_PORT1_9_LINE_1 = 3 | GPIO_LINE_1,	/**< PORT1_9  как вход для линии прерывания 1. */
	GPIO_MUX_PORT2_1_LINE_1 = 4 | GPIO_LINE_1,	/**< PORT2_1  как вход для линии прерывания 1. */
	GPIO_MUX_PORT0_5_LINE_1 = 5 | GPIO_LINE_1,	/**< PORT0_5  как вход для линии прерывания 1. */
	GPIO_MUX_PORT0_13_LINE_1 = 6 | GPIO_LINE_1,	/**< PORT0_13 как вход для линии прерывания 1. */
	GPIO_MUX_PORT1_5_LINE_1 = 7 | GPIO_LINE_1,	/**< PORT1_5  как вход для линии прерывания 1. */
	GPIO_MUX_PORT1_13_LINE_1 = 8 | GPIO_LINE_1,	/**< PORT1_13 как вход для линии прерывания 1. */
	GPIO_MUX_PORT2_5_LINE_1 = 9 | GPIO_LINE_1,	/**< PORT2_5  как вход для линии прерывания 1. */

	GPIO_MUX_LINE_2_PORT0_2 = 0 | GPIO_LINE_2,	/**< PORT0_2  как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT0_10 = 1 | GPIO_LINE_2,	/**< PORT0_10 как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT1_2 = 2 | GPIO_LINE_2,	/**< PORT1_2  как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT1_10 = 3 | GPIO_LINE_2,	/**< PORT1_10 как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT2_2 = 4 | GPIO_LINE_2,	/**< PORT2_2  как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT0_6 = 5 | GPIO_LINE_2,	/**< PORT0_6  как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT0_14 = 6 | GPIO_LINE_2,	/**< PORT0_14 как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT1_6 = 7 | GPIO_LINE_2,	/**< PORT1_6  как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT1_14 = 8 | GPIO_LINE_2,	/**< PORT1_14 как вход для линии прерывания 2. */
	GPIO_MUX_LINE_2_PORT2_6 = 9 | GPIO_LINE_2,	/**< PORT2_6  как вход для линии прерывания 2. */

	GPIO_MUX_PORT0_2_LINE_2 = 0 | GPIO_LINE_2,	/**< PORT0_2  как вход для линии прерывания 2. */
	GPIO_MUX_PORT0_10_LINE_2 = 1 | GPIO_LINE_2,	/**< PORT0_10 как вход для линии прерывания 2. */
	GPIO_MUX_PORT1_2_LINE_2 = 2 | GPIO_LINE_2,	/**< PORT1_2  как вход для линии прерывания 2. */
	GPIO_MUX_PORT1_10_LINE_2 = 3 | GPIO_LINE_2,	/**< PORT1_10 как вход для линии прерывания 2. */
	GPIO_MUX_PORT2_2_LINE_2 = 4 | GPIO_LINE_2,	/**< PORT2_2  как вход для линии прерывания 2. */
	GPIO_MUX_PORT0_6_LINE_2 = 5 | GPIO_LINE_2,	/**< PORT0_6  как вход для линии прерывания 2. */
	GPIO_MUX_PORT0_14_LINE_2 = 6 | GPIO_LINE_2,	/**< PORT0_14 как вход для линии прерывания 2. */
	GPIO_MUX_PORT1_6_LINE_2 = 7 | GPIO_LINE_2,	/**< PORT1_6  как вход для линии прерывания 2. */
	GPIO_MUX_PORT1_14_LINE_2 = 8 | GPIO_LINE_2,	/**< PORT1_14 как вход для линии прерывания 2. */
	GPIO_MUX_PORT2_6_LINE_2 = 9 | GPIO_LINE_2,	/**< PORT2_6  как вход для линии прерывания 2. */

	GPIO_MUX_LINE_3_PORT0_3 = 0 | GPIO_LINE_3,	/**< PORT0_3  как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT0_11 = 1 | GPIO_LINE_3,	/**< PORT0_11 как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT1_3 = 2 | GPIO_LINE_3,	/**< PORT1_3  как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT1_11 = 3 | GPIO_LINE_3,	/**< PORT1_11 как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT2_3 = 4 | GPIO_LINE_3,	/**< PORT2_3  как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT0_7 = 5 | GPIO_LINE_3,	/**< PORT0_7  как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT0_15 = 6 | GPIO_LINE_3,	/**< PORT0_15 как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT1_7 = 7 | GPIO_LINE_3,	/**< PORT1_7  как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT1_15 = 8 | GPIO_LINE_3,	/**< PORT1_15 как вход для линии прерывания 3. */
	GPIO_MUX_LINE_3_PORT2_7 = 9 | GPIO_LINE_3,	/**< PORT2_7  как вход для линии прерывания 3. */

	GPIO_MUX_PORT0_3_LINE_3 = 0 | GPIO_LINE_3,	/**< PORT0_3  как вход для линии прерывания 3. */
	GPIO_MUX_PORT0_11_LINE_3 = 1 | GPIO_LINE_3,	/**< PORT0_11 как вход для линии прерывания 3. */
	GPIO_MUX_PORT1_3_LINE_3 = 2 | GPIO_LINE_3,	/**< PORT1_3  как вход для линии прерывания 3. */
	GPIO_MUX_PORT1_11_LINE_3 = 3 | GPIO_LINE_3,	/**< PORT1_11 как вход для линии прерывания 3. */
	GPIO_MUX_PORT2_3_LINE_3 = 4 | GPIO_LINE_3,	/**< PORT2_3  как вход для линии прерывания 3. */
	GPIO_MUX_PORT0_7_LINE_3 = 5 | GPIO_LINE_3,	/**< PORT0_7  как вход для линии прерывания 3. */
	GPIO_MUX_PORT0_15_LINE_3 = 6 | GPIO_LINE_3,	/**< PORT0_15 как вход для линии прерывания 3. */
	GPIO_MUX_PORT1_7_LINE_3 = 7 | GPIO_LINE_3,	/**< PORT1_7  как вход для линии прерывания 3. */
	GPIO_MUX_PORT1_15_LINE_3 = 8 | GPIO_LINE_3,	/**< PORT1_15 как вход для линии прерывания 3. */
	GPIO_MUX_PORT2_7_LINE_3 = 9 | GPIO_LINE_3,	/**< PORT2_7  как вход для линии прерывания 3. */

	GPIO_MUX_LINE_4_PORT0_4 = 0 | GPIO_LINE_4,	/**< PORT0_4  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT0_12 = 1 | GPIO_LINE_4,	/**< PORT0_12 как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT1_4 = 2 | GPIO_LINE_4,	/**< PORT1_4  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT1_12 = 3 | GPIO_LINE_4,	/**< PORT1_12 как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT2_4 = 4 | GPIO_LINE_4,	/**< PORT2_4  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT0_0 = 5 | GPIO_LINE_4,	/**< PORT0_0  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT0_8 = 6 | GPIO_LINE_4,	/**< PORT0_8  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT1_0 = 7 | GPIO_LINE_4,	/**< PORT1_0  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT1_8 = 8 | GPIO_LINE_4,	/**< PORT1_8  как вход для линии прерывания 4. */
	GPIO_MUX_LINE_4_PORT2_0 = 9 | GPIO_LINE_4,	/**< PORT2_0  как вход для линии прерывания 4. */

	GPIO_MUX_PORT0_4_LINE_4 = 0 | GPIO_LINE_4,	/**< PORT0_4  как вход для линии прерывания 4. */
	GPIO_MUX_PORT0_12_LINE_4 = 1 | GPIO_LINE_4,	/**< PORT0_12 как вход для линии прерывания 4. */
	GPIO_MUX_PORT1_4_LINE_4 = 2 | GPIO_LINE_4,	/**< PORT1_4  как вход для линии прерывания 4. */
	GPIO_MUX_PORT1_12_LINE_4 = 3 | GPIO_LINE_4,	/**< PORT1_12 как вход для линии прерывания 4. */
	GPIO_MUX_PORT2_4_LINE_4 = 4 | GPIO_LINE_4,	/**< PORT2_4  как вход для линии прерывания 4. */
	GPIO_MUX_PORT0_0_LINE_4 = 5 | GPIO_LINE_4,	/**< PORT0_0  как вход для линии прерывания 4. */
	GPIO_MUX_PORT0_8_LINE_4 = 6 | GPIO_LINE_4,	/**< PORT0_8  как вход для линии прерывания 4. */
	GPIO_MUX_PORT1_0_LINE_4 = 7 | GPIO_LINE_4,	/**< PORT1_0  как вход для линии прерывания 4. */
	GPIO_MUX_PORT1_8_LINE_4 = 8 | GPIO_LINE_4,	/**< PORT1_8  как вход для линии прерывания 4. */
	GPIO_MUX_PORT2_0_LINE_4 = 9 | GPIO_LINE_4,	/**< PORT2_0  как вход для линии прерывания 4. */

	GPIO_MUX_LINE_5_PORT0_5 = 0 | GPIO_LINE_5,	/**< PORT0_5  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT0_13 = 1 | GPIO_LINE_5,	/**< PORT0_13 как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT1_5 = 2 | GPIO_LINE_5,	/**< PORT1_5  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT1_13 = 3 | GPIO_LINE_5,	/**< PORT1_13 как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT2_5 = 4 | GPIO_LINE_5,	/**< PORT2_5  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT0_1 = 5 | GPIO_LINE_5,	/**< PORT0_1  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT0_9 = 6 | GPIO_LINE_5,	/**< PORT0_9  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT1_1 = 7 | GPIO_LINE_5,	/**< PORT1_1  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT1_9 = 8 | GPIO_LINE_5,	/**< PORT1_9  как вход для линии прерывания 5. */
	GPIO_MUX_LINE_5_PORT2_1 = 9 | GPIO_LINE_5,	/**< PORT2_1  как вход для линии прерывания 5. */

	GPIO_MUX_PORT0_5_LINE_5 = 0 | GPIO_LINE_5,	/**< PORT0_5  как вход для линии прерывания 5. */
	GPIO_MUX_PORT0_13_LINE_5 = 1 | GPIO_LINE_5,	/**< PORT0_13 как вход для линии прерывания 5. */
	GPIO_MUX_PORT1_5_LINE_5 = 2 | GPIO_LINE_5,	/**< PORT1_5  как вход для линии прерывания 5. */
	GPIO_MUX_PORT1_13_LINE_5 = 3 | GPIO_LINE_5,	/**< PORT1_13 как вход для линии прерывания 5. */
	GPIO_MUX_PORT2_5_LINE_5 = 4 | GPIO_LINE_5,	/**< PORT2_5  как вход для линии прерывания 5. */
	GPIO_MUX_PORT0_1_LINE_5 = 5 | GPIO_LINE_5,	/**< PORT0_1  как вход для линии прерывания 5. */
	GPIO_MUX_PORT0_9_LINE_5 = 6 | GPIO_LINE_5,	/**< PORT0_9  как вход для линии прерывания 5. */
	GPIO_MUX_PORT1_1_LINE_5 = 7 | GPIO_LINE_5,	/**< PORT1_1  как вход для линии прерывания 5. */
	GPIO_MUX_PORT1_9_LINE_5 = 8 | GPIO_LINE_5,	/**< PORT1_9  как вход для линии прерывания 5. */
	GPIO_MUX_PORT2_1_LINE_5 = 9 | GPIO_LINE_5,	/**< PORT2_1  как вход для линии прерывания 5. */

	GPIO_MUX_LINE_6_PORT0_6 = 0 | GPIO_LINE_6,	/**< PORT0_6  как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT0_14 = 1 | GPIO_LINE_6,	/**< PORT0_14 как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT1_6 = 2 | GPIO_LINE_6,	/**< PORT1_6  как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT1_14 = 3 | GPIO_LINE_6,	/**< PORT1_14 как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT2_6 = 4 | GPIO_LINE_6,	/**< PORT2_6  как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT0_2 = 5 | GPIO_LINE_6,	/**< PORT0_2  как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT0_10 = 6 | GPIO_LINE_6,	/**< PORT0_10 как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT1_2 = 7 | GPIO_LINE_6,	/**< PORT1_2  как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT1_10 = 8 | GPIO_LINE_6,	/**< PORT1_10 как вход для линии прерывания 6. */
	GPIO_MUX_LINE_6_PORT2_2 = 9 | GPIO_LINE_6,	/**< PORT2_2  как вход для линии прерывания 6. */

	GPIO_MUX_PORT0_6_LINE_6 = 0 | GPIO_LINE_6,	/**< PORT0_6  как вход для линии прерывания 6. */
	GPIO_MUX_PORT0_14_LINE_6 = 1 | GPIO_LINE_6,	/**< PORT0_14 как вход для линии прерывания 6. */
	GPIO_MUX_PORT1_6_LINE_6 = 2 | GPIO_LINE_6,	/**< PORT1_6  как вход для линии прерывания 6. */
	GPIO_MUX_PORT1_14_LINE_6 = 3 | GPIO_LINE_6,	/**< PORT1_14 как вход для линии прерывания 6. */
	GPIO_MUX_PORT2_6_LINE_6 = 4 | GPIO_LINE_6,	/**< PORT2_6  как вход для линии прерывания 6. */
	GPIO_MUX_PORT0_2_LINE_6 = 5 | GPIO_LINE_6,	/**< PORT0_2  как вход для линии прерывания 6. */
	GPIO_MUX_PORT0_10_LINE_6 = 6 | GPIO_LINE_6,	/**< PORT0_10 как вход для линии прерывания 6. */
	GPIO_MUX_PORT1_2_LINE_6 = 7 | GPIO_LINE_6,	/**< PORT1_2  как вход для линии прерывания 6. */
	GPIO_MUX_PORT1_10_LINE_6 = 8 | GPIO_LINE_6,	/**< PORT1_10 как вход для линии прерывания 6. */
	GPIO_MUX_PORT2_2_LINE_6 = 9 | GPIO_LINE_6,	/**< PORT2_2  как вход для линии прерывания 6. */

	GPIO_MUX_LINE_7_PORT0_7 = 0 | GPIO_LINE_7,	/**< PORT0_7  как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT0_15 = 1 | GPIO_LINE_7,	/**< PORT0_15 как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT1_7 = 2 | GPIO_LINE_7,	/**< PORT1_7  как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT1_15 = 3 | GPIO_LINE_7,	/**< PORT1_15 как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT2_7 = 4 | GPIO_LINE_7,	/**< PORT2_7  как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT0_3 = 5 | GPIO_LINE_7,	/**< PORT0_3  как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT0_11 = 6 | GPIO_LINE_7,	/**< PORT0_11 как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT1_3 = 7 | GPIO_LINE_7,	/**< PORT1_3  как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT1_11 = 8 | GPIO_LINE_7,	/**< PORT1_11 как вход для линии прерывания 7. */
	GPIO_MUX_LINE_7_PORT2_3 = 9 | GPIO_LINE_7,	/**< PORT2_3  как вход для линии прерывания 7. */

	GPIO_MUX_PORT0_7_LINE_7 = 0 | GPIO_LINE_7,	/**< PORT0_7  как вход для линии прерывания 7. */
	GPIO_MUX_PORT0_15_LINE_7 = 1 | GPIO_LINE_7,	/**< PORT0_15 как вход для линии прерывания 7. */
	GPIO_MUX_PORT1_7_LINE_7 = 2 | GPIO_LINE_7,	/**< PORT1_7  как вход для линии прерывания 7. */
	GPIO_MUX_PORT1_15_LINE_7 = 3 | GPIO_LINE_7,	/**< PORT1_15 как вход для линии прерывания 7. */
	GPIO_MUX_PORT2_7_LINE_7 = 4 | GPIO_LINE_7,	/**< PORT2_7  как вход для линии прерывания 7. */
	GPIO_MUX_PORT0_3_LINE_7 = 5 | GPIO_LINE_7,	/**< PORT0_3  как вход для линии прерывания 7. */
	GPIO_MUX_PORT0_11_LINE_7 = 6 | GPIO_LINE_7,	/**< PORT0_11 как вход для линии прерывания 7. */
	GPIO_MUX_PORT1_3_LINE_7 = 7 | GPIO_LINE_7,	/**< PORT1_3  как вход для линии прерывания 7. */
	GPIO_MUX_PORT1_11_LINE_7 = 8 | GPIO_LINE_7,	/**< PORT1_11 как вход для линии прерывания 7. */
	GPIO_MUX_PORT2_3_LINE_7 = 9 | GPIO_LINE_7,	/**< PORT2_3  как вход для линии прерывания 7. */

} HAL_GPIO_Line_Config;

/**
 * @brief Режим прерывания линии GPIO.
 * \note Функция инициализации @ref HAL_GPIO_InitInterruptLine не включает прерывания GPIO в EPIC.
 */
typedef enum __HAL_GPIO_InterruptMode
{
	GPIO_INT_MODE_LOW = 0,												   /**< Режим прерывания по низкому уровню на выводе. */
	GPIO_INT_MODE_HIGH = GPIO_MODE_BIT_LEVEL_M,							   /**< Режим прерывания по высокому уровню на выводе. */
	GPIO_INT_MODE_FALLING = GPIO_MODE_BIT_EDGE_M,						   /**< Режим прерывания по смене уровня на выводе с высокого на низкий. */
	GPIO_INT_MODE_RISING = GPIO_MODE_BIT_LEVEL_M | GPIO_MODE_BIT_EDGE_M,   /**< Режим прерывания по смене уровня на выводе с низкого на высокий. */
	GPIO_INT_MODE_CHANGE = GPIO_MODE_BIT_EDGE_M | GPIO_MODE_BIT_ANYEDGE_M, /**< Режим прерывания по изменению уровня на выводе. */
} HAL_GPIO_InterruptMode;

HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef *GPIO_x, GPIO_InitTypeDef *GPIO_Init);
HAL_StatusTypeDef HAL_GPIO_PinConfig(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin, HAL_GPIO_ModeTypeDef mode, HAL_GPIO_PullTypeDef pull, HAL_GPIO_DSTypeDef driveStrength);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin, GPIO_PinState pinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin);
HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line_Config mux, HAL_GPIO_InterruptMode mode);
HAL_StatusTypeDef HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irqLine);
uint32_t HAL_GPIO_LineInterruptState(HAL_GPIO_Line irqLine);
GPIO_PinState HAL_GPIO_LinePinState(HAL_GPIO_Line irqLine);
void HAL_GPIO_ClearInterrupts();

#endif // MIK32_HAL_GPIO