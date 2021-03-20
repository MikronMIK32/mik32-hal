#include "../mcuduino.h"

int main()
{

	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);
	setPinMode(PORT2, 2, MODE1);
	setPinMode(PORT2, 3, MODE1);

	pinMode(GPIO_2, 0, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 1, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 2, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 3, GPIO_MODE_OUTPUT);

	while (1) {
		delay(1000);
		digitalWrite(GPIO_2, 0, HIGH);
		digitalWrite(GPIO_2, 1, LOW);
		digitalWrite(GPIO_2, 2, HIGH);
		digitalWrite(GPIO_2, 3, LOW);

		delay(1000);
		digitalWrite(GPIO_2, 0, LOW);
		digitalWrite(GPIO_2, 1, HIGH);
		digitalWrite(GPIO_2, 2, LOW);
		digitalWrite(GPIO_2, 3, HIGH);
	}
}

