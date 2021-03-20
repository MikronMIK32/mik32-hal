#include "../mcuduino.h"

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

	pinMode(GPIO_2, 0, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 1, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 2, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 3, GPIO_MODE_OUTPUT);

	pinMode(GPIO_2, 4, GPIO_MODE_INPUT);
	pinMode(GPIO_2, 5, GPIO_MODE_INPUT);
	pinMode(GPIO_2, 6, GPIO_MODE_INPUT);
	pinMode(GPIO_2, 7, GPIO_MODE_INPUT);

	while (1) {
		delay(100);
		digitalWrite(GPIO_2, 0, digitalRead(GPIO_2, 4));
		digitalWrite(GPIO_2, 1, digitalRead(GPIO_2, 5));
		digitalWrite(GPIO_2, 2, digitalRead(GPIO_2, 6));
		digitalWrite(GPIO_2, 3, digitalRead(GPIO_2, 7));
	}
}
