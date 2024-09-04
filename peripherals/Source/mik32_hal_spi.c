#include "mik32_hal_spi.h"

/**
  * @brief  Инициализация SPI MSP.
  * @param  hspi указатель на структуру SPI_HandleTypeDef, которая содержит
  *                 информацию о конфигурации для модуля SPI.
  */
__attribute__((weak)) void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI_0)
    {
        __HAL_PCC_SPI_0_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);

        switch (hspi->Init.ChipSelect)
        {
        case SPI_CS_0:
            GPIO_InitStruct.Pin = GPIO_PIN_4;
            HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
            break;

        case SPI_CS_1:
            GPIO_InitStruct.Pin = GPIO_PIN_14;
            HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
            break;

        case SPI_CS_2:
            GPIO_InitStruct.Pin = GPIO_PIN_15;
            HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
            break;

        case SPI_CS_3:
            GPIO_InitStruct.Pin = GPIO_PIN_6;
            HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);
            break;
        }

        /* В режиме ведущего вывод SPIx_N_SS_IN должен быть в режиме SPI с подтяжкой к питанию. */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        if (hspi->Init.SPI_Mode == HAL_SPI_MODE_MASTER)
        {
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
        }
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
    }

    if (hspi->Instance == SPI_1)
    {
        __HAL_PCC_SPI_1_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);

        switch (hspi->Init.ChipSelect)
        {
        case SPI_CS_0:
            GPIO_InitStruct.Pin = GPIO_PIN_4;
            break;

        case SPI_CS_1:
            GPIO_InitStruct.Pin = GPIO_PIN_5;
            break;

        case SPI_CS_2:
            GPIO_InitStruct.Pin = GPIO_PIN_6;
            break;

        case SPI_CS_3:
            GPIO_InitStruct.Pin = GPIO_PIN_7;
            break;
        }
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        if (hspi->Init.SPI_Mode == HAL_SPI_MODE_MASTER)
        {
            GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
        }
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
    }
}

/**
 * @brief Включить модуль SPI.
 * 
 * Перед включением модуля производится сброс флагов ошибок и очистка буферов TX и RX.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
  *                 информацию о конфигурации для модуля SPI.
 */
void HAL_SPI_Enable(SPI_HandleTypeDef *hspi)
{
    HAL_SPI_ClearError(hspi);
    __HAL_SPI_ENABLE(hspi);
}

/**
 * @brief Выключить модуль SPI.
 * 
 * После выключения модуля производится сброс флагов ошибок и очистка буферов TX и RX.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
void HAL_SPI_Disable(SPI_HandleTypeDef *hspi)
{
    __HAL_SPI_DISABLE(hspi);
    HAL_SPI_ClearError(hspi);
}

/**
 * @brief Задать задержку BTWN.
 * 
 * Задержка в периодах опорного тактового сигнала между снятием сигнала выбора одного ведомого 
 * устройства и установкой сигнала выбора другого ведомого устройства.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param btwn задержка BTWN в периодах опорного тактового сигнала.
 *                  Этот параметр может быть значением в пределах от 0 до 255. 
 */
void HAL_SPI_SetDelayBTWN(SPI_HandleTypeDef *hspi, uint8_t btwn)
{
    hspi->Instance->DELAY &= ~SPI_DELAY_BTWN_M;
    hspi->Instance->DELAY |= SPI_DELAY_BTWN(btwn);
}

/**
 * @brief Задать задержку AFTER.
 * 
 * Задержка в периодах опорного тактового сигнала между последним битом текущего слова
 * и первым битом следующего слова.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param after Задержка AFTER в периодах опорного тактового сигнала.
 *                  Этот параметр может быть числом в пределах от 0 до 255.
 */
void HAL_SPI_SetDelayAFTER(SPI_HandleTypeDef *hspi, uint8_t after)
{
    hspi->Instance->DELAY &= ~SPI_DELAY_AFTER_M;
    hspi->Instance->DELAY |= SPI_DELAY_AFTER(after);
}

/**
 * @brief Задать задержку INIT.
 * 
 * Дополнительная задержка в периодах опорного тактового сигнала между установкой
 * сигнала n_ss_out в «0» и передачей первого бита.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param init задержка INIT в периодах опорного тактового сигнала.
 *                  Этот параметр может быть числом в пределах от 0 до 255.
 */
void HAL_SPI_SetDelayINIT(SPI_HandleTypeDef *hspi, uint8_t init)
{
    hspi->Instance->DELAY &= ~SPI_DELAY_INIT_M;
    hspi->Instance->DELAY |= SPI_DELAY_INIT(init);
}

/**
 * @brief Задать задержку перед передачей.
 * 
 * Модуль SPI в режиме ведомого устройства начинает передачу только когда тактовый сигнал 
 * sclk_in (внешнего ведущего устройства) не изменяется в течение количества периодов опорного 
 * тактового сигнала SPI заданного в этом поле или когда модуль SPI не активен.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param slave_idle_counter задержку перед передачей в периодах опорного тактового сигнала.
 *                  Этот параметр может быть числом в пределах от 0 до 255. 
 */
void HAL_SPI_SetSlaveIdleCounter(SPI_HandleTypeDef *hspi, uint8_t slave_idle_counter)
{
    hspi->Instance->SIC = slave_idle_counter;
}

/**
 * @brief Задать уровень, при котором TX_FIFO считается незаполненным и формируется 
 *        прерывание TX_FIFO_NOT_full (IXR_TXOW).
 *
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param threshold уровень, при котором TX_FIFO считается не заполненным.
 */
void HAL_SPI_SetThresholdTX(SPI_HandleTypeDef *hspi, uint32_t threshold)
{
    hspi->Init.ThresholdTX = threshold;
    hspi->Instance->TX_THR = threshold;
}

/**
 * @brief Получить идентификационный номер модуля. Ожидаемое ID 0x01090100.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @return идентификационный номер модуля.
 */
uint32_t HAL_SPI_ReadModuleID(SPI_HandleTypeDef *hspi)
{
    return hspi->Instance->ID;
}

/**
 * @brief Инициализировать SPI в соответствии с настройками в @ref SPI_HandleTypeDef.
 * 
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    if (hspi == NULL)
    {
        return HAL_ERROR;
    }

    HAL_SPI_MspInit(hspi);

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
    SPI_config |= (hspi->Init.BaudRateDiv << SPI_CONFIG_BAUD_RATE_DIV_S) | /* Настройка делителя частоты */
                  (hspi->Init.ManualCS << SPI_CONFIG_MANUAL_CS_S) |        /* Настройка режима управления сигналом CS */
                  (hspi->Init.CLKPhase << SPI_CONFIG_CLK_PH_S) |           /* Настройка фазы тактового сигнала */
                  (hspi->Init.CLKPolarity << SPI_CONFIG_CLK_POL_S) |       /* Настройка полярности тактового сигнала */
                  (hspi->Init.Decoder << SPI_CONFIG_PERI_SEL_S);           /* Настройка использования внешнего декодера */
                                                                           //(hspi->Init.DataSize << SPI_CONFIG_DATA_SZ_S);               /* Длина передаваемой посылки */

    /* Выбор ведомого в соответствии с режимом ManualCS */
    if (hspi->Init.ManualCS == SPI_MANUALCS_ON)
    {
        /* Ведомое устройство не выбрано. Ручное управление сигналом CS */
        SPI_config |= SPI_CS_NONE << SPI_CONFIG_CS_S;
    }
    else
    {
        /* Выбор ведомого устройства в автоматическом режиме управления CS */
        SPI_config |= hspi->Init.ChipSelect << SPI_CONFIG_CS_S;
    }

    /* Установка выбранных настроек */
    hspi->Instance->CONFIG = SPI_config;

    HAL_SPI_SetDelayBTWN(hspi, 1);
    HAL_SPI_SetDelayAFTER(hspi, 0);
    HAL_SPI_SetDelayINIT(hspi, 0);

    /* уровень при котором регистр TX считается незаполненным и формируется прерывание */
    if (hspi->Init.ThresholdTX > 8)
    {
        return HAL_ERROR;
    }
    HAL_SPI_SetThresholdTX(hspi, hspi->Init.ThresholdTX);

    hspi->TxCount = 0;
    hspi->RxCount = 0;

    hspi->State = HAL_SPI_STATE_READY;

    return error_code;
}

/**
 * @brief Очистить буфер TX_FIFO.
 * 
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
void HAL_SPI_ClearTXFIFO(SPI_HandleTypeDef *hspi)
{
    hspi->Instance->ENABLE |= SPI_ENABLE_CLEAR_TX_FIFO_M;
}

/**
 * @brief Очистить буфер RX_FIFO.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
void HAL_SPI_ClearRXFIFO(SPI_HandleTypeDef *hspi)
{
    hspi->Instance->ENABLE |= SPI_ENABLE_CLEAR_RX_FIFO_M;
}

/**
 * @brief Сбросить флаги ошибок, очистить буферы RX и TX.
 * 
 * @warning Функция выключает модуль SPI.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 **/
void HAL_SPI_ClearError(SPI_HandleTypeDef *hspi)
{
    /* Сброс ошибок */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    __HAL_SPI_DISABLE(hspi);
    HAL_SPI_ClearRXFIFO(hspi);
    HAL_SPI_ClearTXFIFO(hspi);
    volatile uint32_t unused = hspi->Instance->INT_STATUS; /* Очистка флагов ошибок чтением */
    (void) unused;
}

/**
 * @brief Выбрать ведомое устройство.
 * 
 * В ручном режиме управления сигналом выбора ведомого 
 * @ref SPI_InitTypeDef::ManualCS "SPI_HandleTypeDef.Init.ManualCS" = @ref SPI_MANUALCS_ON при вызове функции выбранный вывод
 * перейдет в активное состояние (низкий уровень).
 * 
 * В автоматическом режиме управления сигналом выбора ведомого функция задает один из сигналов
 * CS0 - CS3, который будет использован во время передачи.
 * 
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param CS_M ведомое устройство.
 *      Этот параметр должен быть одним из следующих значений:
 *          - @ref SPI_CS_NONE  - ведомое устройство не выбрано
 *          - @ref SPI_CS_0     - ведомое устройство 1
 *          - @ref SPI_CS_1     - ведомое устройство 2
 *          - @ref SPI_CS_2     - ведомое устройство 3
 *          - @ref SPI_CS_3     - ведомое устройство 4
 *      Если используется внешний декодер (@ref SPI_InitTypeDef::Decoder "SPI_HandleTypeDef.Init.Decoder" = @ref SPI_DECODER_USE),
 *      @ref SPI_InitTypeDef::ChipSelect "SPI_HandleTypeDef.Init.ChipSelect" отображается на выводах CS0 - CS3. 
 * 
 * @warning В ручном режиме перед использованием этой функции следует сначала включить SPI, например, с помощью макроса __HAL_SPI_ENABLE.
 */
void HAL_SPI_CS_Enable(SPI_HandleTypeDef *hspi, uint32_t CS_M)
{
    hspi->Init.ChipSelect = CS_M;
    CS_M = CS_M << SPI_CONFIG_CS_S;
    hspi->Instance->CONFIG = (hspi->Instance->CONFIG & ~SPI_CONFIG_CS_M) | CS_M;
}

/**
 * @brief Перевести активный сигнал выбора ведомого в неактивное состояние (высокий уровень).
 * 
 * Функция устанавливает значение @ref SPI_CS_NONE - ведомые устройства не выбраны.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
void HAL_SPI_CS_Disable(SPI_HandleTypeDef *hspi)
{
    hspi->Init.ChipSelect = SPI_CS_NONE;
    hspi->Instance->CONFIG = (hspi->Instance->CONFIG & ~SPI_CONFIG_CS_M) | SPI_CONFIG_CS_NONE_M;
}

/**
 * @brief Запустить передачу и прием данных.
 * 
 * Байты поочередно передаются и считываются по одному байту.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param TransmitBytes указатель на буфер передаваемых данных.
 * @param ReceiveBytes указатель на буфер считываемых данных.
 * @param DataSize число байт для отправки и приема.
 * @param Timeout продолжительность тайм-аута.
 * @return Статус HAL.
 * 
 * @note Во время обмена пороговое значение ThresholdTX = 1.
 * @warning Если Вы управляете сигналом выбора ведомого в ручном режиме или используете для этого GPIO, 
 *          SPI следует включать до того, как уровень сигнала CS станет активным. Для включения SPI можно 
 *          использовать макрос __HAL_SPI_ENABLE.
 */
HAL_StatusTypeDef HAL_SPI_Exchange(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t DataSize, uint32_t Timeout)
{
    uint32_t txallowed = 1;
    HAL_StatusTypeDef error_code = HAL_OK;
    uint32_t timeout_counter = 0;

    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    hspi->pRxBuffPtr = (uint8_t *)ReceiveBytes;
    hspi->RxCount = DataSize;
    hspi->pTxBuffPtr = (uint8_t *)TransmitBytes;
    hspi->TxCount = DataSize;

    hspi->Instance->TX_THR = 1;

    /* Включить SPI если выключено */
    if (!(hspi->Instance->ENABLE & SPI_ENABLE_M))
    {
        __HAL_SPI_ENABLE(hspi);
    }

    while ((hspi->TxCount > 0) || (hspi->RxCount > 0))
    {
        /* Проверка флага TX_FIFO_NOT_FULL */
        if ((hspi->Instance->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M) && (hspi->TxCount > 0) && (txallowed == 1))
        {
            hspi->Instance->TXDATA = *(hspi->pTxBuffPtr);
            hspi->pTxBuffPtr++;
            hspi->TxCount--;
            /* Следующие данные - прием (Rx). Tx не разрешен */
            txallowed = 0;
        }

        /* Ожидание когда установится флаг RX_FIFO_NOT_EMPTY */
        if ((hspi->Instance->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M) && (hspi->RxCount > 0))
        {
            *(hspi->pRxBuffPtr) = hspi->Instance->RXDATA;
            hspi->pRxBuffPtr++;
            hspi->RxCount--;
            /* Следующие данные - передача (Tx). Tx разрешается */
            txallowed = 1;
        }

        if (((timeout_counter++) >= Timeout) || (Timeout == 0U))
        {
            error_code = HAL_TIMEOUT;
            goto error;
        }
    }

error:
    if (!(hspi->Instance->CONFIG & SPI_CONFIG_MANUAL_CS_M))
    {
        __HAL_SPI_DISABLE(hspi);
        hspi->Instance->ENABLE |= SPI_ENABLE_CLEAR_TX_FIFO_M | SPI_ENABLE_CLEAR_RX_FIFO_M; /* Очистка буферов RX и TX */
    }

    volatile uint32_t unused = hspi->Instance->INT_STATUS; /* Очистка флагов ошибок чтением */
    (void) unused;


    return error_code;
}


/**
 * @brief Запустить передачу и прием данных с прерываниями.
 * 
 * Во время передачи используются следующие прерывания:
 *      - RX_OVERFLOW:  прерывание при переполнении буфера RX_FIFO
 *      - MODE_FAIL: напряжение на выводе n_ss_in не соответствую режиму работы SPI
 *      - TX_FIFO_NOT_FULL: регистр TX_FIFO не заполнен (опустошился ниже значения @ref SPI_InitTypeDef::ThresholdTX "SPI_HandleTypeDef.Init.ThresholdTX")
 *      - RX_FIFO_NOT_EMPTY: буфер RX_FIFO не пустой
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param TransmitBytes указатель на буфер передаваемых данных.
 * @param ReceiveBytes указатель на буфер считываемых данных.
 * @param Size число байт для отправки и приема.
 * @return Статус HAL.
 * 
 * @warning Если Вы управляете сигналом выбора ведомого в ручном режиме или используете для этого GPIO, 
 *       SPI следует включать до того, как уровень сигнала CS станет активным. Для включения SPI можно 
 *       использовать макрос __HAL_SPI_ENABLE.
 */
HAL_StatusTypeDef HAL_SPI_Exchange_IT(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size)
{
    HAL_StatusTypeDef error_code = HAL_OK;

    if ((TransmitBytes == NULL) || (ReceiveBytes == NULL) || (Size == 0))
    {
        error_code = HAL_ERROR;
        return error_code;
    }
    if (hspi->Init.ThresholdTX == 0)
    {
        error_code = HAL_ERROR;
        return error_code;
    }

    hspi->State = HAL_SPI_STATE_READY;

    hspi->pTxBuffPtr = TransmitBytes;
    hspi->TxCount = Size;
    hspi->pRxBuffPtr = ReceiveBytes;
    hspi->RxCount = Size;

    if (!(hspi->Instance->CONFIG & SPI_CONFIG_MANUAL_CS_M))
    {
        /* Очистка ошибок */
        HAL_SPI_ClearError(hspi);
    }
    
    /* Включить SPI если выключено */
    if (!(hspi->Instance->ENABLE & SPI_ENABLE_M))
    {
        __HAL_SPI_ENABLE(hspi);
    }

    HAL_SPI_InterruptEnable(hspi, SPI_INT_STATUS_RX_OVERFLOW_M | SPI_INT_STATUS_MODE_FAIL_M              /* Прерывания ошибок */
                            | SPI_INT_STATUS_TX_FIFO_NOT_FULL_M  | SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M);   /* Прерывания опустошения буфера TX и наличие байтов в буфере RX */

    return error_code;
}
