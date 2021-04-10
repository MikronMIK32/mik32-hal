#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

int main()
{
	noInterrupts();

	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);
	setPinMode(PORT2, 2, MODE1);
	setPinMode(PORT2, 3, MODE1);

	setPinMode(PORT2, 4, MODE1);
	setPinMode(PORT2, 5, MODE1);
	setPinMode(PORT2, 6, MODE1);
	setPinMode(PORT2, 7, MODE1);

	pinModeGPIO(GPIO_2, 0, OUTPUT);
	pinModeGPIO(GPIO_2, 1, OUTPUT);
	pinModeGPIO(GPIO_2, 2, OUTPUT);
	pinModeGPIO(GPIO_2, 3, OUTPUT);

	pinModeGPIO(GPIO_2, 4, INPUT);
	pinModeGPIO(GPIO_2, 5, INPUT);
	pinModeGPIO(GPIO_2, 6, INPUT);
	pinModeGPIO(GPIO_2, 7, INPUT);

	while (1) {
		delay(100);
		digitalWriteGPIO(GPIO_2, 0, digitalReadGPIO(GPIO_2, 4));
		digitalWriteGPIO(GPIO_2, 1, digitalReadGPIO(GPIO_2, 5));
		digitalWriteGPIO(GPIO_2, 2, digitalReadGPIO(GPIO_2, 6));
		digitalWriteGPIO(GPIO_2, 3, digitalReadGPIO(GPIO_2, 7));
	}
}
