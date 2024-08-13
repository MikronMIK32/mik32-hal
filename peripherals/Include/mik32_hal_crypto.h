#ifndef MIK32_HAL_CRYPTO
#define MIK32_HAL_CRYPTO

#include "mik32_hal_pcc.h"
#include "crypto.h"
#include "pad_config.h"
#include "mik32_memory_map.h"


/** 
 * @name Длина вектора инициализации 
 *  @{
 */
#define IV_LENGTH_KUZNECHIK_CBC     CRYPTO_BLOCK_KUZNECHIK          /**< Количество слов вектора инициализации в режиме шифрования CBC (Кузнечик). */
#define IV_LENGTH_MAGMA_CBC         CRYPTO_BLOCK_MAGMA              /**< Количество слов вектора инициализации в режиме шифрования CBC (Магма). */
#define IV_LENGTH_AES_CBC           CRYPTO_BLOCK_AES                /**< Количество слов вектора инициализации в режиме шифрования CBC (AES). */
#define IV_LENGTH_KUZNECHIK_CTR     (CRYPTO_BLOCK_KUZNECHIK >> 1)   /**< Количество слов вектора инициализации в режиме шифрования CTR (Кузнечик). */
#define IV_LENGTH_MAGMA_CTR         (CRYPTO_BLOCK_MAGMA     >> 1)   /**< Количество слов вектора инициализации в режиме шифрования CTR (Магма). */
#define IV_LENGTH_AES_CTR           (CRYPTO_BLOCK_AES       >> 1)   /**< Количество слов вектора инициализации в режиме шифрования CTR (AES). */
/** @} */

/** 
 * @name Длина ключа 
 *  @{
 */
#define CRYPTO_KEY_KUZNECHIK        8       /**< Длина ключа Кузнечик. */
#define CRYPTO_KEY_MAGMA            8       /**< Длина ключа Магма. */
#define CRYPTO_KEY_AES              4       /**< Длина ключа AES. */
/** @} */

/** 
 * @name Длина блока
 *  @{
 */
#define CRYPTO_BLOCK_KUZNECHIK      4       /**< Длина блока Кузнечик. */
#define CRYPTO_BLOCK_MAGMA          2       /**< Длина блока Магма. */
#define CRYPTO_BLOCK_AES            4       /**< Длина блока AES. */
/** @} */

/** 
 * @name Алгоритм шифрования
 *  @{
 */
#define CRYPTO_ALG_KUZNECHIK        0       /**< Алгоритм шифрования Кузнечик. */
#define CRYPTO_ALG_MAGMA            1       /**< Алгоритм шифрования Магма. */
#define CRYPTO_ALG_AES              2       /**< Алгоритм шифрования AES. */
/** @} */

/** 
 * @name Режим шифрования
 *  @{
 */
#define CRYPTO_CIPHER_MODE_ECB      0       /**< Режим шифрования ECB (Electronic Codebook). */
#define CRYPTO_CIPHER_MODE_CBC      1       /**< Режим шифрования CBC (Cipher Block Chaining). */
#define CRYPTO_CIPHER_MODE_CTR      2       /**< Режим шифрования CTR (Counter mode). */
/** @} */

/** 
 * @name Перестановка слова
 *  @{
 */
#define CRYPTO_SWAP_MODE_NONE       0       /**< Нет перестановки. */
#define CRYPTO_SWAP_MODE_HALFWORD   1       /**< Перестановка по полуслову. */
#define CRYPTO_SWAP_MODE_BYTE       2       /**< Перестановки по байтам. */
#define CRYPTO_SWAP_MODE_BIT        3       /**< Перестановка по битам. */
/** @} */

/** 
 * @name Порядок загрузки/выгрузки
 *  @{
 */
#define CRYPTO_ORDER_MODE_LSW       0       /**< Порядка загрузки/выгрузки от младшего слова к старшему. */
#define CRYPTO_ORDER_MODE_MSW       1       /**< Порядка загрузки/выгрузки от старшего слова к младшему. */
/** @} */


#define MAXIMUM_KEY_LENGTH          8       /**< Максимальная длина ключа. */  


/**
 * @brief Настройки Crypto.
 * 
 */
typedef struct __Crypto_HandleTypeDef
{
    CRYPTO_TypeDef *Instance; /**< Базовый адрес регистров Crypto */

    uint8_t Algorithm; /**< Выбор алгоритма шифрования. Этот параметр должен быть одним из значений: #CRYPTO_ALG_KUZNECHIK, #CRYPTO_ALG_MAGMA, #CRYPTO_ALG_AES.*/

    uint8_t CipherMode; /**< Выбор режима шифрования. Этот параметр должен быть одним из значений: #CRYPTO_CIPHER_MODE_ECB, #CRYPTO_CIPHER_MODE_CBC, #CRYPTO_CIPHER_MODE_CTR. */

    uint8_t SwapMode; /**< Перестановка слова. Этот параметр должен быть одним из значений: #CRYPTO_SWAP_MODE_NONE, #CRYPTO_SWAP_MODE_HALFWORD, #CRYPTO_SWAP_MODE_BYTE, #CRYPTO_SWAP_MODE_BIT. */
    
    uint8_t OrderMode; /**< Выбор порядка загрузки/выгрузки. Этот параметр должен быть одним из значений: #CRYPTO_ORDER_MODE_LSW, #CRYPTO_ORDER_MODE_MSW. */

} Crypto_HandleTypeDef;


void HAL_CRYPTO_MspInit(Crypto_HandleTypeDef* hcrypto);
void HAL_Crypto_CounterReset(Crypto_HandleTypeDef *hcrypto);
void HAL_Crypto_WaitReady(Crypto_HandleTypeDef *hcrypto);
void HAL_Crypto_SetAlgorithm(Crypto_HandleTypeDef *hcrypto, uint8_t Algorithm);
void HAL_Crypto_SetCipherMode(Crypto_HandleTypeDef *hcrypto, uint8_t CipherMode);
void HAL_Crypto_SetSwapMode(Crypto_HandleTypeDef *hcrypto, uint8_t SwapMode);
void HAL_Crypto_SetOrderMode(Crypto_HandleTypeDef *hcrypto, uint8_t OrderMode);
void HAL_Crypto_SetIV(Crypto_HandleTypeDef *hcrypto, uint32_t InitVector[], uint32_t IvLength);
void HAL_Crypto_SetKey(Crypto_HandleTypeDef *hcrypto, uint32_t crypto_key[]);
void HAL_Crypto_Init(Crypto_HandleTypeDef *hcrypto);
void HAL_Crypto_Encode(Crypto_HandleTypeDef *hcrypto, uint32_t plain_text[], uint32_t cipher_text[], uint32_t text_length);
void HAL_Crypto_Decode(Crypto_HandleTypeDef *hcrypto, uint32_t cipher_text[], uint32_t plain_text[], uint32_t text_length);

#endif