#include "hal_timer.h"

void delay(uint32_t periodMs) {
	TIMER32_0->Top = periodMs * FREQ_BY_MS;
	TIMER32_0->IntMask = 1;
	TIMER32_0->Enable = TIMER32_ENABLE_M;
	while (TIMER32_0->IntFlags == 0)
		;
	TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	TIMER32_0->IntClear = 1;
}