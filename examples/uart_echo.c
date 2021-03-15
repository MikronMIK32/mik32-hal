z#include "include/mcu32_memory_map.h"
#include "periphery/gpio.h"
#include "periphery/pad_config.h"
#include "periphery/epic.h"
#include "periphery/timer32.h"
#include "libs/uart_lib.h"
#include "csr.h"
#include "scr1_csr_encoding.h"
#include "periphery/epic.h"
#include <stdbool.h>

// HAL
//namespace name {}
#define FREQ 10000000
#define FREQ_BY_MS (FREQ/1000)
#define FREQ_BY_US (FREQ/1000000)

//#define HAL_AS_MACRO

typedef enum {
	HIGH = 1, LOW = 0
} DigitalState;

#ifdef HAL_AS_MACRO
#define delay(periodMs) {\
	TIMER32_0->Top = periodMs * FREQ_BY_MS;\
	TIMER32_0->IntMask = 1;\
	TIMER32_0->Enable = TIMER32_ENABLE_M;\
	while (TIMER32_0->IntFlags == 0);\
	TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);\
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;\
	TIMER32_0->IntClear = 1;\
	}
#else
void delay(uint32_t periodMs) {
	TIMER32_0->Top = periodMs * FREQ_BY_MS;
	TIMER32_0->IntMask = 1;
	TIMER32_0->Enable = TIMER32_ENABLE_M;
	while (TIMER32_0->IntFlags == 0);
	TIMER32_0->Enable &= ~(TIMER32_ENABLE_M);
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	TIMER32_0->IntClear = 1;
}
#endif

void digitalWrite(GPIO_TypeDef *gpio, uint32_t gpioId, DigitalState state) {
	if (state == HIGH) {
		gpio->SET |= 1 << gpioId;
	} else {
		gpio->CLEAR |= 1 << gpioId;
	}
}

bool setPinMode(GPIO_TypeDef *gpio, uint32_t gpioId, uint32_t mode) {
	uint32_t first = ~(3 << (gpioId << 1));
	uint32_t second = mode << (gpioId << 1);

	if (gpio == GPIO_0) {
		PAD_CONFIG->PORT_0_CFG &= first;
		PAD_CONFIG->PORT_0_CFG |= second;

		return true;
	}
	if (gpio == GPIO_1) {
		PAD_CONFIG->PORT_1_CFG &= first;
		PAD_CONFIG->PORT_1_CFG |= second;

		return true;
	}
	if (gpio == GPIO_2) {
		PAD_CONFIG->PORT_2_CFG &= first;
		PAD_CONFIG->PORT_2_CFG |= second;

		return true;
	}

	return false; // Error
}

typedef enum {
	OUTPUT = 1, INPUT = 0
} PinMode_TypeDef;

void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, PinMode_TypeDef mode) {
	if (mode == OUTPUT) {
		gpio->DIRECTION_OUT |= 1 << gpioId;
	} else {
		gpio->DIRECTION_IN |= 1 << gpioId;
	}
}

DigitalState digitalRead(GPIO_TypeDef *gpio, uint32_t gpioId) {
	if (gpio->SET &= 1 << gpioId) {
		return HIGH;
	}
	else {
		return LOW;
	}
}

void EnableInterrupts()
{
    set_csr(mstatus, MSTATUS_MIE);
    set_csr(mie, MIE_MEIE);
}

void DisableInterrupts()
{
    clear_csr(mie, MIE_MEIE);
}



void delayInt(uint32_t periodMs) {
	TIMER32_0->Top = periodMs * FREQ_BY_MS;
	TIMER32_0->IntMask = 1;
	EPIC->MASK_SET = 1;
	TIMER32_0->Enable = TIMER32_ENABLE_M;
}

// END HAL

void __attribute__((weak)) trap_handler()
{
	TIMER32_0->Enable = TIMER32_RESET_VALUE_M;
	uint32_t a = EPIC->STATUS;
	EPIC->RAW_STATUS_RAW = 1;
	a = EPIC->STATUS;
	TIMER32_0->IntClear = 1;
	digitalWrite(GPIO_0, 14, !digitalRead(GPIO_0, 14));
}

int main() {
	EnableInterrupts();

	setPinMode(GPIO_0, 14, 1);

	pinMode(GPIO_0, 14, OUTPUT);

	delayInt(1000);
	while (1) {

	}
}
/*
 __attribute__((interrupt)) void trap_handler() {

 }
 */
