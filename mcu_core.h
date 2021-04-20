#ifndef FPGA_MCU_CORE_H_INCLUDED
#define FPGA_MCU_CORE_H_INCLUDED

#include "csr.h"
#include "scr1_csr_encoding.h"

#include "include/mcu32_memory_map.h"
#include "periphery/pad_config.h"
#include "periphery/epic.h"
#include "periphery/gpio.h"
#include "periphery/gpio_irq.h"

#include <stdbool.h>

__attribute__ ((weak)) void TIMER32_0_TRAP_HANDLER();
__attribute__ ((weak)) void GPIO_IRQ_TRAP_HANDLER();
__attribute__ ((weak)) void TIMER32_1_TRAP_HANDLER();

#define FREQ 10000000

typedef enum {
	PORT0 = 0,
	PORT1 = 1,
	PORT2 = 2
} PadConfigPort;

typedef enum {
	MODE0 = 0,
	MODE1 = 1,
	MODE2 = 2
} PadConfigMode;

bool setPinMode(PadConfigPort port, uint32_t gpioId, PadConfigMode mode);


void interrupts();

void noInterrupts();

typedef enum {
	MODE_LOW = 0,
	MODE_CHANGE = 2,
	MODE_RISING = 3,
	MODE_FALLING = 4,
	MODE_HIGH = 1
} GPIO_InterruptMode;

uint32_t digitalPinToInterrupt(uint32_t gpioId);


void Port0_As_Gpio ();
void Port1_As_Gpio ();
void Port2_As_Gpio ();
void Port0_As_Func1 ();
void Port1_As_Func1 ();
void Port2_As_Func1 ();
void Port0_As_Func2 ();
void Port1_As_Func2 ();
void Port2_As_Func2 ();
void Port0_As_Func3 ();
void Port1_As_Func3 ();
void Port2_As_Func3 ();


int32_t rand();
void srand(uint32_t seed);


#endif /* FPGA_MCU_CORE_H_INCLUDED */
