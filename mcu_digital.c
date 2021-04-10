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


