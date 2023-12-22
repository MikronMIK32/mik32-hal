#include "mik32_hal_w25.h"

// Standard SPI Instructions
const uint8_t WRITE_ENABLE = 0x06;
const uint8_t VOLATILE_SR_WRITE_ENABLE = 0x50;
const uint8_t WRITE_DISABLE = 0x04;
const uint8_t READ_SREG1 = 0x05;
const uint8_t READ_SREG2 = 0x35;
const uint8_t WRITE_SREG = 0x01;
const uint8_t PAGE_PROGRAM = 0x01;
const uint8_t SECTOR_ERASE_4K = 0x20;
const uint8_t BLOCK_ERASE_32K = 0x52;
const uint8_t BLOCK_ERASE_64K = 0xD8;
const uint8_t CHIP_ERASE = 0x60;
const uint8_t ERASE_PROGRAM_SUSPEND = 0x75;
const uint8_t ERASE_PROGRAM_RESUME = 0x7A;
const uint8_t POWER_DOWN = 0xB9;
const uint8_t READ_DATA = 0x03;
const uint8_t FAST_READ = 0x0B;
const uint8_t RELEASE_POWERDOWN_ID = 0xAB;
const uint8_t MANUFACTURER_DEVICE_ID = 0x90;
const uint8_t JEDEC_ID = 0x9F;
const uint8_t READ_UNIQUE_ID = 0x4B;
const uint8_t READ_SFDP_REGISTERS = 0x5A;
const uint8_t ERASE_SECURITY_REGISTERS = 0x44;
const uint8_t PROGRAM_SECURITY_REGISTERS = 0x42;
const uint8_t READ_SECURITY_REGISTERS = 0x48;
const uint8_t ENABLE_QPI = 0x38;
const uint8_t ENABLE_RESET = 0x66;
const uint8_t RESET = 0x99;


SPIFI_CommandTypeDef cmd_write_enable = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = WRITE_ENABLE,
};

SPIFI_CommandTypeDef cmd_volatile_sr_write_enable = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = VOLATILE_SR_WRITE_ENABLE,
};

SPIFI_CommandTypeDef cmd_read_sreg1 = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = READ_SREG1,
};

SPIFI_CommandTypeDef cmd_write_sreg = {
    .Direction = SPIFI_DIRECTION_OUTPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = WRITE_SREG,
};

SPIFI_CommandTypeDef cmd_page_program = {
    .Direction = SPIFI_DIRECTION_OUTPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = PAGE_PROGRAM,
};

void HAL_W25_WriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_write_enable, 0, 0, 0);
}

void HAL_W25_VolatileSRWriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_volatile_sr_write_enable, 0, 0, 0);
}

uint8_t HAL_W25_ReadSREG(SPIFI_HandleTypeDef *spifi, HAL_W25_SREGTypeDef sreg)
{
    uint8_t data = 0;

    SPIFI_CommandTypeDef cmd = cmd_read_sreg1;
    if (sreg == W25_SREG1)
    {
        cmd.OpCode = READ_SREG1;
    }
    else if (sreg == W25_SREG2)
    {
        cmd.OpCode = READ_SREG2;
    }

    HAL_SPIFI_SendCommand(spifi, &cmd, 0, 1, &data);

    return data;
}

void HAL_W25_WriteSREG(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_W25_WriteEnable(spifi);
    HAL_W25_WaitBusy(spifi, 10000);

    HAL_SPIFI_SendCommand(spifi, &cmd_write_sreg, 0, 2, data);
}

void HAL_W25_WriteSREG_Volatile(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_W25_VolatileSRWriteEnable(spifi);
    HAL_W25_WaitBusy(spifi, 10000);

    HAL_SPIFI_SendCommand(spifi, &cmd_write_sreg, 0, 2, data);
}

void HAL_W25_PageProgram(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t dataBytes)
{
    HAL_W25_WriteEnable(spifi);
    HAL_W25_WaitBusy(spifi, 10000);
    HAL_SPIFI_SendCommand(spifi, &cmd_page_program, address, dataLength, dataBytes);
    HAL_W25_WaitBusy(spifi, 10000000);
}
