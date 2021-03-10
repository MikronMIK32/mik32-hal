#ifndef HAL_GPIO_H_INCLUDED
#define HAL_GPIO_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/gpio.h"
#include "periphery/pad_config.h"

#include <stdbool.h>

// HAL

typedef enum {
	HIGH = 1, LOW = 0
} DigitalState;

void digitalWrite(GPIO_TypeDef *gpio, uint32_t gpioId, DigitalState state);

bool setPinMode(GPIO_TypeDef *gpio, uint32_t gpioId, uint32_t mode);

typedef enum {
	OUTPUT = 1, INPUT = 0
} PinMode_TypeDef;

void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, PinMode_TypeDef mode);

// END HAL

#endif
