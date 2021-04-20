#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"

void pwmTick() {
	static volatile uint16_t counter = 0;
	if (counter == 0) {
		digitalWriteGPIO(GPIO_2, 0, HIGH);
	}
	if (counter == 1) {
		digitalWriteGPIO(GPIO_2, 0, LOW);
	}
	counter++;
}

void TIMER32_0_TRAP_HANDLER() {

	TIMER32_0->IntClear = TIMER32_INT_OVERFLOW_M;

	pwmTick();

	//TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	TIMER32_0->IntClear = TIMER32_INT_OVERFLOW_M;
}

int main()
{
	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);
	pinMode(GPIO_2, 0, OUTPUT);
	pinMode(GPIO_2, 1, OUTPUT);
	digitalWriteGPIO(GPIO_2, 0, LOW);

	interrupts();

	TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	TIMER32_0->IntClear = TIMER32_INT_OVERFLOW_M;

	TIMER32_0->Top = 1000;
	TIMER32_0->IntMask = TIMER32_INT_OVERFLOW_M;
	EPIC->MASK_SET |= 1 << EPIC_TIMER32_0_INDEX;
	TIMER32_0->Enable = TIMER32_ENABLE_M;

	/*
	while (TIMER32_0->IntFlags == 0);

	TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	TIMER32_0->IntClear = TIMER32_INT_OVERFLOW_M;*/

	uint32_t st = LOW;
	while (1) {
		delay(1000);
		digitalWriteGPIO(GPIO_2, 1, st);
		if (st == HIGH) {
			st = LOW;
		} else if (st == LOW) {
			st = HIGH;
		}
		//pwmTick();
	}
}
