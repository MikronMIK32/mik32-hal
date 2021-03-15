#include "mcu_digital.h"

void digitalWrite(GPIO_TypeDef *gpio, uint32_t gpioId, DigitalState state) {
	if (state == HIGH) {
		gpio->OUTPUT |= 1 << gpioId;
	} else {
		gpio->OUTPUT &= ~(1 << gpioId);
	}
}

DigitalState digitalRead(GPIO_TypeDef *gpio, uint32_t gpioId) {
	if (gpio->SET & (1 << gpioId)) {
		return HIGH;
	} else {
		return LOW;
	}
}


/*Аналог arduino функции pinMode, устанавливает режим ввода или вывода,
 *Внимание! Не поддерживает INPUT_PULLUP!
 */
void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, PinMode mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT |= 1 << gpioId;
	} else {
		gpio->DIRECTION_IN |= 1 << gpioId;
	}
}


void attachInterrupt(uint32_t irq_line, void* irq, GPIO_InterruptMode mode) {

}

