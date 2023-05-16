#include "mik32_hal_spi.h"

// #define MIK32_SPI_DEBUG


void HAL_SPI_Enable(SPI_HandleTypeDef *hspi)
{
    HAL_SPI_ClearError(hspi);
    hspi->Instance->Enable = SPI_ENABLE_M;
}

void HAL_SPI_Disable(SPI_HandleTypeDef *hspi)
{
    hspi->Instance->Enable &= ~SPI_ENABLE_M;
    HAL_SPI_ClearError(hspi);
}

void HAL_SPI_IntEnable(SPI_HandleTypeDef *hspi, uint32_t int_en)
{
    hspi->Instance->IntEnable |= int_en;
}

void HAL_SPI_IntDisable(SPI_HandleTypeDef *hspi, uint32_t int_dis)
{
    hspi->Instance->IntDisable |= int_dis;
}

void HAL_SPI_SetDelayBTWN(SPI_HandleTypeDef *hspi, uint8_t btwn)
{
    hspi->Instance->Delay &= ~SPI_DELAY_BTWN_M;
    hspi->Instance->Delay |= SPI_DELAY_BTWN(btwn);
}

void HAL_SPI_SetDelayAFTER(SPI_HandleTypeDef *hspi, uint8_t after)
{
    hspi->Instance->Delay &= ~SPI_DELAY_AFTER_M;
    hspi->Instance->Delay |= SPI_DELAY_AFTER(after);
}

void HAL_SPI_SetDelayINIT(SPI_HandleTypeDef *hspi, uint8_t init)
{
    hspi->Instance->Delay &= ~SPI_DELAY_INIT_M;
    hspi->Instance->Delay |= SPI_DELAY_INIT(init);
}

void HAL_SPI_SetSlaveIdleCounter(SPI_HandleTypeDef *hspi, uint8_t slave_idle_counter)
{
    hspi->Instance->SIC = slave_idle_counter;
}

void HAL_SPI_SetThresholdTX(SPI_HandleTypeDef *hspi, uint32_t threshold)
{
    hspi->Init.ThresholdTX = threshold;
    hspi->Instance->TxThr = threshold;
}

uint32_t HAL_SPI_ReadModuleID(SPI_HandleTypeDef *hspi)
{
    return hspi->Instance->ID;
}

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    if (hspi == NULL)
    {
        return HAL_ERROR;
    }


    HAL_SPI_ClearError(hspi);

    /* Выключение модуля SPI */
    HAL_SPI_Disable(hspi);

    uint32_t SPI_config = 0;

    /* Настройка режима ведомого */
    switch (hspi->Init.SPI_Mode)
    {
    case HAL_SPI_MODE_MASTER:
        SPI_config = SPI_CONFIG_MASTER_M;
        break;
    case HAL_SPI_MODE_SLAVE:
        SPI_config = SPI_CONFIG_SLAVE_M;
        hspi->Init.ManualCS = SPI_MANUALCS_OFF;
        break;
    }
    
    /* Настройки SPI */    
    SPI_config |=  (hspi->Init.BaudRateDiv << SPI_CONFIG_BAUD_RATE_DIV_S) |     /* Настройка делителя частоты */        
                   (hspi->Init.ManualCS << SPI_CONFIG_Manual_CS_S) |            /* Настройка режима управления сигналом CS */
                   (hspi->Init.CLKPhase << SPI_CONFIG_CLK_PH_S) |               /* Настройка фазы тактового сигнала */
                   (hspi->Init.CLKPolarity << SPI_CONFIG_CLK_POL_S) |           /* Настройка полярности тактового сигнала */
                   (hspi->Init.Decoder << SPI_CONFIG_PERI_SEL_S);               /* Настройка использования внешнего декодера */
                   //(hspi->Init.DataSize << SPI_CONFIG_DATA_SZ_S);               /* Длина передаваемой посылки */

    
    /* Выбор ведомого в соответствии с режимом ManualCS */
    if(hspi->Init.ManualCS == SPI_MANUALCS_ON)
    {
        /* Ведомое устройство не выбрано. Ручное управление сигналом CS */
        SPI_config |= SPI_CS_NONE << SPI_CONFIG_CS_S;             
    }
    else
    {
        /* Выбор ведомого устройства в автоматическом режиме управления CS */
        SPI_config |= hspi->ChipSelect << SPI_CONFIG_CS_S;
    }


    /* Установка выбранных настроек */
    hspi->Instance->Config = SPI_config;

    /* уровень при котором регистр TX считается незаполненным и формируется прерывание */
    HAL_SPI_SetThresholdTX(hspi, hspi->Init.ThresholdTX);
    
    #ifdef MIK32_SPI_DEBUG
    xprintf("SPI_Init\n");
    #endif
    
    // /* Включение модуля SPI */
    // HAL_SPI_Enable(hspi);

    return error_code;

}

void HAL_SPI_ClearTXFIFO(SPI_HandleTypeDef *hspi)
{
    hspi->Instance->Enable |= SPI_ENABLE_CLEAR_TX_FIFO_M;
    #ifdef MIK32_SPI_DEBUG
    xprintf("TX_Clear\n");
    #endif
}

void HAL_SPI_ClearRXFIFO(SPI_HandleTypeDef *hspi)
{
    hspi->Instance->Enable |= SPI_ENABLE_CLEAR_RX_FIFO_M;
    #ifdef MIK32_SPI_DEBUG
    xprintf("RX_Clear\n");
    #endif
}

void HAL_SPI_ClearError(SPI_HandleTypeDef *hspi)
{
    if(hspi->Error.RXOVR || hspi->Error.ModeFail)
    {
        #ifdef MIK32_SPI_DEBUG
        xprintf("OVR = %d; FAIL = %d\n", hspi->Error.RXOVR, hspi->Error.ModeFail);
        #endif
    } 

    /* Сброс ошибок */
    hspi->Error.RXOVR = SPI_ERROR_RXOVR_OFF;
    hspi->Error.ModeFail = SPI_ERROR_ModeFail_OFF;

    HAL_SPI_ClearRXFIFO(hspi);
    HAL_SPI_ClearTXFIFO(hspi);
    
}

HAL_StatusTypeDef HAL_SPI_WaitTxNotFull(SPI_HandleTypeDef *hspi, uint32_t Timeout)
{
    uint32_t status = 0;
    while (Timeout-- != 0)
    {
        status = hspi->Instance->IntStatus;
        
        if((status & SPI_RX_OVERFLOW_M) || (status & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable))
        {
            
            if(status & SPI_RX_OVERFLOW_M)
            {
                hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
                #ifdef MIK32_SPI_DEBUG
                xprintf("TX_OVR\n");
                #endif
            }

            if ((status & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable))
            {
                hspi->Error.ModeFail = SPI_ERROR_ModeFail_ON;
                #ifdef MIK32_SPI_DEBUG
                xprintf("TX_FAIL\n");
                #endif
            }
            
            return HAL_ERROR;
        }

        if ((status & SPI_TX_FIFO_not_full_M) != 0)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
    
}

HAL_StatusTypeDef HAL_SPI_WaitRxNotEmpty(SPI_HandleTypeDef *hspi, uint32_t Timeout)
{
    uint32_t status;
    while (Timeout-- != 0)
    {
        status = hspi->Instance->IntStatus;

        if((status & SPI_RX_OVERFLOW_M) || (status & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable))
        {
            
            if(status & SPI_RX_OVERFLOW_M)
            {
                hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
                #ifdef MIK32_SPI_DEBUG
                xprintf("TX_OVR\n");
                #endif
            }

            if ((status & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable))
            {
                hspi->Error.ModeFail = SPI_ERROR_ModeFail_ON;
                #ifdef MIK32_SPI_DEBUG
                xprintf("TX_FAIL\n");
                #endif
            }
            
            return HAL_ERROR;
        }

        if ((status & SPI_RX_FIFO_not_empty_M) != 0)
        {
            return HAL_OK;
        }

    }

    return HAL_TIMEOUT;
}

void HAL_SPI_CS_Enable(SPI_HandleTypeDef *hspi, uint32_t CS_M)  
{
    CS_M = CS_M << SPI_CONFIG_CS_S;
    hspi->Instance->Config = (hspi->Instance->Config & ~SPI_CONFIG_CS_M) | CS_M;
}

void HAL_SPI_CS_Disable(SPI_HandleTypeDef *hspi)  
{
	hspi->Instance->Config = (hspi->Instance->Config & ~SPI_CONFIG_CS_M) | SPI_CONFIG_CS_NONE_M;
}

HAL_StatusTypeDef HAL_SPI_Exchange(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    
    if (hspi->Init.ThresholdTX == 0)
    {
        error_code = HAL_ERROR;
        return error_code;
    }
    
    /* Не включать SPI в ручном режиме */
    if(hspi->Init.ManualCS == SPI_MANUALCS_OFF)
    {
        HAL_SPI_Enable(hspi);
    }

    /* Запись (чтение) байтов в буфер */
    for(uint32_t i = 0; i < Size; )
    {    
        uint32_t OffsetTX = 0; /* Количество байтов, которые были записаны в буфер. Ограничивает ThresholdTX */
        uint32_t StatusTX = 0; /* Регистр состояний флагов SPI во время записи в TxData */
        uint32_t TimeoutTX = Timeout; 

        /* */
        if ((error_code = HAL_SPI_WaitTxNotFull(hspi, Timeout)) != HAL_OK)
        {
            return error_code;
        }
        /*****************************************Запись**************************************/
        /* Заполнение буфера до THRESHOLD */
        while (((StatusTX = hspi->Instance->IntStatus) & SPI_TX_FIFO_not_full_M) != 0)
        {
            /* Проверка зависания в ожидании */
            if(TimeoutTX-- == 0) 
            {
                error_code = HAL_TIMEOUT;
                return error_code;
            }

            /* Запись байта */
            hspi->Instance->TxData = TransmitBytes[i + OffsetTX];
            OffsetTX++;

            /* Проверка ошибок во время передачи и отключения */
            if((StatusTX & SPI_RX_OVERFLOW_M) || (StatusTX & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable)) 
            {
                if(StatusTX & SPI_RX_OVERFLOW_M)
                {
                    hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
                    #ifdef MIK32_SPI_DEBUG
                    xprintf("TX_OVR\n");
                    #endif
                }

                if ((StatusTX & SPI_MODE_FAIL_M) || (!hspi->Instance->Enable))
                {
                    hspi->Error.ModeFail = SPI_ERROR_ModeFail_ON;
                    #ifdef MIK32_SPI_DEBUG
                    xprintf("TX_FAIL\n");
                    #endif
                }
                
                error_code = HAL_ERROR;
                return error_code;
            }
            
            /* Если Size не кратно ThresholdTX */
            if ((i + OffsetTX) >= Size)
            {
                break;
            }
            
        }

        if (OffsetTX == 0)
        {
            error_code = HAL_ERROR;
            return error_code;
        }
        
        /* Чтение такого же количества байт сколько было записано (OffsetTX) */
        for (uint32_t OffsetRX = 0; OffsetRX < OffsetTX; OffsetRX++)
        {
            /* Ожидание когда в RX_FIFO появится хотя бы один байт */
            if ((error_code = HAL_SPI_WaitRxNotEmpty(hspi, Timeout)) != HAL_OK)
            {
                return error_code;
            }
            /* Чтение байта */
            ReceiveBytes[i + OffsetRX] = hspi->Instance->RxData;
        }

        /* Сдвиг основного цикла for на количество записанных байтов в данной итерации */
        i += OffsetTX;
         
    }
    /* Не выключать SPI в ручном режиме */
    if(hspi->Init.ManualCS == SPI_MANUALCS_OFF)
    {
        HAL_SPI_Disable(hspi);
    }

    return error_code;
    
}