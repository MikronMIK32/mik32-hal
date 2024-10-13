#ifndef MIK32_HAL_SPI
#define MIK32_HAL_SPI

#include "stddef.h"
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "power_manager.h"
#include "spi.h"
#include "mik32_hal_def.h"
#include "mik32_memory_map.h"


/** Значение timeout по умолчанию. */
#define SPI_TIMEOUT_DEFAULT 1000000

/* Ошибки SPI */
#define HAL_SPI_ERROR_NONE  0b00000000  /**< Значение при отсутствии ошибок. */
#define HAL_SPI_ERROR_MODF  0b00000001  /**< Маска для ошибки MODE_FAIL - напряжение на выводе n_ss_in не соответствую режиму работы SPI. */
#define HAL_SPI_ERROR_OVR   0b00000010  /**< Маска для ошибки RX_OVERFLOW - прерывание при переполнении RX_FIFO. */

/* Выбор ведомых устройств. */
#define SPI_CS_NONE 0b1111      /**< Ведомое устройство не выбрано. */
#define SPI_CS_0    0b1110      /**< Ведомое устройство 1. */
#define SPI_CS_1    0b1101      /**< Ведомое устройство 2. */
#define SPI_CS_2    0b1011      /**< Ведомое устройство 3. */
#define SPI_CS_3    0b0111      /**< Ведомое устройство 4. */

/* Коэффициент деления частоты spi_ref_clk. */
#define SPI_BAUDRATE_DIV4   0b001   /**< Коэффициент деления частоты spi_ref_clk - 4. */
#define SPI_BAUDRATE_DIV8   0b010   /**< Коэффициент деления частоты spi_ref_clk - 8. */
#define SPI_BAUDRATE_DIV16  0b011   /**< Коэффициент деления частоты spi_ref_clk - 16. */
#define SPI_BAUDRATE_DIV32  0b100   /**< Коэффициент деления частоты spi_ref_clk - 32. */
#define SPI_BAUDRATE_DIV64  0b101   /**< Коэффициент деления частоты spi_ref_clk - 64. */
#define SPI_BAUDRATE_DIV128 0b110   /**< Коэффициент деления частоты spi_ref_clk - 128. */
#define SPI_BAUDRATE_DIV256 0b111   /**< Коэффициент деления частоты spi_ref_clk - 256. */

/* Режим управления сигналом выбора ведомого CS. */
#define SPI_MANUALCS_OFF    0   /**< Автоматический режим. */
#define SPI_MANUALCS_ON     1   /**< Ручной режим. */

/* Настройки фазы тактового сигнала. */
#define SPI_PHASE_OFF   0   /**< Тактовая частота SPI активна вне слова */
#define SPI_PHASE_ON    1   /**< Тактовая частота SPI неактивна вне слова */

/* Настройки полярности тактового сигнала вне слова. */
#define SPI_POLARITY_LOW    0  /**< Тактовый сигнал вне слова удерживается на низком уровне. */
#define SPI_POLARITY_HIGH   1  /**< Тактовый сигнал вне слова удерживается на высоком уровне. */

/* Использование внешнего декодера. */
#define SPI_DECODER_NONE 0  /**< Внешний декодер не используется. Выбор только 1 из 4 ведомых устройств. */
#define SPI_DECODER_USE  1  /**< Используется внешний декодер. */

/* Длина передаваемой посылки. */
#define SPI_DATASIZE_8BITS  0  /**< Длина передаваемой посылки - 8 бит. */
#define SPI_DATASIZE_16BITS 1  /**< Длина передаваемой посылки - 16 бит. */
#define SPI_DATASIZE_24BITS 2  /**< Длина передаваемой посылки - 24 бит. */
#define SPI_DATASIZE_32BITS 3  /**< Длина передаваемой посылки - 32 бит. */

/* Значения по умолчанию порогового значения TX_FIFO. */
#define SPI_THRESHOLD_DEFAULT 4 /* Значение Threshold_of_TX_FIFO по умолчанию*/

/* Прерывания. */
#define TX_FIFO_UNDERFLOW   6   /**< Регистр TX FIFO опустошен. */
#define RX_FIFO_FULL        5   /**< Регистр RX_FIFO заполнен. */
#define RX_FIFO_NOT_EMPTY   4   /**< Регистр RX_FIFO не пустой. */
#define TX_FIFO_FULL        3   /**< Регистр TX_FIFO заполнен. */
#define TX_FIFO_NOT_FULL    2   /**< Регистр TX_FIFO не заполнен. */
#define MODE_FAIL           1   /**< Напряжение на выводе n_ss_in не соответствую режиму работы SPI. */
#define RX_OVERFLOW         0   /**< Прерывание при переполнении RX_FIFO, значение сбрасывается при чтении. */

#define SPI_BUFFER_SIZE 8   /**< Размер буферов RX и TX. */

/** @brief  Включить модуль SPI.
  * @param  __HANDLE__ Указатель SPI Handle.
  */
#define __HAL_SPI_ENABLE(__HANDLE__) ((__HANDLE__)->Instance->ENABLE = SPI_ENABLE_M)

/** @brief  Выключить модуль SPI.
  * @param  __HANDLE__ Указатель SPI Handle.
  */
#define __HAL_SPI_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->ENABLE &= ~SPI_ENABLE_M)

/** @brief  Проверить установлен ли указанный флаг SPI или нет.
  * @param  __HANDLE__ Указатель SPI Handle.
  * @param  __FLAG__ Флаг для проверки.
  *         Этот параметр должен быть одним из следующих значений:
  *            - @ref SPI_INT_STATUS_SPI_ACTIVE_M:            Флаг статуса сеанса передачи
  *            - @ref SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M:     Флаг опустошения буфера TX_FIFO в режиме ведомого перед началом обмена
  *            - @ref SPI_INT_STATUS_RX_FIFO_FULL_M:          Флаг заполнения буфера RX_FIFO
  *            - @ref SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M:     Флаг наличия данных в буфере RX_FIFO
  *            - @ref SPI_INT_STATUS_TX_FIFO_FULL_M:          Флаг заполнения буфера TX_FIFO
  *            - @ref SPI_INT_STATUS_TX_FIFO_NOT_FULL_M:      Флаг заполнения буфера TX_FIFO ниже порогового значения
  *            - @ref SPI_INT_STATUS_MODE_FAIL_M:             Флаг детектирования напряжение на выводе n_ss_in не соответствую режиму работы SPI
  *            - @ref SPI_INT_STATUS_RX_OVERFLOW_M:           Флаг переполнения буфера RX_FIFO
  * 
  * @return Состояние флага @p __FLAG__ (TRUE или FALSE).
  */
#define __HAL_SPI_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->ENABLE) & (__FLAG__)) == (__FLAG__))

/**
 * @brief Выбор режима ведущий/ведомый.
 */
typedef enum __HAL_SPI_ModeTypeDef
{
    HAL_SPI_MODE_SLAVE = 0,  /**< Режим ведомого. */
    HAL_SPI_MODE_MASTER = 1, /**< Режим ведущего. */

} HAL_SPI_ModeTypeDef;

/**
 * @brief Состояние модуля SPI.
 */
typedef enum __HAL_SPI_StateTypeDef
{
    HAL_SPI_STATE_READY, /**< Готов к передаче. */
    HAL_SPI_STATE_BUSY,  /**< Идет передача. */
    HAL_SPI_STATE_END,   /**< Передача завершена. */
    HAL_SPI_STATE_ERROR  /**< Ошибка при передаче. */
} HAL_SPI_StateTypeDef;

/**
 * @brief Определение структуры конфигурации SPI
 */
typedef struct __SPI_InitTypeDef
{

    /**
     * @brief Выбор режима ведущий/ведомый.
     *
     * Этот параметр должен быть одним из значений:
     *      - @ref HAL_SPI_MODE_SLAVE  - режим ведомого 
     *      - @ref HAL_SPI_MODE_MASTER - режим ведущего
     */
    HAL_SPI_ModeTypeDef SPI_Mode; 

    /**
     * @brief Коэффициент деления частоты.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_BAUDRATE_DIV4;
     *      - @ref SPI_BAUDRATE_DIV8;
     *      - @ref SPI_BAUDRATE_DIV16;
     *      - @ref SPI_BAUDRATE_DIV32;
     *      - @ref SPI_BAUDRATE_DIV64;
     *      - @ref SPI_BAUDRATE_DIV128;
     *      - @ref SPI_BAUDRATE_DIV256.
     */
    uint8_t BaudRateDiv;

    /**
     * @brief Режим управления сигналом выбора ведомого CS.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_MANUALCS_OFF - сигнал выбора ведомого CS управляется автоматически
     *      - @ref SPI_MANUALCS_ON  - сигнал выбора ведомого CS управляется вручную
     */
    uint8_t ManualCS; 

    /**
     * @brief Настройка фазы тактового сигнала.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_PHASE_OFF   - тактовая частота SPI активна вне слова. Сигналы SS переводятся в неактивное состояние между словами.
     *                               Длительность импульса всегда 1 период тактового сигнала шины APB_P
     *      - @ref SPI_PHASE_ON    - тактовая частота SPI неактивна вне слова. Сигналы SS не переводятся в неактивное состояние между словами 
     */
    uint8_t CLKPhase; 

    /**
     * @brief Настройка полярности тактового сигнала вне слова.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_POLARITY_LOW    - тактовый сигнал вне слова удерживается на низком уровне
     *      - @ref SPI_POLARITY_HIGH   - тактовый сигнал вне слова удерживается на высоком уровне
     */
    uint8_t CLKPolarity;

    /**
     * @brief Использование внешнего декодера.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_DECODER_NONE    - внешний декодер не используется. Выбирается один ведомый из четырех, подключенный к одному из выводов CS0 - CS3
     *      - @ref SPI_DECODER_USE     - используется внешний декодер. Значение @ref SPI_InitTypeDef::ChipSelect "SPI_HandleTypeDef.Init.ChipSelect"
     *                                   отображается на выводах CS0 - CS3
     * 
     */
    uint8_t Decoder;

    /**
     * @brief Пороговое значение TX_FIFO.
     * 
     * Уровень, при котором TX_FIFO считается незаполненным и формируется прерывание TX_FIFO_NOT_full (IXR_TXOW).
     */
    uint8_t ThresholdTX;

    /**
     * @brief Выбранное ведомое устройство.
     * 
     * Этот параметр должен быть одним из значений:
     *      - @ref SPI_CS_NONE    - ведомое устройство не выбрано     
     *      - @ref SPI_CS_0       - ведомое устройство 1
     *      - @ref SPI_CS_1       - ведомое устройство 2 
     *      - @ref SPI_CS_2       - ведомое устройство 3 
     *      - @ref SPI_CS_3       - ведомое устройство 4
     */
    uint8_t ChipSelect;

} SPI_InitTypeDef;

/**
 * @brief Определение структуры SPI Handle.
 */
typedef struct __SPI_HandleTypeDef
{

    SPI_TypeDef *Instance;          /**< Адрес регистров SPI. */

    SPI_InitTypeDef Init;           /**< Параметры SPI. */

    uint8_t ErrorCode;              /**< Код ошибки при работе SPI. */

    HAL_SPI_StateTypeDef State;     /**< Состояние модуля SPI. */

    uint8_t *pTxBuffPtr;            /**< Указатель на буфер для передачи данных по SPI. */

    uint32_t TxCount;               /**< Счетчик байт при передачи данных по SPI. */

    uint8_t *pRxBuffPtr;            /**< Указатель на буфер для считывания данных по SPI. */

    uint32_t RxCount;               /**< Счетчик байт при считывания данных по SPI. */

} SPI_HandleTypeDef;

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_Enable(SPI_HandleTypeDef *hspi);
void HAL_SPI_Disable(SPI_HandleTypeDef *hspi);
void HAL_SPI_SetDelayBTWN(SPI_HandleTypeDef *hspi, uint8_t btwn);
void HAL_SPI_SetDelayAFTER(SPI_HandleTypeDef *hspi, uint8_t after);
void HAL_SPI_SetDelayINIT(SPI_HandleTypeDef *hspi, uint8_t init);
void HAL_SPI_SetSlaveIdleCounter(SPI_HandleTypeDef *hspi, uint8_t slave_idle_counter);
void HAL_SPI_SetThresholdTX(SPI_HandleTypeDef *hspi, uint32_t threshold);
uint32_t HAL_SPI_ReadModuleID(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
void HAL_SPI_ClearTXFIFO(SPI_HandleTypeDef *hspi);
void HAL_SPI_ClearRXFIFO(SPI_HandleTypeDef *hspi);
void HAL_SPI_ClearError(SPI_HandleTypeDef *hspi);
void HAL_SPI_CS_Enable(SPI_HandleTypeDef *hspi, uint32_t CS_M);
void HAL_SPI_CS_Disable(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Exchange(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Exchange_IT(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size);


/**
 * @brief Разрешить прерывания в соответствии с маской.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param IntEnMask маска разрешенных прерываний.
 */
static inline __attribute__((always_inline)) void HAL_SPI_InterruptEnable(SPI_HandleTypeDef *hspi, uint32_t IntEnMask)
{
    hspi->Instance->INT_ENABLE |= IntEnMask;
}

/**
 * @brief Запретить прерывания в соответствии с маской.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param IntDisMask маска запрещенных прерываний.
 */
static inline __attribute__((always_inline)) void HAL_SPI_InterruptDisable(SPI_HandleTypeDef *hspi, uint32_t IntDisMask)
{
    hspi->Instance->INT_DISABLE |= IntDisMask;
}

/**
 * @brief Получить состояние флага прерывания с учетом маски разрешенных прерываний.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 * @param Interrupt Флаг прерывания.
 *                  Этот параметр должен быть одним из следующих значений:
 *                      - @ref TX_FIFO_UNDERFLOW   - регистр TX FIFO опустошен
 *                      - @ref RX_FIFO_FULL        - регистр RX_FIFO заполнен
 *                      - @ref RX_FIFO_NOT_EMPTY   - регистр RX_FIFO не пустой
 *                      - @ref TX_FIFO_FULL        - регистр TX_FIFO заполнен
 *                      - @ref TX_FIFO_NOT_FULL    - регистр TX_FIFO не заполнен
 *                      - @ref MODE_FAIL           - напряжение на выводе n_ss_in не соответствую режиму работы SPI
 *                      - @ref RX_OVERFLOW         - прерывание при переполнении RX_FIFO, значение сбрасывается при чтении
 * @return Состояние флага.
 */
static inline __attribute__((always_inline)) uint8_t HAL_SPI_GetInterruptStatus(SPI_HandleTypeDef *hspi, uint32_t Interrupt)
{
    uint32_t interrupt_status = hspi->Instance->INT_STATUS & hspi->Instance->INT_MASK;

    switch (Interrupt)
    {
    case TX_FIFO_UNDERFLOW:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M) >> SPI_INT_STATUS_TX_FIFO_UNDERFLOW_S;
        break;
    case RX_FIFO_FULL:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_RX_FIFO_FULL_M) >> SPI_INT_STATUS_RX_FIFO_FULL_S;
        break;

    case RX_FIFO_NOT_EMPTY:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M) >> SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_S;
        break;

    case TX_FIFO_FULL:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_TX_FIFO_FULL_M) >> SPI_INT_STATUS_TX_FIFO_FULL_S;
        break;

    case TX_FIFO_NOT_FULL:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M) >> SPI_INT_STATUS_TX_FIFO_NOT_FULL_S;
        break;

    case MODE_FAIL:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_MODE_FAIL_M) >> SPI_INT_STATUS_MODE_FAIL_S;
        break;

    case RX_OVERFLOW:
        interrupt_status = (interrupt_status & SPI_INT_STATUS_RX_OVERFLOW_M) >> SPI_INT_STATUS_RX_OVERFLOW_S;
        break;

    default:
        break;
    }

    return interrupt_status;
}

/**
 * @brief Обработчик прерывания RX_OVERFLOW (IXR_RXOVR) - переполнение буфера RX.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
static inline __attribute__((always_inline)) void HAL_SPI_RXOverflow_IRQ(SPI_HandleTypeDef *hspi)
{
    hspi->State = HAL_SPI_STATE_ERROR;
    hspi->ErrorCode |= HAL_SPI_ERROR_OVR;
    HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_OVERFLOW_M |
                                       SPI_INT_STATUS_MODE_FAIL_M |
                                       SPI_INT_STATUS_TX_FIFO_NOT_FULL_M |
                                       SPI_INT_STATUS_TX_FIFO_FULL_M |
                                       SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M |
                                       SPI_INT_STATUS_RX_FIFO_FULL_M |
                                       SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M);
}

/**
 * @brief Обработчик прерывания MODE_FAIL (IXR_MODF) - напряжение на выводе n_ss_in не соответствую режиму работы SPI.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
static inline __attribute__((always_inline)) void HAL_SPI_ModeFail_IRQ(SPI_HandleTypeDef *hspi)
{
    hspi->State = HAL_SPI_STATE_ERROR;
    hspi->ErrorCode |= HAL_SPI_ERROR_MODF;
    HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_OVERFLOW_M |
                                       SPI_INT_STATUS_MODE_FAIL_M |
                                       SPI_INT_STATUS_TX_FIFO_NOT_FULL_M |
                                       SPI_INT_STATUS_TX_FIFO_FULL_M |
                                       SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M |
                                       SPI_INT_STATUS_RX_FIFO_FULL_M |
                                       SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M);
}

/**
 * @brief Обработчик прерывания TX_FIFO_NOT_full (IXR_TXOW) - регистр TX_FIFO не заполнен (меньше значения ThresholdTX).
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI.
 */
static inline __attribute__((always_inline)) void HAL_SPI_TXFifoNotFull_IRQ(SPI_HandleTypeDef *hspi)
{
    uint32_t write_read_bytes = SPI_BUFFER_SIZE - hspi->Init.ThresholdTX + 1; /* Число байт для записи и чтения в итерации */

    /* Первичное полное заполнение буфера TX */
    if (hspi->State == HAL_SPI_STATE_READY)
    {
        hspi->State = HAL_SPI_STATE_BUSY;

        for (volatile uint32_t i = 0; (i < SPI_BUFFER_SIZE) && hspi->TxCount; i++)
        {
            hspi->Instance->TXDATA = *(hspi->pTxBuffPtr++);
            hspi->TxCount--;
        }
    }
    else // Заполнение буфера TX по write_read_bytes байт
    {
        for (volatile uint32_t i = 0; (i < write_read_bytes) && hspi->TxCount; i++)
        {
            hspi->Instance->TXDATA = *(hspi->pTxBuffPtr++);
            hspi->TxCount--;
        }
    }
    

    if (hspi->TxCount == 0)
    {
        HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_TX_FIFO_NOT_FULL_M);

        if (hspi->RxCount == 0)
        {
            hspi->State = HAL_SPI_STATE_END;
            if (!(hspi->Instance->CONFIG & SPI_CONFIG_MANUAL_CS_M))
            {
                __HAL_SPI_DISABLE(hspi);
                hspi->Instance->ENABLE |= SPI_ENABLE_CLEAR_TX_FIFO_M | SPI_ENABLE_CLEAR_RX_FIFO_M; /* Очистка буферов RX и TX */
            }
            volatile uint32_t unused = hspi->Instance->INT_STATUS;                             /* Очистка флагов ошибок чтением */
            (void)unused;
            HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_OVERFLOW_M |
                                               SPI_INT_STATUS_MODE_FAIL_M |
                                               SPI_INT_STATUS_TX_FIFO_NOT_FULL_M |
                                               SPI_INT_STATUS_TX_FIFO_FULL_M |
                                               SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M |
                                               SPI_INT_STATUS_RX_FIFO_FULL_M |
                                               SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M);
        }
    }
}

/**
 * @brief Обработчик прерывания RX_FIFO_NOT_EMPTY (IXR_RXNEMPTY) - Регистр RX_FIFO не пустой.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI. 
 */
static inline __attribute__((always_inline)) void HAL_SPI_RXFifoNotEmpty_IRQ(SPI_HandleTypeDef *hspi)
{
    uint32_t write_read_bytes = SPI_BUFFER_SIZE - hspi->Init.ThresholdTX + 1; /* Число байт для записи и чтения в итерации */

    for (volatile uint32_t i = 0; (i < write_read_bytes) && hspi->RxCount; i++)
    {
        while (!(hspi->Instance->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M));
        *(hspi->pRxBuffPtr++) = hspi->Instance->RXDATA;
        hspi->RxCount--;
    }

    if (hspi->RxCount == 0)
    {
        HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M | SPI_INT_STATUS_RX_OVERFLOW_M);

        if (hspi->TxCount == 0)
        {
            hspi->State = HAL_SPI_STATE_END;
            if (!(hspi->Instance->CONFIG & SPI_CONFIG_MANUAL_CS_M))
            {
                __HAL_SPI_DISABLE(hspi);
                hspi->Instance->ENABLE |= SPI_ENABLE_CLEAR_TX_FIFO_M | SPI_ENABLE_CLEAR_RX_FIFO_M; /* Очистка буферов RX и TX */
            }
            volatile uint32_t unused = hspi->Instance->INT_STATUS;                             /* Очистка флагов ошибок чтением */
            (void)unused;
            HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_OVERFLOW_M |
                                               SPI_INT_STATUS_MODE_FAIL_M |
                                               SPI_INT_STATUS_TX_FIFO_NOT_FULL_M |
                                               SPI_INT_STATUS_TX_FIFO_FULL_M |
                                               SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M |
                                               SPI_INT_STATUS_RX_FIFO_FULL_M |
                                               SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M);
        }
    }
}

/**
 * @brief Обработчик запроса прерывания SPI.
 * @param hspi указатель на структуру SPI_HandleTypeDef, которая содержит
 *                  информацию о конфигурации для модуля SPI. 
 */
static inline __attribute__((always_inline)) void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
    uint32_t interrupt_status = hspi->Instance->INT_STATUS & hspi->Instance->INT_MASK;

    if (!(interrupt_status & (SPI_INT_STATUS_RX_OVERFLOW_M | SPI_INT_STATUS_MODE_FAIL_M | SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M)))
    {
        if (interrupt_status & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M)
        {
            HAL_SPI_RXFifoNotEmpty_IRQ(hspi);
        }

        if (interrupt_status & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)
        {
            HAL_SPI_TXFifoNotFull_IRQ(hspi);
        }
        return;
    }

    if (interrupt_status & SPI_INT_STATUS_RX_OVERFLOW_M)
    {
        HAL_SPI_RXOverflow_IRQ(hspi);
        return;
    }

    if (interrupt_status & SPI_INT_STATUS_MODE_FAIL_M)
    {
        HAL_SPI_ModeFail_IRQ(hspi);
        return;
    }
}

#endif