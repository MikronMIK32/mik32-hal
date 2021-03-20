#ifndef MCU_DIGITAL_H_INCLUDED
#define MCU_DIGITAL_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/gpio.h"

#include <stdbool.h>


typedef enum {
	HIGH = 1, LOW = 0
} GPIO_DigitalState;

GPIO_DigitalState digitalRead(uint32_t gpioId);

GPIO_DigitalState digitalReadGPIO(GPIO_TypeDef *gpio, uint32_t gpioId);


void digitalWrite(uint32_t gpioId, GPIO_DigitalState state);

void digitalWriteGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_DigitalState state);


typedef enum {
	OUTPUT = 1, INPUT = 0
} GPIO_PinMode;


/*Аналог arduino функции pinMode, устанавливает режим ввода или вывода,
 *Внимание! Не поддерживает INPUT_PULLUP!
 */
void pinMode(uint32_t gpioId, GPIO_PinMode mode);

void pinModeGPIO(GPIO_TypeDef *gpio, uint32_t gpioId, GPIO_PinMode mode);


#endif
