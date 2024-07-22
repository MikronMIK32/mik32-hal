#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <stdbool.h>
#include <string.h>
#include "mcu32_memory_map.h"
#include "uart.h"
#include "gpio.h"


inline void __HAL_USART_Enable(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_UE_M;
}
inline void __HAL_USART_Disable(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_UE_M;
}

/**
 * @brief Разрешить прерывания по признаку ошибки бита четности
 */
inline void HAL_USART_PE_EnableInterrupt(UART_TypeDef* local)       
{
    local->CONTROL1 |= UART_CONTROL1_PEIE_M;
}
/**
 * @brief Запретить прерывания по признаку ошибки бита четности
 */
inline void HAL_USART_PE_DisableInterrupt(UART_TypeDef* local)      
{
    local->CONTROL1 &= ~UART_CONTROL1_PEIE_M;
}
/**
 * @brief Разрешить прерывания по признаку "регистр передатчика пуст"
 */
inline void HAL_USART_TXE_EnableInterrupt(UART_TypeDef* local)      
{
    local->CONTROL1 |= UART_CONTROL1_TXEIE_M;
}
/**
 * @brief Запретить прерывания по признаку "регистр передатчика пуст"
 */
inline void HAL_USART_TXE_DisableInterrupt(UART_TypeDef* local)     
{
    local->CONTROL1 &= ~UART_CONTROL1_TXEIE_M;
}
/**
 * @brief Разрешить прерывания по признаку "передача завершена"
 */
inline void HAL_USART_TXC_EnableInterrupt(UART_TypeDef* local)      
{
    local->CONTROL1 |= UART_CONTROL1_TCIE_M;
}
/* Запретить прерывания по признаку "передача завершена" */
inline void HAL_USART_TXC_DisableInterrupt(UART_TypeDef* local)     
{
    local->CONTROL1 &= ~UART_CONTROL1_TCIE_M;
}
/* Разрешить прерывания по признаку "регистр приемника не пуст" */
inline void HAL_USART_RXNE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_RXNEIE_M;
}
/* Запретить прерывания по признаку "регистр приемника не пуст" */
inline void HAL_USART_RXNE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_RXNEIE_M;
}
/* Разрешить прерывания по признаку "активность на линии RX не обнаружена в течение 8 битовых тактов" */
inline void HAL_USART_IDLE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_IDLEIE_M;
}
/* Запретить прерывания по признаку "активность на линии RX не обнаружена в течение 8 битовых тактов" */
inline void HAL_USART_IDLE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_IDLEIE_M;
}
/* Разрешить прерывания при обнаружении break-состояния на линии RX */
inline void HAL_USART_RX_Break_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL2 |= UART_CONTROL2_LBDIE_M;
}
/* Запретить прерывания при обнаружении break-состояния на линии RX */
inline void HAL_USART_RX_Break_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL2 &= ~UART_CONTROL2_LBDIE_M;
}
/* Разрешить прерывания при изменени уровня линии CTS */
inline void HAL_USART_CTS_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 |= UART_CONTROL3_CTSIE_M;
}
/* Запретить прерывания при изменени уровня линии CTS */
inline void HAL_USART_CTS_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 &= ~UART_CONTROL3_CTSIE_M;
}
/* Разрешить прерывания при обнаружении ошибки приема данных (FE, ORE, NF) */
inline void HAL_USART_RX_Error_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 |= UART_CONTROL3_EIE_M;
}
/* Запретить прерывания при обнаружении ошибки приема данных (FE, ORE, NF) */
inline void HAL_USART_RX_Error_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 &= ~UART_CONTROL3_EIE_M;
}

/*******************************************************************************
 * @brief Функция отправки 1 байта данных на модуль USART для последующей
 * передачи.
 * @param local указатель на структуру-дескриптор модуля USART
 * @param data 1 байт отправляемых данных
 * @return none
 */
inline void HAL_USART_WriteByte(UART_TypeDef* local, char data)
{
    local->TXDATA = data;
}

/*******************************************************************************
 * @brief Чтение принятых модулем USART данных
 * @param local указатель на структуру-дескриптор модуля USART
 * @return 1 байт принятых данных
 */
inline char HAL_USART_ReadByte(UART_TypeDef* local)
{
    return local->RXDATA;
}

/*******************************************************************************
 * @brief Очистка флага изменения состояния линии CTS модуля USART
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_CTS_ClearToggleFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_CTSIF_M;
}

/*******************************************************************************
 * @brief Очистка флага фиксации break-состояния на линии RX модуля USART
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_RX_ClearBreakFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_LBDF_M;
}

/*******************************************************************************
 * @brief Очистка флага TXС ("передача завершена") модуля USART
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_TXC_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_TC_M;
}

/*******************************************************************************
 * @brief Очистка флага TXE ("регистр передатчика пуст") модуля USART
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_TXE_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_TXE_M;
}

/*******************************************************************************
 * @brief Очистка флага RXNE ("регистр приемника не пуст") модуля USART
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_RXNE_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_RXNE_M;
}

/*******************************************************************************
 * @brief Очистка флага IDLE модуля USART.
 * Флаг IDLE устанавливается при отсутствии активности на линии RX в течение 8
 * битовых тактов при установленном флаге RXNE.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_IDLE_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_IDLE_M;
}

/*******************************************************************************
 * @brief Очистка флага ORE модуля USART.
 * Флаг ORE устанавливается при попытке перезаписи принятых по RX данных.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_ReceiveOverwrite_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_ORE_M;
}

/*******************************************************************************
 * @brief Очистка флага NF модуля USART.
 * Флаг NF устанавливается при обнаружении ложных переключений на линии RX.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_NF_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_NF_M;
}

/*******************************************************************************
 * @brief Очистка флага FE модуля USART.
 * Флаг FE устанавливается при обнаружении ошибок в стоп-бите (битах).
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_StopBitError_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_FE_M;
}

/*******************************************************************************
 * @brief Очистка флага PE модуля USART.
 * Флаг PE устанавливается при обнаружении ошибочного бита четности.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_ParityError_ClearFlag(UART_TypeDef* local)
{
    local->FLAGS |= UART_FLAGS_PE_M;
}


/*******************************************************************************
 * @brief Очистка всех флагов модуля USART, кроме модемных
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_ClearFlags(UART_TypeDef* local)
{
    local->FLAGS = 0xFFFFFFFF;
}

/*******************************************************************************
 * @brief Очистка флага изменения состояния линии DCD ("обнаружение несущей")
 * модуля USART.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_DCD_ClearToggleFlag(UART_TypeDef* local)
{
    local->MODEM |= UART_MODEM_DCDIF_M;
}

/*******************************************************************************
 * @brief Очистка флага изменения состояния линии RI ("звонок на телефонной
 * линии") модуля USART.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_RI_ClearToggleFlag(UART_TypeDef* local)
{
    local->MODEM |= UART_MODEM_RIIF_M;
}

/*******************************************************************************
 * @brief Очистка флага изменения состояния линии DSR ("источник данных готов")
 * модуля USART.
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_DSR_ClearToggleFlag(UART_TypeDef* local)
{
    local->MODEM |= UART_MODEM_DSRIF_M;
}

/*******************************************************************************
 * @brief Очитка модемных флагов модуля USART (флаги изменения DCD, RI, DSR)
 * @param local указатель на структуру-дескриптор модуля USART
 * @return none
 */
inline void HAL_USART_ClearModemFlags(UART_TypeDef* local)
{
    local->MODEM |= (UART_MODEM_DCDIF_M | UART_MODEM_RIIF_M | UART_MODEM_DSRIF_M);
}


typedef enum __Enable_or_Disable
{
    Enable = 1,
    Disable = 0
} HAL_USART_EnableDisable_enum;

typedef enum  __Frame_format
{
    Frame_8bit = 0b00,
    Frame_9bit = 0b01,
    Frame_7bit = 0b10
} HAL_USART_Frame_enum;

typedef enum 
{
    MSB_First = 1,
    LSB_First = 0
} HAL_USART_BitDirection_enum;

typedef enum
{
    StopBit_1 = 0,
    StopBit_2 = 1
} HAL_USART_StopBit_enum;

typedef enum
{
    Synchronous_Mode = 1,
    Asynchronous_Mode = 0
} HAL_USART_Mode_enum;

typedef enum
{
    XCK_Mode0 = 0b00,
    XCK_Mode1 = 0b01,
    XCK_Mode2 = 0b10,
    XCK_Mode3 = 0b11
} HAL_USART_XCK_Mode_enum;

typedef enum
{
    Duplex_Mode = 0,
    Semiduplex_Mode = 1
} HAL_USART_ChannelMode_enum;

typedef enum
{
    AlwaysEnable_mode = 0,
    Modem_mode = 1
} HAL_USART_RTS_mode_enum;

typedef struct 
{
    //control1
    /**
     * @brief Прерывание при обнаружении ошибки в бите четности 
     */
    HAL_USART_EnableDisable_enum peie;
    /**
     * @brief Прерывание по флагу "регистр передатчика пуст"
     */
    HAL_USART_EnableDisable_enum txeie;
    /**
     * @brief Прерывание по флагу "передача окончена"
     */
    HAL_USART_EnableDisable_enum tcie;
    /**
     * @brief Прерывание по флагу "регистр приемника не пуст"
     */
    HAL_USART_EnableDisable_enum rxneie;
    /**
     * @brief Прерывание при отсутствии входных транзакций в течение
     * 8 битовых тактов при установленном флаге RXNE
     */
    HAL_USART_EnableDisable_enum idleie;
    //control2
    /**
     * @brief Прерывание при обнаружении break-состояния на линии RX
     */
    HAL_USART_EnableDisable_enum lbdie;
    //control3
    /**
     * @brief Прерывание по изменению состояния линии CTS
     */
    HAL_USART_EnableDisable_enum ctsie;
    /**
     * @brief Прерывание при обнаружении ошибок приема данных (FE, ORE, NF)
     */
    HAL_USART_EnableDisable_enum eie;
} HAL_USART_Interrupt_TypeDef;

typedef struct
{
    /**
     * @brief RTS ("request to send, запрос на передачу", выход,
     * соединяется с CTS внешнего устройства) используется, чтобы
     * оповестить внешнее устройство о том, что данный микроконтроллер
     * готов принять байт данных. Активный уровень низкий. Сигнал RTS
     * устанавливается в "1" аппаратно во время приема стоп-бита,
     * когда модуль USART принял данные, но процессор еще не успел их
     * прочитать. После операции чтения данных из регистра RXDATA
     * сигнал RTS автоматически сбрасывается в "0".
     */
    HAL_USART_EnableDisable_enum rts;
    /**
     * @brief Сигнал CTS ("clear to send, очищен для записи", вход,
     * соединяется с RTS внешнего устройства) оповещает
     * микроконтроллер, что устройство, принимающее данные по линиям
     * USART готово принять данные. Активный уровень низкий. Флаг TXC
     * ("передача завершена") модуля USART не будет установлен до тех
     * пор, пока на линии CTS лог.1.
     */
    HAL_USART_EnableDisable_enum cts;
    /**
     * @brief Сигнал RI ("ring indicator", вход) используется для
     * оповещения данного устройства о том, что внешнее устройство
     * имеет информацию, которую оно готов передать.
     */
    HAL_USART_EnableDisable_enum ri;
    /**
     * @brief Сигнал DSR (вход, соединяется с DTR внешнего устройства)
     * используется для того, чтобы определить, готово ли внешнее
     * устройство к приему данных. Обработка состояния линии DSR
     * должна выполняться программно, при изменении состояния линии
     * устанавливается соответствующий флаг.
     */
    HAL_USART_EnableDisable_enum dsr;
    /**
     * @brief Сигнал DTR (выход, соединяется с DSR внешнего устройства)
     * сообщает внешнему устройству, что микроконтроллер готов
     * принять данные. Линия DTR управляется программно.
     */
    HAL_USART_EnableDisable_enum dtr;
    /**
     * @brief Сигнал DDIS ("driver disable", выход). После чтения байта
     * принятых данных из регистра RXDATA, линия DDIS на 1 такт модуля
     * USART устанавливается в лог.1, затем сбрасывается. Сигнал DDIS
     * может быть использован для оповещения внешнего устройства, что
     * отправленные по USART данные прочитаны.
     */
    HAL_USART_EnableDisable_enum ddis;
    /**
     * @brief Сигнал DCD ("data carrier detect", вход) используется для
     * того, чтобы определять, подключено ли внешнее устройство к данному.
     */
    HAL_USART_EnableDisable_enum dcd;
} HAL_USART_Modem_TypeDef;


typedef struct __SettingTypeDef
{
    UART_TypeDef* Instance;
    /* Settings */
    HAL_USART_EnableDisable_enum transmitting;
    HAL_USART_EnableDisable_enum receiving;
    HAL_USART_Frame_enum frame;
    HAL_USART_EnableDisable_enum parity_bit;
    HAL_USART_EnableDisable_enum parity_bit_inversion;
    HAL_USART_BitDirection_enum bit_direction;
    HAL_USART_EnableDisable_enum data_inversion;
    HAL_USART_EnableDisable_enum tx_inversion;
    HAL_USART_EnableDisable_enum rx_inversion;
    HAL_USART_EnableDisable_enum swap;
    HAL_USART_EnableDisable_enum lbm;
    HAL_USART_StopBit_enum stop_bit;
    HAL_USART_Mode_enum mode;
    HAL_USART_XCK_Mode_enum xck_mode;
    HAL_USART_EnableDisable_enum last_byte_clock;
    HAL_USART_EnableDisable_enum overwrite;
    HAL_USART_RTS_mode_enum rts_mode;
    HAL_USART_EnableDisable_enum dma_tx_request;
    HAL_USART_EnableDisable_enum dma_rx_request;
    HAL_USART_ChannelMode_enum channel_mode;
    HAL_USART_EnableDisable_enum tx_break_mode;
    /* Interrupts */
    HAL_USART_Interrupt_TypeDef Interrupt;
    /* Modem signals */
    HAL_USART_Modem_TypeDef Modem;
    /* Baudrate */
    uint32_t baudrate;

} UART_Setting_TypeDef;


/* Initialization */
void HAL_USART_MspInit(UART_Setting_TypeDef* setting);
HAL_StatusTypeDef HAL_USART_Init(UART_Setting_TypeDef* setting);
/* Flags */
bool HAL_USART_Read_ReceiveEnableAck(UART_TypeDef* local);
bool HAL_USART_Read_TransmitEnableAck(UART_TypeDef* local);
bool HAL_USART_CTS_ReadLevel(UART_TypeDef* local);
bool HAL_USART_CTS_ReadToggleFlag(UART_TypeDef* local);
bool HAL_USART_RX_ReadBreakFlag(UART_TypeDef* local);
bool HAL_USART_TXC_ReadFlag(UART_TypeDef* local);
bool HAL_USART_TXE_ReadFlag(UART_TypeDef* local);
bool HAL_USART_RXNE_ReadFlag(UART_TypeDef* local);
bool HAL_USART_IDLE_ReadFlag(UART_TypeDef* local);
bool HAL_USART_ReceiveOverwrite_ReadFlag(UART_TypeDef* local);
bool HAL_USART_NF_ReadFlag(UART_TypeDef* local);
bool HAL_USART_StopBitError_ReadFlag(UART_TypeDef* local);
bool HAL_USART_ParityError_ReadFlag(UART_TypeDef* local);

void HAL_USART_Transmit(UART_TypeDef* local, char data);
void HAL_USART_Write(UART_TypeDef* local, char* buffer, uint32_t len);
void HAL_USART_Print(UART_TypeDef* local, char* str);
char HAL_USART_Receive(UART_TypeDef* local);
void HAL_USART_Read(UART_TypeDef* local, char* buffer, uint32_t len);
/* Xprintf */
void xputc(char c);
/* Modem signals */
void HAL_USART_Set_DTR(UART_TypeDef* local, HAL_USART_EnableDisable_enum en);
bool HAL_USART_DCD_Status(UART_TypeDef* local);
bool HAL_USART_DCD_ReadToggleFlag(UART_TypeDef* local);
bool HAL_USART_RI_Status(UART_TypeDef* local);
bool HAL_USART_RI_ReadToggleFlag(UART_TypeDef* local);
bool HAL_USART_DSR_Status(UART_TypeDef* local);
bool HAL_USART_DSR_ReadToggleFlag(UART_TypeDef* local);


// void HAL_USART_Printf(UART_TypeDef* local, char* str, ...);
// void HAL_USART_PrintBefore(UART_TypeDef* local, char* str);
// void HAL_USART_Print_c(UART_TypeDef* local, char value);
// void HAL_USART_Print_s(UART_TypeDef* local, char* str);
