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

	pinMode(GPIO_2, 0, OUTPUT);
	pinMode(GPIO_2, 1, OUTPUT);
	pinMode(GPIO_2, 2, OUTPUT);
	pinMode(GPIO_2, 3, OUTPUT);

	pinMode(GPIO_2, 4, INPUT);
	pinMode(GPIO_2, 5, INPUT);
	pinMode(GPIO_2, 6, INPUT);
	pinMode(GPIO_2, 7, INPUT);

	while (1) {
		delay(100);
		digitalWrite(GPIO_2, 0, digitalRead(GPIO_2, 4));
		digitalWrite(GPIO_2, 1, digitalRead(GPIO_2, 5));
		digitalWrite(GPIO_2, 2, digitalRead(GPIO_2, 6));
		digitalWrite(GPIO_2, 3, digitalRead(GPIO_2, 7));
	}
}
