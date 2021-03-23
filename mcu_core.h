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
	LOW = 0,
	CHANGE = 2,
	RISING = 3,
	FALLING = 4,
	HIGH = 1
} GPIO_InterruptMode;

uint32_t digitalPinToInterrupt(uint32_t gpioId);

void attachInterrupt(uint32_t irq_line, void *irq,
		GPIO_InterruptMode mode);


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
