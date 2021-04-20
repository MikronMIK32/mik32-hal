#include "mcu_core.h"

__attribute__ ((weak)) void TIMER32_0_TRAP_HANDLER() {}
__attribute__ ((weak)) void GPIO_IRQ_TRAP_HANDLER() {}
__attribute__ ((weak)) void TIMER32_1_TRAP_HANDLER() {}

void trap_handler() {
	if (EPIC->STATUS & (1 << EPIC_TIMER32_0_INDEX)) {
		TIMER32_0_TRAP_HANDLER();
	}
	if (EPIC->STATUS & (1 << EPIC_GPIO_IRQ_INDEX)) {
		GPIO_IRQ_TRAP_HANDLER();
	}
	if (EPIC->STATUS & (1 << EPIC_TIMER32_1_INDEX)) {
		TIMER32_1_TRAP_HANDLER();
	}
	EPIC->CLEAR = 0xFF;
}

/*Устанавливает одну из функций пина - 0, 2 и 3 для различных периферийных
 *блоков, 1 для ввода-вывода
 */
bool setPinMode(PadConfigPort port, uint32_t gpioId, PadConfigMode mode) {
	if (port == PORT0) {
		PAD_CONFIG->PORT_0_CFG &= ~(3 << (gpioId << 1));
		PAD_CONFIG->PORT_0_CFG |= mode << (gpioId << 1);

		return true;
	}
	if (port == PORT1) {
		PAD_CONFIG->PORT_1_CFG &= ~(3 << (gpioId << 1));
		PAD_CONFIG->PORT_1_CFG |= mode << (gpioId << 1);

		return true;
	}
	if (port == PORT2) {
		PAD_CONFIG->PORT_2_CFG &= ~(3 << (gpioId << 1));
		PAD_CONFIG->PORT_2_CFG |= mode << (gpioId << 1);

		return true;
	}

	return false; // Error
}


void interrupts() {
	set_csr(mstatus, MSTATUS_MIE);
	set_csr(mie, MIE_MEIE);
}


void noInterrupts() {
	clear_csr(mie, MIE_MEIE);
}

uint32_t digitalPinToInterrupt(uint32_t gpioId) {
	switch (gpioId) {
	case 32:
		return 4;
	case 33:
		return 5;
	case 34:
		return 6;
	case 35:
		return 7;
	case 36:
		return 0;
	case 37:
		return 1;
	case 38:
		return 2;
	case 39:
		return 3;
	default:
		return 9;
	}
}




void Port0_As_Gpio ()
{
    PAD_CONFIG->PORT_0_CFG = 0x55555555;
}
void Port1_As_Gpio ()
{
    PAD_CONFIG->PORT_1_CFG = 0x55555555;
}
void Port2_As_Gpio ()
{
    PAD_CONFIG->PORT_2_CFG = 0x55555555;
}
void Port0_As_Func1 ()
{
    PAD_CONFIG->PORT_0_CFG = 0x00000000;
}
void Port1_As_Func1 ()
{
    PAD_CONFIG->PORT_1_CFG = 0x00000000;
}
void Port2_As_Func1 ()
{
    PAD_CONFIG->PORT_2_CFG = 0x00000000;
}

void Port0_As_Func2 ()
{
    PAD_CONFIG->PORT_0_CFG = 0xAAAAAAAA;
}
void Port1_As_Func2 ()
{
    PAD_CONFIG->PORT_1_CFG = 0xAAAAAAAA;
}
void Port2_As_Func2 ()
{
    PAD_CONFIG->PORT_2_CFG = 0xAAAAAAAA;
}

void Port0_As_Func3 ()
{
    PAD_CONFIG->PORT_0_CFG = 0xFFFFFFFF;
}
void Port1_As_Func3 ()
{
    PAD_CONFIG->PORT_1_CFG = 0xFFFFFFFF;
}
void Port2_As_Func3 ()
{
    PAD_CONFIG->PORT_2_CFG = 0xFFFFFFFF;
}

