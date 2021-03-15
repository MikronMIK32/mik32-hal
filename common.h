#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "mcu32_memory_map.h"
#include "periphery/power_manager.h"
#include "periphery/pad_config.h"
#include "periphery/gpio.h"
#include "libs/xprintf.h"




void xputc (char c);
void SystemInit();
void exit();

void EnableInterrupts();
void DisableInterrupts();
void CheckTrapHandlerCalledOnce(int trap_handler_called);

void Port0_As_Gpio ();
void Port1_As_Gpio ();
void Port2_As_Gpio ();
void Port0_As_Func1 ();
void Port1_As_Func1 ();
void Port2_As_Func1 ();
void Port0_As_Func2 ();
void Port1_As_Func2 ();
void Port2_As_Func2 ();
void Port0_As_Func3 ();
void Port1_As_Func3 ();
void Port2_As_Func3 ();

#ifdef TEST_RESULT_ON_GPIO
    #define TEST_ERROR(...)                 \
        do {                                \
            xprintf("\nTEST ERROR: "); \
            xprintf(__VA_ARGS__);      \
            xprintf("\n");\
            GPIO_2->OUTPUT = 0xFF;  \
        } while(0)

    #define TEST_DONE()                     \
        do {                                \
            xprintf("\nTEST DONE\n");    \
            GPIO_2->OUTPUT = 0xF0;  \
        } while(0)
#else 
    #define TEST_ERROR(...)                 \
        do {                                \
            xprintf("\nTEST ERROR: "); \
            xprintf(__VA_ARGS__);      \
            xprintf("\n");\
         } while(0)

    #define TEST_DONE()                     \
        do {                                \
            xprintf("\nTEST DONE\n");    \
        } while(0)
#endif        

#endif
