#include "mik32_hal_dma.h"

/** 
 * @brief Данная переменная хранит последнее записанное значение в регистр CHx_CFG. 
 * @warning Не следует изменять значение данной переменной. 
 */
static uint32_t CFGWriteBuffer[4] = {0}; 

/** 
 * @brief Данная переменная используется для хранения значения битовых полей CONFIG_STATUS[8:6]. 
 * @warning Не следует изменять значение данной переменной. 
 */
static uint32_t ConfigStatusWriteBuffer = 0;


/**
 * @brief Включение тактирования модуля OTP.
 * 
 * Эта функция может быть переопределена пользователем.
 * @param hdma Указатель на структуру для инициализации DMA.
 */
__attribute__((weak)) void HAL_DMA_MspInit(DMA_InitTypeDef* hdma)
{
    __HAL_PCC_DMA_CLK_ENABLE();
}

/**
 * @brief Задать номер канала.
 * @param hdma_channel Указатель на структуру для инициализации канала DMA.
 * @param ChannelIndex Номер канала.
 */
void HAL_DMA_SetChannel(DMA_ChannelHandleTypeDef *hdma_channel, HAL_DMA_ChannelIndexTypeDef ChannelIndex)
{
    hdma_channel->ChannelInit.Channel = ChannelIndex;
}

/**
 * @brief Очистить все флаги локального прерывания.
 * @param hdma Указатель на структуру для инициализации DMA.
 */
void HAL_DMA_ClearLocalIrq(DMA_InitTypeDef *hdma)
{
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
    ConfigStatusWriteBuffer |= DMA_CONFIG_CLEAR_LOCAL_IRQ_M;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
}

/**
 * @brief Очистить флаг глобального прерывания.
 * @param hdma Указатель на структуру для инициализации DMA.
 */
void HAL_DMA_ClearGlobalIrq(DMA_InitTypeDef *hdma)
{
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
    ConfigStatusWriteBuffer |= DMA_CONFIG_CLEAR_GLOBAL_IRQ_M;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
}

/**
 * @brief Очистить флаг прерывания ошибки.
 * @param hdma Указатель на структуру для инициализации DMA.
 */
void HAL_DMA_ClearErrorIrq(DMA_InitTypeDef *hdma)
{
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
    ConfigStatusWriteBuffer |= DMA_CONFIG_CLEAR_ERROR_IRQ_M;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
    ConfigStatusWriteBuffer &= ~(DMA_CONFIG_CLEAR_LOCAL_IRQ_M | DMA_CONFIG_CLEAR_GLOBAL_IRQ_M | DMA_CONFIG_CLEAR_ERROR_IRQ_M);
}

/**
 * @brief Очистить все флаги прерываний.
 * 
 * Очищаются флаги локальных прерываний, глобального прерывания, прерывания ошибки.
 * @param hdma Указатель на структуру для инициализации DMA.
 */
void HAL_DMA_ClearIrq(DMA_InitTypeDef *hdma)
{
    HAL_DMA_ClearLocalIrq(hdma);
    HAL_DMA_ClearGlobalIrq(hdma);
    HAL_DMA_ClearErrorIrq(hdma);
}

/**
 * @brief Задать разрешение чтения текущего статуса канала.
 * 
 * При значении #DMA_CURRENT_VALUE_ENABLE регистры CHx_DST, CHx_SRC, CHx_LEN возвращают текущие значения, 
 * а в CHx_CFG при считывании меняются битовые поля. При значении #DMA_CURRENT_VALUE_DISABLE регистры возвращают значения при настройке.
 * @param hdma Указатель на структуру для инициализации DMA.
 * @param CurrentValue Разрешение или запрет чтения текущего статуса канала.
 */
void HAL_DMA_SetCurrentValue(DMA_InitTypeDef *hdma, HAL_DMA_CurrentValueTypeDef CurrentValue)
{
    hdma->CurrentValue = CurrentValue;
    ConfigStatusWriteBuffer &= ~DMA_CONFIG_CURRENT_VALUE_M;
    ConfigStatusWriteBuffer |= CurrentValue <<  DMA_CONFIG_CURRENT_VALUE_S;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
}

/**
 * @brief Разрешить или запретить глобальное прерывание.
 * @param hdma Указатель на структуру для инициализации DMA.
 * @param Permission Разрешение (#DMA_IRQ_ENABLE) или запрет (#DMA_IRQ_DISABLE) прерывания.
 */
void HAL_DMA_GlobalIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission)
{
    ConfigStatusWriteBuffer &= ~DMA_CONFIG_GLOBAL_IRQ_ENA_M;
    ConfigStatusWriteBuffer |= Permission << DMA_CONFIG_GLOBAL_IRQ_ENA_S;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
}

/**
 * @brief Разрешить или запретить прерывание ошибки.
 * @param hdma Указатель на структуру для инициализации DMA.
 * @param Permission Разрешение (#DMA_IRQ_ENABLE) или запрет (#DMA_IRQ_DISABLE) прерывания.
 */
void HAL_DMA_ErrorIRQEnable(DMA_InitTypeDef *hdma, HAL_DMA_IRQTypeDef Permission)
{
    ConfigStatusWriteBuffer &= ~DMA_CONFIG_ERROR_IRQ_ENA_M;
    ConfigStatusWriteBuffer |= Permission << DMA_CONFIG_ERROR_IRQ_ENA_S;
    hdma->Instance->CONFIG_STATUS = ConfigStatusWriteBuffer;
}

/**
 * @brief Разрешить или запретить локальное прерывание.
 * 
 * Прерывание формируется, когда канал завершил задачу.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @param Permission Разрешение (#DMA_IRQ_ENABLE) или запрет (#DMA_IRQ_DISABLE) прерывания.
 */
void HAL_DMA_LocalIRQEnable(DMA_ChannelHandleTypeDef* hdma_channel, HAL_DMA_IRQTypeDef Permission)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    CFGWriteBuffer[ChannelIndex] &= ~DMA_CH_CFG_IRQ_EN_M;
    CFGWriteBuffer[ChannelIndex] |= Permission << DMA_CH_CFG_IRQ_EN_S;

    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CFG = CFGWriteBuffer[ChannelIndex];
}

/**
 * @brief Инициализация DMA в соответствии с настройками структуры hdma.
 * @param hdma Указатель на структуру для инициализации DMA.
 * @return Статус функции после её выполнения (HAL Status).
 */
HAL_StatusTypeDef HAL_DMA_Init(DMA_InitTypeDef *hdma)
{
    if (hdma == NULL)
    {
        return HAL_ERROR;
    }
    
    HAL_DMA_MspInit(hdma);

    ConfigStatusWriteBuffer = 0;

    HAL_DMA_ClearIrq(hdma);
    HAL_DMA_SetCurrentValue(hdma, hdma->CurrentValue);

    return HAL_OK;
}

/**
 * @brief Ожидание готовности канала.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @param Timeout Количество циклов ожидания. Стандартное значение #DMA_TIMEOUT_DEFAULT.
 * @return Статус функции после её выполнения (HAL Status).
 */
HAL_StatusTypeDef HAL_DMA_Wait(DMA_ChannelHandleTypeDef* hdma_channel, uint32_t Timeout)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t mask = (1 << ChannelIndex) << DMA_STATUS_READY_S;

    while (Timeout-- != 0)
    {
        if ((hdma_channel->dma->Instance->CONFIG_STATUS & mask) != 0)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

/**
 * @brief Получить статус готовности канала.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @return Статус готовности канала. 0 - канал занят, 1 - канал готов к работе.
 */
int HAL_DMA_GetChannelReadyStatus(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    uint32_t status = ((hdma_channel->dma->Instance->CONFIG_STATUS) & ((1 << ChannelIndex) << DMA_STATUS_READY_S));
    status = (status >> DMA_STATUS_READY_S ) >> ChannelIndex;
    return status;
}

/**
 * @brief Получить статус локального прерывания канала.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @return Статус локального прерывания канала. 0 - прерывания нет, 1 - прерывание есть.
 */
int HAL_DMA_GetChannelIrq(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    uint32_t ChannelIrq = (hdma_channel->dma->Instance->CONFIG_STATUS) & ((1 << ChannelIndex) << DMA_STATUS_CHANNEL_IRQ_S);
    ChannelIrq = ( ChannelIrq >> DMA_STATUS_CHANNEL_IRQ_S ) >> ChannelIndex;
    return ChannelIrq;
}

/**
 * @brief Получить статус состояния канала при ошибках на шине.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @return Статус состояния канала при ошибках на шине. 0 - нет ошибки, 1 - есть ошибка.
 */
int HAL_DMA_GetBusError(DMA_ChannelHandleTypeDef* hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    uint32_t BusError = ((hdma_channel->dma->Instance->CONFIG_STATUS) & ((1 << ChannelIndex) << DMA_STATUS_CHANNEL_BUS_ERROR_S));
    BusError = ( BusError >> DMA_STATUS_CHANNEL_BUS_ERROR_S ) >> ChannelIndex;
    return BusError;
}

/**
 * @brief Принудительная остановка работы канала.
 * @param hdma_channel Структура для инициализации канала DMA.
 */
void HAL_DMA_ChannelDisable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    CFGWriteBuffer[ChannelIndex] &= ~(DMA_CH_CFG_ENABLE_M);
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CFG = CFGWriteBuffer[ChannelIndex];
}

/**
 * @brief Инициализация работы канала.
 * @param hdma_channel Структура для инициализации канала DMA.
 */
void HAL_DMA_ChannelEnable(DMA_ChannelHandleTypeDef *hdma_channel)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;
    CFGWriteBuffer[ChannelIndex] |= DMA_CH_CFG_ENABLE_M;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CFG = CFGWriteBuffer[ChannelIndex];
}

/**
 * @brief Запуск работы канала с настройками из структуры hdma_channel.
 * @param hdma_channel Структура для инициализации канала DMA.
 * @param SRC Адрес источника.
 * @param DST Адрес назначения
 * @param Len Количество байт пересылки. Значение следует записывать на 1 меньше числа пересылаемых байт. 
 * Например, для отправки 8 байт значение Len = 7.
 */
void HAL_DMA_Start(DMA_ChannelHandleTypeDef *hdma_channel, void* SRC, void* DST, uint32_t Len)
{
    uint32_t ChannelIndex = hdma_channel->ChannelInit.Channel;

    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].SRC = (uint32_t) SRC;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].DST = (uint32_t) DST;
    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].LEN = Len;

    CFGWriteBuffer[ChannelIndex] &= DMA_CH_CFG_IRQ_EN_M;

    CFGWriteBuffer[ChannelIndex] |= DMA_CH_CFG_ENABLE_M 
        | (hdma_channel->ChannelInit.Priority << DMA_CH_CFG_PRIOR_S) 
        | (hdma_channel->ChannelInit.ReadMode << DMA_CH_CFG_READ_MODE_S) 
        | (hdma_channel->ChannelInit.ReadInc << DMA_CH_CFG_READ_INCREMENT_S) 
        | (hdma_channel->ChannelInit.ReadSize << DMA_CH_CFG_READ_SIZE_S) 
        | (hdma_channel->ChannelInit.ReadBurstSize << DMA_CH_CFG_READ_BURST_SIZE_S) 
        | (hdma_channel->ChannelInit.ReadRequest << DMA_CH_CFG_READ_REQUEST_S) 
        | (hdma_channel->ChannelInit.ReadAck << DMA_CH_CFG_READ_ACK_EN_S) 
        | (hdma_channel->ChannelInit.WriteMode << DMA_CH_CFG_WRITE_MODE_S) 
        | (hdma_channel->ChannelInit.WriteInc << DMA_CH_CFG_WRITE_INCREMENT_S) 
        | (hdma_channel->ChannelInit.WriteSize << DMA_CH_CFG_WRITE_SIZE_S) 
        | (hdma_channel->ChannelInit.WriteBurstSize << DMA_CH_CFG_WRITE_BURST_SIZE_S) 
        | (hdma_channel->ChannelInit.WriteRequest << DMA_CH_CFG_WRITE_REQUEST_S) 
        | (hdma_channel->ChannelInit.WriteAck << DMA_CH_CFG_WRITE_ACK_EN_S);

    hdma_channel->dma->Instance->CHANNELS[ChannelIndex].CFG = CFGWriteBuffer[ChannelIndex];
}

