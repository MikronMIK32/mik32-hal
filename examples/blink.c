#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

#define ledPin 14

void main() {
	setPinMode(PORT0, ledPin, MODE1);
	pinMode(GPIO_0, ledPin, OUTPUT);

	while (1) {
		digitalWriteGPIO(GPIO_0, ledPin, HIGH);
		delay(1000);
		digitalWriteGPIO(GPIO_0, ledPin, LOW);
		delay(1000);
	}
}
