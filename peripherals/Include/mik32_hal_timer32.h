#ifndef MIK32_HAL_TIMER32
#define MIK32_HAL_TIMER32

#include <timer32.h>


typedef struct
{
    uint8_t Source;         
    
    uint8_t Prescaler;      

} Timer32_ClockConfigTypeDef;

typedef struct
{
    TIMER32_TypeDef *Instance;

    Timer32_ClockConfigTypeDef Clock;


} Timer32_HandleTypeDef;

void HAL_Timer32_Stop(TIMER32_TypeDef *timer);

void HAL_Timer32_Init(TIMER32_TypeDef *timer, uint32_t top, uint32_t prescaler, uint32_t control, uint32_t intmask);

void HAL_Timer32_DeInit(TIMER32_TypeDef *timer);

void HAL_Timer32_Start(TIMER32_TypeDef *timer);

void HAL_Timer32_Delay(TIMER32_TypeDef *timer, uint32_t delay_ms);

void HAL_Timer32_DelayUs(TIMER32_TypeDef *timer, uint32_t delay_us);

#endif // MIK32_HAL_TIMER32
