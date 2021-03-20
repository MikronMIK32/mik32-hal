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

void attachInterrupt(uint32_t irq_line, uint32_t mux, void *irq,
		GPIO_InterruptMode mode) {
	GPIO_IRQ_TRAP_HANDLER = irq;
	GPIO_IRQ->CFG = (mux << (irq_line << 2));
	if (mode & LEVEL_LOW) {
		GPIO_IRQ->EDGE &= ~(1 << irq_line);
		GPIO_IRQ->LEVEL_SET &= ~(1 << irq_line);
		GPIO_IRQ->ANYEDGE_SET &= ~(1 << irq_line);
	} else if (mode & LEVEL_CHANGE) {
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


void SystemInit()
{
    Port0_As_Gpio();
    Port1_As_Gpio();
    Port2_As_Gpio();

    GPIO_0->DIRECTION_IN = 0xFFFFFFFF;
    GPIO_1->DIRECTION_IN = 0xFFFFFFFF;
    GPIO_2->DIRECTION_IN = 0xFFFFFFFF;
}

