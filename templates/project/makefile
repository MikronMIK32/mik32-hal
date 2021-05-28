
BASE_PATH = ./

MAKE_COMMON = $(BASE_PATH)/makefile.common
MCU32_HAL_DIR_PATH = $(BASE_PATH)/mcu32-hal-main

#SHARED_DIR_PATH 

DIR_INCLUDE = . $(SHARED_DIR_PATH) 
DIR_INCLUDE += $(SHARED_DIR_PATH)/include 
DIR_INCLUDE += $(MCU32_HAL_DIR_PATH)/src
SHARED_FILES = $(SHARED_DIR_PATH)/runtime/crt0.S 
SHARED_FILES += $(MCU32_HAL_DIR_PATH)/src/mcu_core.c
SHARED_FILES += $(MCU32_HAL_DIR_PATH)/src/mcu_digital.c
SHARED_FILES += $(MCU32_HAL_DIR_PATH)/src/mcu_timer.c

DIR_SRC = $(BASE_PATH)/src
DIR_OBJ = $(BASE_PATH)/build
DIR_BIN = $(BASE_PATH)/build

LD_SCRIPT = $(SHARED_DIR_PATH)/ldscripts/link_ramonly.ld 

CC_OPTS  =  -march=rv32imc -mabi=ilp32  
CC_OPTS +=   -Og -g
CC_OPTS_A += $(CC_OPTS) -D_ASSEMBLER_
LD_OPTS = --print-memory-usage
# Include common compiler options defined in $(MAKE_COMMON)
include $(MAKE_COMMON)


