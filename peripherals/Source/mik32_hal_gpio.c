#include "mik32_hal_gpio.h"

/** Для обхода бага МК, чтение из регистра IRQ_LINE_MUX всегда возвращает 0
 *  \note Используется в функциях HAL_GPIO_InitInterruptLine и HAL_GPIO_DeInitInterruptLine
 */
volatile uint32_t current_irq_line_mux = 0;

/**
 * @brief Инициализация модуля GPIO_x в соответствии с указанными параметрами в GPIO_Init.
 * @param GPIO_x порт GPIO_x, где x может быть (0, 1, 2).
 * @param GPIO_Init указатель на структуру GPIO_InitTypeDef, которая содержит информацию о конфигурации для указанного модуля GPIO.
 * @return статус HAL.
 * @warning функция не включает тактирование выбранного модуля GPIO. Включить тактирование можно, например, с помощью макроса @ref __HAL_PCC_GPIO_2_CLK_ENABLE.
 */
HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef *GPIO_x, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t *port_cfg = 0;
    uint32_t *port_ds = 0;
    uint32_t *port_pupd = 0;

    uint32_t position = 0;
    uint32_t current_pin = 0;

    switch ((uint32_t)GPIO_x)
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
                    GPIO_x->DIRECTION_IN = 1 << position;
                }
                else
                {
                    GPIO_x->DIRECTION_OUT = 1 << position;
                }
            }

            *port_ds = (*port_ds & (~PAD_CONFIG_PIN_M(position))) | PAD_CONFIG_PIN(position, GPIO_Init->DS);
            *port_pupd = (*port_pupd & (~PAD_CONFIG_PIN_M(position))) | PAD_CONFIG_PIN(position, GPIO_Init->Pull);
        }

        position++;
    }

    return HAL_OK;
}

/**
 * @brief Инициализация модуля GPIO_x.
 * @param GPIO_x порт GPIO_x, где x может быть (0, 1, 2).
 * @param pin маска выводов порта GPIO_x, к которым применяются указанные настройки.
 * @param mode режим вывода.
 * @param pull режим подтяжки вывода.
 * @param driveStrength перечисление режимов нагрузочной способности.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_GPIO_PinConfig(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin, HAL_GPIO_ModeTypeDef mode, HAL_GPIO_PullTypeDef pull, HAL_GPIO_DSTypeDef driveStrength)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.DS = driveStrength;
    return HAL_GPIO_Init(GPIO_x, &GPIO_InitStruct);
}

/**
 * @brief Считать текущее состояние выводов порта GPIO_x.
 * @param GPIO_x порт GPIO_x, где x может быть (0, 1, 2).
 * @param pin маска выводов порта GPIO_x, с которых считывание значение.
 * @return @ref GPIO_PIN_HIGH если с одного или больше выводов, указанных в pin, считалась 1. Иначе @ref GPIO_PIN_LOW.
 */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin)
{
    GPIO_PinState bitStatus;

    if ((GPIO_x->SET & pin) != (uint32_t)GPIO_PIN_LOW)
    {
        bitStatus = GPIO_PIN_HIGH;
    }
    else
    {
        bitStatus = GPIO_PIN_LOW;
    }
    return bitStatus;
}

/**
 * @brief Задать логический уровень выходного сигнала для указанных выводов порта GPIO_x.
 * @param GPIO_x порт GPIO_x, где x может быть (0, 1, 2).
 * @param pin маска выводов порта GPIO_x, к которым применяются указанные настройки.
 * @param pinState значение состояние вывода, в которое будут установлены указанные выводы.
 *                  Этот параметр должен быть одним из значений:
 * 		                - @ref GPIO_PIN_LOW низкий выходной уровень
 * 		                - @ref GPIO_PIN_HIGH высокий выходной уровень
 */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin, GPIO_PinState pinState)
{
    if (pinState == GPIO_PIN_LOW)
    {
        GPIO_x->CLEAR = pin;
    }
    else
    {
        GPIO_x->SET = pin;
    }
}

/**
 * @brief Переключить логический уровень выходного сигнала для указанных выводов порта GPIO_x.
 * @param GPIO_x порт GPIO_x, где x может быть (0, 1, 2).
 * @param pin маска выводов порта GPIO_x, к которым применяются указанные настройки.
 */
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO_x, HAL_PinsTypeDef pin)
{
    GPIO_x->OUTPUT ^= pin;
}

/** 
 *  @brief Функция инициализации линии прерывания.
 * \param mux настройка мультиплексора линии прерывания.
 * \param mode режим линии прерывания.
 * @return Статус HAL.
 * \note Номер линии прерывания можно получить после настройки мультиплексора.
 * Введите в mux GPIO_MUX_GPIO_X_X, где X - номера порта и вывода,
 * и нажмите Ctrl+Пробел: появятся варианты констант для данного вывода,
 * далее достаточно выбрать константу для доступной линии.
 * В mode введите GPIO_INT_MODE и нажмите Ctrl+Пробел: появятся варианты типов прерываний канала.
 */
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

/**
 * @brief Функция деинициализации линии прерывания, запрещает прерывание и возвращает настройки по умолчанию для указанной лини.
 * @param irqLine номер линии прерывания.
 * @return Статус HAL.
 */
HAL_StatusTypeDef HAL_GPIO_DeInitInterruptLine(HAL_GPIO_Line irqLine)
{
    int irq_line_num = irqLine >> GPIO_IRQ_LINE_S;

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

/** 
 * @brief Получить состояние линии прерывания.
 * @param irqLine номер линии прерывания.
 * @return Возвращает 1 если сработало прерывание данной линии, иначе 0.
 */
uint32_t HAL_GPIO_LineInterruptState(HAL_GPIO_Line irqLine)
{
    int irq_line_num = irqLine >> GPIO_IRQ_LINE_S;
    return (GPIO_IRQ->INTERRUPT & (1 << (irq_line_num))) != 0;
}

/**
 * @brief Функция чтения логического уровня вывода, подключенного к линии прерывания.
 * @param irqLine номер линии прерывания.
 * @return Логический уровень вывода.
 */
GPIO_PinState HAL_GPIO_LinePinState(HAL_GPIO_Line irqLine)
{
    int irq_line_num = irqLine >> GPIO_IRQ_LINE_S;
    return (GPIO_PinState)((GPIO_IRQ->STATE & (1 << (irq_line_num))) >> irq_line_num);
}

/** 
 *  @brief Функция сброса регистра состояния прерываний.
 *  @note Когда срабатывает прерывание на одной из линии, в регистре INTERRUPT
 *  выставляется 1 в разряде, соответствующем линии прерывания.
 *  После обработки прерывания необходимо сбросить данный регистр
 *  в обработчике прерывания trap_handler().
 *  Если после обработки прерывания регистр не был сброшен,
 *  обработчик будет вызван снова, программа будет бесконечно вызывать обработчик.
 */
void HAL_GPIO_ClearInterrupts()
{
    GPIO_IRQ->CLEAR = 0b11111111;
}
