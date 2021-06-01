#include "../mcu32-hal/mcu_core.h"
#include "../mcu32-hal/mcu_digital.h"
#include "../mcu32-hal/mcu_timer.h"
#include "periphery/uart.h"
#include "libs/uart_lib.h"

#define UART_0_DIVIDER 1042

void xputc(char c)
{
    UART_WriteByte(UART_0, c);
    UART_WaitTransmission(UART_0);
}

void xprintbyte(uint32_t byte)
{
	xputc('0');
	xputc('b');
	for (int i = 32; i >= 0; i--) {
		if (byte & (1 << i)) {
			xputc('1');
		} else {
			xputc('0');
		}
	}
}

int main()
{
	setPinFunction(GPIO_0, 5, 0);
	setPinFunction(GPIO_0, 6, 0);
	UART_Init(UART_0, UART_0_DIVIDER, UART_CONTROL1_TE_M | UART_CONTROL1_RE_M | UART_CONTROL1_M_8BIT_M, 0, 0);

	while (1) {
		UART_WaitReceiving(UART_0);
		char c = UART_0->RXDATA;
		xprintbyte(UART_0->FLAGS);
	}
}
