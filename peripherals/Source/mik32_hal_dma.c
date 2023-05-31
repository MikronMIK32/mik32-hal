#include "mik32_hal_dma.h"

// #define MIK32_DMA_DEBUG
void HAL_DMA_SetChannel(DMA_ChannelHandleTypeDef *hdma_channel, HAL_DMA_ChannelIndexTypeDef ChannelIndex)
{
    hdma_channel->ChannelInit.Channel = ChannelIndex;
}

void HAL_DMA_ClearLocalIrq(DMA_InitTypeDef *hdma)
{
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
    hdma->CNTRWriteBuffer |= DMA_CONTROL_CLEAR_LOCAL_IRQ_M;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
}

void HAL_DMA_ClearGlobalIrq(DMA_InitTypeDef *hdma)
{
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
    hdma->CNTRWriteBuffer |= DMA_CONTROL_CLEAR_GLOBAL_IRQ_M;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
}

void HAL_DMA_ClearErrorIrq(DMA_InitTypeDef *hdma)
{
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
    hdma->CNTRWriteBuffer |= DMA_CONTROL_CLEAR_ERROR_IRQ_M;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;
    hdma->CNTRWriteBuffer &= ~(DMA_CONTROL_CLEAR_LOCAL_IRQ_M | DMA_CONTROL_CLEAR_GLOBAL_IRQ_M | DMA_CONTROL_CLEAR_ERROR_IRQ_M);
}

void HAL_DMA_ClearIrq(DMA_InitTypeDef *hdma)
{
    HAL_DMA_ClearLocalIrq(hdma);
    HAL_DMA_ClearGlobalIrq(hdma);
    HAL_DMA_ClearErrorIrq(hdma);
}

void HAL_DMA_SetCurrentValue(DMA_InitTypeDef *hdma, HAL_DMA_CurrentValueTypeDef CurrentValue)
{
    hdma->CurrentValue = CurrentValue;
    hdma->CNTRWriteBuffer &= ~DMA_CONTROL_CURRENT_VALUE_M;
    hdma->CNTRWriteBuffer |= CurrentValue <<  DMA_CONTROL_CURRENT_VALUE_S;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;

}

int HAL_DMA_GetChannelCurrentValue(DMA_InitTypeDef *hdma)
{
    /* Не реализовано в микроконтроллере */
    return (hdma->Instance->CONTROL & DMA_STATUS_CURRENT_VALUE_M) >> DMA_STATUS_CURRENT_VALUE_S;
}

void HAL_DMA_GlobalIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission)
{
    hdma->CNTRWriteBuffer &= ~DMA_CONTROL_GLOBAL_IRQ_ENA_M;
    hdma->CNTRWriteBuffer |= Permission << DMA_CONTROL_GLOBAL_IRQ_ENA_S;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;
}

void HAL_DMA_ErrorIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission)
{
    hdma->CNTRWriteBuffer &= ~DMA_CONTROL_ERROR_IRQ_ENA_M;
    hdma->CNTRWriteBuffer |= Permission << DMA_CONTROL_ERROR_IRQ_ENA_S;
    hdma->Instance->CONTROL = hdma->CNTRWriteBuffer;
}

void HAL_DMA_LocalIRQEnable(DMA_ChannelHandleTypeDef* hdma_channel, HAL_DMA_IRQTypeDef Permission)
{
    hdma_channel->CFGWriteBuffer &= ~DMA_CFG_CH_EN_IRQ_M;
    hdma_channel->CFGWriteBuffer |= Permission << DMA_CFG_CH_EN_IRQ_S;

    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CONFIG = hdma_channel->CFGWriteBuffer;
}

HAL_StatusTypeDef HAL_DMA_Init(DMA_InitTypeDef *hdma)
{
    if (hdma == NULL)
    {
        return HAL_ERROR;
    }
    
    
    hdma->CNTRWriteBuffer = 0;

    HAL_DMA_ClearIrq(hdma);
    HAL_DMA_SetCurrentValue(hdma, hdma->CurrentValue);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_DMA_Wait(DMA_ChannelHandleTypeDef* hdma_channel, uint32_t Timeout)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t mask = (1 << ChannelIndex) << DMA_STATUS_READY_S;

    while (Timeout-- != 0)
    {
        if ((hdma_channel->dma->Instance->CONTROL & mask) != 0)
        {
            return HAL_OK;
        }
        
    }

    return HAL_TIMEOUT;
    

    // while ((hdma_channel->dma->Instance->CONTROL & mask) == 0)
    // {
    //     if (Timeout-- == 0)
    //     {
    //         #ifdef MIK32_DMA_DEBUG
    //         xprintf("DMA.CONTROL: %d\n", hdma_channel->dma->Instance->CONTROL);
    //         #endif
    //         return HAL_TIMEOUT;
    //     }
        
    // }

    // return HAL_OK;

}

int HAL_DMA_GetChannelReadyStatus(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t status = ((hdma_channel->dma->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_READY_S));
    status = (status >> DMA_STATUS_READY_S ) >> ChannelIndex;
    return status;
}

int HAL_DMA_GetChannelIrq(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    uint32_t ChannelIrq = (hdma_channel->dma->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_IRQ_S);
    ChannelIrq = ( ChannelIrq >> DMA_STATUS_IRQ_S ) >> ChannelIndex;
    return ChannelIrq;
}

int HAL_DMA_GetBusError(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t BusError = ((hdma_channel->dma->Instance->CONTROL) & ((1 << ChannelIndex) << DMA_STATUS_ERROR_S));
    BusError = ( BusError >> DMA_STATUS_ERROR_S ) >> ChannelIndex;
    return BusError;
}

void HAL_DMA_ChannelDisable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    hdma_channel->CFGWriteBuffer &= ~(DMA_CFG_CH_ENABLE_M);
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CONFIG = hdma_channel->CFGWriteBuffer;
}

void HAL_DMA_ChannelEnable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    hdma_channel->CFGWriteBuffer |= DMA_CFG_CH_ENABLE_M;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CONFIG = hdma_channel->CFGWriteBuffer;
}

void HAL_DMA_Start(DMA_ChannelHandleTypeDef *hdma_channel, void* Source, void* Destination, uint32_t Len)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].SOURCE = (uint32_t) Source;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].DESTINATIONS = (uint32_t) Destination;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].LEN = Len;

     
    
    hdma_channel->CFGWriteBuffer |= DMA_CFG_CH_ENABLE_M 
        | (hdma_channel->ChannelInit.Priority << DMA_CFG_CH_PRIOR_S) 
        | (hdma_channel->ChannelInit.ReadMode << DMA_CFG_CH_READ_MODE_S) 
        | (hdma_channel->ChannelInit.ReadInc << DMA_CFG_CH_READ_INCREMENT_S) 
        | (hdma_channel->ChannelInit.ReadSize << DMA_CFG_CH_READ_SIZE_S) 
        | (hdma_channel->ChannelInit.ReadBurstSize << DMA_CFG_CH_READ_BURST_SIZE_S) 
        | (hdma_channel->ChannelInit.ReadRequest << DMA_CFG_CH_READ_REQ_S) 
        | (hdma_channel->ChannelInit.ReadAck << DMA_CFG_CH_ACK_READ_S) 
        | (hdma_channel->ChannelInit.WriteMode << DMA_CFG_CH_WRITE_MODE_S) 
        | (hdma_channel->ChannelInit.WriteInc << DMA_CFG_CH_WRITE_INCREMENT_S) 
        | (hdma_channel->ChannelInit.WriteSize << DMA_CFG_CH_WRITE_SIZE_S) 
        | (hdma_channel->ChannelInit.WriteBurstSize << DMA_CFG_CH_WRITE_BURST_SIZE_S) 
        | (hdma_channel->ChannelInit.WriteRequest << DMA_CFG_CH_WRITE_REQ_S) 
        | (hdma_channel->ChannelInit.WriteAck << DMA_CFG_CH_ACK_WRITE_S);

    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CONFIG = hdma_channel->CFGWriteBuffer;
}

