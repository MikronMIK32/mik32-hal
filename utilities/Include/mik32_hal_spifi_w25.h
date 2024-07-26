#ifndef MIK32_HAL_SPIFI_W25
#define MIK32_HAL_SPIFI_W25

#include "mik32_hal_def.h"
#include "mik32_hal_spifi.h"

typedef enum __HAL_SPIFI_W25_SREGTypeDef
{
    W25_SREG1 = 1,
    W25_SREG2 = 2,
} HAL_SPIFI_W25_SREGTypeDef;

typedef struct __SPIFI_W25_ManufacturerDeviceIDTypeDef
{
    uint8_t Manufacturer;

    uint8_t Device;

} W25_ManufacturerDeviceIDTypeDef;

#define NO_OPCODE 0x20

#define SPIFI_W25_SREG1_BUSY_S 0
#define SPIFI_W25_SREG1_BUSY_M (1 << SPIFI_W25_SREG1_BUSY_S)
#define SPIFI_W25_SREG1_WRITE_ENABLE_S 1
#define SPIFI_W25_SREG1_WRITE_ENABLE_M (1 << SPIFI_W25_SREG1_WRITE_ENABLE_S)
#define SPIFI_W25_SREG1_BLOCK_PROTECT_0_S 2
#define SPIFI_W25_SREG1_BLOCK_PROTECT_0_M (1 << SPIFI_W25_SREG1_BLOCK_PROTECT_0_S)
#define SPIFI_W25_SREG1_BLOCK_PROTECT_1_S 3
#define SPIFI_W25_SREG1_BLOCK_PROTECT_1_M (1 << SPIFI_W25_SREG1_BLOCK_PROTECT_1_S)
#define SPIFI_W25_SREG1_BLOCK_PROTECT_2_S 4
#define SPIFI_W25_SREG1_BLOCK_PROTECT_2_M (1 << SPIFI_W25_SREG1_BLOCK_PROTECT_2_S)
#define SPIFI_W25_SREG1_TOP_BOTTOM_PROTECT_S 5
#define SPIFI_W25_SREG1_TOP_BOTTOM_PROTECT_M (1 << SPIFI_W25_SREG1_TOP_BOTTOM_PROTECT_S)
#define SPIFI_W25_SREG1_SECTOR_PROTECT_S 6
#define SPIFI_W25_SREG1_SECTOR_PROTECT_M (1 << SPIFI_W25_SREG1_SECTOR_PROTECT_S)
#define SPIFI_W25_SREG1_STATUS_REGISTER_PROTECT_0_S 7
#define SPIFI_W25_SREG1_STATUS_REGISTER_PROTECT_0_M (1 << SPIFI_W25_SREG1_STATUS_REGISTER_PROTECT_0_S)

#define SPIFI_W25_SREG2_STATUS_REGISTER_PROTECT_1_S 0
#define SPIFI_W25_SREG2_STATUS_REGISTER_PROTECT_1_M (1 << SPIFI_W25_SREG2_STATUS_REGISTER_PROTECT_1_S)
#define SPIFI_W25_SREG2_QUAD_ENABLE_S 1
#define SPIFI_W25_SREG2_QUAD_ENABLE_M (1 << SPIFI_W25_SREG2_QUAD_ENABLE_S)
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_1_S 3
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_1_M (1 << SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_1_S)
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_2_S 4
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_2_M (1 << SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_2_S)
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_3_S 5
#define SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_3_M (1 << SPIFI_W25_SREG2_SECURITY_REGISTER_LOCK_BIT_3_S)
#define SPIFI_W25_SREG2_COMPLEMENT_PROTECT_S 6
#define SPIFI_W25_SREG2_COMPLEMENT_PROTECT_M (1 << SPIFI_W25_SREG2_COMPLEMENT_PROTECT_S)
#define SPIFI_W25_SREG2_SUSPEND_STATUS_S 7
#define SPIFI_W25_SREG2_SUSPEND_STATUS_M (1 << SPIFI_W25_SREG2_SUSPEND_STATUS_S)

void HAL_SPIFI_W25_WriteEnable(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_W25_WriteDisable(SPIFI_HandleTypeDef *spifi);

uint8_t HAL_SPIFI_W25_ReadSREG(SPIFI_HandleTypeDef *spifi, HAL_SPIFI_W25_SREGTypeDef SREG);

HAL_StatusTypeDef HAL_SPIFI_W25_WriteSREG(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2);

void HAL_SPIFI_W25_WriteSREG_Volatile(SPIFI_HandleTypeDef *spifi, uint8_t sreg1, uint8_t sreg2);

#include "xprintf.h"

static inline __attribute__((always_inline)) HAL_StatusTypeDef HAL_SPIFI_W25_WaitBusy(SPIFI_HandleTypeDef *spifi, uint32_t timeout)
{
    while (timeout-- != 0)
    {
        if ((HAL_SPIFI_W25_ReadSREG(spifi, W25_SREG1) & 1) == 0)
        {
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_SPIFI_W25_WaitBusy_Polling(SPIFI_HandleTypeDef *spifi, uint32_t timeout);

void HAL_SPIFI_W25_PageProgram(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_SectorErase4K(SPIFI_HandleTypeDef *spifi, uint32_t address);

void HAL_SPIFI_W25_ReadData(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

W25_ManufacturerDeviceIDTypeDef HAL_SPIFI_W25_ReadManufacturerDeviceID(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_W25_PageProgram_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_ReadData_Quad(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_ReadData_Quad_IO(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_QPIEnable(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_W25_ReadData_Quad_IO_QPI(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_ReadData_Quad_IO_QPI_XIP(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_ReadData_Quad_IO_QPI_XIP_Init(SPIFI_HandleTypeDef *spifi, uint32_t address, uint16_t dataLength, uint8_t *dataBytes);

void HAL_SPIFI_W25_QPIDisable(SPIFI_HandleTypeDef *spifi);

void HAL_SPIFI_W25_Reset_QPI(SPIFI_HandleTypeDef *spifi);

HAL_StatusTypeDef HAL_SPIFI_W25_QuadEnable(SPIFI_HandleTypeDef *spifi);

HAL_StatusTypeDef HAL_SPIFI_W25_QuadDisable(SPIFI_HandleTypeDef *spifi);

#endif // MIK32_HAL_W25
