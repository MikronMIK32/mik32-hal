#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

#define ledPin 0

void main() {
	setPinFunction(GPIO_2, ledPin, PIN_FUNCTION_GPIO);
	GPIO_SetPinDirection(GPIO_2, ledPin, GPIO_OUTPUT);

	while (1) {
		GPIO_PinToggle(GPIO_2, ledPin);
		delay(1000);
	}
}
