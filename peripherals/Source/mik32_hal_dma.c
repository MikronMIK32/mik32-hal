#include "mik32_hal_dma.h"


void HAL_DMA_SetChannel(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelIndex)
{
    hdma_channel->ChannelInit.Channel = ChannelIndex;
}

void HAL_DMA_SetPriority(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelPriority)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~DMA_CFG_CH_PRIOR_M;
    ChannelConfig |= ChannelPriority << DMA_CFG_CH_PRIOR_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.Priority = ChannelPriority;
}


void HAL_DMA_SetReadMode(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadMode)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(1 << DMA_CFG_CH_READ_MODE_S);
    ChannelConfig |= ChannelReadMode << DMA_CFG_CH_READ_MODE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadMode = ChannelReadMode;
}

void HAL_DMA_SetReadInc(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadInc)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_READ_INCREMENT_M);
    ChannelConfig |= ChannelReadInc << DMA_CFG_CH_READ_INCREMENT_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadInc = ChannelReadInc;
}

void HAL_DMA_SetReadSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadSize)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(0b11 << DMA_CFG_CH_READ_SIZE_S);
    ChannelConfig |= ChannelReadSize << DMA_CFG_CH_READ_SIZE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadSize = ChannelReadSize;
}

void HAL_DMA_SetReadBurstSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadBurstSize)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(0b111 << DMA_CFG_CH_READ_BURST_SIZE_S);
    ChannelConfig |= ChannelReadBurstSize << DMA_CFG_CH_READ_BURST_SIZE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadBurstSize = ChannelReadBurstSize;
}

void HAL_DMA_SetReadRequest(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadRequest)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_READ_REQ_M);
    ChannelConfig |= DMA_CFG_CH_READ_REQ(ChannelReadRequest);

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadRequest = ChannelReadRequest;
}

void HAL_DMA_SetReadAck(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelReadAck)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_ACK_READ_M);
    ChannelConfig |= ChannelReadAck << DMA_CFG_CH_ACK_READ_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.ReadAck = ChannelReadAck;
}

void HAL_DMA_SetWriteMode(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteMode)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(1 << DMA_CFG_CH_WRITE_MODE_S);
    ChannelConfig |= ChannelWriteMode << DMA_CFG_CH_WRITE_MODE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteMode = ChannelWriteMode;
}

void HAL_DMA_SetWriteInc(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteInc)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_WRITE_INCREMENT_M);
    ChannelConfig |= ChannelWriteInc << DMA_CFG_CH_WRITE_INCREMENT_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteInc = ChannelWriteInc;
}

void HAL_DMA_SetWriteSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteSize)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(0b11 << DMA_CFG_CH_WRITE_SIZE_S);
    ChannelConfig |= ChannelWriteSize << DMA_CFG_CH_WRITE_SIZE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteSize = ChannelWriteSize;
}

void HAL_DMA_SetWriteBurstSize(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteBurstSize)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(0b111 << DMA_CFG_CH_WRITE_BURST_SIZE_S);
    ChannelConfig |= ChannelWriteBurstSize << DMA_CFG_CH_WRITE_BURST_SIZE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteBurstSize = ChannelWriteBurstSize;
}

void HAL_DMA_SetWriteRequest(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteRequest)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_WRITE_REQ_M);
    ChannelConfig |= DMA_CFG_CH_WRITE_REQ(ChannelWriteRequest);

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteRequest = ChannelWriteRequest;
}

void HAL_DMA_SetWriteAck(DMA_ChannelHandleTypeDef *hdma_channel, uint32_t ChannelWriteAck)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t ChannelConfig = hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG;
    ChannelConfig &= ~(DMA_CFG_CH_ACK_WRITE_M);
    ChannelConfig |= ChannelWriteAck << DMA_CFG_CH_ACK_WRITE_S;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG = ChannelConfig;
    hdma_channel->ChannelInit.WriteAck = ChannelWriteAck;
}

void HAL_DMA_ChannelInit(DMA_ChannelHandleTypeDef *hdma_channel)
{

    HAL_DMA_SetPriority(hdma_channel, hdma_channel->ChannelInit.Priority);

    HAL_DMA_SetReadMode(hdma_channel, hdma_channel->ChannelInit.ReadMode);
    HAL_DMA_SetReadInc(hdma_channel, hdma_channel->ChannelInit.ReadInc);
    HAL_DMA_SetReadSize(hdma_channel, hdma_channel->ChannelInit.ReadSize);
    HAL_DMA_SetReadBurstSize(hdma_channel, hdma_channel->ChannelInit.ReadBurstSize);
    HAL_DMA_SetReadRequest(hdma_channel, hdma_channel->ChannelInit.ReadRequest);
    HAL_DMA_SetReadAck(hdma_channel, hdma_channel->ChannelInit.ReadAck);

    HAL_DMA_SetWriteMode(hdma_channel, hdma_channel->ChannelInit.WriteMode);
    HAL_DMA_SetWriteInc(hdma_channel, hdma_channel->ChannelInit.WriteInc);
    HAL_DMA_SetWriteSize(hdma_channel, hdma_channel->ChannelInit.WriteSize);
    HAL_DMA_SetWriteBurstSize(hdma_channel, hdma_channel->ChannelInit.WriteBurstSize);
    HAL_DMA_SetWriteRequest(hdma_channel, hdma_channel->ChannelInit.WriteRequest);
    HAL_DMA_SetWriteAck(hdma_channel, hdma_channel->ChannelInit.WriteAck);

}

void HAL_DMA_SetCurrentValue(DMA_InitTypeDef *hdma, uint32_t ChannelStatus)
{
    hdma->CurrentValue = ChannelStatus;
    hdma->Instance->CONTROL = ChannelStatus <<  DMA_CONTROL_CURRENT_VALUE_S;
}

void HAL_DMA_ClearLocalIrq(DMA_InitTypeDef *hdma)
{
    hdma->Instance->CONTROL = DMA_CONTROL_CLEAR_LOCAL_IRQ_M;
}

void HAL_DMA_ClearGlobalIrq(DMA_InitTypeDef *hdma)
{
    hdma->Instance->CONTROL = DMA_CONTROL_CLEAR_GLOBAL_IRQ_M;
}

void HAL_DMA_ClearErrorIrq(DMA_InitTypeDef *hdma)
{
    hdma->Instance->CONTROL = DMA_CONTROL_CLEAR_ERROR_IRQ_M;
}

void HAL_DMA_ClearIrq(DMA_InitTypeDef *hdma)
{
    HAL_DMA_ClearLocalIrq(hdma);
    HAL_DMA_ClearGlobalIrq(hdma);
    HAL_DMA_ClearErrorIrq(hdma);
}

int HAL_DMA_GetChannelCurrentValue(DMA_InitTypeDef *hdma)
{
    return (hdma->Instance->CONTROL & DMA_STATUS_CURRENT_VALUE_M) >> DMA_STATUS_CURRENT_VALUE_S;
}


void HAL_DMA_Init(DMA_InitTypeDef *hdma)
{
    // Прерывания
    // Прерывания
    HAL_DMA_ClearIrq(hdma);
    HAL_DMA_SetCurrentValue(hdma, hdma->CurrentValue);
}

void HAL_DMA_Wait(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t Timeout = 10000000;
    // while (((hdma_channel->Instance->CONTROL) & (DMA_STATUS_READY(ChannelIndex))) == 0)
    while (((hdma_channel->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_READY_S)) == 0)
    {
        //xprintf("DMA.CONTROL: %d\n", hdma_channel->Instance->CONTROL);

        Timeout--;
        if (Timeout == 0)
        {
            #ifdef MIK32_DMA_DEBUG
            xprintf("DMA.CONTROL: %d\n", hdma_channel->Instance->CONTROL);
            #endif
            break;
        }
        
    }
}

int HAL_DMA_GetChannelReadyStatus(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    int status = ((hdma_channel->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_READY_S));
    status = status >> ChannelIndex;
    return status;
}

int HAL_DMA_GetChannelIrq(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    int ChannelIrq = ((hdma_channel->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_IRQ_S));
    ChannelIrq = ChannelIrq >> ChannelIndex;
    return ChannelIrq;
}

int HAL_DMA_GetBusError(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    int BusError = ((hdma_channel->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_ERROR_S));
    BusError = BusError >> ChannelIndex;
    return BusError;
}

void HAL_DMA_ChannelDisable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG &= ~(DMA_CFG_CH_ENABLE_M);
}

void HAL_DMA_ChannelEnable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    hdma_channel->Instance->CHANNELS[ChannelIndex].CONFIG |= DMA_CFG_CH_ENABLE_M;
}

void HAL_DMA_Start(DMA_ChannelHandleTypeDef *hdma_channel, void* Source, void* Destination, uint32_t Len)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    hdma_channel->Instance->CHANNELS[ChannelIndex].SOURCE = (uint32_t) Source;
    hdma_channel->Instance->CHANNELS[ChannelIndex].DESTINATIONS = (uint32_t) Destination;
    hdma_channel->Instance->CHANNELS[ChannelIndex].LEN = Len;

    /* Запуск тактирования канала */
    HAL_DMA_ChannelEnable(hdma_channel);


}

