#ifndef MIK32_HAL_DMA
#define MIK32_HAL_DMA

#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "stddef.h"
#include "dma_config.h"
#include "mik32_memory_map.h"
#include "mik32_hal_def.h"



#define DMA_TIMEOUT_DEFAULT 1000000		/**< Стандартная задержка для ожидания timeout. */


/**
 * @brief Разрешить или запретить читать текущий статус канала.
 * 
 * При разрешении чтения текущего статуса канала регистры CHx_DST, CHx_SRC, CHx_LEN возвращают текущие значения,
 * а в CHx_CFG при считывании меняются битовые поля. 
 */
typedef enum __HAL_DMA_CurrentValueTypeDef
{
	DMA_CURRENT_VALUE_ENABLE = 0,   /**< Текущие значения. */
	DMA_CURRENT_VALUE_DISABLE = 1   /**< Значения при настройке. */
} HAL_DMA_CurrentValueTypeDef;

/**
 * @brief Номер канала.
 * 
 * При совпадении приоритетов каналов меньший номер канала является более приоритетным.
 */
typedef enum __HAL_DMA_ChannelIndexTypeDef
{
	DMA_CHANNEL_0 = 0,
	DMA_CHANNEL_1 = 1,
	DMA_CHANNEL_2 = 2,
	DMA_CHANNEL_3 = 3
} HAL_DMA_ChannelIndexTypeDef;

/**
 * @brief Приоритет канала.
 * 
 * Арбитраж выбора канала происходит за два такта удержания линии запроса. 
 * Если одновременно поступают несколько запросов ПДП, то схема арбитража выбирает наиболее 
 * приоритетную линию запроса в соответствии с программной установкой каждого из каналов.
 * При совпадении приоритетов каналов меньший номер канала является более приоритетным.
 */
typedef enum __HAL_DMA_ChannelPriorityTypeDef
{
	DMA_CHANNEL_PRIORITY_LOW = 0b00,   		/**< Низкий приоритет. */
	DMA_CHANNEL_PRIORITY_MEDIUM  = 0b01,   	/**< Средний приоритет. */ 
	DMA_CHANNEL_PRIORITY_HIGH = 0b10,   	/**< Высокий приоритет. */
	DMA_CHANNEL_PRIORITY_VERY_HIGH = 0b11   /**< Высший приоритет. */
} HAL_DMA_ChannelPriorityTypeDef;

/**
 * @brief Режим адресации источника или назначения.
 */
typedef enum __HAL_DMA_ChannelModeTypeDef
{
	DMA_CHANNEL_MODE_PERIPHERY = 0,		/**< Режим адреса - периферия. */
	DMA_CHANNEL_MODE_MEMORY = 1			/**< Режим адреса - память. */
} HAL_DMA_ChannelModeTypeDef;


/**
 * @brief Наличие инкремента адреса источника или назначения.
 */
typedef enum __HAL_DMA_ChannelIncTypeDef
{
	DMA_CHANNEL_INC_DISABLE = 0,	/* Адрес не инкрементируется. */
	DMA_CHANNEL_INC_ENABLE = 1		/* Адрес инкрементируется. */
} HAL_DMA_ChannelIncTypeDef;

/**
 * @brief Разрядность адреса источника или назначения.
 * @warning Разрядность должна быть кратна количеству байт пересылки (LEN).
 */
typedef enum __HAL_DMA_ChannelSizeTypeDef
{
	DMA_CHANNEL_SIZE_BYTE = 0,			/**< Байт. */
	DMA_CHANNEL_SIZE_HALFWORD = 1,		/**< Полуслово. */
	DMA_CHANNEL_SIZE_WORD = 2			/**< Слово. */
} HAL_DMA_ChannelSizeTypeDef;

/**
 * @brief Периферийные линии источника или назначения.
 */
typedef enum __HAL_DMA_ChannelRequestTypeDef
{
	DMA_CHANNEL_USART_0_REQUEST = 0,
	DMA_CHANNEL_USART_1_REQUEST = 1,
	DMA_CHANNEL_CRYPTO_REQUEST = 2,
	DMA_CHANNEL_SPI_0_REQUEST = 3,
	DMA_CHANNEL_SPI_1_REQUEST = 4,
	DMA_CHANNEL_I2C_0_REQUEST = 5,
	DMA_CHANNEL_I2C_1_REQUEST = 6,
	DMA_CHANNEL_SPIFI_REQUEST = 7,
	DMA_CHANNEL_TIMER32_1_REQUEST = 8,
	DMA_CHANNEL_TIMER32_2_REQUEST = 9,

#ifdef MIK32V0
	DMA_CHANNEL_TIMER32_0_REQUEST = 10
#else // MIK32V2
	DMA_CHANNEL_DAC_0_REQUEST = 10,
	DMA_CHANNEL_DAC_1_REQUEST = 11,
	DMA_CHANNEL_TIMER32_0_REQUEST = 12
#endif // MIK32V0

} HAL_DMA_ChannelRequestTypeDef;

/**
 * @brief Разрешение работы логики с откликом для адресата источника и назначения.
 * 
 * Используется при работе с Timer32. При работе логики с откликом пакет данных передается 
 * каждый раз при переполнении таймера. Если работа с откликом запрещена, то все пакеты передаются сразу после 
 * первого переполнения таймера.
 */
typedef enum
{
	DMA_CHANNEL_ACK_DISABLE = 0,	/**< Работа логики с откликом запрещена. */
	DMA_CHANNEL_ACK_ENABLE = 1 		/**< Работа логики с откликом разрешена. */
} HAL_DMA_ChannelACKTypeDef;

/**
 * @brief Разрешение прерываний.
 * 
 * Используется при настройке глобального прерывания DMA, прерывания DMA при возникновении ошибки и 
 * локального прерывания при завершении работы канала.
 */
typedef enum
{
	DMA_IRQ_DISABLE = 0, 	/* Прерывание запрещено. */
	DMA_IRQ_ENABLE = 1		/* Прерывание разрешено. */
} HAL_DMA_IRQTypeDef;

/**
 * @brief Настройки канала DMA.
 */
typedef struct __DMA_ChannelInitHandleTypeDef
{
	HAL_DMA_ChannelIndexTypeDef Channel;  		/**< Номер канала. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelIndexTypeDef. */
	HAL_DMA_ChannelPriorityTypeDef Priority; 	/**< Приоритет канала. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelPriorityTypeDef. */
	
	HAL_DMA_ChannelModeTypeDef ReadMode;		/**< Режим адресации источника. Данный параметр может иметь значение #DMA_CHANNEL_MODE_PERIPHERY или #DMA_CHANNEL_MODE_MEMORY. */
	HAL_DMA_ChannelIncTypeDef ReadInc;			/**< Наличие инкремента адреса источника. Данный параметр может иметь значение #DMA_CHANNEL_INC_DISABLE или #DMA_CHANNEL_INC_ENABLE. */
	HAL_DMA_ChannelSizeTypeDef ReadSize;   		/**< Разрядность адреса источника. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelSizeTypeDef. @warning Разрядность должна быть кратна количеству байт пересылки (LEN + 1). */
	HAL_DMA_ChannelACKTypeDef ReadAck;			/**< Разрешение работы логики с откликом для адресата источника. Данный параметр может иметь значение #DMA_CHANNEL_ACK_DISABLE или #DMA_CHANNEL_ACK_ENABLE */
	uint32_t ReadBurstSize; 					/**< Количество байт в пакете. Определяется как 2^{ReadBurstSize}. @warning Количество байт в пакете должно быть кратно ReadSize. */
	HAL_DMA_ChannelRequestTypeDef ReadRequest;	/**< Выбор периферийной линии источника при ReadMode = DMA_CHANNEL_MODE_PERIPHERY. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelRequestTypeDef. */

	HAL_DMA_ChannelModeTypeDef WriteMode;		/**< Режим адресации назначения. Данный параметр может иметь значение #DMA_CHANNEL_MODE_PERIPHERY или #DMA_CHANNEL_MODE_MEMORY. */	
	HAL_DMA_ChannelIncTypeDef WriteInc;			/**< Наличие инкремента адреса назначения. Данный параметр может иметь значение #DMA_CHANNEL_INC_DISABLE или #DMA_CHANNEL_INC_ENABLE. */
	HAL_DMA_ChannelSizeTypeDef WriteSize;		/**< Разрядность адреса назначения. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelSizeTypeDef. @warning Разрядность должна быть кратна количеству байт пересылки (LEN + 1). */
	HAL_DMA_ChannelACKTypeDef WriteAck; 		/**< Разрешение работы логики с откликом для адресата назначения. Данный параметр может иметь значение #DMA_CHANNEL_ACK_DISABLE или #DMA_CHANNEL_ACK_ENABLE */
	uint32_t WriteBurstSize;					/**< Количество байт в пакете. Определяется как 2^{WriteBurstSize}. @warning Количество байт в пакете должно быть кратно WriteSize. */
	HAL_DMA_ChannelRequestTypeDef WriteRequest; /**< Выбор периферийной линии назначения при ReadMode = DMA_CHANNEL_MODE_PERIPHERY. Данный параметр может быть одним из значений перечисления @ref HAL_DMA_ChannelRequestTypeDef. */    

} DMA_ChannelInitHandleTypeDef;

/**
 * @brief Структура для инициализации DMA.
 */
typedef struct __DMA_InitTypeDef
{
  DMA_CONFIG_TypeDef *Instance;					/**< Базовый адрес регистров DMA. Этот параметр может иметь значение #DMA_CONFIG. */
  HAL_DMA_CurrentValueTypeDef CurrentValue;		/**< Разрешить или запретить читать текущий статус канала. При значении #DMA_CURRENT_VALUE_ENABLE регистры CHx_DST, CHx_SRC, CHx_LEN возвращают текущие значения, а в CHx_CFG при считывании меняются битовые поля. При значении #DMA_CURRENT_VALUE_DISABLE регистры возвращают значения при настройке. */
} DMA_InitTypeDef;

/**
 * @brief Структура для инициализации канала DMA.
 */
typedef struct __DMA_ChannelHandleTypeDef
{
    DMA_InitTypeDef *dma;						/**< Указатель на структуру для инициализации DMA. */
    DMA_ChannelInitHandleTypeDef ChannelInit;	/**< Настройки канала DMA. */
} DMA_ChannelHandleTypeDef;


void HAL_DMA_MspInit(DMA_InitTypeDef* hdma);
void HAL_DMA_SetChannel(DMA_ChannelHandleTypeDef *hdma_channel, HAL_DMA_ChannelIndexTypeDef ChannelIndex);
void HAL_DMA_ClearLocalIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearGlobalIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearErrorIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_SetCurrentValue(DMA_InitTypeDef *hdma, HAL_DMA_CurrentValueTypeDef CurrentValue);
int HAL_DMA_GetChannelCurrentValue(DMA_InitTypeDef *hdma);
void HAL_DMA_GlobalIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission);
void HAL_DMA_ErrorIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission);
void HAL_DMA_LocalIRQEnable(DMA_ChannelHandleTypeDef* hdma_channel, HAL_DMA_IRQTypeDef Permission);
HAL_StatusTypeDef HAL_DMA_Init(DMA_InitTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Wait(DMA_ChannelHandleTypeDef* hdma_channel, uint32_t Timeout);
int HAL_DMA_GetChannelReadyStatus(DMA_ChannelHandleTypeDef* hdma_channel);
int HAL_DMA_GetChannelIrq(DMA_ChannelHandleTypeDef* hdma_channel);
int HAL_DMA_GetBusError(DMA_ChannelHandleTypeDef* hdma_channel);
void HAL_DMA_ChannelDisable(DMA_ChannelHandleTypeDef *hdma_channel);
void HAL_DMA_ChannelEnable(DMA_ChannelHandleTypeDef *hdma_channel);
void HAL_DMA_Start(DMA_ChannelHandleTypeDef *hdma_channel, void* Source, void* Destination, uint32_t Len);


#endif