#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

#define ledPin 0

void main() {
	setPinFunction(PORT0, ledPin, MODE1);
	GPIO_SetPinDirection(GPIO_2, ledPin, GPIO_OUTPUT);

	while (1) {
		GPIO_PinWrite(GPIO_2, ledPin, GPIO_HIGH);
		delay(1000);
		GPIO_PinWrite(GPIO_2, ledPin, GPIO_LOW);
		delay(1000);
	}
}
