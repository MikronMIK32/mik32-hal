#include "mcu_digital.h"


GPIO_PinState GPIO_PinRead(GPIO_TypeDef *gpio, uint32_t gpioNum) {
	return (gpio->SET & (1 << gpioNum));
	if (gpio->SET & (1 << gpioNum)) {
		return GPIO_HIGH;
	} else {
		return GPIO_LOW;
	}
}


void GPIO_PinWrite(GPIO_TypeDef *gpio, uint32_t gpioNum, GPIO_PinState state) {
	if (state) {
		gpio->SET = 1 << gpioNum;
	} else {
		gpio->CLEAR = 1 << gpioNum;
	}
}

void GPIO_PinToggle(GPIO_TypeDef *gpio, uint32_t gpioNum) {
	gpio->OUTPUT ^= 1 << gpioNum;
}


void GPIO_SetPinDirection(GPIO_TypeDef *gpio, uint32_t gpioNum, GPIO_PinDirection dir) {
	if (dir == GPIO_DIR_OUTPUT) {
		gpio->DIRECTION_OUT = 1 << gpioNum;
	}
	if (dir == GPIO_DIR_INPUT) {
		gpio->DIRECTION_IN = 1 << gpioNum;
	}
}


void GPIO_InitInterruptLine(GPIO_Line irq_line, GPIO_Line_Mux mux,
	GPIO_InterruptMode mode) {
	if (irq_line > 7) return;

	GPIO_IRQ->CFG &= ~(0b1111 << (irq_line << 2));
	GPIO_IRQ->CFG |= (mux << (irq_line << 2));

	if (mode & GPIO_IT_MODE_LOW) {
		GPIO_IRQ->EDGE &= ~(1 << irq_line);
		GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & GPIO_IT_MODE_CHANGE) {
		GPIO_IRQ->EDGE |= (1 << irq_line);
		GPIO_IRQ->LEVEL_SET |= (1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET |= (1 << irq_line);
	} else if (mode & GPIO_IT_MODE_RISING) {
		GPIO_IRQ->EDGE |= (1 << irq_line);
		GPIO_IRQ->LEVEL_SET |= (1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & GPIO_IT_MODE_FALLING) {
		GPIO_IRQ->EDGE |= (1 << irq_line);
		GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & GPIO_IT_MODE_HIGH) {
		GPIO_IRQ->EDGE &= ~(1 << irq_line);
		GPIO_IRQ->LEVEL_SET |= (1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	}

	GPIO_IRQ->ENABLE_SET |= (1 << irq_line);
	EPIC->MASK_SET |= 1 << EPIC_GPIO_IRQ_INDEX;
}


void GPIO_DeInitInterruptLine(GPIO_Line irq_line) {
	if (irq_line > 7) return;

	GPIO_IRQ->CFG &= ~(0b1111 << (irq_line << 2));

	GPIO_IRQ->EDGE &= ~(1 << irq_line);
	GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
	GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);

	GPIO_IRQ->ENABLE_SET &= ~(1 << irq_line);
}


void GPIO_ClearInterrupt() {
	GPIO_IRQ->CLEAR = 0b11111111;
}

