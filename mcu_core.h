#ifndef FPGA_MCU_CORE_H_INCLUDED
#define FPGA_MCU_CORE_H_INCLUDED

#include "csr.h"
#include "scr1_csr_encoding.h"

#include "include/mcu32_memory_map.h"
#include "periphery/pad_config.h"

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


#endif /* FPGA_MCU_CORE_H_INCLUDED */
