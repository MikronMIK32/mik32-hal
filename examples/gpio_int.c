#include "mcu_core.h"
#include "mcu_digital.h"
#include "mcu_timer.h"

/**
 * \file Пример работы с прерываниями портов ввода-вывода
 */

void GPIO_IRQ_TRAP_HANDLER() {
	if (GPIO_LineInterruptState(7)) {
		GPIO_PinWrite(GPIO_2, 0, GPIO_LinePinState(7));
	}
	if (GPIO_LineInterruptState(6)) {
		GPIO_PinWrite(GPIO_2, 1, GPIO_HIGH);
	}
	if (GPIO_LineInterruptState(2)) {
		GPIO_PinWrite(GPIO_2, 1, GPIO_LOW);
	}
	if (GPIO_LineInterruptState(5)) {
		GPIO_PinWrite(GPIO_2, 2, GPIO_HIGH);
	}
	if (GPIO_LineInterruptState(4)) {
		GPIO_PinWrite(GPIO_2, 2, GPIO_LOW);
	}
	GPIO_ClearInterrupt();
}

int main()
{
	setPortFunction(GPIO_2, PIN_FUNCTION_GPIO);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_0 | PIN_1 | PIN_2 | PIN_3, GPIO_DIR_OUTPUT);

	GPIO_PinWrite(GPIO_2, 0, GPIO_LOW);
	GPIO_PinWrite(GPIO_2, 1, GPIO_LOW);
	GPIO_PinWrite(GPIO_2, 2, GPIO_LOW);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_7 | PIN_6 | PIN_5 | PIN_4, GPIO_DIR_INPUT);

	GPIO_InitInterruptLine(GPIO_LINE_7, GPIO_MUX_LINE_7_GPIO_2_7, GPIO_MODE_CHANGE);
	GPIO_InitInterruptLine(GPIO_LINE_6, GPIO_MUX_LINE_6_GPIO_2_6, GPIO_MODE_RISING);
	GPIO_InitInterruptLine(GPIO_LINE_2, GPIO_MUX_LINE_2_GPIO_2_6, GPIO_MODE_FALLING);
	GPIO_InitInterruptLine(GPIO_LINE_5, GPIO_MUX_LINE_5_GPIO_2_5, GPIO_MODE_HIGH);
	GPIO_InitInterruptLine(GPIO_LINE_4, GPIO_MUX_LINE_4_GPIO_2_4, GPIO_MODE_LOW);

	enableInterrupts();

	while (1) {
		delay(1000);
		GPIO_PinToggle(GPIO_2, 3);
	}
}
