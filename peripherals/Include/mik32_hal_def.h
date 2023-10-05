#ifndef MIK32_HAL_DEF
#define MIK32_HAL_DEF


#define HAL_PIN_MASK 	0xFFFF
#define HAL_PORT_S 		16

typedef enum HAL_StatusTypeDef
{
	HAL_OK       = 0x00U,
	HAL_ERROR    = 0x01U,
	HAL_BUSY     = 0x02U,
	HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#endif