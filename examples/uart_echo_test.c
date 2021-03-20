#include "include/mcu32_memory_map.h"
#include "libs/uart_lib.h"

#define UART_0_DIVIDER     174

int main()
{

  UART_Init(UART_0, UART_0_DIVIDER,
      UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M | UART_CONTROL1_RE_M,
      0, 0);

  while (1) {
    UART_WaitReceiving(UART_0);
    char a = UART_ReadByte(UART_0);
    UART_WriteByte(UART_0, a);
    UART_WaitTransmission(UART_0);
  }
}
