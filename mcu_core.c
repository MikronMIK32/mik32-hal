#include "mcu_core.h"


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

