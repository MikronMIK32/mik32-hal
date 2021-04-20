#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"

void pwmTick() {
	static volatile uint8_t counter = 0;
	if (counter == 0) {
		digitalWriteGPIO(GPIO_2, 0, HIGH);
	}
	if (counter == 3) {
		digitalWriteGPIO(GPIO_2, 0, LOW);
	}
	counter++;
}

int main()
{
	setPinMode(PORT2, 0, MODE1);
	pinMode(GPIO_2, 0, OUTPUT);

	uint32_t st = LOW;
	while (1) {
		pwmTick();
	}
}
