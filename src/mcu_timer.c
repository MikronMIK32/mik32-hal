#include "../../mcu32-hal-main/src/mcu_timer.h"

void delay(uint32_t periodMs) {
	DELAY_TIMER->Enable &= ~(TIMER32_ENABLE_M);
	DELAY_TIMER->Enable = TIMER32_RESET_VALUE_M;
	DELAY_TIMER->IntClear = TIMER32_INT_OVERFLOW_M;

	DELAY_TIMER->Top = periodMs * FREQ_BY_MS;
	DELAY_TIMER->IntMask = TIMER32_INT_OVERFLOW_M;
	DELAY_TIMER->Enable = TIMER32_ENABLE_M;
	while (DELAY_TIMER->IntFlags == 0);

	DELAY_TIMER->Enable &= ~(TIMER32_ENABLE_M);
	DELAY_TIMER->Enable = TIMER32_RESET_VALUE_M;
	DELAY_TIMER->IntClear = TIMER32_INT_OVERFLOW_M;
}

void Timer_Init(TIMER32_TypeDef* timer, uint32_t top) {
	timer->Enable &= ~(TIMER32_ENABLE_M);
	timer->Enable = TIMER32_RESET_VALUE_M;
	timer->IntClear = 1;
	timer->Top = top;
}

void Timer_Start(TIMER32_TypeDef* timer) {
	timer->Enable = TIMER32_ENABLE_M;
}

void Timer_StartIT(TIMER32_TypeDef* timer) {
	timer->IntMask = 1;
	EPIC->MASK_SET = 1 << EPIC_TIMER32_1_INDEX;
	timer->Enable = TIMER32_ENABLE_M;
}
