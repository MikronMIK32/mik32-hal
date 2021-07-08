#ifndef FPGA_SYSTEM_LIB_H_INCLUDED
#define FPGA_SYSTEM_LIB_H_INCLUDED

#include "periphery/power_manager.h"

void SYSTEM_CONFIG_EnableAllApbClocks(PM_TypeDef* power_manager);

void SYSTEM_CONFIG_DisableAllApbClocks(PM_TypeDef* power_manager);

void SYSTEM_CONFIG_EnableAllClocks(PM_TypeDef* power_manager);

void SYSTEM_CONFIG_DisableAllClocks(PM_TypeDef* power_manager);


#endif /* FPGA_SYSTEM_LIB_H_INCLUDED */
