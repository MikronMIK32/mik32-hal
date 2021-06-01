#include <mcu_timer32.h>
#include <mcu_core.h>
#include <mcu_digital.h>

/**
 * \file Пример работы с прерываниями портов ввода-вывода
 * \note Пример создавался для работы с моделью МК на ПЛИС, и зависит от
 * переключателей на плате. Переключатели на GPIO_2 с вывода 7 по 4
 * должны быть установлены следующим образом: vvv^. В примере переключатели
 * на выводах 7 и 6 управляют светодиодами 3 и 2 соответственно, переключатель
 * 5 в верхнем положении включает светодиод 1, переключатель 4
 * в нижнем положении выключает светодиод 1. Светодиод 0
 * переключается в главном цикле, и показывает, исполняется ли главный цикл.
 */

void GPIO_IRQ_TRAP_HANDLER() {
	if (GPIO_LineInterruptState(7)) {
		GPIO_PinWrite(GPIO_2, 3, GPIO_LinePinState(7));
	}
	if (GPIO_LineInterruptState(6)) {
		GPIO_PinWrite(GPIO_2, 2, GPIO_HIGH);
	}
	if (GPIO_LineInterruptState(2)) {
		GPIO_PinWrite(GPIO_2, 2, GPIO_LOW);
	}
	if (GPIO_LineInterruptState(5)) {
		GPIO_PinWrite(GPIO_2, 1, GPIO_HIGH);
	}
	if (GPIO_LineInterruptState(4)) {
		GPIO_PinWrite(GPIO_2, 1, GPIO_LOW);
	}
	GPIO_ClearInterrupt();
}

int main()
{
	setPortFunction(GPIO_2, PIN_FUNCTION_GPIO);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_0 | PIN_1 | PIN_2 | PIN_3, GPIO_DIR_OUTPUT);

	GPIO_PIN_MASK_CLEAR(GPIO_2, 0b1111);

	GPIO_SetPinMaskDirection(GPIO_2, PIN_7 | PIN_6 | PIN_5 | PIN_4, GPIO_DIR_INPUT);

	GPIO_InitInterruptLine(GPIO_LINE_7, GPIO_MUX_LINE_7_GPIO_2_7, GPIO_MODE_INT_CHANGE);
	GPIO_InitInterruptLine(GPIO_LINE_6, GPIO_MUX_LINE_6_GPIO_2_6, GPIO_MODE_INT_RISING);
	GPIO_InitInterruptLine(GPIO_LINE_2, GPIO_MUX_LINE_2_GPIO_2_6, GPIO_MODE_INT_FALLING);
	GPIO_InitInterruptLine(GPIO_LINE_5, GPIO_MUX_LINE_5_GPIO_2_5, GPIO_MODE_INT_HIGH);
	GPIO_InitInterruptLine(GPIO_LINE_4, GPIO_MUX_LINE_4_GPIO_2_4, GPIO_MODE_INT_LOW);

	enableInterrupts();

	while (1) {
		delay(500);
		GPIO_PinToggle(GPIO_2, 0);
	}
}
