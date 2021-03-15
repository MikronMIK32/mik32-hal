
#include "common.h"
#include "csr.h"
#include "scr1_csr_encoding.h"
#include "libs/uart_lib.h"

#ifndef UART_0_DIVIDER
    #define UART_0_DIVIDER     174
#endif




void xputc (char c) 
{
    UART_WriteByte(UART_0, c);
    UART_WaitTransmission(UART_0);

}


void SystemInit()
{
    UART_Init(UART_0, UART_0_DIVIDER, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0);
}


void exit()
{
    TEST_DONE();
    xputc(0x04);        // 0x04 - EOT - End Of Transmission 
    while (1) ;
}



void __attribute__((weak)) trap_handler()
{
    xprintf("\n\ntrap_handler\n");
    xprintf("mstatus: 0x%08x\n", read_csr(mstatus));
    xprintf("mtvec: 0x%08x\n", read_csr(mtvec));
    xprintf("mepc: 0x%08x\n", read_csr(mepc));
    xprintf("mcause: 0x%08x\n", read_csr(mcause));
    xprintf("mtval: 0x%08x\n", read_csr(mtval));
    
    exit();
}

void CheckTrapHandlerCalledOnce(int trap_handler_called)
{
    if (trap_handler_called != 1)
    {
        TEST_ERROR("trap_handler called %d times. Expected one time.", 
            trap_handler_called);
        DisableInterrupts();
    }
}

void EnableInterrupts()
{   
    set_csr(mstatus, MSTATUS_MIE);
    set_csr(mie, MIE_MEIE);
}

void DisableInterrupts()
{    
    clear_csr(mie, MIE_MEIE);
}
    


void Port0_As_Gpio ()
{
    PAD_CONFIG->PORT_0_CFG = 0x55555555;
}
void Port1_As_Gpio ()
{
    PAD_CONFIG->PORT_1_CFG = 0x55555555;
}
void Port2_As_Gpio ()
{
    PAD_CONFIG->PORT_2_CFG = 0x55555555;
}
void Port0_As_Func1 ()
{
    PAD_CONFIG->PORT_0_CFG = 0x00000000;
}
void Port1_As_Func1 ()
{
    PAD_CONFIG->PORT_1_CFG = 0x00000000;
}
void Port2_As_Func1 ()
{
    PAD_CONFIG->PORT_2_CFG = 0x00000000;
}

void Port0_As_Func2 ()
{
    PAD_CONFIG->PORT_0_CFG = 0xAAAAAAAA;
}
void Port1_As_Func2 ()
{
    PAD_CONFIG->PORT_1_CFG = 0xAAAAAAAA;
}
void Port2_As_Func2 ()
{
    PAD_CONFIG->PORT_2_CFG = 0xAAAAAAAA;
}

void Port0_As_Func3 ()
{
    PAD_CONFIG->PORT_0_CFG = 0xFFFFFFFF;
}
void Port1_As_Func3 ()
{
    PAD_CONFIG->PORT_1_CFG = 0xFFFFFFFF;
}
void Port2_As_Func3 ()
{
    PAD_CONFIG->PORT_2_CFG = 0xFFFFFFFF;
}

void Port0_As_Gpio ();
void Port1_As_Gpio ();
void Port2_As_Gpio ();
void Port0_As_Func1 ();
void Port1_As_Func1 ();
void Port2_As_Func1 ();
void Port0_As_Func2 ();
void Port1_As_Func2 ();
void Port2_As_Func2 ();
