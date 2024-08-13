#include <mik32_hal_spifi.h>

uint32_t HAL_SPIFI_initStruct2CtrlReg(SPIFI_InitTypeDef *init)
{
    return (SPIFI_CONFIG_CTRL_TIMEOUT(init->timeout) |
            SPIFI_CONFIG_CTRL_CSHIGH(init->CS_Delay) |
            (init->cacheEnable << SPIFI_CONFIG_CTRL_CACHE_EN_S) |
            (init->dataCacheEnable << SPIFI_CONFIG_CTRL_D_CACHE_DIS_S) |
            (init->interruptEnable << SPIFI_CONFIG_CTRL_INTEN_S) |
            (init->mode3Enable << SPIFI_CONFIG_CTRL_MODE3_S) |
            SPIFI_CONFIG_CTRL_SCK_DIV(init->divider) |
            (init->prefetchEnable << SPIFI_CONFIG_CTRL_PREFETCH_DIS_S) |
            (init->dualModeEnable << SPIFI_CONFIG_CTRL_DUAL_S) |
            (init->clockEdge << SPIFI_CONFIG_CTRL_RFCLK_S) |
            (init->feedbackClock << SPIFI_CONFIG_CTRL_FBCLK_S) |
            (init->DMAEnable << SPIFI_CONFIG_CTRL_DMAEN_S));
}

__attribute__((weak)) void HAL_SPIFI_MspInit()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
    HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);

    __HAL_PCC_SPIFI_CLK_ENABLE();
}

void HAL_SPIFI_MemoryMode_Init(SPIFI_HandleTypeDef *spifi, SPIFI_InitTypeDef *init, SPIFI_MemoryCommandTypeDef *mcmd)
{
    HAL_SPIFI_MemoryMode_Init_LL(
        spifi,
        HAL_SPIFI_initStruct2CtrlReg(init),
        init->cacheLimit,
        ((mcmd->interimLength << SPIFI_CONFIG_MCMD_INTLEN_S) |
         (mcmd->fieldForm << SPIFI_CONFIG_MCMD_FIELDFORM_S) |
         (mcmd->frameForm << SPIFI_CONFIG_MCMD_FRAMEFORM_S) |
         (mcmd->opCode << SPIFI_CONFIG_MCMD_OPCODE_S)));
}

void HAL_SPIFI_Init(SPIFI_HandleTypeDef *spifi, SPIFI_InitTypeDef *init)
{
    HAL_SPIFI_Init_LL(
        spifi,
        HAL_SPIFI_initStruct2CtrlReg(init),
        init->cacheLimit);
}

void HAL_SPIFI_Init_LL(SPIFI_HandleTypeDef *spifi, uint32_t ctrlReg, uint32_t climitReg)
{
    HAL_SPIFI_MspInit();
    HAL_SPIFI_Reset(spifi);
    HAL_SPIFI_WaitResetClear(spifi, HAL_SPIFI_TIMEOUT);

    spifi->Instance->CTRL = ctrlReg;
    spifi->Instance->CLIMIT = climitReg;
}

void HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address)
{
    HAL_SPIFI_SendCommand_LL(
        spifi,
        cmd->direction |
            SPIFI_CONFIG_CMD_INTLEN(cmd->interimLength) |
            SPIFI_CONFIG_CMD_FIELDFORM(cmd->fieldForm) |
            SPIFI_CONFIG_CMD_FRAMEFORM(cmd->frameForm) |
            SPIFI_CONFIG_CMD_OPCODE(cmd->opCode),
        address,
        cmd->interimData);
}

HAL_StatusTypeDef HAL_SPIFI_Transmit(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data)
{
    return HAL_SPIFI_Transmit_LL(
        spifi,
        cmd->direction |
            SPIFI_CONFIG_CMD_INTLEN(cmd->interimLength) |
            SPIFI_CONFIG_CMD_FIELDFORM(cmd->fieldForm) |
            SPIFI_CONFIG_CMD_FRAMEFORM(cmd->frameForm) |
            SPIFI_CONFIG_CMD_OPCODE(cmd->opCode),
        address,
        dataLength,
        data,
        cmd->interimData);
}

void HAL_SPIFI_SendCommand_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint32_t interimData)
{
    HAL_SPIFI_ClearInterruptRequest(spifi);
    spifi->Instance->ADDR = address;
    spifi->Instance->IDATA = interimData;
    spifi->Instance->CMD = cmdRegCfg;
}

HAL_StatusTypeDef HAL_SPIFI_Transmit_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data,
    uint32_t interimData)
{
    if (((cmdRegCfg & SPIFI_CONFIG_CMD_DOUT_M) == 0) || (data == 0) || (dataLength == 0))
    {
        return HAL_ERROR;
    }

    HAL_SPIFI_ClearInterruptRequest(spifi);
    spifi->Instance->ADDR = address;
    spifi->Instance->IDATA = interimData;
    spifi->Instance->CMD = cmdRegCfg | SPIFI_CONFIG_CMD_DATALEN(dataLength);

    for (int i = 0; i < dataLength; i++)
    {
        spifi->Instance->DATA8 = data[i];
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SPIFI_Receive_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *data,
    uint32_t interimData)
{
    if (((cmdRegCfg & SPIFI_CONFIG_CMD_DOUT_M) != 0) || (data == 0) || (dataLength == 0))
    {
        return HAL_ERROR;
    }

    HAL_SPIFI_ClearInterruptRequest(spifi);
    spifi->Instance->ADDR = address;
    spifi->Instance->IDATA = interimData;
    spifi->Instance->CMD = cmdRegCfg | SPIFI_CONFIG_CMD_DATALEN(dataLength);

    for (int i = 0; i < dataLength; i++)
    {
        data[i] = (uint8_t)spifi->Instance->DATA8;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SPIFI_ReceiveWords_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t address,
    uint16_t dataLength,
    uint32_t *data,
    uint32_t interimData)
{
    if (((cmdRegCfg & SPIFI_CONFIG_CMD_DOUT_M) != 0) || (data == 0) || (dataLength == 0))
    {
        return HAL_ERROR;
    }

    HAL_SPIFI_ClearInterruptRequest(spifi);
    spifi->Instance->ADDR = address;
    spifi->Instance->IDATA = interimData;
    spifi->Instance->CMD = cmdRegCfg | SPIFI_CONFIG_CMD_DATALEN(dataLength * 4);

    for (int i = 0; i < dataLength; i++)
    {
        data[i] = spifi->Instance->DATA;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SPIFI_WaitCommandComplete_LL(
    SPIFI_HandleTypeDef *spifi,
    uint32_t cmdRegCfg,
    uint32_t timeout)
{
    HAL_StatusTypeDef waitStatus = HAL_SPIFI_WaitInterruptRequest(spifi, timeout);
    if ((waitStatus == HAL_OK) && (cmdRegCfg & SPIFI_CONFIG_CMD_POLL_M))
    {
        if (SPIFI_CONFIG->DATA8 == (cmdRegCfg & SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_M))
        {
            return HAL_OK;
        }
        return HAL_ERROR;
    }
    return waitStatus;
}
