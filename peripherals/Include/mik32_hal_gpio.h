#ifndef MIK32_HAL_GPIO_NEW
#define MIK32_HAL_GPIO_NEW

#include "mik32_hal_def.h"
#include "pad_config.h"
#include "gpio.h"
#include "gpio_irq.h"
#include "mcu32_memory_map.h"

#define GPIO_MODE_BIT_LEVEL_S 0	  // Позиция бита для режима LEVEL_SET в HAL_GPIO_InterruptMode
#define GPIO_MODE_BIT_EDGE_S 1	  // Позиция бита для режима EDGE в HAL_GPIO_InterruptMode
#define GPIO_MODE_BIT_ANYEDGE_S 2 // Позиция бита для режима ANYEDGE в HAL_GPIO_InterruptMode
#define GPIO_MODE_BIT_LEVEL_M (1 << GPIO_MODE_BIT_LEVEL_S)
#define GPIO_MODE_BIT_EDGE_M (1 << GPIO_MODE_BIT_EDGE_S)
#define GPIO_MODE_BIT_ANYEDGE_M (1 << GPIO_MODE_BIT_ANYEDGE_S)
#define GPIO_IRQ_LINE_S 4 // Позиция номера канала в HAL_GPIO_Line_Config


#define PORT0	0
#define PORT1	1
#define PORT2	2
#define PORT_INDEX_S 	30	// Смещение номера порта (PORT0, PORT1, PORT2) в PORTx_y
#define PORT_INDEX_M 	(0b11 << PORT_INDEX_S)   

typedef enum __HAL_PinMapNewTypeDef
{									 // Режим выводов: 0b00     0b0b                0b10                0b11
	PORT0_0 = (PORT0 << PORT_INDEX_S) | (1 << 0), 	// GPIO, 	SPI0_MISO, 			TIMER32_1_CH1, 		REF_TST
	PORT0_1 = (PORT0 << PORT_INDEX_S) | (1 << 1), 	// GPIO, 	SPI0_MOSI, 			TIMER32_1_CH2
	PORT0_2 = (PORT0 << PORT_INDEX_S) | (1 << 2), 	// GPIO, 	SPI0_CLK, 			TIMER32_1_CH3, 		ADC2
	PORT0_3 = (PORT0 << PORT_INDEX_S) | (1 << 3), 	// GPIO, 	SPI0_N_SS_IN, 		TIMER32_1_CH4
	PORT0_4 = (PORT0 << PORT_INDEX_S) | (1 << 4), 	// GPIO, 	SPI0_N_SS_OUT_0, 	TIMER32_1_TX, 		ADC3
	PORT0_5 = (PORT0 << PORT_INDEX_S) | (1 << 5), 	// GPIO, 	UART0_RXD, 			TIMER16_0_IN1
	PORT0_6 = (PORT0 << PORT_INDEX_S) | (1 << 6), 	// GPIO, 	UART0_TXD, 			TIMER16_0_IN2
	PORT0_7 = (PORT0 << PORT_INDEX_S) | (1 << 7), 	// GPIO, 	UART0_NCTS, 		TIMER16_0_OUT, 		ADC4
	PORT0_8 = (PORT0 << PORT_INDEX_S) | (1 << 8), 	// GPIO, 	UART0_NRTS, 		TIMER16_1_IN1
	PORT0_9 = (PORT0 << PORT_INDEX_S) | (1 << 9), 	// GPIO, 	I2C0_SDA, 			TIMER16_1_IN2, 		ADC5
	PORT0_10 = (PORT0 << PORT_INDEX_S) | (1 << 10), // GPIO, 	I2C0_SCL, 			TIMER16_1_OUT
	PORT0_11 = (PORT0 << PORT_INDEX_S) | (1 << 11), // GPIO, 	TDI, 				TIMER16_2_IN1, 		ADC6
	PORT0_12 = (PORT0 << PORT_INDEX_S) | (1 << 12), // GPIO, 	TCK, 				TIMER16_2_IN2
	PORT0_13 = (PORT0 << PORT_INDEX_S) | (1 << 13), // GPIO, 	TMS, 				TIMER16_2_OUT, 		ADC7
	PORT0_14 = (PORT0 << PORT_INDEX_S) | (1 << 14), // GPIO, 	TRST
	PORT0_15 = (PORT0 << PORT_INDEX_S) | (1 << 15), // GPIO, 	TDO
	PORT0_ALL = (PORT0 << PORT_INDEX_S) | 0xFFFF, 	// Маска для всех выводов на PORT0                                                                                                                    
                                                                                                                
	PORT1_0 = (PORT1 << PORT_INDEX_S) | (1 << 0), 	// GPIO, 	SPI1_MISO, 			TIMER32_2_CH1
	PORT1_1 = (PORT1 << PORT_INDEX_S) | (1 << 1), 	// GPIO, 	SPI1_MOSI, 			TIMER32_2_CH2
	PORT1_2 = (PORT1 << PORT_INDEX_S) | (1 << 2), 	// GPIO, 	SPI1_CLK, 			TIMER32_2_CH3
	PORT1_3 = (PORT1 << PORT_INDEX_S) | (1 << 3), 	// GPIO, 	SPI1_N_SS_IN, 		TIMER32_2_CH4
	PORT1_4 = (PORT1 << PORT_INDEX_S) | (1 << 4), 	// GPIO, 	SPI1_N_SS_OUT_0, 	TIMER32_2_TX
	PORT1_5 = (PORT1 << PORT_INDEX_S) | (1 << 5), 	// GPIO, 	SPI1_N_SS_OUT_1, 	UART0_CK, 			ADC0
	PORT1_6 = (PORT1 << PORT_INDEX_S) | (1 << 6), 	// GPIO, 	SPI1_N_SS_OUT_2, 	UART0_DDIS
	PORT1_7 = (PORT1 << PORT_INDEX_S) | (1 << 7), 	// GPIO, 	SPI1_N_SS_OUT_3, 	DIGITAL_TEST_PAD,	ADC1
	PORT1_8 = (PORT1 << PORT_INDEX_S) | (1 << 8), 	// GPIO, 	UART1_RXD
	PORT1_9 = (PORT1 << PORT_INDEX_S) | (1 << 9), 	// GPIO, 	UART1_TXD
	PORT1_10 = (PORT1 << PORT_INDEX_S) | (1 << 10), // GPIO, 	UART1_NCTS
	PORT1_11 = (PORT1 << PORT_INDEX_S) | (1 << 11), // GPIO, 	UART1_NRTS, 		REF_ADC_DAC
	PORT1_12 = (PORT1 << PORT_INDEX_S) | (1 << 12), // GPIO, 	I2C1_SDA, 			UART0_NDTR, 		DAC0
	PORT1_13 = (PORT1 << PORT_INDEX_S) | (1 << 13), // GPIO, 	I2C1_SCL, 			UART0_NDCD, 		DAC1
	PORT1_14 = (PORT1 << PORT_INDEX_S) | (1 << 14), // GPIO, 	SPI0_N_SS_OUT_1, 	UART0_NDSR
	PORT1_15 = (PORT1 << PORT_INDEX_S) | (1 << 15), // GPIO, 	SPI0_N_SS_OUT_2, 	UART0_NRI, 			TEST MUX OUT
	PORT1_ALL = (PORT1 << PORT_INDEX_S) | 0xFFFF,   // Маска для всех выводов на PORT1                                                                                                                  
                                                                                                                
	PORT2_0 = (PORT2 << PORT_INDEX_S) | 1 << 0, 	// GPIO, 	SPIFI_SCLK, 		UART1_NDTR
	PORT2_1 = (PORT2 << PORT_INDEX_S) | 1 << 1, 	// GPIO, 	SPIFI_CS, 			UART1_DCD
	PORT2_2 = (PORT2 << PORT_INDEX_S) | 1 << 2, 	// GPIO, 	SPIFI_DATA_0, 		UART1_NDSR
	PORT2_3 = (PORT2 << PORT_INDEX_S) | 1 << 3, 	// GPIO, 	SPIFI_DATA_1, 		UART1_NRI
	PORT2_4 = (PORT2 << PORT_INDEX_S) | 1 << 4, 	// GPIO, 	SPIFI_DATA_2
	PORT2_5 = (PORT2 << PORT_INDEX_S) | 1 << 5, 	// GPIO, 	SPIFI_DATA_3
	PORT2_6 = (PORT2 << PORT_INDEX_S) | 1 << 6, 	// GPIO, 	SPI0_N_SS_OUT_3, 	UART1_CK
	PORT2_7 = (PORT2 << PORT_INDEX_S) | 1 << 7, 	// GPIO
	PORT2_ALL = (PORT2 << PORT_INDEX_S) | 0xFF      // Маска для всех выводов на PORT2                                                                                                                 
} HAL_PinMapNewTypeDef;

typedef enum __GPIO_PinState
{
	GPIO_PIN_LOW = 0,
	GPIO_PIN_HIGH = 1
} GPIO_PinState;

typedef enum __HAL_GPIO_ModeTypeDef
{
#ifdef MIK32V0
	HAL_GPIO_MODE_GPIO_INPUT = 0b101,  /* Режим вывода - GPIO. Вход */
	HAL_GPIO_MODE_GPIO_OUTPUT = 0b001, /* Режим вывода - GPIO. Выход */
	HAL_GPIO_MODE_SERIAL = 0b00,	   /* Режим вывода - последовательный интерфейс */
#endif

#ifdef MIK32V2
	HAL_GPIO_MODE_GPIO_INPUT = 0b100,  /* Режим вывода - GPIO. Вход */
	HAL_GPIO_MODE_GPIO_OUTPUT = 0b000, /* Режим вывода - GPIO. Выход */
	HAL_GPIO_MODE_SERIAL = 0b01,	   /* Режим вывода - последовательный интерфейс */
#endif

	HAL_GPIO_MODE_TIMER_SERIAL = 0b10, /* Режим вывода - последовательный интерфейс или таймер */
	HAL_GPIO_MODE_ANALOG = 0b11		   /* Аналоговый сигнал */
} HAL_GPIO_ModeTypeDef;

typedef enum __HAL_GPIO_PullTypeDef
{
	HAL_GPIO_PULL_NONE = 0b00, /* Подтяжки не подключаются */
	HAL_GPIO_PULL_UP = 0b01,   /* Подтяжки к питанию */
	HAL_GPIO_PULL_DOWN = 0b10  /* Подтяжки к земле */
} HAL_GPIO_PullTypeDef;

typedef enum __HAL_GPIO_DSTypeDef
{
	HAL_GPIO_DS_2MA = 0b00, /* 2мА */
	HAL_GPIO_DS_4MA = 0b01, /* 4мА */
	HAL_GPIO_DS_8MA = 0b10	/* 8мА */
} HAL_GPIO_DSTypeDef;

typedef struct __GPIO_InitTypeDef
{
	HAL_PinMapNewTypeDef Pin;  /* Маска выводов */
	HAL_GPIO_ModeTypeDef Mode; /* Режим выводов или направление GPIO */
	HAL_GPIO_PullTypeDef Pull; /* Подтяжка */
	HAL_GPIO_DSTypeDef DS;	   /* Нагрузочная способность выводов. Выходной ток */
} GPIO_InitTypeDef;

HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin);

typedef enum __HAL_GPIO_Line
{
	GPIO_LINE_0 = 0 << GPIO_IRQ_LINE_S, /* Линия прерываний 0 */
	GPIO_LINE_1 = 1 << GPIO_IRQ_LINE_S, /* Линия прерываний 1 */
	GPIO_LINE_2 = 2 << GPIO_IRQ_LINE_S, /* Линия прерываний 2 */
	GPIO_LINE_3 = 3 << GPIO_IRQ_LINE_S, /* Линия прерываний 3 */
	GPIO_LINE_4 = 4 << GPIO_IRQ_LINE_S, /* Линия прерываний 4 */
	GPIO_LINE_5 = 5 << GPIO_IRQ_LINE_S, /* Линия прерываний 5 */
	GPIO_LINE_6 = 6 << GPIO_IRQ_LINE_S, /* Линия прерываний 6 */
	GPIO_LINE_7 = 7 << GPIO_IRQ_LINE_S, /* Линия прерываний 7 */
} HAL_GPIO_Line;

/**
 * @brief Interrupt line configuration
 *
 * @note use form 0b0LLLMMMM, where L - line number bits, M - mux configuration bits
 */
typedef enum __HAL_GPIO_Line_Config
{
	GPIO_MUX_LINE_0_PORT0_0 = 0 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_8 = 1 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_0 = 2 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_8 = 3 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT2_0 = 4 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_4 = 5 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT0_12 = 6 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_4 = 7 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT1_12 = 8 | GPIO_LINE_0,
	GPIO_MUX_LINE_0_PORT2_4 = 9 | GPIO_LINE_0,

	GPIO_MUX_PORT0_0_LINE_0 = 0 | GPIO_LINE_0,
	GPIO_MUX_PORT0_8_LINE_0 = 1 | GPIO_LINE_0,
	GPIO_MUX_PORT1_0_LINE_0 = 2 | GPIO_LINE_0,
	GPIO_MUX_PORT1_8_LINE_0 = 3 | GPIO_LINE_0,
	GPIO_MUX_PORT2_0_LINE_0 = 4 | GPIO_LINE_0,
	GPIO_MUX_PORT0_4_LINE_0 = 5 | GPIO_LINE_0,
	GPIO_MUX_PORT0_12_LINE_0 = 6 | GPIO_LINE_0,
	GPIO_MUX_PORT1_4_LINE_0 = 7 | GPIO_LINE_0,
	GPIO_MUX_PORT1_12_LINE_0 = 8 | GPIO_LINE_0,
	GPIO_MUX_PORT2_4_LINE_0 = 9 | GPIO_LINE_0,

	GPIO_MUX_LINE_1_PORT0_1 = 0 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_9 = 1 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_1 = 2 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_9 = 3 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT2_1 = 4 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_5 = 5 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT0_13 = 6 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_5 = 7 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT1_13 = 8 | GPIO_LINE_1,
	GPIO_MUX_LINE_1_PORT2_5 = 9 | GPIO_LINE_1,

	GPIO_MUX_PORT0_1_LINE_1 = 0 | GPIO_LINE_1,
	GPIO_MUX_PORT0_9_LINE_1 = 1 | GPIO_LINE_1,
	GPIO_MUX_PORT1_1_LINE_1 = 2 | GPIO_LINE_1,
	GPIO_MUX_PORT1_9_LINE_1 = 3 | GPIO_LINE_1,
	GPIO_MUX_PORT2_1_LINE_1 = 4 | GPIO_LINE_1,
	GPIO_MUX_PORT0_5_LINE_1 = 5 | GPIO_LINE_1,
	GPIO_MUX_PORT0_13_LINE_1 = 6 | GPIO_LINE_1,
	GPIO_MUX_PORT1_5_LINE_1 = 7 | GPIO_LINE_1,
	GPIO_MUX_PORT1_13_LINE_1 = 8 | GPIO_LINE_1,
	GPIO_MUX_PORT2_5_LINE_1 = 9 | GPIO_LINE_1,

	GPIO_MUX_LINE_2_PORT0_2 = 0 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_10 = 1 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_2 = 2 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_10 = 3 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT2_2 = 4 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_6 = 5 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT0_14 = 6 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_6 = 7 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT1_14 = 8 | GPIO_LINE_2,
	GPIO_MUX_LINE_2_PORT2_6 = 9 | GPIO_LINE_2,

	GPIO_MUX_PORT0_2_LINE_2 = 0 | GPIO_LINE_2,
	GPIO_MUX_PORT0_10_LINE_2 = 1 | GPIO_LINE_2,
	GPIO_MUX_PORT1_2_LINE_2 = 2 | GPIO_LINE_2,
	GPIO_MUX_PORT1_10_LINE_2 = 3 | GPIO_LINE_2,
	GPIO_MUX_PORT2_2_LINE_2 = 4 | GPIO_LINE_2,
	GPIO_MUX_PORT0_6_LINE_2 = 5 | GPIO_LINE_2,
	GPIO_MUX_PORT0_14_LINE_2 = 6 | GPIO_LINE_2,
	GPIO_MUX_PORT1_6_LINE_2 = 7 | GPIO_LINE_2,
	GPIO_MUX_PORT1_14_LINE_2 = 8 | GPIO_LINE_2,
	GPIO_MUX_PORT2_6_LINE_2 = 9 | GPIO_LINE_2,

	GPIO_MUX_LINE_3_PORT0_3 = 0 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_11 = 1 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_3 = 2 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_11 = 3 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT2_3 = 4 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_7 = 5 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT0_15 = 6 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_7 = 7 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT1_15 = 8 | GPIO_LINE_3,
	GPIO_MUX_LINE_3_PORT2_7 = 9 | GPIO_LINE_3,

	GPIO_MUX_PORT0_3_LINE_3 = 0 | GPIO_LINE_3,
	GPIO_MUX_PORT0_11_LINE_3 = 1 | GPIO_LINE_3,
	GPIO_MUX_PORT1_3_LINE_3 = 2 | GPIO_LINE_3,
	GPIO_MUX_PORT1_11_LINE_3 = 3 | GPIO_LINE_3,
	GPIO_MUX_PORT2_3_LINE_3 = 4 | GPIO_LINE_3,
	GPIO_MUX_PORT0_7_LINE_3 = 5 | GPIO_LINE_3,
	GPIO_MUX_PORT0_15_LINE_3 = 6 | GPIO_LINE_3,
	GPIO_MUX_PORT1_7_LINE_3 = 7 | GPIO_LINE_3,
	GPIO_MUX_PORT1_15_LINE_3 = 8 | GPIO_LINE_3,
	GPIO_MUX_PORT2_7_LINE_3 = 9 | GPIO_LINE_3,

	GPIO_MUX_LINE_4_PORT0_4 = 0 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_12 = 1 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_4 = 2 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_12 = 3 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT2_4 = 4 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_0 = 5 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT0_8 = 6 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_0 = 7 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT1_8 = 8 | GPIO_LINE_4,
	GPIO_MUX_LINE_4_PORT2_0 = 9 | GPIO_LINE_4,

	GPIO_MUX_PORT0_4_LINE_4 = 0 | GPIO_LINE_4,
	GPIO_MUX_PORT0_12_LINE_4 = 1 | GPIO_LINE_4,
	GPIO_MUX_PORT1_4_LINE_4 = 2 | GPIO_LINE_4,
	GPIO_MUX_PORT1_12_LINE_4 = 3 | GPIO_LINE_4,
	GPIO_MUX_PORT2_4_LINE_4 = 4 | GPIO_LINE_4,
	GPIO_MUX_PORT0_0_LINE_4 = 5 | GPIO_LINE_4,
	GPIO_MUX_PORT0_8_LINE_4 = 6 | GPIO_LINE_4,
	GPIO_MUX_PORT1_0_LINE_4 = 7 | GPIO_LINE_4,
	GPIO_MUX_PORT1_8_LINE_4 = 8 | GPIO_LINE_4,
	GPIO_MUX_PORT2_0_LINE_4 = 9 | GPIO_LINE_4,

	GPIO_MUX_LINE_5_PORT0_5 = 0 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_13 = 1 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_5 = 2 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_13 = 3 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT2_5 = 4 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_1 = 5 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT0_9 = 6 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_1 = 7 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT1_9 = 8 | GPIO_LINE_5,
	GPIO_MUX_LINE_5_PORT2_1 = 9 | GPIO_LINE_5,

	GPIO_MUX_PORT0_5_LINE_5 = 0 | GPIO_LINE_5,
	GPIO_MUX_PORT0_13_LINE_5 = 1 | GPIO_LINE_5,
	GPIO_MUX_PORT1_5_LINE_5 = 2 | GPIO_LINE_5,
	GPIO_MUX_PORT1_13_LINE_5 = 3 | GPIO_LINE_5,
	GPIO_MUX_PORT2_5_LINE_5 = 4 | GPIO_LINE_5,
	GPIO_MUX_PORT0_1_LINE_5 = 5 | GPIO_LINE_5,
	GPIO_MUX_PORT0_9_LINE_5 = 6 | GPIO_LINE_5,
	GPIO_MUX_PORT1_1_LINE_5 = 7 | GPIO_LINE_5,
	GPIO_MUX_PORT1_9_LINE_5 = 8 | GPIO_LINE_5,
	GPIO_MUX_PORT2_1_LINE_5 = 9 | GPIO_LINE_5,

	GPIO_MUX_LINE_6_PORT0_6 = 0 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_14 = 1 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_6 = 2 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_14 = 3 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT2_6 = 4 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_2 = 5 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT0_10 = 6 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_2 = 7 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT1_10 = 8 | GPIO_LINE_6,
	GPIO_MUX_LINE_6_PORT2_2 = 9 | GPIO_LINE_6,

	GPIO_MUX_PORT0_6_LINE_6 = 0 | GPIO_LINE_6,
	GPIO_MUX_PORT0_14_LINE_6 = 1 | GPIO_LINE_6,
	GPIO_MUX_PORT1_6_LINE_6 = 2 | GPIO_LINE_6,
	GPIO_MUX_PORT1_14_LINE_6 = 3 | GPIO_LINE_6,
	GPIO_MUX_PORT2_6_LINE_6 = 4 | GPIO_LINE_6,
	GPIO_MUX_PORT0_2_LINE_6 = 5 | GPIO_LINE_6,
	GPIO_MUX_PORT0_10_LINE_6 = 6 | GPIO_LINE_6,
	GPIO_MUX_PORT1_2_LINE_6 = 7 | GPIO_LINE_6,
	GPIO_MUX_PORT1_10_LINE_6 = 8 | GPIO_LINE_6,
	GPIO_MUX_PORT2_2_LINE_6 = 9 | GPIO_LINE_6,

	GPIO_MUX_LINE_7_PORT0_7 = 0 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_15 = 1 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_7 = 2 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_15 = 3 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT2_7 = 4 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_3 = 5 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT0_11 = 6 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_3 = 7 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT1_11 = 8 | GPIO_LINE_7,
	GPIO_MUX_LINE_7_PORT2_3 = 9 | GPIO_LINE_7,

	GPIO_MUX_PORT0_7_LINE_7 = 0 | GPIO_LINE_7,
	GPIO_MUX_PORT0_15_LINE_7 = 1 | GPIO_LINE_7,
	GPIO_MUX_PORT1_7_LINE_7 = 2 | GPIO_LINE_7,
	GPIO_MUX_PORT1_15_LINE_7 = 3 | GPIO_LINE_7,
	GPIO_MUX_PORT2_7_LINE_7 = 4 | GPIO_LINE_7,
	GPIO_MUX_PORT0_3_LINE_7 = 5 | GPIO_LINE_7,
	GPIO_MUX_PORT0_11_LINE_7 = 6 | GPIO_LINE_7,
	GPIO_MUX_PORT1_3_LINE_7 = 7 | GPIO_LINE_7,
	GPIO_MUX_PORT1_11_LINE_7 = 8 | GPIO_LINE_7,
	GPIO_MUX_PORT2_3_LINE_7 = 9 | GPIO_LINE_7,

} HAL_GPIO_Line_Config;

/** Режим прерывания линии GPIO
 * \note Функция инициализации не включает прерывания GPIO в EPIC
 */
typedef enum __HAL_GPIO_InterruptMode
{
	GPIO_INT_MODE_LOW = 0,												   /* Режим прерывания по низкому уровню на выводе */
	GPIO_INT_MODE_HIGH = GPIO_MODE_BIT_LEVEL_M,							   /* Режим прерывания по высокому уровню на выводе */
	GPIO_INT_MODE_FALLING = GPIO_MODE_BIT_EDGE_M,						   /* Режим прерывания по смене уровня на выводе с высокого на низкий */
	GPIO_INT_MODE_RISING = GPIO_MODE_BIT_LEVEL_M | GPIO_MODE_BIT_EDGE_M,   /* Режим прерывания по смене уровня на выводе с низкого на высокий */
	GPIO_INT_MODE_CHANGE = GPIO_MODE_BIT_EDGE_M | GPIO_MODE_BIT_ANYEDGE_M, /* Режим прерывания по изменению уровня на выводе */
} HAL_GPIO_InterruptMode;

/** Функция инициализации линии прерывания
 *

 * \param mux Настройка мультиплексора линии прерывания
 * \param mode Режим линии прерывания
 * \note Номер линии прерывания можно получить после настройки мультиплексора.
 * Введите в mux GPIO_MUX_GPIO_X_X, где X - номера порта и вывода,
 * и нажмите Ctrl+Пробел: появятся варианты констант для данного вывода,
 * далее достаточно выбрать константу для доступной линии.
 * В mode введите GPIO_INT_MODE и нажмите Ctrl+Пробел: появятся варианты типов прерываний канала.
 */
HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line_Config mux, HAL_GPIO_InterruptMode mode);

/** Функция деинициализации линии прерывания, выключает линию и сбрасывает её настройки
 *
 * \param irq_line Номер линии прерывания
 */
HAL_StatusTypeDef HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irq_line);

/** Функция получения состояния линии прерывания
 *
 * \param irq_line Номер линии прерывания
 * \return Возвращает true если сработало прерывание данной линии
 */
uint32_t HAL_GPIO_LineInterruptState(HAL_GPIO_Line irq_line);

/** Функция чтения логического уровня вывода, подключенного к линии прерывания
 *
 * \param irq_line Номер линии прерывания
 * \return Логический уровень вывода
 */
GPIO_PinState HAL_GPIO_LinePinState(HAL_GPIO_Line irq_line);

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