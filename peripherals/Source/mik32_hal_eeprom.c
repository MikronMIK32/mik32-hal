#include "mik32_hal_eeprom.h"

const uint8_t N_LD_DEFAULT = 1;
const uint8_t N_R_2_DEFAULT = 1;

HAL_StatusTypeDef HAL_EEPROM_Init(HAL_EEPROM_HandleTypeDef *eeprom)
{
    if (eeprom->Instance != EEPROM_REGS)
    {
        return HAL_ERROR;
    }

    eeprom->Instance->EECON = 0;
    HAL_EEPROM_SetMode(eeprom, eeprom->Mode);
    HAL_EEPROM_SetErrorCorrection(eeprom, eeprom->ErrorCorrection);
    HAL_EEPROM_SetInterrupt(eeprom, eeprom->EnableInterrupt);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_EEPROM_Erase(
    HAL_EEPROM_HandleTypeDef *eeprom,
    uint16_t address,
    uint8_t erasedWordsCount,
    HAL_EEPROM_WriteBehaviourTypeDef erasedPages,
    uint32_t timeout)
{
    eeprom->Instance->EECON |= EEPROM_EECON_BWE_M | (erasedPages << EEPROM_EECON_WRBEH_S);

    if (eeprom->Mode == HAL_EEPROM_MODE_TWO_STAGE)
    {
        eeprom->Instance->EEA = address;
        for (int i = 0; i < erasedWordsCount; i++)
        {
            eeprom->Instance->EEDAT = 0;
        }
    }
    else
    {
        HAL_StatusTypeDef status = HAL_OK;
        for (int i = 0; i < erasedWordsCount; i++)
        {
            eeprom->Instance->EEA = address + i * 4;
            eeprom->Instance->EEDAT = 0;
            if ((status = HAL_EEPROM_WaitBusy(eeprom, timeout)) != HAL_OK)
                return status;
        }
    }

    eeprom->Instance->EECON |= EEPROM_EECON_OP(EEPROM_EECON_OP_ER) | EEPROM_EECON_EX_M;

    return HAL_EEPROM_WaitBusy(eeprom, timeout);
}

HAL_StatusTypeDef HAL_EEPROM_Write(
    HAL_EEPROM_HandleTypeDef *eeprom,
    uint16_t address,
    uint32_t *data,
    uint8_t length,
    HAL_EEPROM_WriteBehaviourTypeDef writedPages,
    uint32_t timeout)
{
    eeprom->Instance->EECON |= EEPROM_EECON_BWE_M | (writedPages << EEPROM_EECON_WRBEH_S);

    if (eeprom->Mode == HAL_EEPROM_MODE_TWO_STAGE)
    {
        eeprom->Instance->EEA = address;
        for (int i = 0; i < length; i++)
        {
            eeprom->Instance->EEDAT = data[i];
        }
    }
    else
    {
        HAL_StatusTypeDef status = HAL_OK;
        for (int i = 0; i < length; i++)
        {
            eeprom->Instance->EEA = address + i * 4;
            eeprom->Instance->EEDAT = data[i];
            if ((status = HAL_EEPROM_WaitBusy(eeprom, timeout)) != HAL_OK)
                return status;
        }
    }

    eeprom->Instance->EECON |= EEPROM_EECON_OP(EEPROM_EECON_OP_PR) | EEPROM_EECON_EX_M;

    return HAL_EEPROM_WaitBusy(eeprom, timeout);
}

HAL_StatusTypeDef HAL_EEPROM_Read(HAL_EEPROM_HandleTypeDef *eeprom, uint16_t address, uint32_t *data, uint8_t length, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (eeprom->Mode == HAL_EEPROM_MODE_TWO_STAGE)
    {
        eeprom->Instance->EEA = address;
        for (uint32_t i = 0; i < length; i++)
        {
            data[i] = eeprom->Instance->EEDAT;
        }
    }
    else
    {
        
        for (uint32_t i = 0; i < length; i++)
        {
            eeprom->Instance->EEA = address + i * 4;
            if ((status = HAL_EEPROM_WaitBusy(eeprom, timeout)) != HAL_OK)
                return status;
            data[i] = eeprom->Instance->EEDAT;
        }
    }

    return status;
}

void HAL_EEPROM_SetMode(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_ModeTypeDef mode)
{
    eeprom->Mode = mode;

    if (eeprom->Mode == HAL_EEPROM_MODE_THREE_STAGE)
    {
        eeprom->Instance->EECON |= EEPROM_EECON_APBNWS_M;
    }
    else
    {
        eeprom->Instance->EECON &= ~EEPROM_EECON_APBNWS_M;
    }
}

void HAL_EEPROM_SetErrorCorrection(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_ErrorCorrectionTypeDef errorCorrection)
{
    eeprom->ErrorCorrection = errorCorrection;

    if (eeprom->ErrorCorrection == HAL_EEPROM_ECC_DISABLE)
    {
        eeprom->Instance->EECON |= EEPROM_EECON_DISECC_M;
    }
    else
    {
        eeprom->Instance->EECON &= ~EEPROM_EECON_DISECC_M;
    }
}

void HAL_EEPROM_SetInterrupt(HAL_EEPROM_HandleTypeDef *eeprom, HAL_EEPROM_EnableInterruptTypeDef enableInterrupt)
{
    eeprom->EnableInterrupt = enableInterrupt;

    if (eeprom->EnableInterrupt == HAL_EEPROM_SERR_ENABLE)
    {
        eeprom->Instance->EECON |= EEPROM_EECON_IESERR_M;
    }
    else
    {
        eeprom->Instance->EECON &= ~EEPROM_EECON_IESERR_M;
    }
}

const int32_t N_EP_1_denominator = 1000 * 1000 * 1000 / (2 * 1000 * 1000);

const int32_t N_EP_2_reduce = 5 * 1000;
const int32_t N_EP_2_numerator = 15 * 1000 / N_EP_2_reduce;
const int32_t N_EP_2_denominator = 1000 * 1000 * 1000 / N_EP_2_reduce;

void HAL_EEPROM_CalculateTimings(HAL_EEPROM_HandleTypeDef *eeprom, int32_t frequency)
{
    eeprom->Timings.N_LD = N_LD_DEFAULT;
    eeprom->Timings.N_R_2 = N_R_2_DEFAULT;

    eeprom->Timings.N_EP_1 = frequency / N_EP_1_denominator;

    eeprom->Timings.N_EP_2 = (frequency * N_EP_2_numerator) / N_EP_2_denominator;

    int32_t n_r_1_step_1 = (frequency / 1000) * 51;
    int32_t n_r_1_step_2 = n_r_1_step_1 / (1000 * 1000);

    if ((n_r_1_step_1 % (1000 * 1000)) != 0)
    {
        n_r_1_step_2 += 1;
    }

    eeprom->Timings.N_R_1 = n_r_1_step_2;
}

HAL_StatusTypeDef HAL_EEPROM_GetECC(HAL_EEPROM_HandleTypeDef *eeprom, uint16_t address, uint8_t *buf_value_ecc, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;
    eeprom->Instance->EEA = address;
    if (eeprom->Mode == HAL_EEPROM_MODE_THREE_STAGE)
    {
        
        if ((status = HAL_EEPROM_WaitBusy(eeprom, timeout)) != HAL_OK)
            return status;
    }
    *buf_value_ecc = eeprom->Instance->EERB;
    return status;
}