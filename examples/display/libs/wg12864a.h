#ifndef FPGA_MCU32_HAL_EXAMPLES_LIBS_WG12864A_H_
#define FPGA_MCU32_HAL_EXAMPLES_LIBS_WG12864A_H_

#include "../../../mcu32-hal/mcu_digital.h"

#define PIN_CS1 8
#define PIN_CS2 9
#define PIN_E 10
#define PIN_DI 11
#define PIN_RW 12

#define PIN_CS1_M (1<<PIN_CS1)
#define PIN_CS2_M (1<<PIN_CS2)
#define PIN_E_M (1<<PIN_E)
#define PIN_DI_M (1<<PIN_DI)
#define PIN_RW_M (1<<PIN_RW)

void WriteCmd(uint8_t cs, uint8_t byte);

void WriteData(uint8_t cs, uint8_t byte);

void LCD_Init();

void WriteByte(uint8_t cs, uint8_t y, uint8_t x, uint8_t byte);

void FillCheckers();

void FillBlack();

void FillWhite();


#endif /* FPGA_MCU32_HAL_EXAMPLES_LIBS_WG12864A_H_ */
