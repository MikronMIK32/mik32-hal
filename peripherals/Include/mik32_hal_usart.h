#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <stdbool.h>
#include <string.h>
#include "mcu32_memory_map.h"
#include "uart.h"
#include "gpio.h"


inline void __HAL_USART_Enable(UART_TypeDef* local)     { local->CONTROL1 |= UART_CONTROL1_UE_M; }
inline void __HAL_USART_Disable(UART_TypeDef* local)    { local->CONTROL1 &= ~UART_CONTROL1_UE_M; }
inline void __HAL_USART_TX_Enable(UART_TypeDef* local)  { local->CONTROL1 |= UART_CONTROL1_TE_M; }
inline void __HAL_USART_TX_Disable(UART_TypeDef* local) { local->CONTROL1 &= ~UART_CONTROL1_RE_M; }
inline void __HAL_USART_RX_Enable(UART_TypeDef* local)  { local->CONTROL1 |= UART_CONTROL1_RE_M; }
inline void __HAL_USART_RX_Disable(UART_TypeDef* local) { local->CONTROL1 &= ~UART_CONTROL1_RE_M; }

inline void HAL_USART_PE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_PEIE_M;
}
inline void HAL_USART_PE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_PEIE_M;
}
inline void HAL_USART_TXE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_TXEIE_M;
}
inline void HAL_USART_TXE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_TXEIE_M;
}
inline void HAL_USART_TXC_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_TCIE_M;
}
inline void HAL_USART_TXC_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_TCIE_M;
}
inline void HAL_USART_RXNE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_RXNEIE_M;
}
inline void HAL_USART_RXNE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_RXNEIE_M;
}
inline void HAL_USART_IDLE_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 |= UART_CONTROL1_IDLEIE_M;
}
inline void HAL_USART_IDLE_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL1 &= ~UART_CONTROL1_IDLEIE_M;
}
inline void HAL_USART_RX_Break_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL2 |= UART_CONTROL2_LBDIE_M;
}
inline void HAL_USART_RX_Break_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL2 &= ~UART_CONTROL2_LBDIE_M;
}
inline void HAL_USART_CTS_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 |= UART_CONTROL3_CTSIE_M;
}
inline void HAL_USART_CTS_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 &= ~UART_CONTROL3_CTSIE_M;
}
inline void HAL_USART_RX_Error_EnableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 |= UART_CONTROL3_EIE_M;
}
inline void HAL_USART_RX_Error_DisableInterrupt(UART_TypeDef* local)
{
    local->CONTROL3 &= ~UART_CONTROL3_EIE_M;
}


typedef enum 
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
    Enable = 1,
    Disable = 0
} HAL_USART_EnableDisable_enum;

typedef enum
{
    Duplex_Mode = 0,
    Semiduplex_Mode = 1
} HAL_USART_ChannelMode_enum;

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
    HAL_USART_EnableDisable_enum rts;
    HAL_USART_EnableDisable_enum cts;
    HAL_USART_EnableDisable_enum ri;
    HAL_USART_EnableDisable_enum dsr;
    HAL_USART_EnableDisable_enum dtr;
    HAL_USART_EnableDisable_enum ddis;
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
    HAL_USART_EnableDisable_enum cts_processing;
    HAL_USART_EnableDisable_enum rts_processing;
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
void HAL_USART_CTS_ClearToggleFlag(UART_TypeDef* local);
bool HAL_USART_CTS_ReadToggleFlag(UART_TypeDef* local);
void HAL_USART_RX_ClearBreakFlag(UART_TypeDef* local);
bool HAL_USART_RX_ReadBreakFlag(UART_TypeDef* local);
void HAL_USART_TXC_ClearFlag(UART_TypeDef* local);
bool HAL_USART_TXC_ReadFlag(UART_TypeDef* local);
void HAL_USART_TXE_ClearFlag(UART_TypeDef* local);
bool HAL_USART_TXE_ReadFlag(UART_TypeDef* local);
void HAL_USART_RXNE_ClearFlag(UART_TypeDef* local);
bool HAL_USART_RXNE_ReadFlag(UART_TypeDef* local);
void HAL_USART_IDLE_ClearFlag(UART_TypeDef* local);
bool HAL_USART_IDLE_ReadFlag(UART_TypeDef* local);
void HAL_USART_ReceiveOverwrite_ClearFlag(UART_TypeDef* local);
bool HAL_USART_ReceiveOverwrite_ReadFlag(UART_TypeDef* local);
void HAL_USART_NF_ClearFlag(UART_TypeDef* local);
bool HAL_USART_NF_ReadFlag(UART_TypeDef* local);
void HAL_USART_StopBitError_ClearFlag(UART_TypeDef* local);
bool HAL_USART_StopBitError_ReadFlag(UART_TypeDef* local);
void HAL_USART_ParityError_ClearFlag(UART_TypeDef* local);
bool HAL_USART_ParityError_ReadFlag(UART_TypeDef* local);
void HAL_USART_ClearFlags(UART_TypeDef* local);
/* Writing & reading */
void HAL_USART_WriteByte(UART_TypeDef* local, char data);
void HAL_USART_Transmit(UART_TypeDef* local, char data);
void HAL_USART_Write(UART_TypeDef* local, char* buffer, uint32_t len);
void HAL_USART_Print(UART_TypeDef* local, char* str);
char HAL_USART_ReadByte(UART_TypeDef* local);
char HAL_USART_Receive(UART_TypeDef* local);
void HAL_USART_Read(UART_TypeDef* local, char* buffer, uint32_t len);
/* Xprintf */
void xputc(char c);
/* Modem signals */
void HAL_USART_Set_DTR(UART_TypeDef* local, HAL_USART_EnableDisable_enum en);
bool HAL_USART_DCD_Status(UART_TypeDef* local);
void HAL_USART_DCD_ClearToggleFlag(UART_TypeDef* local);
bool HAL_USART_DCD_ReadToggleFlag(UART_TypeDef* local);
bool HAL_USART_RI_Status(UART_TypeDef* local);
void HAL_USART_RI_ClearToggleFlag(UART_TypeDef* local);
bool HAL_USART_RI_ReadToggleFlag(UART_TypeDef* local);
bool HAL_USART_DSR_Status(UART_TypeDef* local);
void HAL_USART_DSR_ClearToggleFlag(UART_TypeDef* local);
bool HAL_USART_DSR_ReadToggleFlag(UART_TypeDef* local);
void HAL_USART_ClearModemFlags(UART_TypeDef* local);


// void HAL_USART_Printf(UART_TypeDef* local, char* str, ...);
// void HAL_USART_PrintBefore(UART_TypeDef* local, char* str);
// void HAL_USART_Print_c(UART_TypeDef* local, char value);
// void HAL_USART_Print_s(UART_TypeDef* local, char* str);
