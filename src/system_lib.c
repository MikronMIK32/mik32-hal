#include "system_lib.h"

void SYSTEM_CONFIG_EnableAllApbClocks(PM_TypeDef* power_manager)
{
    power_manager->CLK_APB_P_SET = 0xFFFFFFFF;
    power_manager->CLK_APB_M_SET = 0xFFFFFFFF;
}


void SYSTEM_CONFIG_DisableAllApbClocks(PM_TypeDef* power_manager)
{
    power_manager->CLK_APB_M_CLEAR = 0xFFFFFFFF;
    power_manager->CLK_APB_P_CLEAR = 0xFFFFFFFF;
}


void SYSTEM_CONFIG_EnableAllClocks(PM_TypeDef* power_manager)
{
    power_manager->CLK_APB_P_SET = 0xFFFFFFFF;
    power_manager->CLK_APB_M_SET = 0xFFFFFFFF;
    power_manager->CLK_AHB_SET   = 0xFFFFFFFF;
}


void SYSTEM_CONFIG_DisableAllClocks(PM_TypeDef* power_manager)
{
    power_manager->CLK_APB_M_CLEAR = 0xFFFFFFFF;
    power_manager->CLK_APB_P_CLEAR = 0xFFFFFFFF;
    power_manager->CLK_AHB_CLEAR   = 0xFFFFFFFF;
    
}

