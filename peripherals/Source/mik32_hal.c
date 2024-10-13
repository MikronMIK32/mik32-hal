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
 */
__attribute__((section(".ram_text")))void HAL_ProgramDelayMs(uint32_t time_ms)
{
    //register uint32_t del = 1777 * (HAL_PCC_GetSysClockFreq() / (PM->DIV_AHB+1) / 1000UL) / 16000;
    register uint32_t del = 10695 * (HAL_PCC_GetSysClockFreq() / (PM->DIV_AHB+1) / 1000UL) / 32000;
    asm volatile(
        "beq    %[count], x0, end_metka_%="         "\n\t"
        "cycle_main_%=:"
        //"lui    t0, 0"                              "\n\t"
        //"addi   t0, %[temp], 1777"                  "\n\t"
        //"li     %[del], 3555/2"                     "\n\t"
        //"li     t0,10695"                            "\n\t"
        "mv     t0, %[del]"                         "\n\t"
        "cycle_internal_%=:"
        "addi   t0, t0, -1"                         "\n\t"
        "bne    t0, x0, cycle_internal_%="          "\n\t"
        "addi   %[count], %[count], -1"             "\n\t"
        "bne    %[count], x0, cycle_main_%="        "\n\t"
        "end_metka_%=:"
        ::
        [count] "r" (time_ms), [del] "r" (del) : "t0"
    );
}

/**
 * @brief Функция программной задержки в микросекундах
 * @p time_us - время в миллисекундах. Максимальное значение - 0xFFFFFFFF
 * 
 * Чем меньше частота AHB, тем больше будет погрешность задержки
 */
__attribute__((section(".ram_text")))void HAL_ProgramDelayUs(uint32_t time_us)
{
    //time_us >>= 3;
    //register uint32_t del = 14 * (HAL_PCC_GetSysClockFreq() / (PM->DIV_AHB+1) / 1000UL) / 16000;
    register uint32_t del = 10 * (HAL_PCC_GetSysClockFreq() / (PM->DIV_AHB+1) / 1000UL) / 32000;
    asm volatile(
        "beq    %[count], x0, end_metka_%="         "\n\t"
        "cycle_main_%=:"
        "mv     t0, %[del]"                         "\n\t"
        "cycle_internal_%=:"
        "addi   t0, t0, -1"                         "\n\t"
        "bne    t0, x0, cycle_internal_%="          "\n\t"
        "addi   %[count], %[count], -1"             "\n\t"
        "bne    %[count], x0, cycle_main_%="        "\n\t"
        "end_metka_%=:"
        ::
        [count] "r" (time_us), [del] "r" (del) : "t0"
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
