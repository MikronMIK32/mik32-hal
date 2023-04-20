#ifndef MIK32_HAL_I2C
#define MIK32_HAL_I2C

#include "mcu32_memory_map.h"
#include "i2c.h"
#include "stdbool.h"


#define I2C_DATA_BYTES 10

/* Title: Макросы */

/*
 * Define: I2C_TIMEOUT
 * Количество циклов ожидания установки флага TXIS или RXNE
 *
 */
#define I2C_TIMEOUT 1000000   /* Количество циклов ожидания установки флага TXIS или RXNE */
/*
 * Define: I2C_NBYTE_MAX
 * Максимальлное количество байт в посылке (NBYTES)
 *
 */
#define I2C_NBYTE_MAX 255   /* Максимальлное количество байт в посылке (NBYTES) */

/*
 * Defines: Ошибки I2C
 *
 * I2C_ERROR_NONE    - Ошибок нет
 * I2C_ERROR_TIMEOUT - Превышено ожидание установки флага TXIS или RXNE
 * I2C_ERROR_NACK    - Во время передачи не получено подтверждение данных (NACK)
 * I2C_ERROR_BERR    - Ошибка шины 
 * I2C_ERROR_ARLO    - Проигрыш арбитража 
 * I2C_ERROR_OVR     - Переполнение или недозагрузка 
 *
 */
/* I2C_error - номера ошибок I2C*/
#define I2C_ERROR_NONE      0  /* Ошибок нет */
#define I2C_ERROR_TIMEOUT   1  /* Превышено ожидание установки флага TXIS или RXNE */
#define I2C_ERROR_NACK      2  /* Во время передачи не получено подтверждение данных (NACK) */
#define I2C_ERROR_BERR      3  /* Ошибка шины */
#define I2C_ERROR_ARLO      4  /* Проигрыш арбитража */
#define I2C_ERROR_OVR       5  /* Переполнение или недозагрузка */

/*
 * Defines: Режим адресации
 *
 * I2C_ADDRESSINGMODE_7BIT  - 7 битный адрес
 * I2C_ADDRESSINGMODE_10BIT - 10 битный адрес
 *
 */
/* I2C_addressing_mode - Режим адреса */
#define I2C_ADDRESSINGMODE_7BIT  0    /* 7 битный адрес */
#define I2C_ADDRESSINGMODE_10BIT 1    /* 10 битный адрес */

/*
 * Defines: Дополнительный адрес
 *
 * Дополнительный маскируемый 7 битный адрес
 *
 * I2C_DUALADDRESS_DISABLE - Выключить дополнительный адрес
 * I2C_DUALADDRESS_ENABLE  - Включить дополнительный адрес
 *
 */
/* I2C_dual_addressing_mode - Режим дополнительнрого адреса 7 бит */
#define I2C_DUALADDRESS_DISABLE 0   /* Выключить дополнительный адрес */
#define I2C_DUALADDRESS_ENABLE  1   /* Включить дополнительный адрес */

/*
 * Defines: Режим удержания SCL ведомым
 *
 * Отключение растягивания тактового сигнала в режиме "ведомый".
 * 
 * Изменение значения допускается только при выключенном блоке (PE=0). В режиме "ведущий" бит должен быть установлен в "0"
 *
 * I2C_NOSTRETCH_DISABLE - Растягивание активно
 * I2C_NOSTRETCH_ENABLE  - растягивание выключено
 *
 */
/* I2C_nostretch_mode - Режим удержания SCL ведомым */
#define I2C_NOSTRETCH_DISABLE 0   /* Растягивание активно */
#define I2C_NOSTRETCH_ENABLE  1   /* растягивание выключено */

/*
 * Defines: Режим аппаратного контроля передачи данных
 *
 * Режим аппаратного контроля передачи данных в режиме "ведомый"
 *
 * I2C_SBC_DISABLE - Аппаратный контроль выключен
 * I2C_SBC_ENABLE  - Аппаратный контроль включен
 *
 */
/* I2C_sbc_mode - Режим аппаратного контроля байта ведомым */
#define I2C_SBC_DISABLE 0   /* Аппаратный контроль выключен */
#define I2C_SBC_ENABLE  1   /* Аппаратный контроль включен */

/*
 * Defines: Режим перезаписи NBYTES
 *
 * Режим аппаратного контроля передачи данных в режиме "ведомый"
 *
 * I2C_RELOAD_DISABLE - Транзакция завершена после пересылки NBYTES байт данных (на шине ожидаются STOP или RESTART)
 * I2C_RELOAD_ENABLE  - Транзакция не завершена после пересылки NBYTES байт данных (значение NBYTES будет перезаписано)
 *
 */
/* I2C_reload_mode - Режим перезаписи NBYTES:  */
#define I2C_RELOAD_DISABLE 0    /* Транзакция завершена после пересылки NBYTES байт данных (на шине ожидаются STOP или RESTART) */
#define I2C_RELOAD_ENABLE  1    /* Транзакция не завершена после пересылки NBYTES байт данных (значение NBYTES будет перезаписано) */

/*
 * Defines: Сдвиг адреса
 *
 * Основной 7 битный адрес, записываемый в функции передачи, сдвигается на 1 бит вправо.
 *
 * SHIFT_ADDRESS_DISABLE - Адрес не сдвигается
 * SHIFT_ADDRESS_ENABLE  - Адрес сдвигается
 *
 */
/* I2C_address_shift - Сдвиг адреса ведомого */
#define SHIFT_ADDRESS_DISABLE 0   /* SHIFT_ADDRESS_DISABLE - Адрес не сдвигается */
#define SHIFT_ADDRESS_ENABLE  1   /* SHIFT_ADDRESS_ENABLE  - Адрес сдвигается */

/*
 * Defines: Режим автоматического окончания
 *
 * Управление режимом автоматического окончания в режиме "ведущий"
 *
 * AUTOEND_DISABLE - Режим автоматического окончания отключен
 * AUTOEND_ENABLE  - Режим автоматического окончания включен
 *
 */
/* I2C_autoend_mode - Режим автоматического окончания */
#define AUTOEND_DISABLE 0   /* Режим автоматического окончания отключен */
#define AUTOEND_ENABLE  1   /* Режим автоматического окончания включен */

/*
 * Defines: Направление передачи
 *
 * Направление передачи в режиме "ведущий"
 *
 * I2C_TRANSFER_WRITE - Ведущий запрашивает транзакцию записи
 * I2C_TRANSFER_READ  - Ведущий запрашивает транзакцию чтения
 *
 */
/* I2C_transfer_direction - Направление передачи */
#define I2C_TRANSFER_WRITE 0    /* Ведущий запрашивает транзакцию записи */
#define I2C_TRANSFER_READ  1    /* Ведущий запрашивает транзакцию чтения */

/*
 * Defines: Маска дополнительного адреса
 * 
 * Изменение битов допускается при OA2EN = 0.
 * 
 * I2C_OWNADDRESS2_MASK_DISABLE - Нет маски
 * I2C_OWNADDRESS2_MASK_111111x - Сравниваются только OA2[7:2]
 * I2C_OWNADDRESS2_MASK_11111xx - Сравниваются только OA2[7:3];
 * I2C_OWNADDRESS2_MASK_1111xxx - Сравниваются только OA2[7:4];
 * I2C_OWNADDRESS2_MASK_111xxxx - Сравниваются только OA2[7:5];
 * I2C_OWNADDRESS2_MASK_11xxxxx - Сравниваются только OA2[7:6];
 * I2C_OWNADDRESS2_MASK_1xxxxxx - Сравниваются только OA2[7];
 * I2C_OWNADDRESS2_MASK_1111111 - OA2[7:1] маскируются, подтверждаются (ACK) все 7-битные адреса (кроме зарезервированных)
 *
 */
/* I2C_OwnAddress2_mask - Маска второго собственного адреса */
#define I2C_OWNADDRESS2_MASK_DISABLE 0    /* Нет маски */
#define I2C_OWNADDRESS2_MASK_111111x 1    /* Сравниваются только OA2[7:2] */
#define I2C_OWNADDRESS2_MASK_11111xx 2    /* Сравниваются только OA2[7:3]; */
#define I2C_OWNADDRESS2_MASK_1111xxx 3    /* Сравниваются только OA2[7:4]; */
#define I2C_OWNADDRESS2_MASK_111xxxx 4    /* Сравниваются только OA2[7:5]; */
#define I2C_OWNADDRESS2_MASK_11xxxxx 5    /* Сравниваются только OA2[7:6]; */
#define I2C_OWNADDRESS2_MASK_1xxxxxx 6    /* Сравниваются только OA2[7]; */
#define I2C_OWNADDRESS2_MASK_1111111 7    /* OA2[7:1] маскируются, подтверждаются (ACK) все 7-битные адреса (кроме зарезервированных) */

/*
 * Defines: Цифровой фильтр
 *
 * Изменение значения допускается только при выключенном блоке (PE=0).
 *
 * I2C_FILTER_OFF           - цифровой фильтр выключен;
 * I2C_FILTER_1CLOCKCYCLES  - цифровой фильтр установлен на 1 такт I2CCLK; 
 * I2C_FILTER_2CLOCKCYCLES  - цифровой фильтр установлен на 2 такта I2CCLK; 
 * I2C_FILTER_15CLOCKCYCLES - цифровой фильтр установлен на 15 тактов I2CCLK 
 *
 */
/* I2C_filter - Цифровой фильтр */
#define I2C_FILTER_OFF           0
#define I2C_FILTER_1CLOCKCYCLES  1
#define I2C_FILTER_2CLOCKCYCLES  2
#define I2C_FILTER_15CLOCKCYCLES 15

/* Title: Перечисления */

/*
 * Enum: HAL_I2C_ModeTypeDef
 * Выбор режима ведущий/ведомый
 *
 * HAL_I2C_MODE_SLAVE -  Режим ведомого.
 * HAL_I2C_MODE_MASTER - Режим ведущего.
 *
 */
typedef enum {
  HAL_I2C_MODE_MASTER = 0, /* Режим ведущего */
  HAL_I2C_MODE_SLAVE = 1,  /* Режим ведомого */

} HAL_I2C_ModeTypeDef;

/* Title: Структуры */

/*
 * Struct: I2C_InitTypeDef
 *
 * Настройки инициализации I2C
 * 
 */
typedef struct
{

  uint32_t ClockSpeed; /* Задает тактовую частоту.
                          Этот параметр должен быть меньше чем 400kHz */

/*
 * Variable: AddressingMode
 * Режим адресации
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <I2C_ADDRESSINGMODE_7BIT>;
 * - <I2C_ADDRESSINGMODE_10BIT>. 
 *  
 *
 */
  uint32_t AddressingMode;

/*
 * Variable: OwnAddress1
 * Основной собственный адрес.
 * 
 * Этот параметр должен быть 7-битным или 10-битным числом (в зависимости от режима адресации)
 *
 */
  uint32_t OwnAddress1;

/*
 * Variable: DualAddressMode
 * Активация дополнительного 7 битного адреса
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <I2C_DUALADDRESS_DISABLE>;
 * - <I2C_DUALADDRESS_ENABLE>. 
 * 
 *
 */
  uint32_t DualAddressMode; 

/*
 * Variable: OwnAddress2
 * Дополнительный 7 битный адрес
 * 
 * Этот параметр должен быть 7 битным числом.
 *
 */
  uint32_t OwnAddress2;

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
  uint32_t OwnAddress2Mask;

/*
 * Variable: NoStretchMode
 * Растягивания тактового сигнала в режиме "ведомый".
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <I2C_NOSTRETCH_DISABLE>
 * - <I2C_NOSTRETCH_ENABLE>
 *
 */
  uint32_t NoStretchMode;

/*
 * Variable: SBCMode
 * Режим аппаратного контроля передачи данных в режиме "ведомый"
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <I2C_SBC_DISABLE>
 * - <I2C_SBC_ENABLE>
 *
 */
  uint32_t SBCMode;

/*
 * Variable: ReloadMode
 * Режим аппаратного контроля передачи данных в режиме "ведомый"
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <I2C_RELOAD_DISABLE>
 * - <I2C_RELOAD_ENABLE>
 *
 */
  uint32_t ReloadMode;

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
  uint32_t AutoEnd;

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
  uint32_t Filter;

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

/*
 * Struct: I2C_HandleTypeDef
 *
 * Настройки I2C
 * 
 */
typedef struct
{
/*
 * Variable: Instance
 * Базоый адрес регистров I2C
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
 * Variable: pBuff
 * Указатель на буфер передачи
 *
 */
  uint8_t *pBuff;
  
/*
 * Variable: TransferSize
 * Количество байт при передаче данных
 *
 */
  uint32_t TransferSize;
  
/*
 * Variable: TransferDirection
 * Направление передачи
 *
 */
  uint8_t TransferDirection;

/*
 * Variable: Mode
 * Режим I2C (ведущий или ведомый)
 *
 */
  HAL_I2C_ModeTypeDef Mode;
  
/*
 * Variable: ErrorCode
 * Код ошибки I2C
 *
 */
  uint32_t ErrorCode;
  
/*
 * Variable: SlaveAddress
 * Адрес ведомого устройства
 *
 */
  uint16_t SlaveAddress;    /* Адрес ведомого устройства */
  
/*
 * Variable: ShiftAddress
 * Основной 7 битный адрес, записываемый в функции передачи, сдвигается на 1 бит вправо.
 * 
 * Этот параметр должен быть одним из значений:
 * 
 * - <SHIFT_ADDRESS_DISABLE>
 * - <SHIFT_ADDRESS_ENABLE>
 *
 */
  uint8_t ShiftAddress;

} I2C_HandleTypeDef;

/* Title: Функции */

/*
 * Function: HAL_I2C_Disable
 * Выключить модуль I2C
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Disable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Enable
 * Включить модуль I2C
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Enable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_ReloadDisable
 * Выключить режим перезаписи NBYTES
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_ReloadDisable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_ReloadEnable
 * Включить режим перезаписи NBYTES
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_ReloadEnable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_SetFilter
 * Задать режим цифрового фильтра в соответсвии с настройками hi2c
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_SetFilter(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_SetClockSpeed
 * Задать значения временных ограчений в соответсвии с настройками hi2c
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_SetClockSpeed(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_MasterInit
 * Инициализация ведущего в соответсвии с настройками hi2c
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_MasterInit(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_SlaveInit
 * Инициализация ведомого в соответсвии с настройками hi2c
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_SlaveInit(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Init
 * Инициализация ведущего или ведомого в соответсвии с настройками hi2c
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_CheckError
 * Проверить и обработать ошибки
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_CheckError(I2C_HandleTypeDef *hi2c);

/* Ведущий */

/*
 * Function: HAL_I2C_Master_Stop
 * Сформировать сигнал STOP в режиме "ведущий". 
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_Stop(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Master_Restart
 * Запустить повторную инициализацию для сброса текущего состояния
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_Restart(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Master_CheckError
 * Проверить и обработать ошибки ведущего
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_CheckError(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Master_Transfer_Init
 * Подготовка перед передачей данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_Transfer_Init(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Master_WriteNBYTE
 * Отправить данные ведущим.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * data - Массив для передаваемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_WriteNBYTE(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Master_WriteNBYTE
 * Прочитать данные ведущим.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * data - Массив для считываемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_ReadNBYTE(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Master_WriteNBYTE
 * Отправить данные ведущим, а затем проверить и обработать ошибки
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * data - Массив для передаваемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_Write(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Master_Read
 * Прочитать данные ведущим, а затем проверить и обработать ошибки
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * data - Массив для считываемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Master_Read(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint8_t data[], uint32_t byte_count);

/* Ведомый */

/*
 * Function: HAL_I2C_Slave_SBC
 * Функция обработчик контроля байта ведомым
 * 
 * Используется при выключенном режиме аппаратного контроля байта ведомым. 
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * byte_count - Номер текущего байта
 * 
 * Returns:
 * void
 */
extern void HAL_I2C_Slave_SBC(I2C_HandleTypeDef *hi2c, uint32_t byte_count);

/*
 * Function: HAL_I2C_Slave_GetRequestedAddress
 * Получить запрошенный адрес
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * (uint8_t ) - запрошенный адрес
 */
uint8_t HAL_I2C_Slave_GetRequestedAddress(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_WaitADDR
 * Ожидать соответствия с собственным адресом. 
 * 
 * Функция ожидает установку флага ADDR, а затем сбрасывает его.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * void
 */
void HAL_I2C_Slave_WaitADDR(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_GetDirection
 * Получить направление передачи 
 * 
 * Возвращает значение флага DIR
 * 
 * - DIR = 0. Режим ведомого - приемник;
 * - DIR = 1. Режим ведомого - передатчик.
 * 
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * (int ) - Значение флага DIR
 */
int HAL_I2C_Slave_GetDirection(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_ACK
 * Сформировать сигнал ACK в режиме "ведомый"
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * void
 */
void HAL_I2C_Slave_ACK(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_NACK
 * Сформировать сигнал NACK в режиме "ведомый"
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * void
 */
void HAL_I2C_Slave_NACK(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_NACK
 * Очистить флаги ошибок ведомого
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * 
 * Returns:
 * void
 */
void HAL_I2C_Slave_CleanFlag(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_Restart
 * Запустить повторную инициализацию для сброса текущего состояния
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_Restart(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_CheckError
 * Проверить и обработать ошибки ведомого
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_CheckError(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_I2C_Slave_WriteNBYTE
 * Отправить данные ведомым.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * data - Массив для передаваемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_WriteNBYTE(I2C_HandleTypeDef *hi2c, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Slave_ReadNBYTE
 * Прочитать данные ведомым.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * data - Массив для считываемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_ReadNBYTE(I2C_HandleTypeDef *hi2c, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Slave_Write
 * Отправить данные ведомым, а затем проверить и обработать ошибки
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * data - Массив для передаваемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_Write(I2C_HandleTypeDef *hi2c, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_I2C_Master_Read
 * Прочитать данные ведомым, а затем проверить и обработать ошибки
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * data - Массив для считываемых данных
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_I2C_Slave_Read(I2C_HandleTypeDef *hi2c, uint8_t data[], uint32_t byte_count);

/*
 * Function: HAL_DMA_I2C_Master_Write
 * Отправить данные ведущим с использованием DMA.
 * 
 * Канал DMA должен быть настроен до вызова этой функции.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_Master_Write(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint32_t byte_count);

/*
 * Function: HAL_DMA_I2C_Master_Read
 * Прочитать данные ведущим с использованием DMA.
 * 
 * Канал DMA должен быть настроен до вызова этой функции.
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 * slave_adr - Адрес ведомого
 * byte_count - Количество байт данных
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_Master_Read(I2C_HandleTypeDef *hi2c, uint16_t slave_adr, uint32_t byte_count);

/*
 * Function: HAL_DMA_I2C_TXEnable
 * Включить режим поддержки DMA при передаче данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_TXEnable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_DMA_I2C_RXEnable
 * Включить режим поддержки DMA при приеме данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_RXEnable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_DMA_I2C_Enable
 * Включить режим поддержки DMA при передаче и приеме данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_Enable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_DMA_I2C_TXDisable
 * Выключить режим поддержки DMA при передаче данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_TXDisable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_DMA_I2C_RXDisable
 * Выключить режим поддержки DMA при приеме данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_RXDisable(I2C_HandleTypeDef *hi2c);

/*
 * Function: HAL_DMA_I2C_Disable
 * Выключить режим поддержки DMA при передаче и приеме данных
 *
 * Parameters:
 * hi2c - Указатель на структуру с настройками I2C
 *
 * Returns:
 * void
 */
void HAL_DMA_I2C_Disable(I2C_HandleTypeDef *hi2c);

#endif