#include "mik32_hal_spifi_w25.h"

#define SPIFI_W25_WRITE_SREG_BUSY 10000
#define SPIFI_W25_PROGRAM_BUSY 100000

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

const uint32_t cmd_write_enable =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(WRITE_ENABLE);

const uint32_t cmd_volatile_sr_write_enable =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(VOLATILE_SR_WRITE_ENABLE);

const uint32_t cmd_read_sreg1 =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(READ_SREG1);

const uint32_t cmd_read_sreg2 =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(READ_SREG2);

const uint32_t cmd_write_sreg =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(WRITE_SREG);

const uint32_t cmd_page_program =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(PAGE_PROGRAM);

const uint32_t cmd_sector_erase_4k =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(SECTOR_ERASE_4K);

const uint32_t cmd_read_data =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(READ_DATA);

const uint32_t cmd_manufacturer_device_id =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(MANUFACTURER_DEVICE_ID);

const uint32_t cmd_quad_page_program =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_DATA_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(QUAD_PAGE_PROGRAM);

const uint32_t cmd_fast_read_quad_output =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(1) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_DATA_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(FAST_READ_QUAD_OUTPUT);

const uint32_t cmd_fast_read_quad_io =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(3) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_OPCODE_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(FAST_READ_QUAD_IO);

const uint32_t cmd_read_sreg1_polling =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_POLL_INDEX(0) |
    SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE(0) |
    SPIFI_CONFIG_CMD_POLL_M |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(READ_SREG1);

void HAL_SPIFI_W25_WriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_write_enable, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_W25_VolatileSRWriteEnable(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_volatile_sr_write_enable, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

uint8_t HAL_SPIFI_W25_ReadSREG(SPIFI_HandleTypeDef *spifi, HAL_SPIFI_W25_SREGTypeDef sreg)
{
    uint8_t byte = 0;

    uint32_t cmd = cmd_read_sreg1;
    if (sreg == W25_SREG1)
    {
        cmd = cmd_read_sreg1;
    }
    else if (sreg == W25_SREG2)
    {
        cmd = cmd_read_sreg2;
    }

    HAL_SPIFI_SendCommand_LL(spifi, cmd, 0, 1, &byte, 0, 0, HAL_SPIFI_TIMEOUT);

    return byte;
}

HAL_StatusTypeDef HAL_SPIFI_W25_WriteSREG(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_SPIFI_W25_WriteEnable(spifi);

    HAL_SPIFI_SendCommand_LL(spifi, cmd_write_sreg, 0, 2, 0, data, 0, HAL_SPIFI_TIMEOUT);
    return HAL_SPIFI_W25_WaitBusy(spifi, SPIFI_W25_WRITE_SREG_BUSY);
}

void HAL_SPIFI_W25_WriteSREG_Volatile(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2)
{
    uint8_t data[2] = {sreg1, sreg2};

    HAL_SPIFI_W25_VolatileSRWriteEnable(spifi);

    HAL_SPIFI_SendCommand_LL(spifi, cmd_write_sreg, 0, 2, 0, data, 0, HAL_SPIFI_TIMEOUT);
}

HAL_StatusTypeDef HAL_SPIFI_W25_WaitBusy_Polling(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    return HAL_SPIFI_SendCommand_LL(spifi, cmd_read_sreg1_polling, 0, 0, 0, 0, 0, timeout);
}

void HAL_SPIFI_W25_PageProgram(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand_LL(spifi, cmd_page_program, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_W25_WaitBusy(spifi, SPIFI_W25_PROGRAM_BUSY);
}

void HAL_SPIFI_W25_SectorErase4K(SPIFI_HandleTypeDef *spifi, uint32_t address)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand_LL(spifi, cmd_sector_erase_4k, address, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_W25_WaitBusy(spifi, SPIFI_W25_PROGRAM_BUSY);
}

void HAL_SPIFI_W25_ReadData(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_read_data, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

W25_ManufacturerDeviceIDTypeDef HAL_SPIFI_W25_ReadManufacturerDeviceID(SPIFI_HandleTypeDef *spifi)
{
    uint8_t data[2] = {0};

    HAL_SPIFI_SendCommand_LL(spifi, cmd_manufacturer_device_id, 0, 2, data, 0, 0, HAL_SPIFI_TIMEOUT);

    return (W25_ManufacturerDeviceIDTypeDef){
        .Manufacturer = data[0],
        .Device = data[1],
    };
}

void HAL_SPIFI_W25_PageProgram_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_W25_WriteEnable(spifi);
    HAL_SPIFI_SendCommand_LL(spifi, cmd_quad_page_program, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_W25_WaitBusy(spifi, SPIFI_W25_PROGRAM_BUSY);
}

void HAL_SPIFI_W25_ReadData_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_fast_read_quad_output, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_W25_ReadData_Quad_IO(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_fast_read_quad_io, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

HAL_StatusTypeDef HAL_SPIFI_W25_QuadEnable(SPIFI_HandleTypeDef *spifi)
{
    uint8_t sreg1 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG1);
    uint8_t sreg2 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG2);

    return HAL_SPIFI_W25_WriteSREG(spifi, sreg1, sreg2 | HAL_SPIFI_W25_SREG2_BUSY);
}

HAL_StatusTypeDef HAL_SPIFI_W25_QuadDisable(SPIFI_HandleTypeDef *spifi)
{
    uint8_t sreg1 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG1);
    uint8_t sreg2 = HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG2);

    return HAL_SPIFI_W25_WriteSREG(spifi, sreg1, sreg2 & (~HAL_SPIFI_W25_SREG2_BUSY));
}
