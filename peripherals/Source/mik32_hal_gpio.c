#include "mik32_hal_gpio.h"

HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t *port_cfg = 0;
    uint32_t *port_ds = 0;
    uint32_t *port_pupd = 0;

    uint32_t position = 0;
    uint32_t current_pin = 0;

    GPIO_Init->Pin &= ~PORT_INDEX_M;

    switch ((uint32_t)GPIOx)
    {
    case (uint32_t)GPIO_0:
        port_cfg = (uint32_t *)&(PAD_CONFIG->PORT_0_CFG);
        port_ds = (uint32_t *)&(PAD_CONFIG->PORT_0_DS);
        port_pupd = (uint32_t *)&(PAD_CONFIG->PORT_0_PUPD);
        break;
    case (uint32_t)GPIO_1:
        port_cfg = (uint32_t *)&(PAD_CONFIG->PORT_1_CFG);
        port_ds = (uint32_t *)&(PAD_CONFIG->PORT_1_DS);
        port_pupd = (uint32_t *)&(PAD_CONFIG->PORT_1_PUPD);
        break;
    case (uint32_t)GPIO_2:
        port_cfg = (uint32_t *)&(PAD_CONFIG->PORT_2_CFG);
        port_ds = (uint32_t *)&(PAD_CONFIG->PORT_2_DS);
        port_pupd = (uint32_t *)&(PAD_CONFIG->PORT_2_PUPD);
        break;

    default:
        return HAL_ERROR;
        break;
    }

    while (((GPIO_Init->Pin) >> position) != 0)
    {
        current_pin = GPIO_Init->Pin & (1 << position);

        if (current_pin)
        {
            *port_cfg = (*port_cfg & (~PAD_CONFIG_PIN_M(position))) | PAD_CONFIG_PIN(position, GPIO_Init->Mode & 0b11);

            if ((GPIO_Init->Mode == HAL_GPIO_MODE_GPIO_INPUT) || (GPIO_Init->Mode == HAL_GPIO_MODE_GPIO_OUTPUT))
            {
                if (GPIO_Init->Mode == HAL_GPIO_MODE_GPIO_INPUT)
                {
                    GPIOx->DIRECTION_IN = 1 << position;
                }
                else
                {
                    GPIOx->DIRECTION_OUT = 1 << position;
                }
            }

            *port_ds = (*port_ds & (~PAD_CONFIG_PIN_M(position))) | PAD_CONFIG_PIN(position, GPIO_Init->DS);
            *port_pupd = (*port_pupd & (~PAD_CONFIG_PIN_M(position))) | PAD_CONFIG_PIN(position, GPIO_Init->Pull);
        }

        position++;
    }

    return HAL_OK;
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin)
{
    GPIO_PinState bitstatus;

    GPIO_Pin &= ~PORT_INDEX_M;

    if ((GPIOx->SET & GPIO_Pin) != (uint32_t)GPIO_PIN_LOW)
    {
        bitstatus = GPIO_PIN_HIGH;
    }
    else
    {
        bitstatus = GPIO_PIN_LOW;
    }
    return bitstatus;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_LOW)
    {
        GPIOx->CLEAR = GPIO_Pin;
    }
    else
    {
        GPIOx->SET = GPIO_Pin;
    }
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, HAL_PinMapNewTypeDef GPIO_Pin)
{
    GPIOx->OUTPUT ^= GPIO_Pin;
}

/** Для обхода бага МК, чтение из регистра IRQ_LINE_MUX всегда возвращает 0
 *  \note Используется в функциях HAL_GPIO_InitInterruptLine и HAL_GPIO_DeInitInterruptLine
 */
volatile uint32_t current_irq_line_mux = 0;

HAL_StatusTypeDef HAL_GPIO_InitInterruptLine(HAL_GPIO_Line_Config mux, HAL_GPIO_InterruptMode mode)
{
    int irq_line_num = mux >> GPIO_IRQ_LINE_S;
    mode &= 0b111;
    if (irq_line_num > 7)
        return HAL_ERROR;

    current_irq_line_mux &= ~GPIO_IRQ_LINE_MUX_M(irq_line_num);
    current_irq_line_mux |= GPIO_IRQ_LINE_MUX(mux, irq_line_num);
    GPIO_IRQ->LINE_MUX = current_irq_line_mux;

    if (mode & GPIO_MODE_BIT_LEVEL_M) // GPIO_INT_MODE_HIGH, GPIO_INT_MODE_RISING
    {
        GPIO_IRQ->LEVEL_SET = (1 << irq_line_num); // нарастающий фронт или логический уровень 1
    }
    else
    {
        GPIO_IRQ->LEVEL_CLEAR = (1 << irq_line_num); // спадающий фронт или логический уровень 0
    }

    if (mode & GPIO_MODE_BIT_EDGE_M) // GPIO_INT_MODE_FALLING, GPIO_INT_MODE_RISING, GPIO_INT_MODE_CHANGE
    {
        GPIO_IRQ->EDGE = (1 << irq_line_num); // тип прерывания - событие (фронт/спад)
    }
    else
    {
        GPIO_IRQ->LEVEL = (1 << irq_line_num); // тип прерывания - логический уровень
    }

    if (mode & GPIO_MODE_BIT_ANYEDGE_M) // GPIO_INT_MODE_CHANGE
    {
        GPIO_IRQ->ANY_EDGE_SET = (1 << irq_line_num); // разрешено прерывание по любому фронту
    }
    else
    {
        GPIO_IRQ->ANY_EDGE_CLEAR = (1 << irq_line_num); // запрещено прерывание по любому фронту
    }

    GPIO_IRQ->ENABLE_SET = (1 << irq_line_num);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irq_line)
{
    int irq_line_num = irq_line >> GPIO_IRQ_LINE_S;

    if (irq_line_num > 7)
        return HAL_ERROR;

    GPIO_IRQ->ENABLE_CLEAR = (1 << irq_line_num);

    current_irq_line_mux &= ~GPIO_IRQ_LINE_MUX_M(irq_line_num);
    GPIO_IRQ->LINE_MUX = current_irq_line_mux;

    GPIO_IRQ->LEVEL = (1 << irq_line_num);
    GPIO_IRQ->LEVEL_CLEAR = (1 << irq_line_num);
    GPIO_IRQ->ANY_EDGE_CLEAR = (1 << irq_line_num);

    return HAL_OK;
}

uint32_t HAL_GPIO_LineInterruptState(HAL_GPIO_Line irq_line)
{
    int irq_line_num = irq_line >> GPIO_IRQ_LINE_S;
    return (GPIO_IRQ->INTERRUPT & (1 << (irq_line_num))) != 0;
}

GPIO_PinState HAL_GPIO_LinePinState(HAL_GPIO_Line irq_line)
{
    int irq_line_num = irq_line >> GPIO_IRQ_LINE_S;
    return (GPIO_PinState)((GPIO_IRQ->STATE & (1 << (irq_line_num))) >> irq_line_num);
}

void HAL_GPIO_ClearInterrupts()
{
    GPIO_IRQ->CLEAR = 0b11111111;
}
