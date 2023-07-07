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
    uint32_t Top;

    TIMER32_TypeDef *Instance;

    uint8_t Enable;

    Timer32_ClockConfigTypeDef Clock;

    uint32_t InterruptMask;

    uint8_t CountMode;

} Timer32_HandleTypeDef;

typedef struct
{
    TIMER32_CHANNEL_TypeDef *Instance;

    uint8_t Enable;

    uint8_t PWM_Invert;

    uint8_t Mode;

    uint8_t Edge;

    uint32_t OCR;

    uint32_t ICR;
    
    uint8_t Noise;

} Timer32_Channel_HandleTypeDef;


void HAL_Timer32_Init(Timer32_HandleTypeDef *timer);

void HAL_Timer32_DeInit(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Enable(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Disable(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Stop(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Start(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Top_Set(Timer32_HandleTypeDef *timer, uint32_t top);

void HAL_Timer32_Clock_Set(Timer32_HandleTypeDef *timer, Timer32_ClockConfigTypeDef clockConfig);

void HAL_Timer32_InterruptMask_Set(Timer32_HandleTypeDef *timer, uint32_t intMask);

void HAL_Timer32_CountMode_Set(Timer32_HandleTypeDef *timer, uint8_t countMode);

uint32_t HAL_Timer32_Value_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Value_Clean(Timer32_HandleTypeDef *timer);

uint32_t HAL_Timer32_InterruptFlags_Get(Timer32_HandleTypeDef *timer);

void HAL_Timer32_InterruptFlags_Clean(Timer32_HandleTypeDef *timer);

void HAL_Timer32_Channel_Init(Timer32_Channel_HandleTypeDef *timer);

void HAL_Timer32_Channel_DeInit(Timer32_Channel_HandleTypeDef *timer);

void HAL_Timer32_Channel_PWM_Invert_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t PWM_Invert);

void HAL_Timer32_Channel_Mode_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t mode);

void HAL_Timer32_Channel_Edge_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t edge);

void HAL_Timer32_Channel_OCR_Set(Timer32_Channel_HandleTypeDef *timer, uint32_t OCR);

void HAL_Timer32_Channel_ICR_Set(Timer32_Channel_HandleTypeDef *timer, uint32_t ICR);

void HAL_Timer32_Channel_Noise_Set(Timer32_Channel_HandleTypeDef *timer, uint8_t noise);

#endif // MIK32_HAL_TIMER32
