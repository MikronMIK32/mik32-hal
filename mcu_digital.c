#include "mcu_digital.h"


GPIO_DigitalState digitalReadGPIO(GPIO_TypeDef *gpio, uint32_t gpioId) {
	if (gpio->SET & (1 << gpioId)) {
		return HIGH;
	} else {
		return LOW;
	}
}


void digitalWriteGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_DigitalState state) {
	if (state) {
		gpio->SET = 1 << gpioId;
	} else {
		gpio->CLEAR = 1 << gpioId;
	}
}


void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT = 1 << gpioId;
	} else if (mode == INPUT) {
		gpio->DIRECTION_IN = 1 << gpioId;
	}
}

void pinModeGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT = 1 << gpioId;
	} else {
		gpio->DIRECTION_IN = 1 << gpioId;
	}
}

void attachInterruptGPIO(uint32_t irq_line, uint32_t mux,
	GPIO_InterruptMode mode) {
	GPIO_IRQ->CFG = (mux << (irq_line << 2));
	if (mode & MODE_LOW) {
		GPIO_IRQ->EDGE &= ~(1 << irq_line);
		GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & MODE_CHANGE) {
		GPIO_IRQ->EDGE |= (1 << irq_line);
		GPIO_IRQ->LEVEL_SET |= (1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET |= (1 << irq_line);
	}
	GPIO_IRQ->ENABLE_SET = (1 << irq_line);
	EPIC->MASK_SET = 1 << EPIC_GPIO_IRQ_INDEX;
}

