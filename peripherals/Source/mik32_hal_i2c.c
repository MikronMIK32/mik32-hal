#include "mik32_hal_i2c.h"

__attribute__((weak)) void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (hi2c->Instance == I2C_0)
    {
        __HAL_PCC_I2C_0_CLK_ENABLE();
        
        GPIO_InitStruct.Pin = PORT0_9 | PORT0_10;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
    }

    if (hi2c->Instance == I2C_1)
    {
        __HAL_PCC_I2C_1_CLK_ENABLE();

        GPIO_InitStruct.Pin = PORT1_12 | PORT1_13;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
    }

}

void HAL_I2C_Disable(I2C_HandleTypeDef *hi2c)
{
    hi2c->Instance->CR1 &= ~I2C_CR1_PE_M;
}

void HAL_I2C_Reset(I2C_HandleTypeDef *hi2c)
{
    hi2c->ErrorCode = I2C_ERROR_NONE;
    
    hi2c->Instance->CR1 &= ~I2C_CR1_PE_M;
    hi2c->Instance->CR1 |= I2C_CR1_PE_M;
}

void HAL_I2C_Enable(I2C_HandleTypeDef *hi2c)
{
    hi2c->Instance->CR1 |= I2C_CR1_PE_M;
}

void HAL_I2C_AnalogFilterInit(I2C_HandleTypeDef *hi2c, HAL_I2C_AnalogFilterTypeDef AnalogFilter)
{
    hi2c->Init.AnalogFilter = AnalogFilter;
    hi2c->Instance->CR1 &= ~I2C_CR1_ANFOFF_M;
    hi2c->Instance->CR1 |= AnalogFilter << I2C_CR1_ANFOFF_S;
}

void HAL_I2C_DigitalFilterInit(I2C_HandleTypeDef *hi2c, HAL_I2C_DigitalFilterTypeDef DigitalFilter)
{
    /* DNF можно менять только при PE = 0 */
    hi2c->Instance->CR1 &= ~I2C_CR1_PE_M;

    hi2c->Init.DigitalFilter = DigitalFilter;
    hi2c->Instance->CR1 &= ~I2C_CR1_DNF_M;
    hi2c->Instance->CR1 |= I2C_CR1_DNF(DigitalFilter);
}

void HAL_I2C_SetClockSpeed(I2C_HandleTypeDef *hi2c)
{
    /* TIMING можно менять только при PE = 0 */
    hi2c->Instance->CR1 &= ~I2C_CR1_PE_M;
    
    /*
    * Инициализация i2c
    * TIMING - регистр таймингов
    * 
    * SCLDEL(0-15) - Задержка между изменением SDA и фронтом SCL в режиме ведущего и ведомого при NOSTRETCH = 0
    * 
    * SDADEL(0-15) - Задержка между спадом SCL и изменением SDA в режиме ведущего и ведомого при NOSTRETCH = 0
    * 
    * SCLL(0-255) - Время удержания SCL в состоянии логического «0» в режиме ведущего
    * 
    * SCLH(0-255) - Время удержания SCL в состоянии логической «1» в режиме ведущего
    * 
    * PRESC(0-15) - Делитель частоты I2CCLK. Используется для вычисления периода сигнала TPRESC для счетчиков предустановки, 
    * удержания, уровня «0» и «1»
    * 
    */
    hi2c->Instance->TIMINGR = I2C_TIMINGR_SCLDEL(hi2c->Clock.SCLDEL) | I2C_TIMINGR_SDADEL(hi2c->Clock.SDADEL) |
        I2C_TIMINGR_SCLL(hi2c->Clock.SCLL) | I2C_TIMINGR_SCLH(hi2c->Clock.SCLH) | I2C_TIMINGR_PRESC(hi2c->Clock.PRESC);
}

void HAL_I2C_NoStretchMode(I2C_HandleTypeDef *hi2c, HAL_I2C_NoStretchModeTypeDef NoStretchMode)
{
    /* NOSTRETCH можно менять только при PE = 0 */
    hi2c->Instance->CR1 &= ~I2C_CR1_PE_M;
    
    hi2c->Init.NoStretchMode = NoStretchMode;
    hi2c->Instance->CR1 &= ~I2C_CR1_NOSTRETCH_M;
    hi2c->Instance->CR1 |= NoStretchMode << I2C_CR1_NOSTRETCH_S;
}

void HAL_I2C_OwnAddress1(I2C_HandleTypeDef *hi2c)
{
    /* OA1, OA1MODE можно менять при OA1EN = 0 */
    hi2c->Instance->OAR1 = 0;

    /* Запись собственного адреса 1 */
    if (hi2c->Init.OwnAddress1 <= I2C_ADDRESS_7BIT_MAX) /* Настройка 7 битного адреса */
    {
        hi2c->Instance->OAR1 &= ~I2C_OAR1_OA1MODE_M; /* 7 битный режим */
        hi2c->Instance->OAR1 |= ((hi2c->Init.OwnAddress1 & 0x7F) << 1) << I2C_OAR1_OA1_S;
    }
    else /* Настройка 10 битного адреса */
    {
        hi2c->Instance->OAR1 |= I2C_OAR1_OA1MODE_M; /* 10 битный режим */
        hi2c->Instance->OAR1 |= (hi2c->Init.OwnAddress1 & 0x3FF) << I2C_OAR1_OA1_S;
    }

    /* Запись собственного адреса 1 */
    if (hi2c->Init.OwnAddress1 <= I2C_ADDRESS_7BIT_MAX) /* 7 битный адрес */
    {
        hi2c->Instance->OAR1 |= ((hi2c->Init.OwnAddress1 & 0x7F) << 1) << I2C_OAR1_OA1_S;
    }
    else /* 10 битный адрес */
    {
        hi2c->Instance->OAR1 |= (hi2c->Init.OwnAddress1 & 0x3FF) << I2C_OAR1_OA1_S;
    }
    
    

    
    // switch (hi2c->Init.AddressingMode)
    // {
    // case I2C_ADDRESSINGMODE_7BIT: 
    //     hi2c->Instance->OAR1 |= ((hi2c->Init.OwnAddress1 & 0x7F) << 1) << I2C_OAR1_OA1_S;
    //     break;
    // case I2C_ADDRESSINGMODE_10BIT: 
    //     hi2c->Instance->OAR1 |= (hi2c->Init.OwnAddress1 & 0x3FF) << I2C_OAR1_OA1_S;
    //     break;
    
    // default:
    //     break;
    // }

    hi2c->Instance->OAR1 |= I2C_OAR1_OA1EN_M;
    
}

void HAL_I2C_OwnAddress2(I2C_HandleTypeDef *hi2c)
{
    /* OA2, OA2MODE, OA2MSK можно менять при OA2EN = 0 */
    hi2c->Instance->OAR2 = 0;

    hi2c->Instance->OAR2 |= hi2c->Init.OwnAddress2Mask << I2C_OAR2_OA2MSK_S;

    /* Запись собственного адреса 2 */
    hi2c->Instance->OAR2 |= (hi2c->Init.OwnAddress2 & 0x7F) << I2C_OAR2_OA2_S;

    /* Включить дополнительный собственный адрес 2 */
    if (hi2c->Init.DualAddressMode == I2C_DUALADDRESS_ENABLE)
    {
        hi2c->Instance->OAR2 |= I2C_OAR2_OA2EN_M;
    }
    
}

void HAL_I2C_GeneralCall(I2C_HandleTypeDef *hi2c, HAL_I2C_GeneralCallTypeDef GeneralCall)
{
    hi2c->Init.GeneralCall = GeneralCall;
    hi2c->Instance->CR1 &= ~I2C_CR1_GCEN_M;
    hi2c->Instance->CR1 |= GeneralCall << I2C_CR1_GCEN_S;
}

void HAL_I2C_SBCMode(I2C_HandleTypeDef *hi2c, HAL_I2C_SBCModeTypeDef SBCMode)
{
    hi2c->Init.SBCMode = SBCMode;
    hi2c->Instance->CR1 &= ~I2C_CR1_SBC_M;
    hi2c->Instance->CR1 |= SBCMode << I2C_CR1_SBC_S;
}

void HAL_I2C_SlaveInit(I2C_HandleTypeDef *hi2c)
{
    /* Основной и дополнительный собственный адрес */
    HAL_I2C_OwnAddress1(hi2c);
    HAL_I2C_OwnAddress2(hi2c);

    /* GCEN - general call  */
    HAL_I2C_GeneralCall(hi2c, hi2c->Init.GeneralCall);
    
    /* Configure SBC in I2C_CR1* */
    HAL_I2C_SBCMode(hi2c, hi2c->Init.SBCMode);

    /* Enable interrupts and/or DMA in I2C_CR1 */
}

void HAL_I2C_MasterInit(I2C_HandleTypeDef *hi2c)
{
    /* Enable interrupts and/or DMA in I2C_CR1 */

}

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_MspInit(hi2c);
    
    /* Clear PE in I2C_CR1 */
    HAL_I2C_Disable(hi2c);

    /* Configure ANFOFF and DNF[3:0] in I2C_CR1 */
    HAL_I2C_AnalogFilterInit(hi2c, hi2c->Init.AnalogFilter);
    HAL_I2C_DigitalFilterInit(hi2c, hi2c->Init.DigitalFilter);

    /* Configure PRESC[3:0], SDADEL[3:0], SCLDEL[3:0], SCLH[7:0]，SCLL[7:0] in l2C_TIMINGR */
    HAL_I2C_SetClockSpeed(hi2c);

    if (hi2c->Init.Mode == HAL_I2C_MODE_MASTER)
    {
        /* Configure NOSTRETCH in I2C_CR1 */
        HAL_I2C_NoStretchMode(hi2c, I2C_NOSTRETCH_DISABLE);
    }
    else
    {
        /* Configure NOSTRETCH in I2C_CR1 */
        HAL_I2C_NoStretchMode(hi2c, hi2c->Init.NoStretchMode);
    }
    

    /* Set PE in I2C_CR1 */
    HAL_I2C_Enable(hi2c);

    switch (hi2c->Init.Mode)
    {
    case HAL_I2C_MODE_SLAVE:
        HAL_I2C_SlaveInit(hi2c);
        break;

    case HAL_I2C_MODE_MASTER:
        HAL_I2C_MasterInit(hi2c);
        break;
    
    default:
        return HAL_ERROR;
        break;
    }
    
    hi2c->State = HAL_I2C_STATE_READY;
    return HAL_OK;
}

void HAL_I2C_AutoEnd(I2C_HandleTypeDef *hi2c, HAL_I2C_AutoEndModeTypeDef AutoEnd)
{
    hi2c->Instance->CR2 &= ~I2C_CR2_AUTOEND_M;
    hi2c->Instance->CR2 |= AutoEnd << I2C_CR2_AUTOEND_S;
}

HAL_StatusTypeDef HAL_I2C_Master_WaitTC(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    /* Ожидание совпадения адреса */
    while (Timeout--)
    {
        if (hi2c->Instance->ISR & I2C_ISR_TC_M)
        {
            return HAL_OK;
        }
    }

    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Master_WaitTCR(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    /* Ожидание совпадения адреса */
    while (Timeout--)
    {
        if (hi2c->Instance->ISR & I2C_ISR_TCR_M)
        {
            return HAL_OK;
        }
    }

    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Master_WaitTXIS(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t status_isr = 0;
    
    /* Ожидание совпадения адреса */
    while (Timeout--)
    {
        status_isr = hi2c->Instance->ISR;
        
        if (status_isr & I2C_ISR_NACKF_M)
        {
            hi2c->ErrorCode = I2C_ERROR_NACK;
            return HAL_ERROR;
        }

        if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M))
        {
            if (status_isr & I2C_ISR_BERR_M)
            {
                hi2c->ErrorCode = I2C_ERROR_BERR;
                return HAL_ERROR;
            }

            if (status_isr & I2C_ISR_ARLO_M)
            {
                hi2c->ErrorCode = I2C_ERROR_ARLO;
                return HAL_ERROR;
            }
        }

        
        if (status_isr & I2C_ISR_TXIS_M)
        {
            return HAL_OK;
        }
    }

    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Master_WaitRXNE(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t status_isr = 0;
    /* Ожидание совпадения адреса */
    while (Timeout--)
    {
        status_isr = hi2c->Instance->ISR;

        if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M))
        {
            if (status_isr & I2C_ISR_BERR_M)
            {
                hi2c->ErrorCode = I2C_ERROR_BERR;
                return HAL_ERROR;
            }

            if (status_isr & I2C_ISR_ARLO_M)
            {
                hi2c->ErrorCode = I2C_ERROR_ARLO;
                return HAL_ERROR;
            }
        }

        if (status_isr & I2C_ISR_RXNE_M)
        {
            return HAL_OK;
        }
    }

    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

void HAL_I2C_Master_SlaveAddress(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress)
{
    hi2c->Instance->CR2 &= ~I2C_CR2_SADD_M;

    if (SlaveAddress <= I2C_ADDRESS_7BIT_MAX) /* 7 битный адрес */
    {
        hi2c->Instance->CR2 &= ~I2C_CR2_ADD10_M;
        hi2c->Instance->CR2 |= ((SlaveAddress & 0x7F) << 1) << I2C_CR2_SADD_S;
    }
    else /* 10 битный адрес */
    {
        hi2c->Instance->CR2 |= I2C_CR2_ADD10_M;
        hi2c->Instance->CR2 &= ~I2C_CR2_HEAD10R_M; /* ведущий отправляет полную последовательность для чтения для 10 битного адреса */
        hi2c->Instance->CR2 |= (SlaveAddress & 0x3FF) << I2C_CR2_SADD_S;
    }
    
}

HAL_StatusTypeDef HAL_I2C_WaitBusy(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    /* Ожидание совпадения адреса */
    while (hi2c->Instance->ISR & I2C_ISR_BUSY_M)
    {
        if (Timeout-- == 0)
        {
            hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

void HAL_I2C_Master_NBYTES(I2C_HandleTypeDef *hi2c)
{
    hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
    /* Подготовка перед отправкой */
    
    if ((hi2c->TransferSize - hi2c->TransferCount) <= I2C_NBYTE_MAX)
    {
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(hi2c->TransferSize - hi2c->TransferCount);
        hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
        HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
    }
    else /* DataSize > 255 */
    {
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(I2C_NBYTE_MAX);
        /* При RELOAD = 1 AUTOEND игнорируется */
        hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
    }
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    uint32_t nbytes = 0;

    /* Подготовка перед отправкой */
    if (DataSize <= I2C_NBYTE_MAX)
    {
        nbytes = DataSize;
        DataSize = 0;
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);

        hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
        HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
    }
    else /* DataSize > 255 */
    {
        nbytes = I2C_NBYTE_MAX;
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);

        DataSize-=I2C_NBYTE_MAX;

        /* При RELOAD = 1 AUTOEND игнорируется */

        hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
    }
    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - запись */
    hi2c->Instance->CR2 &= ~I2C_CR2_RD_WRN_M;
    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;

    while (1)
    {

        for (uint32_t i = 0; i < nbytes; i++)
        {
            if ((error_code = HAL_I2C_Master_WaitTXIS(hi2c, Timeout)) != HAL_OK)
            {
                return error_code;
            }
            hi2c->Instance->TXDR = *pData;
            pData++;
        }

        if (DataSize == 0)
        {
            /* Все данные отправлены */
            if (hi2c->Instance->CR2 & I2C_CR2_AUTOEND_M)
            {
                /* BUSY */
                error_code =  HAL_I2C_WaitBusy(hi2c, Timeout); /* Ожидание сигнала STOP */
            }
            else
            {
                /* TC */
                error_code = HAL_I2C_Master_WaitTC(hi2c, Timeout); /* Ожидание сигнала конца передачи. STOP не отправляется */
            }
            
            break;
        }

        if ((error_code = HAL_I2C_Master_WaitTCR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }

        if (DataSize <= I2C_NBYTE_MAX)
        {
            nbytes = DataSize;
            hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
            hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);
            DataSize = 0;
            hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
            HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
        }
        else
        {
            nbytes = I2C_NBYTE_MAX;
            hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
            hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);
            DataSize -= I2C_NBYTE_MAX;
            hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
        }
        
        
    }
    
    
    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    uint32_t nbytes = 0;

    /* Подготовка перед отправкой */
    if (DataSize <= I2C_NBYTE_MAX)
    {
        nbytes = DataSize;
        DataSize = 0;
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);

        hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
        HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
    }
    else /* DataSize > 255 */
    {
        nbytes = I2C_NBYTE_MAX;
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);

        DataSize-=I2C_NBYTE_MAX;

        /* При RELOAD = 1 AUTOEND игнорируется */

        hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
    }
    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - чтение */
    hi2c->Instance->CR2 |= I2C_CR2_RD_WRN_M;
    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;


    while(1)
    {
        for (uint32_t i = 0; i < nbytes; i++)
        {
            if ((error_code = HAL_I2C_Master_WaitRXNE(hi2c, Timeout)) != HAL_OK)
            {
                return error_code;
            }

            *pData = hi2c->Instance->RXDR;
            pData++;
        }

        if (DataSize == 0)
        {
            /* Все данные прочитаны */
            if (hi2c->Instance->CR2 & I2C_CR2_AUTOEND_M)
            {
                /* BUSY */
                error_code =  HAL_I2C_WaitBusy(hi2c, Timeout); /* Ожидание сигнала STOP */
            }
            else
            {
                /* TC */
                error_code = HAL_I2C_Master_WaitTC(hi2c, Timeout); /* Ожидание сигнала конца передачи. STOP не отправляется */
            }

            break;
        }

        if ((error_code = HAL_I2C_Master_WaitTCR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }

        if (hi2c->Instance->ISR & I2C_ISR_TCR_M)
        {
            if (DataSize <= I2C_NBYTE_MAX)
            {
                nbytes = DataSize;
                hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
                hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);
                DataSize = 0;
                hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
                HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
            }
            else
            {
                nbytes = I2C_NBYTE_MAX;
                hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
                hi2c->Instance->CR2 |= I2C_CR2_NBYTES(nbytes);
                DataSize -= I2C_NBYTE_MAX;
                hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
            }
        }
        
        
    }
    
    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_WaitADDR(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    /* Ожидание совпадения адреса */
    while (!(hi2c->Instance->ISR & I2C_ISR_ADDR_M))
    {
        if (Timeout-- == 0)
        {
            hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Slave_WaitTXIS(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t status_isr = 0;
    /* Ожидание флага TXIS */

    if (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)
    {
        while (Timeout--)
        {
            status_isr = hi2c->Instance->ISR;

            if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M | I2C_ISR_OVR_M))
            {
                if (status_isr & I2C_ISR_BERR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_BERR;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_ARLO_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_ARLO;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_OVR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_OVR;
                    return HAL_ERROR;
                }
            }

            if (status_isr & I2C_ISR_STOPF_M)
            {
                hi2c->ErrorCode = I2C_ERROR_STOP;
                return HAL_ERROR;
            }
            
            if (status_isr & I2C_ISR_TXIS_M)
            {
                return HAL_OK;
            }
        }
    }
    else
    {
        while (Timeout--)
        {
            status_isr = hi2c->Instance->ISR;
            
            if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M))
            {
                if (status_isr & I2C_ISR_BERR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_BERR;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_ARLO_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_ARLO;
                    return HAL_ERROR;
                }
            }
            
            if (hi2c->Instance->ISR & I2C_ISR_TXIS_M)
            {
                return HAL_OK;
            }
        }
    }

    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Slave_WaitRXNE(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t status_isr = 0;
    /* Ожидание RXNE */
    if (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)
    {
        while (Timeout--)
        {
            status_isr = hi2c->Instance->ISR;

            if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M | I2C_ISR_OVR_M))
            {
                if (status_isr & I2C_ISR_BERR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_BERR;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_ARLO_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_ARLO;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_OVR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_OVR;
                    return HAL_ERROR;
                }
            }

            if (status_isr & I2C_ISR_STOPF_M)
            {
                hi2c->ErrorCode = I2C_ERROR_STOP;
                return HAL_ERROR;
            }
            
            
            if (status_isr & I2C_ISR_RXNE_M)
            {
                return HAL_OK;
            }
            
        }

    }
    else
    {
        while (Timeout--)
        {
            status_isr = hi2c->Instance->ISR;

            if (status_isr & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M))
            {
                if (status_isr & I2C_ISR_BERR_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_BERR;
                    return HAL_ERROR;
                }

                if (status_isr & I2C_ISR_ARLO_M)
                {
                    hi2c->ErrorCode = I2C_ERROR_ARLO;
                    return HAL_ERROR;
                }
            }

            if (status_isr & I2C_ISR_RXNE_M)
            {
                return HAL_OK;
            }
            
        }
    }



    hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
    
    if (!(hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* NOSTRETCH = 0 */
    {
        /* Ожидание вызова адреса */
        if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }
        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;

        if(hi2c->Instance->OAR1 & I2C_OAR1_OA1MODE_M)
        {
            /* Ожидание вызова адреса */
            if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
            {
                return error_code;
            }
        }

        /* Сброс содержимого TXDR */
        hi2c->Instance->ISR |= I2C_ISR_TXE_M;

        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;

    }

    /* Первая запись делается заранее */
    hi2c->Instance->TXDR = pData[0];
    /* Запись байта */
    for (uint32_t tx_count = 1; tx_count < DataSize; tx_count++)
    {
        if ((error_code = HAL_I2C_Slave_WaitTXIS(hi2c, Timeout)) != HAL_OK)
        {
            /* Сброс содержимого TXDR */
            hi2c->Instance->ISR |= I2C_ISR_TXE_M;
            // hi2c->Instance->ICR |= I2C_ICR_TXIS_M;

            /* Сброс флага детектирования STOP на шине */
            hi2c->Instance->ICR |= I2C_ICR_STOPCF_M;
            return error_code;
        }

        hi2c->Instance->TXDR = pData[tx_count];
    }

    if ((error_code = HAL_I2C_WaitBusy(hi2c, Timeout)) != HAL_OK)
    {
        return error_code;
    }
    
    
    /* Сброс содержимого TXDR */
    hi2c->Instance->ISR |= I2C_ISR_TXE_M;
    // hi2c->Instance->ICR |= I2C_ICR_TXIS_M;

    /* Сброс флага детектирования STOP на шине */
    hi2c->Instance->ICR |= I2C_ICR_STOPCF_M;
    

    return error_code;
    
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if (!(hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* NOSTRETCH = 0 - с растягиванием SCL*/
    {
        /* Ожидание вызова адреса */
        if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }

        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;
    }
    else if (hi2c->Instance->CR1 & I2C_CR1_SBC_M) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }
    

    /* Чтение байта */
    for (uint32_t rx_count = 0; rx_count < DataSize; rx_count++)
    {
        if ((error_code = HAL_I2C_Slave_WaitRXNE(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }
        pData[rx_count] = hi2c->Instance->RXDR;
    } 

    if ((error_code = HAL_I2C_WaitBusy(hi2c, Timeout)) != HAL_OK)
    {
        return error_code;
    }
    
    /* Сброс флага детектирования STOP на шине */
    hi2c->Instance->ICR |= I2C_ICR_STOPCF_M;

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_WaitTCR(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    /* Ожидание совпадения адреса */
    while (Timeout--)
    {
        if (hi2c->Instance->ISR & I2C_ISR_TCR_M)
        {
            return HAL_OK;
        }
    }

    return HAL_TIMEOUT;
}

void HAL_I2C_Slave_ACK(I2C_HandleTypeDef *hi2c)
{
    /* Формирование ACK */
    hi2c->Instance->CR2 &= ~I2C_CR2_NACK_M; 
}

void HAL_I2C_Slave_NACK(I2C_HandleTypeDef *hi2c)
{
    /* Формирование NACK */
    hi2c->Instance->CR2 |= I2C_CR2_NACK_M; 
}

__attribute__ ((weak)) HAL_StatusTypeDef HAL_I2C_Slave_SBC(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t ByteCount)
{
    hi2c->Instance->CR2 &= ~I2C_CR2_NACK_M; /* Формирование ACK */
    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Slave_ReceiveSBC(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    if (!(hi2c->Instance->CR1 & I2C_CR1_SBC_M))
    {
        return HAL_ERROR;
    }
    else if (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)
    {
        /* Режим SBC несовместим с режимом NOSTRETCH */
        return HAL_ERROR;
    }


    /* Ожидание вызова адреса */
    if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
    {
        return error_code;
    }
    hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;
    hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
    hi2c->Instance->CR2 |= I2C_CR2_NBYTES(0x1);
    /* Сброс флага ADDR */
    hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;

    /* Чтение байта */
    for (uint32_t rx_count = 0; rx_count < DataSize; rx_count++)
    {
        /* Ожидание приема байта */
        if ((error_code = HAL_I2C_Slave_WaitTCR(hi2c, Timeout)) != HAL_OK) 
        {
            return error_code;
        }

        pData[rx_count] = hi2c->Instance->RXDR;

        if ((error_code = HAL_I2C_Slave_SBC(hi2c, pData, DataSize, rx_count)) != HAL_OK)
        {
            return error_code;
        }

        if (rx_count <= DataSize-1)
        {
            hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
            hi2c->Instance->CR2 |= I2C_CR2_NBYTES(0x1);
        }
    }


    if ((error_code = HAL_I2C_WaitBusy(hi2c, Timeout)) != HAL_OK)
    {
        return error_code;
    }
    
    /* Сброс флага детектирования STOP на шине */
    hi2c->Instance->ICR |= I2C_ICR_STOPCF_M;

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    /* Подготовка перед отправкой */
    if (DataSize <= I2C_NBYTE_MAX)
    {
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(DataSize);

        HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
    }
    else /* DataSize > 255 */
    {
        return HAL_ERROR;
    }
    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - запись */
    hi2c->Instance->CR2 &= ~I2C_CR2_RD_WRN_M;

    /* Разрешение поддержки DMA при передаче */
    hi2c->Instance->CR1 |= I2C_CR1_TXDMAEN_M;

    /* Настройка и включение канала DMA */
    HAL_DMA_Start(hi2c->hdmatx, pData, (void*)&hi2c->Instance->TXDR, DataSize - 1);

    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    /* Подготовка перед приемом */
    if (DataSize <= I2C_NBYTE_MAX)
    {
        hi2c->Instance->CR2 &= ~I2C_CR2_NBYTES_M;
        hi2c->Instance->CR2 |= I2C_CR2_NBYTES(DataSize);

        HAL_I2C_AutoEnd(hi2c, hi2c->Init.AutoEnd);
    }
    else /* DataSize > 255 */
    {
        return HAL_ERROR;
    }
    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - чтение */
    hi2c->Instance->CR2 |= I2C_CR2_RD_WRN_M;

    /* Разрешение поддержки DMA при передаче */
    hi2c->Instance->CR1 |= I2C_CR1_RXDMAEN_M;

    /* Настройка и включение канала DMA */
    HAL_DMA_Start(hi2c->hdmarx, (void*)&hi2c->Instance->RXDR, pData, DataSize - 1);

    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    // /* Первая запись делается заранее */
    // hi2c->Instance->TXDR = pData[0];

    /* Разрешение поддержки DMA при передаче */
    hi2c->Instance->CR1 |= I2C_CR1_TXDMAEN_M;

    /* Настройка и включение канала DMA */
    HAL_DMA_Start(hi2c->hdmatx, pData, (void*)&hi2c->Instance->TXDR, DataSize - 1);

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;
    
    if (!(hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* NOSTRETCH = 0 */
    {
        /* Ожидание вызова адреса */
        if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }
        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;

        if(hi2c->Instance->OAR1 & I2C_OAR1_OA1MODE_M)
        {
            /* Ожидание вызова адреса */
            if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
            {
                return error_code;
            }
        }

        /* Сброс содержимого TXDR */
        hi2c->Instance->ISR |= I2C_ISR_TXE_M;

        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;

    }

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    /* Разрешение поддержки DMA при передаче */
    hi2c->Instance->CR1 |= I2C_CR1_RXDMAEN_M;

    /* Настройка и включение канала DMA */
    HAL_DMA_Start(hi2c->hdmarx, (void*)&hi2c->Instance->RXDR, pData, DataSize - 1);

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if (!(hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* NOSTRETCH = 0 - с растягиванием SCL*/
    {
        /* Ожидание вызова адреса */
        if ((error_code = HAL_I2C_Slave_WaitADDR(hi2c, Timeout)) != HAL_OK)
        {
            return error_code;
        }

        /* Сброс флага ADDR */
        hi2c->Instance->ICR |= I2C_ICR_ADDRCF_M;
    }
    else if (hi2c->Instance->CR1 & I2C_CR1_SBC_M) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }


    return error_code;
}


void HAL_I2C_InterruptDisable(I2C_HandleTypeDef *hi2c, uint32_t IntDisMask)
{
    IntDisMask &= I2C_INTMASK;
    hi2c->Instance->CR1 &= ~IntDisMask;
}

void HAL_I2C_InterruptEnable(I2C_HandleTypeDef *hi2c, uint32_t IntEnMask)
{
    IntEnMask &= I2C_INTMASK;
    hi2c->Instance->CR1 |= IntEnMask;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    HAL_I2C_Master_NBYTES(hi2c);

    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - запись */
    hi2c->Instance->CR2 &= ~I2C_CR2_RD_WRN_M;

    /* Выключить все прерывания I2C */
    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    /* Включение прерываний */
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_TCIE_M
                                    | I2C_CR1_NACKIE_M
                                    | I2C_CR1_TXIE_M
                                    );

    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;

    return error_code;

}

HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    HAL_I2C_Master_NBYTES(hi2c);

    /* Задать адрес и режим адресации */
    HAL_I2C_Master_SlaveAddress(hi2c, SlaveAddress);
    /* Задать направление передачи - чтение */
    hi2c->Instance->CR2 |= I2C_CR2_RD_WRN_M;
    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_TCIE_M
                                    | I2C_CR1_ADDRIE_M
                                    | I2C_CR1_RXIE_M
                                    );
    /* Старт */
    hi2c->Instance->CR2 |= I2C_CR2_START_M;
 
    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if ((hi2c->Instance->CR1 & I2C_CR1_SBC_M) && (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }

    // /* Сброс содержимого TXDR */
    // hi2c->Instance->ISR |= I2C_ISR_TXE_M;

    /* Первая запись делается заранее */
    hi2c->Instance->TXDR = pData[hi2c->TransferCount++];
    hi2c->pBuffPtr++;

    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_STOPIE_M
                                   | I2C_CR1_ADDRIE_M
                                    | I2C_CR1_TXIE_M
                                    );


    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit_NOSTRETCH_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if ((hi2c->Instance->CR1 & I2C_CR1_SBC_M) && (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }

    // /* Сброс содержимого TXDR */
    // hi2c->Instance->ISR |= I2C_ISR_TXE_M;

    /* Первая запись делается заранее */
    hi2c->Instance->TXDR = pData[hi2c->TransferCount++];
    hi2c->pBuffPtr++;

    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_STOPIE_M
                                    | I2C_CR1_TXIE_M
                                    );


    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if ((hi2c->Instance->CR1 & I2C_CR1_SBC_M) && (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }
    

    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_STOPIE_M
                                    | I2C_CR1_ADDRIE_M
                                    | I2C_CR1_RXIE_M
                                    );

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive_NOSTRETCH_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    hi2c->Instance->CR2 &= ~I2C_CR2_RELOAD_M;

    if ((hi2c->Instance->CR1 & I2C_CR1_SBC_M) && (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)) /* Режим SBC несовместим с режимом NOSTRETCH */
    {
        return HAL_ERROR;
    }
    

    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_STOPIE_M
                                    | I2C_CR1_RXIE_M
                                    );

    return error_code;
}

HAL_StatusTypeDef HAL_I2C_Slave_ReceiveSBC_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize)
{
    HAL_StatusTypeDef error_code = HAL_OK;
    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->pBuffPtr = pData;
	hi2c->TransferSize = DataSize;
    hi2c->TransferCount = 0;

    if (!(hi2c->Instance->CR1 & I2C_CR1_SBC_M))
    {
        return HAL_ERROR;
    }
    else if (hi2c->Instance->CR1 & I2C_CR1_NOSTRETCH_M)
    {
        /* Режим SBC несовместим с режимом NOSTRETCH */
        return HAL_ERROR;
    }

    hi2c->Instance->CR2 |= I2C_CR2_RELOAD_M;

    HAL_I2C_InterruptDisable(hi2c, I2C_INTMASK);
    HAL_I2C_InterruptEnable(hi2c,     I2C_CR1_ERRIE_M
                                    | I2C_CR1_STOPIE_M
                                    | I2C_CR1_ADDRIE_M
                                    | I2C_CR1_RXIE_M
                                    );


    return error_code;
}


// void HAL_I2C_IRQHandler(I2C_HandleTypeDef *hi2c)
// {
//     uint32_t int_mask = hi2c->Instance->CR1 & I2C_INTMASK; /* разрешенные прерывания  */
//     uint32_t interrupt_status = hi2c->Instance->ISR; /* Флаги */

//     if ((interrupt_status & I2C_ISR_ADDR_M) && (int_mask & I2C_CR1_ADDRIE_M))
//     {
//         HAL_I2C_ADDR_Callback(hi2c);
//     }

//     if ((interrupt_status & (I2C_ISR_BERR_M | I2C_ISR_ARLO_M | I2C_ISR_OVR_M)) && (int_mask & I2C_CR1_ERRIE_M))
//     {
//         HAL_I2C_ERR_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_NACKF_M) && (int_mask & I2C_CR1_NACKIE_M))
//     {
//         HAL_I2C_NACK_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_STOPF_M) && (int_mask & I2C_CR1_STOPIE_M))
//     {
//         HAL_I2C_STOP_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_TXIS_M) && (int_mask & I2C_CR1_TXIE_M))
//     {
//         HAL_I2C_TXIS_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_RXNE_M) && (int_mask & I2C_CR1_RXIE_M))
//     {
//         HAL_I2C_RXNE_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_TCR_M) && (int_mask & I2C_CR1_TCIE_M))
//     {
//         HAL_I2C_TCR_Callback(hi2c);
//     }

//     if ((interrupt_status & I2C_ISR_TC_M) && (int_mask & I2C_CR1_TCIE_M))
//     {
//         HAL_I2C_TC_Callback(hi2c);
//     }

// }

