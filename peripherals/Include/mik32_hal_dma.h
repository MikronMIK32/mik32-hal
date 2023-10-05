#ifndef MIK32_HAL_DMA
#define MIK32_HAL_DMA

#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "stddef.h"
#include "dma_config.h"
#include "mcu32_memory_map.h"
#include "mik32_hal_def.h"



#define DMA_TIMEOUT_DEFAULT 1000000


/* ReadStatus. Разрешить читать текущий статус канала */
typedef enum
{
	DMA_CURRENT_VALUE_ENABLE = 0,   /* Текущие значения */
	DMA_CURRENT_VALUE_DISABLE = 1   /* Значения при настройке */
} HAL_DMA_CurrentValueTypeDef;



/* Номер канала */
typedef enum
{
	DMA_CHANNEL_0 = 0,
	DMA_CHANNEL_1 = 1,
	DMA_CHANNEL_2 = 2,
	DMA_CHANNEL_3 = 3
} HAL_DMA_ChannelIndexTypeDef;


/* Приоритет канала */
typedef enum
{
	DMA_CHANNEL_PRIORITY_LOW = 0b00,   		/* Низкий */
	DMA_CHANNEL_PRIORITY_MEDIUM  = 0b01,   	/* Средний */
	DMA_CHANNEL_PRIORITY_HIGH = 0b10,   	/* Высокий */
	DMA_CHANNEL_PRIORITY_VERY_HIGH = 0b11   /* Приоритетный */
} HAL_DMA_ChannelPriorityTypeDef;


/* Режим адресации источника или назначения */
typedef enum
{
	DMA_CHANNEL_MODE_PERIPHERY = 0,		/* Периферия */
	DMA_CHANNEL_MODE_MEMORY = 1			/* Память */
} HAL_DMA_ChannelModeTypeDef;


/* Инкремент адреса источника или назначения */
typedef enum
{
	DMA_CHANNEL_INC_DISABLE = 0,	/* Нет инкремента */
	DMA_CHANNEL_INC_ENABLE = 1		/* Есть инкремент */
} HAL_DMA_ChannelIncTypeDef;

/* Разрядность адреса источника или назначения. Должно быть кратно data_len */
typedef enum
{
	DMA_CHANNEL_SIZE_BYTE = 0,			/* Байт */
	DMA_CHANNEL_SIZE_HALFWORD = 1,		/* Полуслово */
	DMA_CHANNEL_SIZE_WORD = 2			/* Слово */
} HAL_DMA_ChannelSizeTypeDef;

/* ReadRequest/WriteRequest */

/* Для макетного образца V1 */
// #define DMA_CHANNEL_USART_0_REQUEST        0
// #define DMA_CHANNEL_USART_1_REQUEST        1
// #define DMA_CHANNEL_CRYPTO_REQUEST         2
// #define DMA_CHANNEL_SPI_0_REQUEST          3
// #define DMA_CHANNEL_SPI_1_REQUEST          4
// #define DMA_CHANNEL_I2C_0_REQUEST          5
// #define DMA_CHANNEL_I2C_1_REQUEST          6
// #define DMA_CHANNEL_SPIFI_REQUEST          7
// #define DMA_CHANNEL_Timer32_1_REQUEST      8
// #define DMA_CHANNEL_Timer32_2_REQUEST      9  
// #define DMA_CHANNEL_DAC_0_REQUEST          10 
// #define DMA_CHANNEL_DAC_1_REQUEST          11 
// #define DMA_CHANNEL_Timer32_0_REQUEST      12 

/* Линии запросов от периферии. Для макетного образца V0 */
typedef enum
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
	DMA_CHANNEL_TIMER32_0_REQUEST = 10
} HAL_DMA_ChannelRequestTypeDef;


/* Работа логики с откликом для адресата источника или назначения */
typedef enum
{
	DMA_CHANNEL_ACK_DISABLE = 0,	/* Запрещено */
	DMA_CHANNEL_ACK_ENABLE = 1 		/* Разрешено */
} HAL_DMA_ChannelACKTypeDef;


/* Разрешение прерываний */
typedef enum
{
	DMA_IRQ_DISABLE = 0, 	/* Запрещено */
	DMA_IRQ_ENABLE = 1		/* Разрешено */
} HAL_DMA_IRQTypeDef;


typedef struct
{
	HAL_DMA_ChannelIndexTypeDef Channel;  

	HAL_DMA_ChannelPriorityTypeDef Priority; 

						
	HAL_DMA_ChannelModeTypeDef ReadMode;

	HAL_DMA_ChannelIncTypeDef ReadInc;

	HAL_DMA_ChannelSizeTypeDef ReadSize;      /* Должно быть кратно data_len */

	HAL_DMA_ChannelACKTypeDef ReadAck;

	uint32_t ReadBurstSize; /* Должно быть кратно read_size */

	uint32_t ReadRequest;


	HAL_DMA_ChannelModeTypeDef WriteMode;

	HAL_DMA_ChannelIncTypeDef WriteInc;

	HAL_DMA_ChannelSizeTypeDef WriteSize;     /* Должно быть кратно data_len */

	HAL_DMA_ChannelACKTypeDef WriteAck; 

	uint32_t WriteBurstSize; /* Должно быть кратно write_size */   

	uint32_t WriteRequest;     

}DMA_ChannelInitHandleTypeDef;

typedef struct
{
  DMA_CONFIG_TypeDef *Instance;

  HAL_DMA_CurrentValueTypeDef CurrentValue;

  uint32_t CNTRWriteBuffer;

}DMA_InitTypeDef;

typedef struct
{
    DMA_InitTypeDef *dma;

    DMA_ChannelInitHandleTypeDef ChannelInit;

    uint32_t CFGWriteBuffer;
    
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