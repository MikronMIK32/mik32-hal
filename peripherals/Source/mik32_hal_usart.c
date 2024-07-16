#include "mik32_hal_usart.h"

/*******************************************************************************
 * @brief USART GPIO initializing
 * @param local - pointer to USART structure-descriptor
 * @return none
 */
__attribute__((weak)) void HAL_USART_MspInit(UART_TypeDef* local)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; 

    if (local == UART_0)
    {
        __HAL_PCC_UART_0_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
    }

    if (local == UART_1)
    {
        __HAL_PCC_UART_1_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = HAL_GPIO_MODE_TIMER_SERIAL;
        GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
        HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);
    }
}


/*******************************************************************************
 * @brief USART initializing
 * @param setting - pointer to USART setting structure
 * @return HAL_OK or HAL_ERROR
 */
HAL_StatusTypeDef HAL_USART_Init(UART_Setting_TypeDef* setting)
{
    __HAL_USART_Disable(setting->Instance);
    HAL_USART_MspInit(setting->Instance);
    /* CONTROL1 */
    uint32_t control1 = 0;
    if (setting->frame & 0b01)      control1 |= UART_CONTROL1_M0_M;
    if (setting->frame & 0b10)      control1 |= UART_CONTROL1_M1_M;
    if (setting->parity_bit)        control1 |= UART_CONTROL1_PCE_M;
    if (setting->parity_bit_inversion) control1 |= UART_CONTROL1_PS_M;
    if (setting->parity_interrupt)  control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.peie)    control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.txeie)   control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.tcie)    control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.rxneie)  control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.idleie)  control1 |= UART_CONTROL1_PEIE_M;
    if (setting->Interrupt.peie)    control1 |= UART_CONTROL1_PEIE_M;
    //Save data
    setting->Instance->CONTROL1 = control1;
    /* CONTROL2 */
    uint32_t control2 = 0;
    if (setting->bit_direction)     control2 |= UART_CONTROL2_MSBFIRST_M;
    if (setting->data_inversion)    control2 |= UART_CONTROL2_DATAINV_M;
    if (setting->tx_inversion)      control2 |= UART_CONTROL2_TXINV_M;
    if (setting->rx_inversion)      control2 |= UART_CONTROL2_RXINV_M;
    if (setting->swap)              control2 |= UART_CONTROL2_SWAP_M;
    if (setting->lbm)               control2 |= UART_CONTROL2_LBM_M;
    if (setting->stop_bit)          control2 |= UART_CONTROL2_STOP_1_M;
    if (setting->mode)              control2 |= UART_CONTROL2_CLKEN_M;
    if (setting->xck_mode & 0b10)   control2 |= UART_CONTROL2_CPOL_M;
    if (setting->xck_mode & 0b01)   control2 |= UART_CONTROL2_CPHA_M;
    if (setting->last_byte_clock)   control2 |= UART_CONTROL2_LBCL_M;
    if (setting->Interrupt.lbdie)   control2 |= UART_CONTROL2_LBDIE_M;
    //Save data
    setting->Instance->CONTROL2 = control2;
    /* CONTROL3 */
    uint32_t control3 = 0;
    if (setting->overwrite)         control3 |= UART_CONTROL3_OVRDIS_M;
    if (setting->cts_processing)    control3 |= UART_CONTROL3_CTSE_M;
    if (setting->rts_processing)    control3 |= UART_CONTROL3_RTSE_M;
    if (setting->dma_tx_request)    control3 |= UART_CONTROL3_DMAT_M;
    if (setting->dma_rx_request)    control3 |= UART_CONTROL3_DMAR_M;
    if (setting->channel_mode)      control3 |= UART_CONTROL3_HDSEL_M;
    if (setting->tx_break_mode)     control3 |= UART_CONTROL3_BKRQ_M;
    if (setting->Interrupt.ctsie)   control3 |= UART_CONTROL3_CTSIE_M;
    if (setting->Interrupt.eie)     control3 |= UART_CONTROL3_EIE_M;
    setting->Instance->CONTROL3 = control3;

    /* Baudrate */
    setting->Instance->DIVIDER = (HAL_PCC_GetSysClockFreq() / (PM->DIV_AHB+1) / (PM->DIV_APB_P+1)) /
        setting->baudrate;
    if (setting->Instance->DIVIDER < 16) return HAL_ERROR;
    
    __HAL_USART_TX_Enable(setting->Instance);
    __HAL_USART_RX_Enable(setting->Instance);
    __HAL_USART_Enable(setting->Instance);

    if (setting->Instance->CONTROL1 | UART_CONTROL1_TE_M)
        while(!HAL_USART_Read_TransmitReady_Flag(setting->Instance));
    if (setting->Instance->CONTROL1 | UART_CONTROL1_RE_M)
        while(!HAL_USART_Read_ReceiveReady_Flag(setting->Instance));
    return HAL_OK;
}

/*******************************************************************************
 * @brief UART send 1 byte of data for transmission
 * @param local - pointer to USART structure-descriptor
 * @param data - a byte of data to send
 * @return none
 */
void HAL_USART_WriteByte(UART_TypeDef* local, char data)
{
    local->TXDATA = data;
}

/*******************************************************************************
 * @brief USART transmit 1 byte of data
 * @param local - pointer to USART structure-descriptor
 * @param data - a byte of data to transmit by USART
 * @return none
 */
void HAL_USART_Transmit(UART_TypeDef* local, char data)
{
    HAL_USART_WriteByte(local, data);
    while (!HAL_USART_Read_TC_Flag(local));
}

/*******************************************************************************
 * @brief USART write array of data
 * @param local - pointer to USART structure-descriptor
 * @param buffer - pointer to array of data to transmit
 * @param len - buffer size
 * @return none
 */
void HAL_USART_Write(UART_TypeDef* local, char* buffer, uint32_t len)
{
    for (uint32_t i=0; i<len; i++) HAL_USART_Transmit(local, buffer[i]);
}

/*******************************************************************************
 * @brief USART print a string
 * @param local - pointer to USART structure-descriptor
 * @param str - pointer to string to transmit
 * @return none
 */
void HAL_USART_Print(UART_TypeDef* local, char* str)
{
    uint32_t i = 0;
    while (str[i] != '\0')
    {
        HAL_USART_Transmit(local, str[i]);
        i += 1;
    }
}

/*******************************************************************************
 * @brief Read data from USART module
 * @param local - pointer to USART structure-descriptor
 * @return read data
 */
char HAL_USART_ReadByte(UART_TypeDef* local)
{
    return local->RXDATA;
}

/**
 * @brief USART receive 1 byte of data
 * @param local - pointer to USART structure-descriptor
 * @return received data
 */
char HAL_USART_Receive(UART_TypeDef* local)
{
    while(HAL_USART_Read_RC_Flag(local));
    return HAL_USART_ReadByte(local);
}

/**
 * @brief USART receive array of data
 * @param local - pointer to USART structure-descriptor
 * @param buffer - pointer to array of data to receive
 * @param len - buffer size
 * @return none
 */
void HAL_USART_Read(UART_TypeDef* local, char* buffer, uint32_t len)
{
    for (uint32_t i=0; i<len; i++) buffer[i] = HAL_USART_Receive(local);
}



bool HAL_USART_Read_ReceiveReady_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_REACK_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_TransmitReady_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_TEACK_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_CTS_Level(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_CTS_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_CTS_Change_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_CTSIF_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_RX_Break_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_LBDF_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_TC_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_TC_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_TXE_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_TXE_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_RC_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_RXNE_M) == 0) return true;
    else return false;
}

bool HAL_USART_Read_IDLE_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_IDLE_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_ReceiveOverwrite_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_ORE_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_NF_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_NF_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_StopBit_Error_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_FE_M) == 0) return false;
    else return true;
}

bool HAL_USART_Read_Parity_Error_Flag(UART_TypeDef* local)
{
    if ((local->FLAGS & UART_FLAGS_PE_M) == 0) return false;
    else return true;
}

/**
 * @brief xputc - sending data for xprintf
 * @param c - symbol
 * @return none
 */
void __attribute__((weak)) xputc(char c)
{
	HAL_USART_Transmit(UART_0, c);
}
