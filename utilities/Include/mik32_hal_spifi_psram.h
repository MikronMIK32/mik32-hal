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

#ifndef MIK32_HAL_SPIFI_PSRAM
#define MIK32_HAL_SPIFI_PSRAM

#include "mik32_hal_def.h"
#include "mik32_hal_spifi.h"


/* Функции для работы с PSRAM */


/* Только для режима SPI */

void HAL_SPIFI_PSRAM_Read_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Enter_QPI_Mode_SPI(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_PSRAM_Read_ID_SPI(SPIFI_HandleTypeDef *spifi, uint8_t *dataBytes);


/* Только для режима QPI */

void HAL_SPIFI_PSRAM_Exit_QPI_Mode_QPI(SPIFI_HandleTypeDef *spifi);


/* Общие для режимов SPI и QPI */

void HAL_SPIFI_PSRAM_Fast_Read_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Fast_Read_Quad_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Fast_Read_Quad_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Write_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Write_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Quad_Write_SPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Quad_Write_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_PSRAM_Reset_SPI(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_PSRAM_Reset_QPI(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_PSRAM_Wrap_Boundary_Toggle_SPI(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_PSRAM_Wrap_Boundary_Toggle_QPI(SPIFI_HandleTypeDef *spifi);

#endif // MIK32_HAL_SPIFI_PSRAM
