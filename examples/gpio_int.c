#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

void GPIO_IRQ_TRAP_HANDLER() {
	if (GPIO_IRQ->INTERRUPTS & (1 << 7)) {
		digitalWriteGPIO(GPIO_2, 0, GPIO_IRQ->STATE & (1 << 7));
	}
	GPIO_IRQ->CLEAR = 0xFF;
}

int main()
{
	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);
	setPinMode(PORT2, 7, MODE1);
	pinMode(GPIO_2, 0, OUTPUT);
	pinMode(GPIO_2, 1, OUTPUT);
	digitalWriteGPIO(GPIO_2, 0, LOW);

	pinMode(GPIO_2, 7, INPUT);
	attachInterruptGPIO(7, 4, MODE_CHANGE);
	interrupts();

	while (1) {
		delay(1000);
		digitalWriteGPIO(GPIO_2, 1, LOW);
		delay(1000);
		digitalWriteGPIO(GPIO_2, 1, HIGH);
	}
}
