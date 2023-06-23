#ifndef MIK32_HAL_SSD130
#define MIK32_HAL_SSD130

#ifndef SSD1306_128x64
#define SSD1306_128x32
#endif

#define BRIGHTNESS_FULL         0xFF
#define BRIGHTNESS_HAL          0x7F

#define START_PAGE  	        0
#define END_PAGE                7
#define START_COLUMN  	        0
#define END_COLUMN              127

/* Десятые часов */
#define START_COLUMN_TH         0
#define END_COLUMN_TH           24
/* Единицы часов */
#define START_COLUMN_H          25
#define END_COLUMN_H            49

/* Разделитель - двоеточие */
#define START_COLUMN_COLON      50
#define END_COLUMN_COLON        74

/* Десятые минут */
#define START_COLUMN_TM         75
#define END_COLUMN_TM           99
/* Единицы минут */
#define START_COLUMN_M          100
#define END_COLUMN_M            124

#define SYMBOL_COLON            58
#define SYMBOL_SMILE            13

#include <stdint.h>
#include "mik32_hal_i2c.h"

/* COM (СЛЕДУЮЩИЙ БАЙТ будет командой), DAT (СЛЕДУЮЩИЙ БАЙТ будет данными) и DATS (ВСЕ СЛЕДУЮЩИЕ БАЙТЫ будут данными) */	
#define DATS 0b01000000
#define DAT 0b11000000	
#define COM 0b10000000


HAL_StatusTypeDef HAL_SSD1306_Init(I2C_HandleTypeDef *hi2c, uint8_t brightness);
HAL_StatusTypeDef HAL_SSD1306_SetBorder(I2C_HandleTypeDef *hi2c, uint8_t start_col, uint8_t end_col, uint8_t start_page, uint8_t end_page);
HAL_StatusTypeDef HAL_SSD1306_CLR_SCR(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_SSD1306_Write(I2C_HandleTypeDef *hi2c, uint8_t symbol);

#endif
