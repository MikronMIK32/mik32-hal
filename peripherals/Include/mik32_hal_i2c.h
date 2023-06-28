#ifndef MIK32_HAL_I2C
#define MIK32_HAL_I2C


#include "mik32_hal_def.h"
#include "i2c.h"
#include "mcu32_memory_map.h"
#include "mik32_hal_dma.h"


/*
 * Define: I2C_TIMEOUT
 * Количество циклов ожидания установки флага TXIS или RXNE
 *
 */
#define I2C_TIMEOUT_DEFAULT 	1000000		/* Количество циклов ожидания установки флага TXIS или RXNE */
/*
 * Define: I2C_NBYTE_MAX
 * Максимальлное количество байт в посылке (NBYTES)
 *
 */
#define I2C_NBYTE_MAX 	255   /* Максимальное количество байт в посылке (NBYTES) */

#define I2C_ADDRESS_7BIT_MAX 	0x7F   /* Максимальный 7 битный адрес */
#define I2C_ADDRESS_10BIT_MAX 	0x3FF   /* Максимальный 10 битный адрес */

/* I2C_error - номера ошибок I2C*/
/* Номер канала */
typedef enum
{
	I2C_ERROR_NONE = 0,    /* Ошибок нет */
	I2C_ERROR_TIMEOUT = 1, /* Превышено ожидание установки флага TXIS или RXNE */
	I2C_ERROR_NACK = 2,    /* Во время передачи не получено подтверждение данных (NACK) */  
	I2C_ERROR_BERR = 3,    /* Ошибка шины */ 
	I2C_ERROR_ARLO = 4,    /* Проигрыш арбитража */
	I2C_ERROR_OVR = 5,     /* Переполнение или недозагрузка */ 
	I2C_ERROR_STOP = 6,   /* Обнаружение STOP на линии */ 
} HAL_I2C_ErrorTypeDef;

/* I2C_addressing_mode - Режим адреса */
typedef enum
{
	I2C_ADDRESSINGMODE_7BIT = 0,    /* 7 битный адрес */
	I2C_ADDRESSINGMODE_10BIT = 1    /* 10 битный адрес */ 
} HAL_I2C_AddressingModeTypeDef;

/* I2C_dual_addressing_mode - Режим дополнительного адреса 7 бит */
typedef enum
{
	I2C_DUALADDRESS_DISABLE = 0,   /* Выключить дополнительный адрес */
	I2C_DUALADDRESS_ENABLE = 1   /* Включить дополнительный адрес */
} HAL_I2C_DualAddressTypeDef;

/* I2C_general_call_mode - Адрес общего вызова */
typedef enum
{
	I2C_GENERALCALL_DISABLE = 0,  	/* Выключить адрес общего вызова */
	I2C_GENERALCALL_ENABLE = 1   	/* Включить адрес общего вызова */
} HAL_I2C_GeneralCallTypeDef;

/* I2C_nostretch_mode - Режим удержания SCL ведомым */
typedef enum
{
	I2C_NOSTRETCH_DISABLE = 0,   /* Растягивание активно */
	I2C_NOSTRETCH_ENABLE = 1   	 /* растягивание выключено */
} HAL_I2C_NoStretchModeTypeDef;

/* I2C_sbc_mode - Режим аппаратного контроля байта ведомым */
typedef enum
{
	I2C_SBC_DISABLE = 0,   /* Аппаратный контроль выключен */
	I2C_SBC_ENABLE = 1     /* Аппаратный контроль включен */
} HAL_I2C_SBCModeTypeDef;

/* I2C_reload_mode - Режим перезаписи NBYTES:  */
typedef enum
{
	I2C_RELOAD_DISABLE = 0,  /* Транзакция завершена после пересылки NBYTES байт данных (на шине ожидаются STOP или RESTART) */
	I2C_RELOAD_ENABLE = 1    /* Транзакция не завершена после пересылки NBYTES байт данных (значение NBYTES будет перезаписано) */
} HAL_I2C_ReloadModeTypeDef;

/* I2C_autoend_mode - Режим автоматического окончания */
typedef enum
{
	I2C_AUTOEND_DISABLE = 0,   /* Режим автоматического окончания отключен */
	I2C_AUTOEND_ENABLE = 1     /* Режим автоматического окончания включен */
} HAL_I2C_AutoEndModeTypeDef;

/* I2C_transfer_direction - Направление передачи */
typedef enum
{
	I2C_TRANSFER_WRITE = 0,   /* Ведущий запрашивает транзакцию записи */
	I2C_TRANSFER_READ = 1    /* Ведущий запрашивает транзакцию чтения */
} HAL_I2C_TransferDirectionTypeDef;

/* I2C_OwnAddress2_mask - Маска второго собственного адреса */
typedef enum
{
	I2C_OWNADDRESS2_MASK_DISABLE = 0,    /* Нет маски */
	I2C_OWNADDRESS2_MASK_111111x = 1,    /* Сравниваются только OA2[7:2] */
	I2C_OWNADDRESS2_MASK_11111xx = 2,    /* Сравниваются только OA2[7:3]; */
	I2C_OWNADDRESS2_MASK_1111xxx = 3,    /* Сравниваются только OA2[7:4]; */
	I2C_OWNADDRESS2_MASK_111xxxx = 4,    /* Сравниваются только OA2[7:5]; */
	I2C_OWNADDRESS2_MASK_11xxxxx = 5,    /* Сравниваются только OA2[7:6]; */
	I2C_OWNADDRESS2_MASK_1xxxxxx = 6,    /* Сравниваются только OA2[7]; */
	I2C_OWNADDRESS2_MASK_1111111 = 7     /* OA2[7:1] маскируются, подтверждаются (ACK) все 7-битные адреса (кроме зарезервированных) */
} HAL_I2C_OwnAddress2MaskTypeDef;

/* I2C_digital_filter - Цифровой фильтр */
typedef enum
{
	I2C_DIGITALFILTER_OFF           = 0,
	I2C_DIGITALFILTER_1CLOCKCYCLES  = 1,
	I2C_DIGITALFILTER_2CLOCKCYCLES  = 2,
	I2C_DIGITALFILTER_15CLOCKCYCLES = 15
} HAL_I2C_DigitalFilterTypeDef;

/* I2C_analog_filter - Цифровой фильтр */
typedef enum
{
	I2C_ANALOGFILTER_ENABLE = 0,
	I2C_ANALOGFILTER_DISABLE = 1
} HAL_I2C_AnalogFilterTypeDef;

typedef enum 
{
  HAL_I2C_MODE_MASTER = 0, /* Режим ведущего */
  HAL_I2C_MODE_SLAVE = 1,  /* Режим ведомого */

} HAL_I2C_ModeTypeDef;


typedef struct
{

	HAL_I2C_ModeTypeDef Mode; /* Режим ведомый или ведущий */

	/*
	* Variable: OwnAddress1
	* Основной собственный адрес.
	*
	* Когда включен дополнительный адрес, длинна основного адреса должна быть 7 бит.
	* 
	* Этот параметр должен быть 7-битным или 10-битным числом
	*
	*/
	uint32_t OwnAddress1;

	/*
	* Variable: DualAddressMode
	* Активация дополнительного 7 битного адреса
	*
	* Можно использовать, если длина основного адреса 7 бит
	* 
	* Этот параметр должен быть одним из значений:
	* 
	* - <I2C_DUALADDRESS_DISABLE>;
	* - <I2C_DUALADDRESS_ENABLE>. 
	* 
	*
	*/
	HAL_I2C_DualAddressTypeDef DualAddressMode; 

	/*
	* Variable: OwnAddress2
	* Дополнительный 7 битный адрес
	*
	* Можно использовать, если длина основного адреса 7 бит
	* 
	* Этот параметр должен быть 7 битным числом.
	*
	*/
	uint32_t OwnAddress2;

	HAL_I2C_GeneralCallTypeDef GeneralCall; 

	/*
	* Variable: OwnAddress2Mask
	* Маска сравнения дополнительного 7 битного адреса
	* 
	* Этот параметр должен быть одним из значений:
	*
	* - <I2C_OWNADDRESS2_MASK_DISABLE>
	* - <I2C_OWNADDRESS2_MASK_111111x>
	* - <I2C_OWNADDRESS2_MASK_11111xx>
	* - <I2C_OWNADDRESS2_MASK_1111xxx>
	* - <I2C_OWNADDRESS2_MASK_111xxxx>
	* - <I2C_OWNADDRESS2_MASK_11xxxxx>
	* - <I2C_OWNADDRESS2_MASK_1xxxxxx>
	* - <I2C_OWNADDRESS2_MASK_1111111>
	* 
	*/
	HAL_I2C_OwnAddress2MaskTypeDef OwnAddress2Mask;

	/*
	* Variable: NoStretchMode
	* Растягивания тактового сигнала в режиме "ведомый".
	*
	* Не совместим с режимом SBC
	*
	* В режиме ведущий значение всегда I2C_NOSTRETCH_DISABLE
	* 
	* Этот параметр должен быть одним из значений:
	* 
	* - <I2C_NOSTRETCH_DISABLE>
	* - <I2C_NOSTRETCH_ENABLE>
	*
	*/
	HAL_I2C_NoStretchModeTypeDef NoStretchMode;

	/*
	* Variable: SBCMode
	* Режим аппаратного контроля передачи данных в режиме "ведомый"
	*
	* Не совместим с режимом NOSTRETCH
	* 
	* Этот параметр должен быть одним из значений:
	* 
	* - <I2C_SBC_DISABLE>
	* - <I2C_SBC_ENABLE>
	*
	*/
	HAL_I2C_SBCModeTypeDef SBCMode;

	/*
	* Variable: AutoEnd
	* Управление режимом автоматического окончания в режиме "ведущий"
	* 
	* Этот параметр должен быть одним из значений:
	* 
	* - <AUTOEND_DISABLE>
	* - <AUTOEND_ENABLE>
	*
	*/
	HAL_I2C_AutoEndModeTypeDef AutoEnd;

	/*
	* Variable: Filter
	* Цифровой фильтр
	* 
	* Этот параметр должен быть одним из значений:
	* 
	* - <I2C_FILTER_OFF>
	* - <I2C_FILTER_1CLOCKCYCLES>
	* - <I2C_FILTER_2CLOCKCYCLES>
	* - <I2C_FILTER_15CLOCKCYCLES>
	*
	*/
	HAL_I2C_DigitalFilterTypeDef DigitalFilter;

	HAL_I2C_AnalogFilterTypeDef AnalogFilter;

} I2C_InitTypeDef;

/*
 * Struct: I2C_ClockTypeDef
 *
 * Настройки временных ограничений 
 * 
 */
typedef struct
{
/*
 * Variable: PRESC
 * Предварительный делитель частоты I2CCLK.
 * 
 * Используется для вычисления значения t_PRESC используемого счетчиками предустановки, удержания, низкого и высокого уровней.
 * 
 * Этот параметр может быть числом в пределах от 0 до 15
 *
 */
  uint32_t PRESC;
              
/*
 * Variable: SCLDEL
 * Длительность предустановки данных.
 * 
 * Задержка между изменением SDA и фронтом SCL.
 * 
 * Этот параметр может быть числом в пределах от 0 до 15.
 *
 */
  uint32_t SCLDEL;

/*
 * Variable: SDADEL
 * Длительность предустановки данных.
 * 
 * Задержка между спадом SCL и изменением SDA в режиме ведущего и ведомого при  NOSTRETCH = 0.
 * 
 * Этот параметр может быть числом в пределах от 0 до 15.
 *
 */
  uint32_t SDADEL;

/*
 * Variable: SCLH
 * Длительность удержания SCL в состоянии логической "1" в режиме "ведущий".
 * 
 * Этот параметр может быть от 0 до 255.
 *
 */
  uint32_t SCLH;

/*
 * Variable: SCLL
 * Длительность удержания SCL в состоянии логического "0" в режиме "ведущий".
 * 
 * Этот параметр может быть от 0 до 255.
 *
 */
  uint32_t SCLL;

} I2C_ClockTypeDef;


typedef struct
{
	/*
	* Variable: Instance
	* Базовый адрес регистров I2C
	*
	*/
	I2C_TypeDef *Instance;

	/*
	* Variable: Init
	* Параметры инициализации I2C
	*
	*/
	I2C_InitTypeDef Init;

	/*
	* Variable: Clock
	* Параметры частоты I2C
	*
	*/
	I2C_ClockTypeDef Clock;

	/*
	* Variable: ErrorCode
	* Код ошибки I2C
	*
	*/
	HAL_I2C_ErrorTypeDef ErrorCode;

	/*
	* Variable: hdmatx
	* Канала DMA для отправки данных
	*/
	DMA_ChannelHandleTypeDef *hdmatx;

	/*
	* Variable: hdmarx
	* Канала DMA для приема данных
	*/
	DMA_ChannelHandleTypeDef *hdmarx;

} I2C_HandleTypeDef;



void HAL_I2C_Disable(I2C_HandleTypeDef *hi2c);
void HAL_I2C_Reset(I2C_HandleTypeDef *hi2c);
void HAL_I2C_Enable(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AnalogFilterInit(I2C_HandleTypeDef *hi2c, HAL_I2C_AnalogFilterTypeDef AnalogFilter);
void HAL_I2C_DigitalFilterInit(I2C_HandleTypeDef *hi2c, HAL_I2C_DigitalFilterTypeDef DigitalFilter);
void HAL_I2C_SetClockSpeed(I2C_HandleTypeDef *hi2c);
void HAL_I2C_NoStretchMode(I2C_HandleTypeDef *hi2c, HAL_I2C_NoStretchModeTypeDef NoStretchMode);
void HAL_I2C_OwnAddress1(I2C_HandleTypeDef *hi2c);
void HAL_I2C_OwnAddress2(I2C_HandleTypeDef *hi2c);
void HAL_I2C_GeneralCall(I2C_HandleTypeDef *hi2c, HAL_I2C_GeneralCallTypeDef GeneralCall);
void HAL_I2C_SBCMode(I2C_HandleTypeDef *hi2c, HAL_I2C_SBCModeTypeDef SBCMode);
void HAL_I2C_SlaveInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AutoEnd(I2C_HandleTypeDef *hi2c, HAL_I2C_AutoEndModeTypeDef AutoEnd);
HAL_StatusTypeDef HAL_I2C_Master_WaitTXIS(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_WaitRXNE(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
void HAL_I2C_Master_SlaveAddress(I2C_HandleTypeDef *hi2h, uint16_t SlaveAddress);
HAL_StatusTypeDef HAL_I2C_WaitBusy(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_WaitADDR(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_WaitTXIS(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_WaitRXNE(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_WaitTCR(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
void HAL_I2C_Slave_ACK(I2C_HandleTypeDef *hi2c);
void HAL_I2C_Slave_NACK(I2C_HandleTypeDef *hi2c);
extern HAL_StatusTypeDef HAL_I2C_Slave_SBC(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t ByteCount);
HAL_StatusTypeDef HAL_I2C_Slave_ReceiveSBC(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize);
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t SlaveAddress, uint8_t *pData, uint16_t DataSize);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t DataSize, uint32_t Timeout);
#endif