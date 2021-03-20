#include "mcu_digital.h"


GPIO_DigitalState digitalRead(uint32_t gpioId) {
	if (gpioId < 16) {
		if (GPIO_0->SET & (1 << gpioId)) {
			return HIGH;
		} else {
			return LOW;
		}
	} else if (gpioId < 32) {
		if (GPIO_1->SET & (1 << (gpioId & 15))) {
			return HIGH;
		} else {
			return LOW;
		}
	} else {
		if (GPIO_2->SET & (1 << (gpioId & 15))) {
			return HIGH;
		} else {
			return LOW;
		}
	}
}

GPIO_DigitalState digitalReadGPIO(GPIO_TypeDef *gpio, uint32_t gpioId) {
	if (gpio->SET & (1 << gpioId)) {
		return HIGH;
	} else {
		return LOW;
	}
}


void digitalWrite(uint32_t gpioId, GPIO_DigitalState state) {
	if (gpioId < 16) {
		if (state) {
			GPIO_0->SET = (1 << gpioId);
		} else {
			GPIO_0->CLEAR = (1 << gpioId);
		}
	} else if (gpioId < 32) {
		if (state) {
			GPIO_1->SET = (1 << gpioId & 15);
		} else {
			GPIO_1->CLEAR = (1 << gpioId & 15);
		}
	} else {
		if (state) {
			GPIO_2->SET = (1 << gpioId & 15);
		} else {
			GPIO_2->CLEAR = (1 << gpioId & 15);
		}
	}
}

void digitalWriteGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_DigitalState state) {
	if (state) {
		gpio->SET = 1 << gpioId;
	} else {
		gpio->CLEAR = 1 << gpioId;
	}
}


void pinMode(uint32_t gpioId, GPIO_PinMode mode) {
	if (gpioId < 16) {
		if (mode == OUTPUT) {
			GPIO_0->DIRECTION_OUT |= 1 << gpioId;
		} else {
			GPIO_0->DIRECTION_IN |= 1 << gpioId;
		}
	} else if (gpioId < 32) {
		if (mode == OUTPUT) {
			GPIO_1->DIRECTION_OUT |= 1 << (1 << gpioId & 15);
		} else {
			GPIO_1->DIRECTION_IN |= 1 << (1 << gpioId & 15);
		}
	} else {
		if (mode == OUTPUT) {
			GPIO_2->DIRECTION_OUT |= 1 << (1 << gpioId & 15);
		} else {
			GPIO_2->DIRECTION_IN |= 1 << (1 << gpioId & 15);
		}
	}
}

void pinModeGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT = 1 << gpioId;
	} else {
		gpio->DIRECTION_IN = 1 << gpioId;
	}
}


