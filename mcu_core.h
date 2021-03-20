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
	LEVEL_LOW = 1,
	LEVEL_CHANGE = 2,
	RISING = 4,
	FALLING = 8,
	LEVEL_HIGH = 16
} GPIO_InterruptMode;

void attachInterrupt(uint32_t irq_line, uint32_t mux, void *irq,
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


#endif /* FPGA_MCU_CORE_H_INCLUDED */
