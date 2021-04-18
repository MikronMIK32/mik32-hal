#ifndef MCU_DIGITAL_H_INCLUDED
#define MCU_DIGITAL_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/gpio.h"

#include <stdbool.h>

#include "mcu_core.h"


typedef enum {
	HIGH = 1, LOW = 0
} GPIO_DigitalState;

GPIO_DigitalState digitalReadGPIO(GPIO_TypeDef *gpio, uint32_t gpioId);


void digitalWriteGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_DigitalState state);


typedef enum {
	OUTPUT = 1, INPUT = 0
} GPIO_PinMode;


/*Аналог arduino функции pinMode, устанавливает режим ввода или вывода,
 *Внимание! Не поддерживает INPUT_PULLUP!
 */
void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode);

void pinModeGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode);

void attachInterruptGPIO(uint32_t irq_line, uint32_t mux,
		GPIO_InterruptMode mode);


#endif
