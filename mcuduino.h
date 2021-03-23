#ifndef MCUDUINO_H_INCLUDED
#define MCUDUINO_H_INCLUDED

//Выводы нумеруются от 0 до 39, начиная с GPIO_0_0

#include "mcu_core.h"
#include "mcu_digital.h"
#include "mcu_analog.h"
#include "mcu_timer.h"


void setup();

inline void loop();

int main() {

	// INIT GPIO

	Port0_As_Gpio();
	Port1_As_Gpio();
	Port2_As_Gpio();
	GPIO_0->DIRECTION_IN = 0xFFFFFFFF;
	GPIO_1->DIRECTION_IN = 0xFFFFFFFF;
	GPIO_2->DIRECTION_IN = 0xFFFFFFFF;

	// END INIT GPIO

	interrupts();

	setup();

	while (1) {
		loop();
	}
}

#endif
