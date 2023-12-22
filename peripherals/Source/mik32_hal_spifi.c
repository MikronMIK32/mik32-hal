#include <mik32_hal_spifi.h>

__attribute__((weak)) void HAL_SPIFI_MspInit(SPIFI_HandleTypeDef *spifi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_PCC_SPIFI_CLK_ENABLE();

    GPIO_InitStruct.Pin = PORT2_0 | PORT2_1;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PORT2_2 | PORT2_3 | PORT2_4 | PORT2_5;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;
    HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);
}

void HAL_SPIFI_MemoryMode_Init(SPIFI_MemoryModeConfig_HandleTypeDef *spifi)
{
    HAL_SPIFI_MspInit(spifi);

    spifi->Instance->STAT |= SPIFI_CONFIG_STAT_RESET_M;
    spifi->Instance->CLIMIT = spifi->CacheLimit; // Граница кеширования
    if (spifi->CacheEnable)
    {
        spifi->Instance->CTRL |= SPIFI_CONFIG_CTRL_CACHE_EN_M;
    }
    else
    {
        spifi->Instance->CTRL &= ~SPIFI_CONFIG_CTRL_CACHE_EN_M;
    }

    // Настройка команды чтения
    spifi->Instance->MCMD = ((spifi->Command.InterimLength << SPIFI_CONFIG_MCMD_INTLEN_S) |
                             (spifi->Command.FieldForm << SPIFI_CONFIG_MCMD_FIELDFORM_S) |
                             (spifi->Command.FrameForm << SPIFI_CONFIG_MCMD_FRAMEFORM_S) |
                             (spifi->Command.OpCode << SPIFI_CONFIG_MCMD_OPCODE_S));
}

void SPIFI_WaitIntrqTimeout()
{
    while (~(SPIFI_CONFIG->STAT & SPIFI_CONFIG_STAT_INTRQ_M))
        ;
}

void HAL_SPIFI_SendCommand(
    SPIFI_HandleTypeDef *spifi,
    SPIFI_CommandTypeDef *cmd,
    uint32_t address,
    uint16_t dataLength,
    uint8_t *dataBytes)
{
    // spifi->Instance->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    spifi->Instance->ADDR = address;
    if (cmd->InterimLength > 0)
    {
        spifi->Instance->IDATA = cmd->InterimData;
    }
    spifi->Instance->CMD = ((cmd->OpCode << SPIFI_CONFIG_CMD_OPCODE_S) |
                            (cmd->FrameForm << SPIFI_CONFIG_CMD_FRAMEFORM_S) |
                            (cmd->FieldForm << SPIFI_CONFIG_CMD_FIELDFORM_S) |
                            (dataLength << SPIFI_CONFIG_CMD_DATALEN_S) |
                            (cmd->InterimLength << SPIFI_CONFIG_CMD_INTLEN_S) |
                            (cmd->Direction << SPIFI_CONFIG_CMD_DOUT_S));

    // SPIFI_WaitIntrqTimeout();

    if (cmd->Direction == SPIFI_DIRECTION_INPUT)
    {
        for (int i = 0; i < dataLength; i++)
        {
            dataBytes[i] = (uint8_t)spifi->Instance->DATA8;
        }
    }
    else
    {
        if ((dataBytes == 0) && (dataLength > 0))
        {
            // xprintf("dataByte zero pointer\n");
            // Сделать возврат ошибки
            return;
        }
        for (int i = 0; i < dataLength; i++)
        {
            spifi->Instance->DATA8 = dataBytes[i];
        }
    }
}

bool HAL_SPIFI_IsMemoryModeEnabled(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_MCINIT_M) != 0;
}

void HAL_SPIFI_Reset(SPIFI_HandleTypeDef *spifi)
{
    spifi->Instance->STAT = SPIFI_CONFIG_STAT_RESET_M;
}

bool HAL_SPIFI_IsReady(SPIFI_HandleTypeDef *spifi)
{
    return (spifi->Instance->STAT & SPIFI_CONFIG_STAT_RESET_M) == 0;
}
