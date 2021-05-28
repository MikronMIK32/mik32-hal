#include "../../../mcu32-hal-main/examples/libs/wg12864a.h"

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

void WriteCmd(uint8_t cs, uint8_t byte) {
	if (cs & 1) {
		GPIO_PIN_MASK_SET(GPIO_1, PIN_CS1_M);
	} else {
		GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_CS1_M);
	}
	if (cs & 2) {
		GPIO_PIN_MASK_SET(GPIO_1, PIN_CS2_M);
	} else {
		GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_CS2_M);
	}

	GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_DI_M);

	for (int i = 0; i < 8; i++) {
		if ((byte >> i) & 1) {
			GPIO_PIN_MASK_SET(GPIO_1, 1 << i);
		} else {
			GPIO_PIN_MASK_CLEAR(GPIO_1, 1 << i);
		}
	}

	//for (volatile int i = 0; i < 10; ++i);
	GPIO_PIN_MASK_SET(GPIO_1, PIN_E_M);
	for (volatile int i = 0; i < 2; ++i);
	GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_E_M);
	//for (volatile int i = 0; i < 10; ++i);
	//delay(1);
	//GPIO_PinWrite(GPIO_1, PIN_CS1, GPIO_LOW);
	//GPIO_PinWrite(GPIO_1, PIN_CS2, GPIO_LOW);
}

void WriteData(uint8_t cs, uint8_t byte) {
	if (cs & 1) {
		GPIO_PIN_MASK_SET(GPIO_1, PIN_CS1_M);
	} else {
		GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_CS1_M);
	}
	if (cs & 2) {
		GPIO_PIN_MASK_SET(GPIO_1, PIN_CS2_M);
	} else {
		GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_CS2_M);
	}

	GPIO_PIN_MASK_SET(GPIO_1, PIN_DI_M);

	for (int i = 0; i < 8; i++) {
		if ((byte >> i) & 1) {
			GPIO_PIN_MASK_SET(GPIO_1, 1 << i);
		} else {
			GPIO_PIN_MASK_CLEAR(GPIO_1, 1 << i);
		}
	}

	//for (volatile int i = 0; i < 10; ++i);
	GPIO_PIN_MASK_SET(GPIO_1, PIN_E_M);
	for (volatile int i = 0; i < 2; ++i);
	GPIO_PIN_MASK_CLEAR(GPIO_1, PIN_E_M);
	//for (volatile int i = 0; i < 10; ++i);
	//delay(1);
	//GPIO_PinWrite(GPIO_1, PIN_CS1, GPIO_LOW);
	//GPIO_PinWrite(GPIO_1, PIN_CS2, GPIO_LOW);
}

void LCD_Init() {
	WriteCmd(3, 0x3F);
	WriteCmd(3, 0x40);
	WriteCmd(3, 0xB8);
	WriteCmd(3, 0xC0);
}

void WriteByte(uint8_t cs, uint8_t y, uint8_t x, uint8_t byte) {
	WriteCmd(cs, 0x40 + y);
	WriteCmd(cs, 0xB8 + x);
	WriteData(cs, byte);
}

void FillCheckers() {
	int f = 0;
	for (int i = 0; i < 8; i++) {
		f = !f;
		for (int j = 0; j < 8; j++) {
			f = !f;
			for (int k = 0; k < 8; k++) {
				WriteByte(3, (j<<3) + k, i, f ? 0xFF : 0);
			}
		}
	}
}

void FillBlack() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 64; j++) {
			WriteByte(3, j, i, 0);
		}
	}
}

void FillWhite() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 64; j++) {
			WriteByte(3, j, i, 0xFF);
		}
	}
}

