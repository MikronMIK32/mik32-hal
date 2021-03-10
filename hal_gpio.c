#include "hal_gpio.h"

// HAL

void digitalWrite(GPIO_TypeDef *gpio, uint32_t gpioId, DigitalState state) {
	if (state == HIGH) {
		gpio->OUTPUT |= 1 << gpioId;
	} else {
		gpio->OUTPUT &= ~(1 << gpioId);
	}
}

bool setPinMode(GPIO_TypeDef *gpio, uint32_t gpioId, uint32_t mode) {
	uint32_t first = ~(3 << (gpioId << 1));
	uint32_t second = mode << (gpioId << 1);

	if (gpio == GPIO_0) {
		PAD_CONFIG->PORT_0_CFG &= first;
		PAD_CONFIG->PORT_0_CFG |= second;

		return true;
	}
	if (gpio == GPIO_1) {
		PAD_CONFIG->PORT_1_CFG &= first;
		PAD_CONFIG->PORT_1_CFG |= second;

		return true;
	}
	if (gpio == GPIO_2) {
		PAD_CONFIG->PORT_2_CFG &= first;
		PAD_CONFIG->PORT_2_CFG |= second;

		return true;
	}

	return false; // Error
}

void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, PinMode_TypeDef mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT |= 1 << gpioId;
	} else {
		gpio->DIRECTION_IN |= 1 << gpioId;
	}
}

// END HAL
