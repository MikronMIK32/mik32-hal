#include "../mcuduino.h"

void my_handler(uint32_t interrupt, uint32_t states) {
	if (interrupt & (1 << 7)) {
		digitalWrite(GPIO_2, 0, states & (1 << 7));
	}
}

int main()
{
	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);

	setPinMode(PORT2, 7, MODE1);

	pinMode(GPIO_2, 0, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 1, GPIO_MODE_OUTPUT);
	digitalWrite(GPIO_2, 0, LOW);

	pinMode(GPIO_2, 7, GPIO_MODE_INPUT);

	attachInterrupt(7, 4, my_handler, LEVEL_CHANGE);

	interrupts();

	while (1) {
		delay(1000);
		digitalWrite(GPIO_2, 1, LOW);

		delay(1000);
		digitalWrite(GPIO_2, 1, HIGH);
	}
}


