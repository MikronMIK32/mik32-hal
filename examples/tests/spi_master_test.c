#include "../mcuduino.h"

#include "include/mcu32_memory_map.h"
#include "libs/spi_lib.h"

int main()
{
	setPinMode(PORT2, 0, MODE1);
	setPinMode(PORT2, 1, MODE1);

	pinMode(GPIO_2, 0, GPIO_MODE_OUTPUT);
	pinMode(GPIO_2, 1, GPIO_MODE_OUTPUT);

	SPI_Init(SPI_0, SPI_CONFIG_MASTER_M | SPI_CONFIG_CS_0_M);
	uint8_t input[2] = {0, 0};
	uint8_t output[2] = {42, 0};
	delay(100);
	SPI_CS_Enable(SPI_0, SPI_CONFIG_CS_0_M);
	SPI_Exchange(SPI_0, input, output, 2);

	delay(100);

	digitalWrite(GPIO_2, 0, input[0]);
	digitalWrite(GPIO_2, 1, input[1]);

	while (1) {

	}
}
