#include "mik32_hal_gpio.h"

HAL_StatusTypeDef HAL_GPIO_PinFullInit(HAL_PinMapTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection,
                                       HAL_PinLevelTypeDef PinLevel, HAL_PinPullModeTypeDef PullMode,
                                       HAL_PinCurrentModeTypeDef Current)
{
    HAL_StatusTypeDef status = HAL_OK;
    status |= HAL_PadConfig_PinMode(PinMask, PIN_MODE1);
    status |= HAL_PadConfig_PinPull(PinMask, PullMode);
    status |= HAL_PadConfig_PinCurrent(PinMask, Current);

    status |= HAL_GPIO_PinDirection(PinMask, PinDirection);

    return status;
}

HAL_StatusTypeDef HAL_GPIO_PinInputInit(HAL_PinMapTypeDef PinMask, HAL_PinPullModeTypeDef PullMode)
{
    if (HAL_PadConfig_PinMode(PinMask, PIN_MODE1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    HAL_PadConfig_PinPull(PinMask, PullMode);

    if (HAL_GPIO_PinDirection(PinMask, PIN_INPUT) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_PinOutputInit(HAL_PinMapTypeDef PinMask, HAL_PinLevelTypeDef PinLevel)
{
    if (HAL_PadConfig_PinMode(PinMask, PIN_MODE1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    HAL_PadConfig_PinPull(PinMask, PIN_PULL_NONE);

    if (HAL_GPIO_PinDirection(PinMask, PIN_OUTPUT) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_PinDirection(HAL_PinMapTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection)
{
    uint32_t Port = PinMask & ~HAL_PIN_MASK;
    return HAL_GPIO_PortDirection(Port, PinMask, PinDirection);
}

HAL_StatusTypeDef HAL_GPIO_PortDirection(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinDirectionTypeDef PinDirection)
{
    GPIO_TypeDef *port;

    switch (Port)
    {
    case PORT0:
        port = GPIO_0;
        break;
    case PORT1:
        port = GPIO_1;
        break;
    case PORT2:
        port = GPIO_2;
        break;

    default:
        return HAL_ERROR;
    }

    if (PinDirection == PIN_INPUT)
    {
        port->DIRECTION_IN = PinMask & HAL_PIN_MASK;
    }
    else if (PinDirection == PIN_OUTPUT)
    {
        port->DIRECTION_OUT = PinMask & HAL_PIN_MASK;
    }
    else
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_PinWrite(HAL_PinMapTypeDef PinMask, HAL_PinLevelTypeDef PinLevel)
{
    uint32_t Port = PinMask & ~HAL_PIN_MASK;
    GPIO_TypeDef *port;

    switch (Port)
    {
    case PORT0:
        port = GPIO_0;
        break;
    case PORT1:
        port = GPIO_1;
        break;
    case PORT2:
        port = GPIO_2;
        break;

    default:
        return HAL_ERROR;
    }

    if (PinLevel == PIN_LOW)
    {
        port->CLEAR = PinMask & HAL_PIN_MASK;
    }
    else
    {
        port->SET = PinMask & HAL_PIN_MASK;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_PortWrite(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask, HAL_PinLevelTypeDef PinLevel)
{
    GPIO_TypeDef *port;

    switch (Port)
    {
    case PORT0:
        port = GPIO_0;
        break;
    case PORT1:
        port = GPIO_1;
        break;
    case PORT2:
        port = GPIO_2;
        break;

    default:
        return HAL_ERROR;
    }

    if (PinLevel == PIN_LOW)
    {
        port->CLEAR = PinMask & HAL_PIN_MASK;
    }
    else
    {
        port->SET = PinMask & HAL_PIN_MASK;
    }

    return HAL_OK;
}

HAL_PinLevelTypeDef HAL_GPIO_PinRead(HAL_PinMapTypeDef Pin)
{
    uint32_t Port = Pin & ~HAL_PIN_MASK;

    switch (Port)
    {
    case PORT0:
        return GPIO_0->SET & Pin;
    case PORT1:
        return GPIO_1->SET & Pin;
    case PORT2:
        return GPIO_2->SET & Pin;

    default:
        return 0;
    }
}

uint16_t HAL_GPIO_PortRead(HAL_PortTypeDef Port)
{
    switch (Port)
    {
    case PORT0:
        return GPIO_0->SET;
    case PORT1:
        return GPIO_1->SET;
    case PORT2:
        return GPIO_2->SET;

    default:
        return 0;
    }
}

HAL_StatusTypeDef HAL_GPIO_PinToggle(HAL_PinMapTypeDef PinMask)
{
    uint32_t Port = PinMask & ~HAL_PIN_MASK;
    return HAL_GPIO_PortToggle(Port, PinMask);
}

HAL_StatusTypeDef HAL_GPIO_PortToggle(HAL_PortTypeDef Port, HAL_PinTypeDef PinMask)
{
    switch (Port)
    {
    case PORT0:
        GPIO_0->OUTPUT ^= PinMask & HAL_PIN_MASK;
        break;
    case PORT1:
        GPIO_1->OUTPUT ^= PinMask & HAL_PIN_MASK;
        break;
    case PORT2:
        GPIO_2->OUTPUT ^= PinMask & HAL_PIN_MASK;
        break;

    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}

/** Для обхода бага МК, чтение из регистра IRQ_LINE_MUX всегда возвращает 0
 *  \note Используется в функциях HAL_GPIO_InitInterruptLine и HAL_GPIO_DeInitInterruptLine
 */
uint32_t current_irq_line_mux = 0;

HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line irq_line, HAL_GPIO_Line_Mux mux,
                            HAL_GPIO_InterruptMode mode)
{
    if (irq_line > 7)
        return HAL_ERROR;

    current_irq_line_mux &= ~(0b1111 << (irq_line << 2));
    current_irq_line_mux |= (mux << (irq_line << 2));
    GPIO_IRQ->CFG = current_irq_line_mux;

    if (mode & GPIO_MODE_BIT_LEVEL_M)
    {
        GPIO_IRQ->LEVEL_SET = (1 << irq_line);
    }
    else
    {
        GPIO_IRQ->LEVEL_CLEAR = (1 << irq_line);
    }

    if (mode & GPIO_MODE_BIT_EDGE_M)
    {
        GPIO_IRQ->EDGE |= (1 << irq_line);
    }
    else
    {
        GPIO_IRQ->EDGE &= ~(1 << irq_line);
    }

    if (mode & GPIO_MODE_BIT_ANYEDGE_M)
    {
        GPIO_IRQ->ANYEDGE_SET = (1 << irq_line);
    }
    else
    {
        GPIO_IRQ->ANYEDGE_CLEAR = (1 << irq_line);
    }

    GPIO_IRQ->ENABLE_SET = (1 << irq_line);

    return HAL_OK;
}

void HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irq_line)
{
    if (irq_line > 7)
        return;

    current_irq_line_mux &= ~(0b1111 << (irq_line << 2));
    GPIO_IRQ->CFG = current_irq_line_mux;

    GPIO_IRQ->EDGE &= ~(1 << irq_line);
    GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
    GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);

    GPIO_IRQ->ENABLE_SET &= ~(1 << irq_line);
}

HAL_PinLevelTypeDef HAL_GPIO_LineInterruptState(HAL_GPIO_Line irq_line)
{
    return (GPIO_IRQ->INTERRUPTS & (1 << (irq_line))) != 0;
}

HAL_PinLevelTypeDef HAL_GPIO_LinePinState(HAL_GPIO_Line irq_line)
{
    return GPIO_IRQ->STATE & (1 << (irq_line));
}

void HAL_GPIO_ClearInterrupts()
{
    GPIO_IRQ->CLEAR = 0b11111111;
}
