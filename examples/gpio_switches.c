#include "../../mcu32-hal-main/src/mcu_core.h"
#include "../../mcu32-hal-main/src/mcu_digital.h"
#include "../../mcu32-hal-main/src/mcu_timer.h"

/**
 * \file Пример работы с портом ввода-вывода, чтение и вывод логического уровня
 */

int main()
{
	setPortFunction(GPIO_2, PIN_FUNCTION_GPIO);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_0 | PIN_1 | PIN_2 | PIN_3, GPIO_DIR_OUTPUT);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_4 | PIN_5 | PIN_6 | PIN_7, GPIO_DIR_INPUT);

	while (1) {
		delay(50);
		GPIO_PinWrite(GPIO_2, 0, GPIO_PinRead(GPIO_2, 4));
		GPIO_PinWrite(GPIO_2, 1, GPIO_PinRead(GPIO_2, 5));
		GPIO_PinWrite(GPIO_2, 2, GPIO_PinRead(GPIO_2, 6));
		GPIO_PinWrite(GPIO_2, 3, GPIO_PinRead(GPIO_2, 7));
	}
}
