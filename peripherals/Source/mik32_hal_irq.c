#include "mik32_hal_irq.h"

// #define MIK32_IRQ_DEBUG

void HAL_IRQ_EnableInterrupts()
{   
    set_csr(mstatus, MSTATUS_MIE);
    set_csr(mie, MIE_MEIE);
}

void HAL_IRQ_DisableInterrupts()
{    
    clear_csr(mie, MIE_MEIE);
}

void HAL_EPIC_MaskSet(uint32_t InterruptMask)
{
    EPIC->MASK_EDGE_SET |= InterruptMask;
}

void HAL_EPIC_MaskClear(uint32_t InterruptMask)
{
    EPIC->MASK_EDGE_CLEAR |= InterruptMask;
}

void HAL_EPIC_MaskLevelSet(uint32_t InterruptMask)
{
    EPIC->MASK_LEVEL_SET |= InterruptMask;
}

void HAL_EPIC_MaskLevelClear(uint32_t InterruptMask)
{
    EPIC->MASK_LEVEL_CLEAR |= InterruptMask;
}

uint32_t HAL_EPIC_GetStatus()
{
    return EPIC->STATUS;
}

uint32_t HAL_EPIC_GetRawStatus()
{
    return EPIC->RAW_STATUS;
}












