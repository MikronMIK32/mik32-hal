#include "../mcuduino.h"

#define ledPin 32

void setup() {
	pinMode(32, OUTPUT);
}

inline void loop() {
	digitalWrite(ledPin, HIGH);
	delay(1000);
	digitalWrite(ledPin, LOW);
	delay(1000);
}
