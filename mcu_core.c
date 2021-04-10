#include "mcu_core.h"

void (*GPIO_IRQ_TRAP_HANDLER)(uint32_t, uint32_t) = 0;

void trap_handler() {
	if (EPIC->STATUS & (1 << EPIC_GPIO_IRQ_INDEX)) {
		GPIO_IRQ_TRAP_HANDLER(GPIO_IRQ->INTERRUPTS, GPIO_IRQ->STATE);
		GPIO_IRQ->CLEAR = 0xFF;
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

void attachInterrupt(uint32_t irq_line, uint32_t mux, void *irq,
		GPIO_InterruptMode mode) {
	GPIO_IRQ_TRAP_HANDLER = irq;
	GPIO_IRQ->CFG = (mux << (irq_line << 2));
	if (mode & MODE_LOW) {
		GPIO_IRQ->EDGE &= ~(1 << irq_line);
		GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & MODE_CHANGE) {
		GPIO_IRQ->EDGE |= (1 << irq_line);
		GPIO_IRQ->LEVEL_SET |= (1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET |= (1 << irq_line);
	}
	GPIO_IRQ->ENABLE_SET = (1 << irq_line);
	EPIC->MASK_SET = 1 << EPIC_GPIO_IRQ_INDEX;
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


static uint32_t miState;
int32_t rand() {
	miState ^= (miState << 13);
	miState ^= (miState >> 17);
	miState ^= (miState << 15);

	return (miState * 1332534557) & 0x7FFFFFFF;
}

void srand(uint32_t seed) {
// a zero seed will not work!
	if (seed == 0) {
		seed = 0x55aaff00;
	}

	miState = seed;
}

