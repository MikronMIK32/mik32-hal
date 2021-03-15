#ifndef MCU_DIGITAL_H_INCLUDED
#define MCU_DIGITAL_H_INCLUDED

#include "include/mcu32_memory_map.h"
#include "periphery/gpio.h"
#include "periphery/gpio_irq.h"
#include "periphery/pad_config.h"

#include <stdbool.h>

// HAL

typedef enum {
	HIGH = 1, LOW = 0
} DigitalState;

void digitalWrite(GPIO_TypeDef *gpio, uint32_t gpioId, DigitalState state);

DigitalState digitalRead(GPIO_TypeDef *gpio, uint32_t gpioId);


typedef enum {
	OUTPUT = 1, INPUT = 0
} PinMode;


void pinMode(GPIO_TypeDef *gpio, uint32_t gpioId, PinMode mode);


typedef enum {
	LEVEL_LOW = 0,
	LEVEL_CHANGE = 1,
	RISING = 2,
	FALLING = 3,
	LEVEL_HIGH = 4
} GPIO_InterruptMode;

void attachInterrupt(uint32_t irq_line, void* irq, GPIO_InterruptMode mode);

// END HAL

#endif
