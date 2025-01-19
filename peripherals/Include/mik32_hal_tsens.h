#ifndef MIK32_TSENS
#define MIK32_TSENS

#ifndef MIK32V0

#include "analog_reg.h"
#include "pad_config.h"
#include "mik32_memory_map.h"
#include "power_manager.h"
#include "mik32_hal_def.h"
#include "mik32_hal_pcc.h"


#define TSENS_OPTIMAL_FREQUENCY 40000                                                                                               /**< Рекомендуемая частота работы термосенсора. */

#define __HAL_TSENS_FORCE_RESET(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CFG &= ~TSENS_CFG_NRST_M)                                /**< Установить состояние сброса термосенсора. */
#define __HAL_TSENS_RELEASE_RESET(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CFG |= TSENS_CFG_NRST_M)                               /**< Снять состояние сброса термосенсора. */

#define __HAL_TSENS_SINGLE_START(__HANDLE__) ((__HANDLE__)->Instance->TSENS_SINGLE = 1)                                             /**< Запустить одиночное измерение температуры. */
#define __HAL_TSENS_CONTINUOUS_START(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CONTINUOUS = 1)                                     /**< Запуск непрерывного измерения температуры. */
#define __HAL_TSENS_CONTINUOUS_STOP(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CONTINUOUS = 0)                                      /**< Остановка непрерывного измерения температуры. */
#define __HAL_TSENS_GET_EOC(__HANDLE__) (((__HANDLE__)->Instance->TSENS_VALUE & TSENS_VALUE_EOC_M) == TSENS_VALUE_EOC_M)            /**< Получить состояние флага окончания преобразования (EOC). */
#define __HAL_TSENS_READ_MEASUREMENT(__HANDLE__) ((__HANDLE__)->Instance->TSENS_VALUE & TSENS_VALUE_VALUE_M)                        /**< Получить результат измерения термосенсора. */

#define __HAL_TSENS_IRQ_ENABLE_LOWIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ |= TSENS_IRQ_LOW_MASK_M)                       /**< Разрешить прерывание по выходу за нижний порог температуры. */
#define __HAL_TSENS_IRQ_DISABLE_LOWIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ &= ~TSENS_IRQ_LOW_MASK_M)                     /**< Запретить прерывание по выходу за нижний порог температуры. */
#define __HAL_TSENS_IRQ_CLEAR_LOWIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CLEAR_IRQ = TSENS_CLEAR_IRQ_LOW_CLEAR_M)            /**< Очистить флаг прерывания по выходу за нижний порог температуры (LOW_IRQ). */
#define __HAL_TSENS_IRQ_GET_LOWIRQ(__HANDLE__) (((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_LOW_IRQ_M) == TSENS_IRQ_LOW_IRQ_M)   /**< Получить состояние флага прерывания по выходу за нижний порог температуры (LOW_IRQ). */
/** Получить состояние флага прерывания по выходу за нижний порог температуры (LOW_IRQ) с учетом маски разрешенных прерываний. */
#define __HAL_TSENS_IRQ_GET_LOWIRQ_MASK(__HANDLE__) ((((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_LOW_IRQ_M) & (((__HANDLE__)->Instance->TSENS_IRQ & 0b111) << 3)) == TSENS_IRQ_LOW_IRQ_M) 

#define __HAL_TSENS_IRQ_ENABLE_HIIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ |= TSENS_IRQ_HI_MASK_M)                         /**< Разрешить прерывание по выходу за верхний порог температуры. */
#define __HAL_TSENS_IRQ_DISABLE_HIIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ &= ~TSENS_IRQ_HI_MASK_M)                       /**< Запретить прерывание по выходу за верхний порог температуры. */
#define __HAL_TSENS_IRQ_CLEAR_HIIRQ(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CLEAR_IRQ = TSENS_CLEAR_IRQ_HI_CLEAR_M)              /**< Очистить флаг прерывания по выходу за верхний порог температуры (HI_IRQ). */
#define __HAL_TSENS_IRQ_GET_HIIRQ(__HANDLE__) (((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_HI_IRQ_M) == TSENS_IRQ_HI_IRQ_M)      /**< Получить состояние флага прерывания по выходу за верхний порог температуры (HI_IRQ). */
/** Получить состояние флага прерывания по выходу за верхний порог температуры (HI_IRQ) с учетом маски разрешенных прерываний. */
#define __HAL_TSENS_IRQ_GET_HIIRQ_MASK(__HANDLE__) ((((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_HI_IRQ_M) & (((__HANDLE__)->Instance->TSENS_IRQ & 0b111) << 3)) == TSENS_IRQ_HI_IRQ_M)

#define __HAL_TSENS_IRQ_ENABLE_EOC(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ |= TSENS_IRQ_EOC_MASK_M)                           /**< Разрешить прерывание по окончанию преобразования. */       
#define __HAL_TSENS_IRQ_DISABLE_EOC(__HANDLE__) ((__HANDLE__)->Instance->TSENS_IRQ &= ~TSENS_IRQ_EOC_MASK_M)                         /**< Запретить прерывание по окончанию преобразования. */           
#define __HAL_TSENS_IRQ_CLEAR_EOC(__HANDLE__) ((__HANDLE__)->Instance->TSENS_CLEAR_IRQ = TSENS_CLEAR_IRQ_EOC_CLEAR_M)                /**< Очистить флаг прерывания по окончанию преобразования (EOC_IRQ). */                   
#define __HAL_TSENS_IRQ_GET_EOC(__HANDLE__) (((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_EOC_IRQ_M) == TSENS_IRQ_EOC_IRQ_M)       /**< Получить состояние флага прерывания по окончанию преобразования (EOC_IRQ). */   
/** Получить состояние флага прерывания по окончанию преобразования (EOC_IRQ) с учетом маски разрешенных прерываний. */                        
#define __HAL_TSENS_IRQ_GET_EOC_MASK(__HANDLE__) ((((__HANDLE__)->Instance->TSENS_IRQ & TSENS_IRQ_EOC_IRQ_M) & (((__HANDLE__)->Instance->TSENS_IRQ & 0b111) << 3)) == TSENS_IRQ_EOC_IRQ_M)

/** 
 * Разрешить прерывание . __INTERRUPT__ может быть одним из значений:
 *      - TSENS_IRQ_LOW_MASK_M: выход за нижний порог температуры;
 *      - TSENS_IRQ_HI_MASK_M:  выход за верхний порог температуры;
 *      - TSENS_IRQ_EOC_MASK_M: окончание преобразования. 
 */  
#define __HAL_TSENS_IRQ_ENABLE(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->TSENS_IRQ |= (__INTERRUPT__))   
/** 
 * Запретить прерывание. __INTERRUPT__ может быть одним из значений:
 *      - TSENS_IRQ_LOW_MASK_M: выход за нижний порог температуры;
 *      - TSENS_IRQ_HI_MASK_M:  выход за верхний порог температуры;
 *      - TSENS_IRQ_EOC_MASK_M: окончание преобразования. 
 */                                                            
#define __HAL_TSENS_IRQ_DISABLE(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->TSENS_IRQ &= ~(__INTERRUPT__))     
/** 
 * Очистить флаг прерывания. __FLAG__ может быть одним из значений:
 *      - TSENS_CLEAR_IRQ_LOW_CLEAR_M: выход за нижний порог температуры;
 *      - TSENS_CLEAR_IRQ_HI_CLEAR_M:  выход за верхний порог температуры;
 *      - TSENS_CLEAR_IRQ_EOC_CLEAR_M: окончание преобразования. 
 */                       
#define __HAL_TSENS_IRQ_CLEAR(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->TSENS_CLEAR_IRQ = (__FLAG__))        
/** 
 * Получить состояние флага прерывания. __FLAG__ может быть одним из значений:
 *      - TSENS_IRQ_LOW_IRQ_M: выход за нижний порог температуры;
 *      - TSENS_IRQ_HI_IRQ_M:  выход за верхний порог температуры;
 *      - TSENS_IRQ_EOC_IRQ_M: окончание преобразования. 
 */                      
#define __HAL_TSENS_IRQ_GET(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->TSENS_IRQ & (__FLAG__)) == (__FLAG__))   
/** 
 * Получить состояние флага прерывания с учетом маски разрешенных прерываний.. __FLAG__ может быть одним из значений:
 *      - TSENS_IRQ_LOW_IRQ_M: выход за нижний порог температуры;
 *      - TSENS_IRQ_HI_IRQ_M:  выход за верхний порог температуры;
 *      - TSENS_IRQ_EOC_IRQ_M: окончание преобразования. 
 */                                       
#define __HAL_TSENS_IRQ_GET_MASK(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->TSENS_IRQ & (__FLAG__)) & (((__HANDLE__)->Instance->TSENS_IRQ & 0b111) << 3)) == (__FLAG__))


/*
 * Define
 * Константы-параметры функции установки источника тактирования TSENS
 */

/**
 * @brief Перечисление источников тактирования термосенсора.
 */
typedef enum __HAL_TSENS_ClockTypeDef
{
    HAL_TSENS_SYS_CLK         = 0x0,    /**< Системная частота (sys_clk). */
    HAL_TSENS_HCLK            = 0x1,    /**< Частота шины AHB (hclk). */
    HAL_TSENS_OSC32M          = 0x2,    /**< Частота внешнего высокочастотного осциллятора до 32 МГц. */
    HAL_TSENS_HSI32M          = 0x3,    /**< Частота внутреннего высокочастотного осциллятора 32 МГц. */
    HAL_TSENS_OSC32K          = 0x4,    /**< Частота внешнего осциллятора 32 кГц. */
    HAL_TSENS_LSI32K          = 0x5     /**< Частота внутреннего осциллятора 32 кГц. */
} HAL_TSENS_ClockTypeDef;

/**
 * @brief Определение структуры TSENS Handle.
 */
typedef struct __TSENS_HandleTypeDef
{

    ANALOG_REG_TypeDef *Instance;   /**< Адрес регистров блока управления аналоговой подсистемой. */

    HAL_TSENS_ClockTypeDef Clock;   /**< Источник тактирования TSENS. */

    uint32_t Frequency;             /**< Частота работы температурного сенсора в Гц. Значение этого параметра может быть не больше 100кГц. */

} TSENS_HandleTypeDef;

/**
 * @brief Возвращаемая структура для функции @ref HAL_TSENS_SingleStart.
 */
typedef struct __TSENS_ValueTypeDef
{
    HAL_StatusTypeDef statusHAL;  /**< Статус HAL. */

    uint32_t value;   /**< Значение температуры в градусах Цельсия. Значение в 100 раз больше. */

} TSENS_ValueTypeDef;


void HAL_TSENS_MspInit(TSENS_HandleTypeDef *htsens);
HAL_StatusTypeDef HAL_TSENS_Init(TSENS_HandleTypeDef *htsens);
HAL_StatusTypeDef HAL_TSENS_ClockSource(TSENS_HandleTypeDef *htsens, HAL_TSENS_ClockTypeDef clk_source);
HAL_StatusTypeDef HAL_TSENS_ClockDivider(TSENS_HandleTypeDef *htsens, uint16_t clk_div);
HAL_StatusTypeDef HAL_TSENS_Clock(TSENS_HandleTypeDef *htsens, uint32_t f_enter);


HAL_StatusTypeDef HAL_TSENS_SetLowThresholdRaw(TSENS_HandleTypeDef *htsens, uint16_t low_tres);
HAL_StatusTypeDef HAL_TSENS_SetHiThresholdRaw(TSENS_HandleTypeDef *htsens, uint16_t hi_tres);
HAL_StatusTypeDef HAL_TSENS_SetLowThreshold(TSENS_HandleTypeDef *htsens, int low_temp_border);
HAL_StatusTypeDef HAL_TSENS_SetHiThreshold(TSENS_HandleTypeDef *htsens, int hi_temp_border);


uint32_t HAL_TSENS_GetTemperature(TSENS_HandleTypeDef *htsens);
void HAL_TSENS_ContinuousStart(TSENS_HandleTypeDef *htsens);
TSENS_ValueTypeDef HAL_TSENS_SingleStart(TSENS_HandleTypeDef *htsens, uint32_t timeout);
void HAL_TSENS_ContinuousStart_IT(TSENS_HandleTypeDef *htsens);
void HAL_TSENS_SingleStart_IT(TSENS_HandleTypeDef *htsens);
void HAL_TSENS_Stop_IT(TSENS_HandleTypeDef *htsens);


#endif // MIK32V0
#endif // MIK32_TSENS