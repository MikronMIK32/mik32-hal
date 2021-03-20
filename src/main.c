#include "../mcuduino.h"

int main()
{

	pinMode(32, OUTPUT);
	pinMode(33, OUTPUT);
	pinMode(34, OUTPUT);
	pinMode(35, OUTPUT);

	while (1) {
		delay(1000);
		digitalWrite(32, HIGH);
		digitalWrite(33, LOW);
		digitalWrite(34, HIGH);
		digitalWrite(35, LOW);

		delay(1000);
		digitalWrite(32, LOW);
		digitalWrite(33, HIGH);
		digitalWrite(34, LOW);
		digitalWrite(35, HIGH);
	}
}

