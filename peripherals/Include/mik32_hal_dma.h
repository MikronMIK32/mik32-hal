#ifndef MIK32_HAL_DMA
#define MIK32_HAL_DMA

#include "dma_config.h"
#include "mcu32_memory_map.h"


/* ReadStatus. Разрешить читать текущий статус канала */
#define DMA_CURRENT_VALUE_DISABLED    0
#define DMA_CURRENT_VALUE_ENABLED     1


/* Channel */
#define DMA_CHANNEL_0     0
#define DMA_CHANNEL_1     1
#define DMA_CHANNEL_2     2
#define DMA_CHANNEL_3     3 

/* Priority */
#define DMA_CHANNEL_PRIORITY_LOW           0b00   /* Низкий */
#define DMA_CHANNEL_PRIORITY_MEDIUM        0b01   /* Средний */
#define DMA_CHANNEL_PRIORITY_HIGH          0b10   /* Высокий */
#define DMA_CHANNEL_PRIORITY_VERY_HIGH     0b11   /* Приоритетный */

/* ReadMode/WriteMode */
#define DMA_CHANNEL_MODE_PERIPHERY      0
#define DMA_CHANNEL_MODE_MEMORY         1

/* ReadInc/WriteInc */
#define DMA_CHANNEL_INC_DISABLE         0
#define DMA_CHANNEL_INC_ENABLE          1

/* ReadSize/WriteSize. Должно быть кратно data_len */
#define DMA_CHANNEL_SIZE_BYTE           0
#define DMA_CHANNEL_SIZE_HALFWORD       1
#define DMA_CHANNEL_SIZE_WORD           2

/* ReadRequest/WriteRequest */
#define DMA_CHANNEL_USART_0_REQUEST        0
#define DMA_CHANNEL_USART_1_REQUEST        1
#define DMA_CHANNEL_CRYPTO_REQUEST         2
#define DMA_CHANNEL_SPI_0_REQUEST          3
#define DMA_CHANNEL_SPI_1_REQUEST          4
#define DMA_CHANNEL_I2C_0_REQUEST          5
#define DMA_CHANNEL_I2C_1_REQUEST          6
#define DMA_CHANNEL_SPIFI_REQUEST          7
#define DMA_CHANNEL_Timer32_1_REQUEST      8
#define DMA_CHANNEL_Timer32_2_REQUEST      9    /* #define DMA_TIMER32_1_INDEX         8*/
#define DMA_CHANNEL_DAC_0_REQUEST          10   /* #define DMA_TIMER32_2_INDEX         9*/
#define DMA_CHANNEL_DAC_1_REQUEST          11   /* #define DMA_TIMER32_0_INDEX         10 */
#define DMA_CHANNEL_Timer32_0_REQUEST      12  

/* ReadAck/WriteAck */
#define DMA_CHANNEL_ACK_DISABLE            0 
#define DMA_CHANNEL_ACK_ENABLE             1 


typedef struct
{
  uint32_t Channel;  

  uint32_t Priority; 
                     
  uint32_t ReadMode;

  uint32_t ReadInc;

  uint32_t ReadSize;      /* Должно быть кратно data_len */
  
  uint32_t ReadBurstSize; /* Должно быть кратно read_size */

  uint32_t ReadRequest;

  uint32_t ReadAck;

  uint32_t WriteMode;

  uint32_t WriteInc;

  uint32_t WriteSize;     /* Должно быть кратно data_len */

  uint32_t WriteBurstSize; /* Должно быть кратно write_size */   

  uint32_t WriteRequest;     

  uint32_t WriteAck;    

}DMA_ChannelInitHandleTypeDef;

typedef struct
{
  DMA_CONFIG_TypeDef *Instance;

  uint32_t GlobalIrq;

  uint32_t ErrorIrq;

  uint32_t CurrentValue;

}DMA_InitTypeDef;

typedef struct
{
    DMA_CONFIG_TypeDef *Instance;

    DMA_ChannelInitHandleTypeDef ChannelInit;
    
} DMA_ChannelHandleTypeDef;


void HAL_DMA_SetChannel(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelIndex);
void HAL_DMA_SetPriority(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelPriority);
void HAL_DMA_SetReadMode(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadMode);
void HAL_DMA_SetReadInc(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadInc);
void HAL_DMA_SetReadSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadSize);
void HAL_DMA_SetReadBurstSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadBurstSize);
void HAL_DMA_SetReadRequest(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadRequest);
void HAL_DMA_SetReadAck(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadAck);
void HAL_DMA_SetWriteMode(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteMode);
void HAL_DMA_SetWriteInc(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteInc);
void HAL_DMA_SetWriteSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteSize);
void HAL_DMA_SetWriteBurstSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteBurstSize);
void HAL_DMA_SetWriteRequest(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteRequest);
void HAL_DMA_SetWriteAck(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteAck);
void HAL_DMA_ChannelInit(DMA_ChannelHandleTypeDef *hdma_channel);
void HAL_DMA_SetCurrentValue(DMA_InitTypeDef *hdma, uint32_t ChannelStatus);
void HAL_DMA_ClearLocalIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearGlobalIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearErrorIrq(DMA_InitTypeDef *hdma);
void HAL_DMA_ClearIrq(DMA_InitTypeDef *hdma);
int HAL_DMA_GetChannelCurrentValue(DMA_InitTypeDef *hdma);
void HAL_DMA_Init(DMA_InitTypeDef *hdma);
void HAL_DMA_Wait(DMA_ChannelHandleTypeDef* hdma_channel);
int HAL_DMA_GetChannelReadyStatus(DMA_ChannelHandleTypeDef* hdma_channel);
int HAL_DMA_GetChannelIrq(DMA_ChannelHandleTypeDef* hdma_channel);
int HAL_DMA_GetBusError(DMA_ChannelHandleTypeDef* hdma_channel);
void HAL_DMA_ChannelDisable(DMA_ChannelHandleTypeDef *hdma_channel);
void HAL_DMA_ChannelEnable(DMA_ChannelHandleTypeDef *hdma_channel);
void HAL_DMA_Start(DMA_ChannelHandleTypeDef *hdma_channel, void* Source, void* Destination, uint32_t Len);





#endif