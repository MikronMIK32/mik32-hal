#include <mcu_timer32.h>
#include "../../../../mcu32-hal-main/src/mcu_core.h"
#include "../../../../mcu32-hal-main/src/mcu_digital.h"

/**
 * \file Пример работы с портом ввода-вывода с мигающим светодиодом
 */

#define LED_PIN 0

void main() {
	setPinFunction(GPIO_2, LED_PIN, PIN_FUNCTION_GPIO);
	GPIO_SetPinDirection(GPIO_2, LED_PIN, GPIO_OUTPUT);

	while (1) {
		GPIO_PinToggle(GPIO_2, LED_PIN);
		delay(1000);
	}
}
