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



typedef enum 
{
    Frame_8bit = 0b00,
    Frame_9bit = 0b01,
    Frame_7bit = 0b10
} HAL_USART_Frame_enum;

typedef enum
{
    ParityBit_Inverted = 1,
    ParityBit_NotInverted = 0
} HAL_USART_PB_Inversion_enum;


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
    HAL_USART_EnableDisable_enum peie;
    HAL_USART_EnableDisable_enum txeie;
    HAL_USART_EnableDisable_enum tcie;
    HAL_USART_EnableDisable_enum rxneie;
    HAL_USART_EnableDisable_enum idleie;
    //control2
    HAL_USART_EnableDisable_enum lbdie;
    //control3
    HAL_USART_EnableDisable_enum ctsie;
    HAL_USART_EnableDisable_enum eie;
} HAL_USART_Interrupt_TypeDef;

typedef struct __SettingTypeDef
{
    UART_TypeDef* Instance;
    /* Settings */
    HAL_USART_Frame_enum frame;
    HAL_USART_EnableDisable_enum parity_bit;
    HAL_USART_EnableDisable_enum parity_bit_inversion;
    HAL_USART_EnableDisable_enum parity_interrupt;
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
    /* Baudrate */
    uint32_t baudrate;

} UART_Setting_TypeDef;


/* Initialization */
void HAL_USART_MspInit(UART_TypeDef* uart);
HAL_StatusTypeDef HAL_USART_Init(UART_Setting_TypeDef* setting);

/* Flags */
bool HAL_USART_Read_ReceiveReady_Flag(UART_TypeDef* local);
bool HAL_USART_Read_TransmitReady_Flag(UART_TypeDef* local);
bool HAL_USART_Read_CTS_Level(UART_TypeDef* local);
bool HAL_USART_Read_CTS_Change_Flag(UART_TypeDef* local);
bool HAL_USART_Read_RX_Break_Flag(UART_TypeDef* local);
bool HAL_USART_Read_TC_Flag(UART_TypeDef* local);
bool HAL_USART_Read_TXE_Flag(UART_TypeDef* local);
bool HAL_USART_Read_RC_Flag(UART_TypeDef* local);
bool HAL_USART_Read_IDLE_Flag(UART_TypeDef* local);
bool HAL_USART_Read_ReceiveOverwrite_Flag(UART_TypeDef* local);
bool HAL_USART_Read_NF_Flag(UART_TypeDef* local);
bool HAL_USART_Read_StopBit_Error_Flag(UART_TypeDef* local);
bool HAL_USART_Read_Parity_Error_Flag(UART_TypeDef* local);

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

// void HAL_USART_Printf(UART_TypeDef* local, char* str, ...);
// void HAL_USART_PrintBefore(UART_TypeDef* local, char* str);
// void HAL_USART_Print_c(UART_TypeDef* local, char value);
// void HAL_USART_Print_s(UART_TypeDef* local, char* str);
