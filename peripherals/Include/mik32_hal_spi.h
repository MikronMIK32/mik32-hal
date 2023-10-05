#ifndef MIK32_HAL_SPI
#define MIK32_HAL_SPI


#include "stddef.h"
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "power_manager.h"
#include "spi.h"
#include "mik32_hal_def.h"
#include "mcu32_memory_map.h"
// #include "inttypes.h"
// #include "stdbool.h"



/* Title: Макросы */

#define SPI_TIMEOUT_DEFAULT 1000000

/*
 * Defines: Выбор ведомых устройств 
 *
 * SPI_CS_NONE - Ведомое устройство не выбрано;
 * SPI_CS_0    - Ведомое устройство 1;
 * SPI_CS_1    - Ведомое устройство 2;
 * SPI_CS_2    - Ведомое устройство 3;
 * SPI_CS_3    - Ведомое устройство 4.
 *
 */
/* SPI_CS - Выбор ведомых устройств */
#define SPI_CS_NONE   0b1111     /* Ведомое устройство не выбрано */
#define SPI_CS_0      0b1110     /* Ведомое устройство 1 */
#define SPI_CS_1      0b1101     /* Ведомое устройство 2 */
#define SPI_CS_2      0b1011     /* Ведомое устройство 3 */
#define SPI_CS_3      0b0111     /* Ведомое устройство 4 */

/*
 * Defines: Делитель частоты
 *
 * Коэффициент деления частоты spi_ref_clk
 *
 * SPI_BAUDRATE_DIV4   - Деление на 4;
 * SPI_BAUDRATE_DIV8   - Деление на 8;
 * SPI_BAUDRATE_DIV16  - Деление на 16;
 * SPI_BAUDRATE_DIV32  - Деление на 32;
 * SPI_BAUDRATE_DIV64  - Деление на 64;
 * SPI_BAUDRATE_DIV128 - Деление на 128;
 * SPI_BAUDRATE_DIV256 - Деление на 256.
 *
 */
/* SPI_BaudRateDiv - Коэффициент деления частоты spi_ref_clk */
#define SPI_BAUDRATE_DIV4       0b001   /* Деление на 4 */
#define SPI_BAUDRATE_DIV8       0b010   /* Деление на 8 */
#define SPI_BAUDRATE_DIV16      0b011   /* Деление на 16 */
#define SPI_BAUDRATE_DIV32      0b100   /* Деление на 32 */
#define SPI_BAUDRATE_DIV64      0b101   /* Деление на 64 */
#define SPI_BAUDRATE_DIV128     0b110   /* Деление на 128 */
#define SPI_BAUDRATE_DIV256     0b111   /* Деление на 256 */

/*
 * Defines: Ручной режим
 *
 * Режим управления сигналом выбора ведомого CS
 *
 * SPI_MANUALCS_OFF - Автоматический режим;
 * SPI_MANUALCS_ON  - Ручной режим.
 *
 */
/* SPI_ManualCS - Режим управления сигналом выбора ведомого CS */
#define SPI_MANUALCS_OFF      0   /* Автоматический режим */
#define SPI_MANUALCS_ON       1   /* Ручной режим */

/*
 * Defines: Фаза тактового сигнала
 *
 * Настройки фазы тактового сигнала
 *
 * SPI_PHASE_OFF - Тактовая частота SPI активна вне слова;
 * SPI_PHASE_ON  - Тактовая частота SPI неактивна вне слова.
 *
 */
/* SPI_CLKPhase - Настройки фазы тактового сигнала */
#define SPI_PHASE_OFF   0  /* Тактовая частота SPI активна   вне слова */
#define SPI_PHASE_ON    1  /* Тактовая частота SPI неактивна вне слова */

/*
 * Defines: Полярность тактового сигнала
 *
 * Настройки полярности тактового сигнала вне слова
 *
 * SPI_POLARITY_LOW  - Тактовый сигнал вне слова удерживается на низком  уровне;
 * SPI_POLARITY_HIGH - Тактовый сигнал вне слова удерживается на высоком уровне.
 *
 */
/* SPI_CLKPolarity - Настройки полярности тактового сигнала вне слова */
#define SPI_POLARITY_LOW    0   /* Тактовый сигнал вне слова удерживается на низком  уровне */
#define SPI_POLARITY_HIGH   1   /* Тактовый сигнал вне слова удерживается на высоком уровне */

/*
 * Defines: Внешний декодер
 *
 * SPI_DECODER_NONE - Внешний декодер не используется. Выбор только 1 из 4-х ведомых устройств;
 * SPI_DECODER_USE  - Используется внешний декодер.
 *
 */
/* SPI_Decoder - Использование внешнего декодера */
#define SPI_DECODER_NONE     0   /* Внешний декодер не используется. Выбор только 1 из 4 ведомых устройств */
#define SPI_DECODER_USE      1   /* Используется внешний декодер */

/*
 * Defines: Длина передаваемой посылки
 *
 * Определяет количество байт, при пересылке которых CS не будет менять свое состояние.
 *
 * SPI_DATASIZE_8BITS  - Длина передаваемой посылки 8 бит;
 * SPI_DATASIZE_16BITS - Длина передаваемой посылки 16 бит;
 * SPI_DATASIZE_24BITS - Длина передаваемой посылки 24 бит;
 * SPI_DATASIZE_32BITS - Длина передаваемой посылки 32 бит.
 *
 */
/* SPI_DataSize - Длина передаваемой посылки */
#define SPI_DATASIZE_8BITS      0   /* Длина передаваемой посылки - 8 бит */
#define SPI_DATASIZE_16BITS     1   /* Длина передаваемой посылки - 16 бит */
#define SPI_DATASIZE_24BITS     2   /* Длина передаваемой посылки - 24 бит */
#define SPI_DATASIZE_32BITS     3   /* Длина передаваемой посылки - 32 бит */

/* SPI_Error - Ошибки SPI */
#define SPI_ERROR_RXOVR_OFF         (uint8_t)0
#define SPI_ERROR_RXOVR_ON          (uint8_t)1
#define SPI_ERROR_MODEFAIL_OFF      (uint8_t)0
#define SPI_ERROR_MODEFAIL_ON       (uint8_t)1

/* Значения по умолчанию */
#define SPI_THRESHOLD_DEFAULT   1   /* Значение Threshold_of_TX_FIFO по умолчанию*/

/* Прерывания */
#define TX_FIFO_UNDERFLOW   6   /* Регистр TX FIFO опустошен */
#define RX_FIFO_FULL        5   /* Регистр RX_FIFO заполнен */
#define RX_FIFO_NOT_EMPTY   4   /* Регистр RX_FIFO не пустой */
#define TX_FIFO_FULL        3   /* Регистр TX_FIFO заполнен */
#define TX_FIFO_NOT_FULL    2   /* Регистр TX_FIFO не заполнен */
#define MODE_FAIL           1   /* Напряжение на выводе n_ss_in не соответствую режиму работы SPI */
#define RX_OVERFLOW         0   /* Прерывание при переполнении RX_FIFO, значение сбрасывается при чтении */

#define IXR_TXUF        TX_FIFO_UNDERFLOW   /* Регистр TX FIFO опустошен */
#define IXR_RXFULL      RX_FIFO_FULL        /* Регистр RX_FIFO заполнен */
#define IXR_RXNEMPTY    RX_FIFO_NOT_EMPTY   /* Регистр RX_FIFO не пустой */
#define IXR_TXFULL      TX_FIFO_FULL        /* Регистр TX_FIFO заполнен */
#define IXR_TXOW        TX_FIFO_NOT_FULL    /* Регистр TX_FIFO не заполнен */
#define IXR_MODF        MODE_FAIL           /* Напряжение на выводе n_ss_in не соответствую режиму работы SPI */
#define IXR_RXOVR       RX_OVERFLOW         /* Прерывание при переполнении RX_FIFO, значение сбрасывается при чтении */

#define SPI_IRQ_DISABLE        0
#define SPI_IRQ_ENABLE         1

/* Title: Перечисления */

/*
 * Enum: HAL_SPI_ModeTypeDef
 * Выбор режима ведущий/ведомый
 *
 * HAL_SPI_MODE_SLAVE - Режим ведомого.
 * HAL_SPI_MODE_MASTER - Режим ведущего.
 *
 */
typedef enum 
{
    HAL_SPI_MODE_SLAVE = 0,     /* Режим ведомого */
    HAL_SPI_MODE_MASTER = 1,    /* Режим ведущего */

} HAL_SPI_ModeTypeDef;

typedef enum
{
  HAL_SPI_STATE_READY,    /* Готов к передаче */
  HAL_SPI_STATE_BUSY,    /* Идет передача */
  HAL_SPI_STATE_END,    /* Передача завершена */
  HAL_SPI_STATE_ERROR    /* Ошибка при передаче */
} HAL_SPI_StateTypeDef;

/* Title: Структуры */

/*
 * Struct: HAL_SPI_ErrorTypeDef
 * Ошибки при работе SPI
 * 
 */
typedef struct 
{
    /*
    * Variable: RXOVR
    * Переполнение RX_FIFO
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_ERROR_RXOVR_OFF>;
    * - <SPI_ERROR_RXOVR_ON>. 
    *
    */
    uint8_t RXOVR;

    /*
    * Variable: ModeFail
    * Напряжение на выводе n_ss_in не соответствую режиму работы SPI
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_ERROR_ModeFail_OFF>;
    * - <SPI_ERROR_MODEFAIL_ON>. 
    *
    */
    uint8_t ModeFail;

} HAL_SPI_ErrorTypeDef;

/*
 * Struct: SPI_InitTypeDef
 * Настройки инициализации SPI
 * 
 */
typedef struct
{
    /*
    * Variable: SPI_Mode
    * Режим SPI (ведомый/ведущий)
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <HAL_SPI_MODE_SLAVE>;
    * - <HAL_SPI_MODE_MASTER>. 
    *
    */
    HAL_SPI_ModeTypeDef SPI_Mode;   /* Режим SPI */
    
    /*
    * Variable: BaudRateDiv
    * Коэффициент деления частоты
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_BAUDRATE_DIV4>
    * - <SPI_BAUDRATE_DIV8>
    * - <SPI_BAUDRATE_DIV16>
    * - <SPI_BAUDRATE_DIV32>
    * - <SPI_BAUDRATE_DIV64>
    * - <SPI_BAUDRATE_DIV128>
    * - <SPI_BAUDRATE_DIV256>
    *
    */
    uint8_t BaudRateDiv;            /* Коэффициент деления частоты */ 

    /*
    * Variable: ManualCS
    * Режим управления сигналом выбора ведомого CS
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_MANUALCS_OFF>;
    * - <SPI_MANUALCS_ON>.
    *
    */
    uint8_t ManualCS;               /* Режим управления сигналом выбора ведомого - CS */

    /*
    * Variable: CLKPhase
    * Настройка фазы тактового сигнала
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_PHASE_OFF>;
    * - <SPI_PHASE_ON>.
    *
    */
    uint8_t CLKPhase;               /* Настройка фазы тактового сигнала */

    /*
    * Variable: CLKPolarity
    * Настройка полярности тактового сигнала вне слова
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_POLARITY_LOW>;
    * - <SPI_POLARITY_HIGH>.
    *
    */
    uint8_t CLKPolarity;            /* Настройка полярности тактового сигнала вне слова */

    /*
    * Variable: Decoder
    * Использование внешнего декодера
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_DECODER_NONE>;
    * - <SPI_DECODER_USE>.
    *
    */
    uint8_t Decoder;                /* Использование внешнего декодера */

    // /*
    // * Variable: DataSize
    // * Длина передаваемой посылки
    // * 
    // * Этот параметр должен быть одним из значений:
    // * 
    // * - <SPI_DATASIZE_8BITS>;
    // * - <SPI_DATASIZE_16BITS>;
    // * - <SPI_DATASIZE_24BITS>;
    // * - <SPI_DATASIZE_32BITS>;
    // *
    // */
    // uint8_t DataSize;               /* Длина передаваемой посылки */

    uint8_t ThresholdTX;            /* Уровень при котором регистр TX считается незаполненным и формируется прерывание */

    /*
    * Variable: ChipSelect
    * Выбранное ведомое устройство
    * 
    * Этот параметр должен быть одним из значений:
    * 
    * - <SPI_CS_NONE>;
    * - <SPI_CS_0>;
    * - <SPI_CS_1>;
    * - <SPI_CS_2>;
    * - <SPI_CS_3>.
    *
    */
    uint8_t ChipSelect;


} SPI_InitTypeDef;


/*
 * Struct: SPI_HandleTypeDef
 * Настройки SPI
 * 
 */
typedef struct
{
   /*
    * Variable: Instance
    * Базоый адрес регистров SPI
    *
    */
    SPI_TypeDef *Instance;

    /*
    * Variable: Init
    * Параметры SPI
    *
    */
    SPI_InitTypeDef Init;

    /*
    * Variable: Error
    * Ошибки при работе SPI
    *
    */
    HAL_SPI_ErrorTypeDef Error;

    HAL_SPI_StateTypeDef State;


    uint32_t TransferSize;
    uint8_t *pTxBuffPtr;
    uint32_t TxCount;
    uint8_t *pRxBuffPtr;
    uint32_t RxCount;

    
} SPI_HandleTypeDef;


/* Title: Функции */

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi);

/*
 * Function: HAL_SPI_Enable
 * Включить модуль SPI
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
void HAL_SPI_Enable(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_Disable
 * Выключить модуль SPI
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
void HAL_SPI_Disable(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_IntEnable
 * Разрешить прерывания в соответствии с маской int_en 
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * IntEnMask - Маска прерываний
 *
 * Returns:
 * void
 */
static inline __attribute__((always_inline)) void HAL_SPI_InterruptEnable(SPI_HandleTypeDef *hspi, uint32_t IntEnMask)
{
    hspi->Instance->INT_ENABLE |= IntEnMask;
}

/*
 * Function: HAL_SPI_IntDisable
 * Запретить прерывания в соответствии с маской int_dis
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * IntDisMask - Маска прерываний
 *
 * Returns:
 * void
 */
static inline __attribute__((always_inline)) void HAL_SPI_InterruptDisable(SPI_HandleTypeDef *hspi, uint32_t IntDisMask)
{
    hspi->Instance->INT_DISABLE |= IntDisMask;
}

/*
 * Function: HAL_SPI_IntEnable
 * Получить состояние прерывания
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * Interrupt - Название прерывания 
 *
 * Returns:
 * (uint8_t ) - Состояние прерывания.
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

/*
 * Function: HAL_SPI_SetDelayBTWN
 * Задать задержку BTWN
 * 
 * Задержка в периодах опорного тактового сигнала или внешнего тактового сигнала ext_clk 
 * между снятием сигнала выбора одного ведомого устройства и установкой сигнала выбора другого ведомого устройства.
 * 
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * btwn - Задержка BTWN
 *
 * Returns:
 * void
 */
void HAL_SPI_SetDelayBTWN(SPI_HandleTypeDef *hspi, uint8_t btwn);

/*
 * Function: HAL_SPI_SetDelayAFTER
 * Задать задержку AFTER
 * 
 * Задержка в периодах опорного тактового сигнала или внешнего тактового сигнала ext_clk между последним битом текущего
 * слова и первым битом следующего слова.
 * 
 * Этот параметр может быть числом в пределах от 0 до 255.
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * after - Задержка AFTER
 *
 * Returns:
 * void
 */
void HAL_SPI_SetDelayAFTER(SPI_HandleTypeDef *hspi, uint8_t after);

/*
 * Function: HAL_SPI_SetDelayINIT
 * Задать задержку INIT
 * 
 * Дополнительная задержка в периодах опорного тактового сигнала или внешнего тактового сигнала ext_clk между установкой 
 * сигнала n_ss_out в «0» и передачей первого бита.
 * 
 * Этот параметр может быть числом в пределах от 0 до 255.
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * init - Задержка INIT
 *
 * Returns:
 * void
 */
void HAL_SPI_SetDelayINIT(SPI_HandleTypeDef *hspi, uint8_t init);

/*
 * Function: HAL_SPI_SetSlaveIdleCounter
 * Задать задержку перед передачей
 * 
 * Модуль SPI в режиме ведомого устройства начинает передачу только когда тактовый сигнал sclk_in (внешнего ведущего устройства)
 * не изменяется в течение количества периодов опорного тактового сигнала SPI заданного в этом поле или когда модуль SPI не активен.
 * 
 * Этот параметр может быть числом в пределах от 0 до 255.
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * slave_idle_counter - Задержка в тактах сигнала sclk_in
 *
 * Returns:
 * void
 */
void HAL_SPI_SetSlaveIdleCounter(SPI_HandleTypeDef *hspi, uint8_t slave_idle_counter);

/*
 * Function: HAL_SPI_SetThresholdTX
 * Задать ThresholdTX
 * 
 * Задает уровень, при котором TX_FIFO считается не заполненным и формируется прерывание.
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 * threshold - Уровень, при котором TX_FIFO считается не заполненным.
 *
 * Returns:
 * void
 */
void HAL_SPI_SetThresholdTX(SPI_HandleTypeDef *hspi, uint32_t threshold);

/*
 * Function: HAL_SPI_ReadModuleID
 * Получить идентификационный номер модуля
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * (uint32_t ) - Идентификационный номер модуля
 */
uint32_t HAL_SPI_ReadModuleID(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_Init
 * Инициализировать Crypto в соответствии с настройками <SPI_HandleTypeDef> *hspi
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_ClearTXFIFO
 * Очистить TX_FIFO
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
void HAL_SPI_ClearTXFIFO(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_ClearRXFIFO
 * Очистить RX_FIFO
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
void HAL_SPI_ClearRXFIFO(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_ClearError
 * Сбросить состояния ошибок, очистить буферы
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
void HAL_SPI_ClearError(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_WaitTxNotFull
 * Ожидать пока регистр TX_FIFO заполнен меньше чем THRESHOLD;
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPI_WaitTxNotFull(SPI_HandleTypeDef *hspi, uint32_t Timeout)
{
    uint32_t status = 0;
    while (Timeout-- != 0)
    {
        status = hspi->Instance->INT_STATUS;

        if ((status & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M) != 0)
        {
            return HAL_OK;
        }
    }

    if((status & (SPI_INT_STATUS_RX_OVERFLOW_M | SPI_INT_STATUS_MODE_FAIL_M)) || (!hspi->Instance->ENABLE))
    {
        
        if(status & SPI_INT_STATUS_RX_OVERFLOW_M)
        {
            hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
            #ifdef MIK32_SPI_DEBUG
            xprintf("TX_OVR\n");
            #endif
        }
        else
        {
            hspi->Error.ModeFail = SPI_ERROR_MODEFAIL_ON;
            #ifdef MIK32_SPI_DEBUG
            xprintf("TX_FAIL\n");
            #endif
        }

        return HAL_ERROR;
    }

    return HAL_TIMEOUT;
    
}

/*
 * Function: HAL_SPI_WaitRxNotEmpty
 * Ожидать пока в RX_FIFO есть хотя бы один байт
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi
 *
 * Returns:
 * void
 */
static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPI_WaitRxNotEmpty(SPI_HandleTypeDef *hspi, uint32_t Timeout)
{
    uint32_t status = 0;
    while (Timeout-- != 0)
    {
        status = hspi->Instance->INT_STATUS;

        if ((status & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M) != 0)
        {
            return HAL_OK;
        }

    }

    if((status & (SPI_INT_STATUS_RX_OVERFLOW_M | SPI_INT_STATUS_MODE_FAIL_M)) || (!hspi->Instance->ENABLE))
    {
        
        if(status & SPI_INT_STATUS_RX_OVERFLOW_M)
        {
            hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
            #ifdef MIK32_SPI_DEBUG
            xprintf("TX_OVR\n");
            #endif
        }
        else
        {
            hspi->Error.ModeFail = SPI_ERROR_MODEFAIL_ON;
            #ifdef MIK32_SPI_DEBUG
            xprintf("TX_FAIL\n");
            #endif
        }

        return HAL_ERROR;
    }

    return HAL_TIMEOUT;
}

/*
 * Function: HAL_SPI_CS_Enable
 * Перевести CS_M в активное состояние
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi;
 * CS_M - Выбор ведомых устройств.
 * Этот параметр должен быть одним из значений:
 * 
 * - <SPI_CS_NONE>;
 * - <SPI_CS_0>;
 * - <SPI_CS_1>;
 * - <SPI_CS_2>;
 * - <SPI_CS_3>.
 *
 * Returns:
 * void
 */
void HAL_SPI_CS_Enable(SPI_HandleTypeDef *hspi, uint32_t CS_M);

/*
 * Function: HAL_SPI_CS_Disable
 * Перевести активный CS в неактивное состояние
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi;
 * Этот параметр должен быть одним из значений:
 * 
 * - <SPI_CS_NONE>;
 * - <SPI_CS_0>;
 * - <SPI_CS_1>;
 * - <SPI_CS_2>;
 * - <SPI_CS_3>.
 * 
 * Returns:
 * void
 */
void HAL_SPI_CS_Disable(SPI_HandleTypeDef *hspi);

/*
 * Function: HAL_SPI_Exchange
 * Запустить обмен данными
 *
 * Parameters:
 * hspi - Указатель на структуру с настройками spi;
 * TransmitBytes - Массив данных для отправки;
 * ReceiveBytes - Массив данных для приема;
 * Size - количество байт данных
 * 
 * Returns:
 * void
 */
HAL_StatusTypeDef HAL_SPI_Exchange(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_SPI_Exchange_IT(SPI_HandleTypeDef *hspi, uint8_t TransmitBytes[], uint8_t ReceiveBytes[], uint32_t Size);


static inline __attribute__((always_inline)) void HAL_SPI_RXOverflow_IRQ(SPI_HandleTypeDef *hspi)
{
    hspi->State = HAL_SPI_STATE_ERROR;
    hspi->Error.RXOVR = SPI_ERROR_RXOVR_ON;
    HAL_SPI_InterruptDisable(hspi,   SPI_INT_STATUS_RX_OVERFLOW_M       | 
                                SPI_INT_STATUS_MODE_FAIL_M         | 
                                SPI_INT_STATUS_TX_FIFO_NOT_FULL_M  | 
                                SPI_INT_STATUS_TX_FIFO_FULL_M      | 
                                SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M | 
                                SPI_INT_STATUS_RX_FIFO_FULL_M      | 
                                SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M
                                );
}

static inline __attribute__((always_inline)) void HAL_SPI_ModeFail_IRQ(SPI_HandleTypeDef *hspi)
{
    hspi->State = HAL_SPI_STATE_ERROR;
    hspi->Error.ModeFail = SPI_ERROR_MODEFAIL_ON;
    HAL_SPI_InterruptDisable(hspi,   SPI_INT_STATUS_RX_OVERFLOW_M       | 
                                SPI_INT_STATUS_MODE_FAIL_M         | 
                                SPI_INT_STATUS_TX_FIFO_NOT_FULL_M  | 
                                SPI_INT_STATUS_TX_FIFO_FULL_M      | 
                                SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M | 
                                SPI_INT_STATUS_RX_FIFO_FULL_M      | 
                                SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M
                                );
}

static inline __attribute__((always_inline)) void HAL_SPI_TXFifoNotFull_IRQ(SPI_HandleTypeDef *hspi)
{
    if (hspi->TxCount < hspi->TransferSize)
    {
        
        hspi->State = HAL_SPI_STATE_BUSY;

        for (int i = 0; i < hspi->Init.ThresholdTX && (hspi->TxCount < hspi->TransferSize); i++)
        {
            hspi->Instance->TXDATA = *((uint8_t *)hspi->pTxBuffPtr);
            hspi->pTxBuffPtr++;
            hspi->TxCount++;
        }
        
        HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_TX_FIFO_NOT_FULL_M);
        HAL_SPI_InterruptEnable(hspi, SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M);

    }

}

static inline __attribute__((always_inline)) void HAL_SPI_TXFifoFull_IRQ(SPI_HandleTypeDef *hspi)
{
    /* code */
}

static inline __attribute__((always_inline)) void HAL_SPI_RXFifoNotEmpty_IRQ(SPI_HandleTypeDef *hspi)
{
    if (hspi->RxCount < hspi->TransferSize)
    {
        hspi->State = HAL_SPI_STATE_BUSY;

        *((uint8_t *)hspi->pRxBuffPtr) = (uint8_t)hspi->Instance->RXDATA;
        hspi->pRxBuffPtr++;
        hspi->RxCount++;

        // for (int i = 0; i < hspi->Init.ThresholdTX && (hspi->RxCount < hspi->TransferSize); i++)
        // {
        //     *((uint8_t *)hspi->pRxBuffPtr) = (uint8_t)hspi->Instance->RXDATA;
        //     hspi->pRxBuffPtr++;
        //     hspi->RxCount++;
        //     // for (volatile int i = 0; i < 10; i++);
        // }

        if (hspi->RxCount % hspi->Init.ThresholdTX == 0)
        {
            HAL_SPI_InterruptDisable(hspi, SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M);
            HAL_SPI_InterruptEnable(hspi, SPI_INT_STATUS_TX_FIFO_NOT_FULL_M);
        }
        

    }


    if ((hspi->RxCount >= hspi->TransferSize) && (hspi->TxCount >= hspi->TransferSize))
    {
        HAL_SPI_InterruptDisable(hspi,   SPI_INT_STATUS_RX_OVERFLOW_M       | 
                                        SPI_INT_STATUS_MODE_FAIL_M         | 
                                        SPI_INT_STATUS_TX_FIFO_NOT_FULL_M  | 
                                        SPI_INT_STATUS_TX_FIFO_FULL_M      | 
                                        SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M | 
                                        SPI_INT_STATUS_RX_FIFO_FULL_M      | 
                                        SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M
                                        );
        
        /* Конец передачи. Не выключать SPI в ручном режиме управления CS */
        if(hspi->Init.ManualCS != SPI_MANUALCS_ON)
        {
            HAL_SPI_Disable(hspi);
        }
        
        

        hspi->State = HAL_SPI_STATE_END;

    }
}

static inline __attribute__((always_inline)) void HAL_SPI_RXFifoFull_IRQ(SPI_HandleTypeDef *hspi)
{
    /* code */
}

static inline __attribute__((always_inline)) void HAL_SPI_TXFifoUnderflow_IRQ(SPI_HandleTypeDef *hspi)
{
    /* code */
}

static inline __attribute__((always_inline)) void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
    uint32_t interrupt_status = hspi->Instance->INT_STATUS & hspi->Instance->INT_MASK;

    if (interrupt_status & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)
    {
        HAL_SPI_TXFifoNotFull_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_TX_FIFO_FULL_M)
    {
        HAL_SPI_TXFifoFull_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M)
    {
        HAL_SPI_RXFifoNotEmpty_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_RX_FIFO_FULL_M)
    {
        HAL_SPI_RXFifoFull_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M)
    {
        HAL_SPI_TXFifoUnderflow_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_RX_OVERFLOW_M)
    {
        HAL_SPI_RXOverflow_IRQ(hspi);
    }

    if (interrupt_status & SPI_INT_STATUS_MODE_FAIL_M)
    {
        HAL_SPI_ModeFail_IRQ(hspi);
    }

}


#endif