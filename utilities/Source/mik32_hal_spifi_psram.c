/*
 *  Список поддерживаемых с MIK32 Амур моделей псевдостатического ОЗУ (PSRAM):
 *
 *  APS6404L-3SQR — 3,3В ≤84МГц SPI/QSPI/QPI PSRAM производства «AP Memory»;
 *  ESP-PSRAM64H  — 3,3В ≤84МГц SPI/QSPI/QPI PSRAM производства «Espressif»;
 *  IPS6404L-SQ   — 3,3В ≤84МГц SPI/QSPI/QPI PSRAM производства «IPUS Limited».
 * 
 *  !!! Внимание !!!
 *
 *  При испытании микросхемы с маркировкой «APS6404L-3SQR» выявилось следующее:
 *
 *  Команда чтения "FAST_READ" (0x0B) в режиме QPI приводит к ошибкам в данных,
 *  поэтому применять её не следует во избежание непредсказуемого поведения ПО.
 *
 *  Достоверность описанной проблемы относится ко времени 14 августа 2024 года.
 *
 */
 
#include "mik32_hal_spifi_psram.h"

/* Инструкции управления SPI/QPI PSRAM */
typedef enum __HAL_SPIFI_PSRAM_OPCodesTypeDef
{
    /* Только для режима SPI */
    READ = 0x03,
    FAST_READ = 0x0B,
    ENTER_QPI_MODE = 0x35,
    READ_ID = 0x9F,
 
    /* Только для режима QPI */
    EXIT_QPI_MODE = 0xF5,
    
    /* Общие для режимов SPI и QPI */
    FAST_READ_QUAD = 0xEB,
    WRITE = 0x02,
    QUAD_WRITE = 0x38,
    RESET_ENABLE = 0x66,
    RESET = 0x99,
    WRAP_BOUNDARY_TOGGLE = 0xC0
    
} HAL_SPIFI_PSRAM_OPCodesTypeDef;


/* Настройки SPIFI команд для различных операций с PSRAM */


/* Только для режима SPI */

const uint32_t cmd_read_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(READ);
    
const uint32_t cmd_fast_read_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(1) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(FAST_READ);

const uint32_t cmd_enter_qpi_mode_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(ENTER_QPI_MODE);

const uint32_t cmd_read_id_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(READ_ID);


/* Только для режима QPI */

const uint32_t cmd_exit_qpi_mode_qpi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(EXIT_QPI_MODE);


/* Общие для режимов SPI и QPI */

const uint32_t cmd_fast_read_quad_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(3) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_OPCODE_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(FAST_READ_QUAD);

const uint32_t cmd_fast_read_quad_qpi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(3) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(FAST_READ_QUAD);

const uint32_t cmd_write_spi =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(WRITE);

const uint32_t cmd_write_qpi =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(WRITE);

const uint32_t cmd_quad_write_spi =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_OPCODE_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(QUAD_WRITE);

const uint32_t cmd_quad_write_qpi =
    SPIFI_DIRECTION_OUTPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(WRITE);

const uint32_t cmd_reset_enable_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(RESET_ENABLE);

const uint32_t cmd_reset_enable_qpi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(RESET_ENABLE);

const uint32_t cmd_reset_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(RESET);

const uint32_t cmd_reset_qpi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(RESET);

const uint32_t cmd_wrap_boundary_toggle_spi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(WRAP_BOUNDARY_TOGGLE);

const uint32_t cmd_wrap_boundary_toggle_qpi =
    SPIFI_DIRECTION_INPUT |
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE) |
    SPIFI_CONFIG_CMD_OPCODE(WRAP_BOUNDARY_TOGGLE);


/* Функции для работы с PSRAM */


/* Только для режима SPI */

void HAL_SPIFI_PSRAM_Read_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_read_spi, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Fast_Read_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_fast_read_spi, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Enter_QPI_Mode_SPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_enter_qpi_mode_spi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Read_ID_SPI(SPIFI_HandleTypeDef *spifi, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_read_id_spi, 0, 8, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}


/* Только для режима QPI */

void HAL_SPIFI_PSRAM_Exit_QPI_Mode_QPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_exit_qpi_mode_qpi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}


/* Общие для режимов SPI и QPI */

void HAL_SPIFI_PSRAM_Fast_Read_Quad_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_fast_read_quad_spi, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Fast_Read_Quad_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_fast_read_quad_qpi, address, dataLength, dataBytes, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Write_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_write_spi, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Write_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_write_qpi, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Quad_Write_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_quad_write_spi, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Quad_Write_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_quad_write_qpi, address, dataLength, 0, dataBytes, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Reset_SPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_reset_enable_spi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_SendCommand_LL(spifi, cmd_reset_spi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Reset_QPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_reset_enable_qpi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_SendCommand_LL(spifi, cmd_reset_qpi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Wrap_Boundary_Toggle_SPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_wrap_boundary_toggle_spi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

void HAL_SPIFI_PSRAM_Wrap_Boundary_Toggle_QPI(SPIFI_HandleTypeDef *spifi)
{
    HAL_SPIFI_SendCommand_LL(spifi, cmd_wrap_boundary_toggle_qpi, 0, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
}

