#include "mik32_hal_spifi_w25.h"

typedef enum __HAL_SPIFI_W25_OPCodesTypeDef
{
    // Standard SPI Instructions
    WRITE_ENABLE = 0x06,
    VOLATILE_SR_WRITE_ENABLE = 0x50,
    WRITE_DISABLE = 0x04,
    READ_SREG1 = 0x05,
    READ_SREG2 = 0x35,
    WRITE_SREG = 0x01,
    PAGE_PROGRAM = 0x02,
    SECTOR_ERASE_4K = 0x20,
    BLOCK_ERASE_32K = 0x52,
    BLOCK_ERASE_64K = 0xD8,
    CHIP_ERASE = 0x60,
    ERASE_PROGRAM_SUSPEND = 0x75,
    ERASE_PROGRAM_RESUME = 0x7A,
    POWER_DOWN = 0xB9,
    READ_DATA = 0x03,
    FAST_READ = 0x0B,
    RELEASE_POWERDOWN_ID = 0xAB,
    MANUFACTURER_DEVICE_ID = 0x90,
    JEDEC_ID = 0x9F,
    READ_UNIQUE_ID = 0x4B,
    READ_SFDP_REGISTERS = 0x5A,
    ERASE_SECURITY_REGISTERS = 0x44,
    PROGRAM_SECURITY_REGISTERS = 0x42,
    READ_SECURITY_REGISTERS = 0x48,
    ENABLE_QPI = 0x38,
    ENABLE_RESET = 0x66,
    RESET = 0x99,

    // Quad SPI Instructions
    QUAD_PAGE_PROGRAM = 0x32,
    FAST_READ_QUAD_OUTPUT = 0x6B,
    FAST_READ_QUAD_IO = 0xEB,
    WORD_READ_QUAD_IO = 0xE7,
    OCTAL_WORD_READ_QUAD_IO = 0xE3,
    SET_BURST_WITH_WRAP = 0x77,
    MANUFACTURER_DEVICE_ID_BY_QUAD_IO = 0x94,
} HAL_SPIFI_W25_OPCodesTypeDef;

#define HAL_SPIFI_W25_SREG2_BUSY 2

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

SPIFI_CommandTypeDef cmd_sector_erase_4k = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = SECTOR_ERASE_4K,
};

SPIFI_CommandTypeDef cmd_read_data = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = READ_DATA,
};

SPIFI_CommandTypeDef cmd_manufacturer_device_id = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_ALL_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = MANUFACTURER_DEVICE_ID,
};

SPIFI_CommandTypeDef cmd_quad_page_program = {
    .Direction = SPIFI_DIRECTION_OUTPUT,
    .FieldForm = SPIFI_FIELDFORM_DATA_PARALLEL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 0,
    .OpCode = QUAD_PAGE_PROGRAM,
};

SPIFI_CommandTypeDef cmd_fast_read_quad_output = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_DATA_PARALLEL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 1,
    .OpCode = FAST_READ_QUAD_OUTPUT,
};

SPIFI_CommandTypeDef cmd_fast_read_quad_io = {
    .Direction = SPIFI_DIRECTION_INPUT,
    .FieldForm = SPIFI_FIELDFORM_COMMAND_SERIAL,
    .FrameForm = SPIFI_FRAMEFORM_OPCODE_3ADDR,
    .InterimData = 0,
    .InterimLength = 3,
    .OpCode = FAST_READ_QUAD_IO,
};

void HAL_SPIFI_W25_WriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_write_enable, 0, 0, 0);
}

void HAL_SPIFI_W25_VolatileSRWriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_volatile_sr_write_enable, 0, 0, 0);
}

uint8_t HAL_SPIFI_W25_ReadSREG(SPIFI_HandleTypeDef *spifi, HAL_SPIFI_W25_SREGTypeDef sreg)
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

void HAL_SPIFI_W25_WriteSREG(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_SPIFI_W25_WriteEnable(spifi);

    HAL_SPIFI_SendCommand(spifi, &cmd_write_sreg, 0, 2, data);
    HAL_SPIFI_W25_WaitBusy(spifi, 10000);
}

void HAL_SPIFI_W25_WriteSREG_Volatile(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_SPIFI_W25_VolatileSRWriteEnable(spifi);

    HAL_SPIFI_SendCommand(spifi, &cmd_write_sreg, 0, 2, data);
}

void HAL_SPIFI_W25_PageProgram(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand(spifi, &cmd_page_program, address, dataLength, dataBytes);
    HAL_SPIFI_W25_WaitBusy(spifi, 100000);
}

void HAL_SPIFI_W25_SectorErase4K(SPIFI_HandleTypeDef *spifi, uint32_t address)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand(spifi, &cmd_sector_erase_4k, address, 0, 0);
    HAL_SPIFI_W25_WaitBusy(spifi, 100000);
}

void HAL_SPIFI_W25_ReadData(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_read_data, address, dataLength, dataBytes);
}

W25_ManufacturerDeviceIDTypeDef HAL_SPIFI_W25_ReadManufacturerDeviceID(SPIFI_HandleTypeDef *spifi)
{
    uint8_t data[2] = {0};

    HAL_SPIFI_SendCommand(spifi, &cmd_manufacturer_device_id, 0, 2, data);

    W25_ManufacturerDeviceIDTypeDef return_value = {
        .Manufacturer = data[0],
        .Device = data[1],
    };

    return return_value;
}

void HAL_SPIFI_W25_PageProgram_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand(spifi, &cmd_quad_page_program, address, dataLength, dataBytes);
    HAL_SPIFI_W25_WaitBusy(spifi, 100000);
}

void HAL_SPIFI_W25_ReadData_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_fast_read_quad_output, address, dataLength, dataBytes);
}

void HAL_SPIFI_W25_ReadData_Quad_IO(SPIFI_HandleTypeDef *spifi, uint32_t address, uint8_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand(spifi, &cmd_fast_read_quad_io, address, dataLength, dataBytes);
}

HAL_StatusTypeDef HAL_SPIFI_W25_QuadEnable(SPIFI_HandleTypeDef *spifi)
{
    uint8_t sreg1 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG1);
    uint8_t sreg2 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG2);

    HAL_SPIFI_W25_WriteSREG(spifi, sreg1, sreg2 | HAL_SPIFI_W25_SREG2_BUSY);
}

HAL_StatusTypeDef HAL_SPIFI_W25_QuadDisable(SPIFI_HandleTypeDef *spifi)
{
    uint8_t sreg1 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG1);
    uint8_t sreg2 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG2);

    HAL_SPIFI_W25_WriteSREG(spifi, sreg1, sreg2 & (~HAL_SPIFI_W25_SREG2_BUSY));
}


