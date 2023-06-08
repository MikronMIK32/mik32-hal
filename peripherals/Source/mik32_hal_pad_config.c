#include "mik32_hal_pad_config.h"


HAL_StatusTypeDef HAL_PadConfig_SetPortReg(uint32_t* PortReg, HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, uint32_t Mode)
{
    Mode &= MODE_MASK;
    PinMask &= HAL_PIN_MASK;
    uint32_t PinMax = 0;

    switch (Port)
    {
    case PORT0:
        PinMax = 15;
        break;
    case PORT1:
        PinMax = 15;
        break;
    case PORT2:
        PinMax = 7;
        break;
    
    default:
        return HAL_ERROR;
        break;
    }

    uint32_t config = *PortReg;

    for (uint32_t pin = 0; pin <= PinMax; pin++)
    {
        if (PinMask & (1 << pin))
        {
            config &= ~(0b11 << (pin << 1));
            config |= Mode << (pin << 1);
        }
    }
    
    *PortReg = config;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_PadConfig_PinMode(HAL_PinMapTypeDef PinMask, HAL_PinModeTypeDef PinMode)
{
    uint32_t port = PinMask & ~HAL_PIN_MASK;    /* Номер порта */
    return HAL_PadConfig_PortMode(port, PinMask, PinMode);
}

HAL_StatusTypeDef HAL_PadConfig_PortMode(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinModeTypeDef PinMode)
{
    switch (Port)
    {
    case PORT0:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_0_CFG), Port, PinMask, PinMode);
        break;
    case PORT1:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_1_CFG), Port, PinMask, PinMode);
        break;
    case PORT2:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_2_CFG), Port, PinMask, PinMode);
        break;
    
    default:
        return HAL_ERROR;
        break;
    }
}

HAL_StatusTypeDef HAL_PadConfig_PinPull(HAL_PinMapTypeDef PinMask, HAL_PinPullModeTypeDef PullMode)
{
    uint32_t port = PinMask & ~HAL_PIN_MASK;    /* Номер порта */
    return HAL_PadConfig_PortPull(port, PinMask, PullMode);
}

HAL_StatusTypeDef HAL_PadConfig_PortPull(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinPullModeTypeDef PullMode)
{
    switch (Port)
    {
    case PORT0:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_0_PUD), Port, PinMask, PullMode);
        break;
    case PORT1:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_1_PUD), Port, PinMask, PullMode);
        break;
    case PORT2:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_2_PUD), Port, PinMask, PullMode);
        break;
    
    default:
        return HAL_ERROR;
        break;
    }
}

HAL_StatusTypeDef HAL_PadConfig_PinCurrent(HAL_PinMapTypeDef PinMask, HAL_PinCurrentModeTypeDef Current)
{
    uint32_t port = PinMask & ~HAL_PIN_MASK;    /* Номер порта */
    return HAL_PadConfig_PortCurrent(port, PinMask, Current);
}

HAL_StatusTypeDef HAL_PadConfig_PortCurrent(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinCurrentModeTypeDef Current)
{
    switch (Port)
    {
    case PORT0:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_0_DS), Port, PinMask, Current);
        break;
    case PORT1:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_1_DS), Port, PinMask, Current);
        break;
    case PORT2:
        return HAL_PadConfig_SetPortReg((uint32_t*) &(PAD_CONFIG->PORT_2_DS), Port, PinMask, Current);
        break;
    
    default:
        return HAL_ERROR;
        break;
    }
}


