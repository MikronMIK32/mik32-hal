#include "mik32_hal.h"


__attribute__((weak)) void HAL_MspInit()
{
    __HAL_PCC_PAD_CONFIG_CLK_ENABLE();   
    __HAL_PCC_EPIC_CLK_ENABLE();
}

HAL_StatusTypeDef HAL_Init()
{
	HAL_MspInit();

	return HAL_OK;
}


/**
 * @brief Функция программной задержки в миллисекундах
 * @p time_ms - время в миллисекундах. Максимальное значение - 0xFFFFFFFF
 * 
 * TODO: добавить обработку случая, когда time_ms = 0
 */
__attribute__((section(".ram_text")))void HAL_ProgramDelayMs(uint32_t time_ms)
{
    asm volatile(
        "beq    %[count], x0, end_metka_%="         "\n\t"
        "cycle_main_%=:"
        //"lui    t0, 0"                              "\n\t"
        //"addi   t0, %[temp], 1777"                  "\n\t"
        "li     t0, 3554"                           "\n\t"
        "cycle_internal_%=:"
        "addi   t0, t0, -1"                         "\n\t"
        "bne    t0, x0, cycle_internal_%="          "\n\t"
        "addi   %[count], %[count], -1"             "\n\t"
        "bne    %[count], x0, cycle_main_%="        "\n\t"
        "end_metka_%=:"
        ::
        [count] "r" (time_ms)
    );
}

__attribute__((section(".ram_text")))void HAL_ProgramDelayUs(uint32_t time_us)
{
    time_us >>= 1;
    asm volatile(
        "beq    %[count], x0, end_metka_%="         "\n\t"
        "cycle_main_%=:"
        "li     t0, 7"                              "\n\t"
        "cycle_internal_%=:"
        "addi   t0, t0, -1"                         "\n\t"
        "bne    t0, x0, cycle_internal_%="          "\n\t"
        "addi   %[count], %[count], -1"             "\n\t"
        "bne    %[count], x0, cycle_main_%="        "\n\t"
        "end_metka_%=:"
        ::
        [count] "r" (time_us)
    );
}

__attribute__((weak)) uint32_t HAL_Micros()
{
    return 0;
}

__attribute__((weak)) uint32_t HAL_Millis()
{
    return 0;
}

__attribute__((weak)) void HAL_DelayMs(uint32_t time_ms)
{
    HAL_ProgramDelayMs(time_ms);
}

__attribute__((weak)) void HAL_DelayUs(uint32_t time_us)
{
    HAL_ProgramDelayUs(time_us);
}
