#include "mik32_hal_otp.h"
#include "mik32_hal.h"

/**
 * @brief Включение тактирования модуля OTP.
 * 
 * Эта функция может быть переопределена пользователем
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 */
__attribute__((weak)) void HAL_OTP_MspInit(OTP_HandleTypeDef* hotp)
{
    __HAL_PCC_OTP_CONTROLLER_CLK_ENABLE();
}

/**
 * @brief Включить/выключить режим пониженного энергопотребления.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 * @param PowerOff Режим пониженного энергопотребления.
 */
void HAL_OTP_PowerOff(OTP_HandleTypeDef *hotp, uint8_t PowerOff)
{
    uint32_t OtpadjConfig = hotp->Instance->OTPADJ;

    OtpadjConfig &= ~OTP_OTPADJ_POWER_OFF_I_M;
    OtpadjConfig |= PowerOff << OTP_OTPADJ_POWER_OFF_I_S;

    hotp->Instance->OTPADJ = OtpadjConfig;

    /* После выхода из режима пониженного энергопотребления необходимо выдержать паузу */
    for (volatile int i = 0; i < 1000; i++);  
}

/**
 * @brief Задать напряжение на UPP матрице.
 *
 * @param hotp Указатель на структуру с настройками OTP.
 * @param UppReadVoltage Напряжение на UPP матрице.
 */
void HAL_OTP_SetUppRead(OTP_HandleTypeDef *hotp, uint8_t UppReadVoltage)
{
    uint32_t OtpadjConfig = hotp->Instance->OTPADJ;

    OtpadjConfig &= ~OTP_OTPADJ_SEL_UPP_READ_I_M;
    OtpadjConfig |= UppReadVoltage << OTP_OTPADJ_SEL_UPP_READ_I_S;

    hotp->Instance->OTPADJ = OtpadjConfig;
}

/**
 * @brief Задать ток считывания.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 * @param ReadCur Ток считывания.
 */
void HAL_OTP_SetReadCur(OTP_HandleTypeDef *hotp, uint8_t ReadCur)
{
    uint32_t OtpadjConfig = hotp->Instance->OTPADJ;

    OtpadjConfig &= ~OTP_OTPADJ_SEL_READ_CUR_I_M;
    OtpadjConfig |= ReadCur << OTP_OTPADJ_SEL_READ_CUR_I_S;

    hotp->Instance->OTPADJ = OtpadjConfig;
}

/**
 * @brief Инициализация рекомендуемых временных интервалов.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 */
void HAL_OPT_TimeInit(OTP_HandleTypeDef *hotp)
{
    uint8_t APBMDivider = PM->DIV_APB_M;
    uint8_t AHBDivider = PM->DIV_AHB;
    uint32_t OscillatorSystem = PM->AHB_CLK_MUX & PM_AHB_CLK_MUX_M;  
    
    uint32_t OtpadjConfig = hotp->Instance->OTPADJ;
    uint32_t otpwt1_config = hotp->Instance->OTPWT1;
    uint32_t otpwt2_config = hotp->Instance->OTPWT2;

    /* Рекомендуемое значение для N_RA, N_RH, N_SU, N_H. ceil(40/Period), где Period в нс */
    uint32_t Div = 0; 
    /* Тактирование от 32МГц */
    if((OscillatorSystem == PM_AHB_CLK_MUX_HSI32M_M) || (OscillatorSystem == PM_AHB_CLK_MUX_OSC32M_M))
    {
        Div = 1000;
    }

    /* Тактирование от 32кГц */
    if((OscillatorSystem == PM_AHB_CLK_MUX_LSI32K_M) || (OscillatorSystem == PM_AHB_CLK_MUX_OSC32K_M))
    {
        Div = 1000000;
    }

    uint32_t recommended_value = 0; 
    if((40 * 32) % (((APBMDivider + 1) * (AHBDivider + 1)) * Div) == 0)
    {
        recommended_value = (40 * 32) / (((APBMDivider + 1) * (AHBDivider + 1)) * Div);
    }
    else
    {
        recommended_value = (40 * 32) / (((APBMDivider + 1) * (AHBDivider + 1)) * Div) + 1;
    }


    /* Рекомендуемое значение N_W = 50 000 000 нс / Pclk, где Pclk – период тактового сигнала в нс */
    uint32_t recommended_N_W = 0; 
    if((OscillatorSystem == PM_AHB_CLK_MUX_HSI32M_M) || (OscillatorSystem == PM_AHB_CLK_MUX_OSC32M_M))
    {
        recommended_N_W = (5 * 32 * 10000) / ((APBMDivider + 1) * (AHBDivider + 1));
    }

    /* Тактирование от 32кГц */
    if((OscillatorSystem == PM_AHB_CLK_MUX_LSI32K_M) || (OscillatorSystem == PM_AHB_CLK_MUX_OSC32K_M))
    {
        recommended_N_W = (5 * 32 * 10) / ((APBMDivider + 1) * (AHBDivider + 1));
    }

    OtpadjConfig &= ~OTP_OTPADJ_N_RSU_M; /* При частоте менее 200МГц рекомендуемое значение 0 */

    OtpadjConfig &= ~OTP_OTPADJ_N_RA_M;
    OtpadjConfig |= recommended_value << OTP_OTPADJ_N_RA_S;

    OtpadjConfig &= ~OTP_OTPADJ_N_RH_M;
    OtpadjConfig |= recommended_value << OTP_OTPADJ_N_RH_S;

    otpwt1_config &= ~OTP_OTPWT1_N_SU_M;
    otpwt1_config |= recommended_value << OTP_OTPWT1_N_SU_S;

    otpwt1_config &= ~OTP_OTPWT1_N_H_M;
    otpwt1_config |= recommended_value << OTP_OTPWT1_N_H_S;

    otpwt2_config &= ~OTP_OTPWT2_N_W_M;
    otpwt2_config |= recommended_N_W << OTP_OTPWT2_N_W_S;

    hotp->Instance->OTPADJ = OtpadjConfig;
    hotp->Instance->OTPWT1 = otpwt1_config;
    hotp->Instance->OTPWT2 = otpwt2_config;

}

/**
 * @brief 
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 * @param ReadMode Режим чтения
 */
void HAL_OPT_SetReadMode(OTP_HandleTypeDef *hotp, uint8_t ReadMode)
{
    uint32_t OtpconConfig = hotp->Instance->OTPCON;

    OtpconConfig &= ~OTP_OTPCON_APBNWS_M;
    OtpconConfig |= ReadMode << OTP_OTPCON_APBNWS_S;

    hotp->Instance->OTPCON = OtpconConfig;
    hotp->ReadMode = ReadMode;
}

/**
 * @brief Инициализировать OTP.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 */
void HAL_OTP_Init(OTP_HandleTypeDef *hotp)
{
    
    HAL_OTP_MspInit(hotp);
    
    // /* Настройка временных ограничений */
    HAL_OPT_TimeInit(hotp);

    // /* Выбор напряжения на UPP матрицы */
    HAL_OTP_SetUppRead(hotp, OTP_UPP_READ_2_5V);

    /* Режим чтения */
    HAL_OPT_SetReadMode(hotp, hotp->ReadMode);
}

/**
 * @brief Ожидать сброса флага BSY.
 * 
 * Флаг BSY = 1 - Блок занят (выполняется запрошенная операция).
 * Используется при операции записи и чтении в 3 этапа.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 */
void HAL_OTP_WaitBSY(OTP_HandleTypeDef *hotp)
{
    /* Опрос флага BSY пока он не станет очищен */
    while (hotp->Instance->OTPSTA & OTP_OTPSTA_BSY_M);
}

/**
 * @brief Записать массив данных в тестовый столбец.
 * 
 * @warning Если в массиве Data количество бит больше размера столбца, начиная с Address, то запись продолжится с начала столбца.
 *
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Address Начальная ячейка столбца. Значение может быть в пределах от 0 до 7.
 * @param Data Данные для записи в тестовый столбец.
 * @param DataLength Размер массива Data.
 */
void HAL_OTP_WriteTestColumn(OTP_HandleTypeDef *hotp, uint8_t Address, uint32_t Data[], uint32_t DataLength)
{
    /* OTPA[4:3] = 10b - тестовый столбец OTP */
    hotp->Instance->OTPA = 0b10000 + (Address & 0b111);

    for (uint32_t i = 0; i < DataLength; i++)
    {
        hotp->Instance->OTPDAT = Data[i];
        HAL_OTP_WaitBSY(hotp);
    }
}

/**
 * @brief Записать данные в тестовую строку.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Data Данные для записи в тестовую строку.
 */
void HAL_OTP_WriteTestRow(OTP_HandleTypeDef *hotp, uint32_t Data)
{
    /* OTPA[4:3] = 01b - тестовая строка OTP */
    hotp->Instance->OTPA = 0b01000;

    hotp->Instance->OTPDAT = Data;
    HAL_OTP_WaitBSY(hotp);
}

/**
 * @brief Записать бит в тестовую ячейку.
 * 
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Data Бит для записи в тестовую ячейку.
 */
void HAL_OTP_WriteTestBit(OTP_HandleTypeDef *hotp, uint32_t Data)
{
    /* OTPA[4:3] = 11b - последняя тестовая ячейка в тестовой строке */
    hotp->Instance->OTPA = 0b11000;

    hotp->Instance->OTPDAT = Data;
    HAL_OTP_WaitBSY(hotp);
}

/**
 * @brief Записать данные в основной массив OTP.
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Address Начальная строка. Значение может быть в пределах от 0 до 7.
 * @param Data Данные для записи в основной массив OTP.
 * @param DataLength Размер массива Data.
 */
void HAL_OTP_Write(OTP_HandleTypeDef *hotp, uint8_t Address, uint32_t Data[], uint32_t DataLength)
{
    /* OTPA[4:3] = 00b - основной массив OTP */
    hotp->Instance->OTPA = 0b00000 + (Address & 0b111);

    for (uint32_t i = 0; i < DataLength; i++)
    {
        hotp->Instance->OTPDAT = Data[i];
        HAL_OTP_WaitBSY(hotp);
    }
}

/**
 * @brief Прочитать данные из тестового столбца.
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Address Начальная ячейка столбца. Значение может быть в пределах от 0 до 7.
 * @param DataRead Массив для считывания данных тестового столбца.
 * @param DataLength Размер массива DataRead.
 */
void HAL_OTP_ReadTestColumn(OTP_HandleTypeDef *hotp, uint8_t Address, uint32_t DataRead[], uint32_t DataLength)
{
    if (hotp->ReadMode == OPT_READ_3STAGES)     /* Чтение в 3 этапа. Без вставки тактов ожидания. С опросом BSY. Без автоинкрементирования адреса OTPA */
    {
        /* OTPA[4:3] = 10b - тестовый столбец OTP */
        uint8_t address_mask = 0b00111;
        uint8_t address_column_mask = (1 << 4);

        for (uint32_t i = 0; i < DataLength; i++)
        {
            hotp->Instance->OTPA = ((Address + i) & address_mask) | address_column_mask;
            HAL_OTP_WaitBSY(hotp);
            DataRead[i] = hotp->Instance->OTPDAT;
        }
    }
    else    /* Чтение в 2 этапа. Со вставкой тактов ожидания. Без опроса BSY. С автоинкрементированием адреса OTPA. */
    {
        /* OTPA[4:3] = 10b - тестовый столбец OTP */
        uint8_t address_column = 0b10000 + (Address & 0b111); 
        hotp->Instance->OTPA = address_column;
        DataRead[0] = hotp->Instance->OTPDAT;
        
        for (uint32_t i = 0; i < DataLength; i++)
        {
            DataRead[i] = hotp->Instance->OTPDAT;
        }
    }
    
    
}

/**
 * @brief Прочитать данные из тестовой строки.
 * @param hotp Указатель на структуру с настройками OTP.
 * @return Тестовая строка.
 */
uint32_t HAL_OTP_ReadTestRow(OTP_HandleTypeDef *hotp)
{    
    uint32_t DataRead = 0;
    
    /* OTPA[4:3] = 01b - тестовая строка OTP */
    hotp->Instance->OTPA = 0b01000;

    if (hotp->ReadMode == OPT_READ_3STAGES)     /* Чтение в 3 этапа. Без вставки тактов ожидания. С опросом BSY. Без автоинкрементирования адреса OTPA */
    {
        HAL_OTP_WaitBSY(hotp);
    }
    else
    {
        DataRead = hotp->Instance->OTPDAT;
    }
    
    DataRead = hotp->Instance->OTPDAT;

    return DataRead;
}

/**
 * @brief Прочитать бит из тестовой ячейки.
 * @param hotp hotp - Указатель на структуру с настройками OTP.
 * @return Бит из тестовой ячейки.
 */
uint32_t HAL_OTP_ReadTestBit(OTP_HandleTypeDef *hotp)
{
    uint32_t DataRead = 0;
    
    /* OTPA[4:3] = 11b - последняя тестовая ячейка в тестовой строке */
    hotp->Instance->OTPA = 0b11000;

    if (hotp->ReadMode == OPT_READ_3STAGES)     /* Чтение в 3 этапа. Без вставки тактов ожидания. С опросом BSY. Без автоинкрементирования адреса OTPA */
    {
        HAL_OTP_WaitBSY(hotp);
    }
    else
    {
        DataRead = hotp->Instance->OTPDAT;
    }

    DataRead = hotp->Instance->OTPDAT;

    return DataRead;
}

/**
 * @brief Прочитать данные из основного массива OTP.
 * @param hotp Указатель на структуру с настройками OTP.
 * @param Address Начальная строка основного массива OTP. Значение может быть в пределах от 0 до 7.
 * @param DataRead Массив для считывания данных основного массива OTP.
 * @param DataLength Размер массива DataRead.
 */
void HAL_OTP_Read(OTP_HandleTypeDef *hotp, uint8_t Address, uint32_t DataRead[], uint32_t DataLength)
{
    if (hotp->ReadMode == OPT_READ_3STAGES)     /* Чтение в 3 этапа. Без вставки тактов ожидания. С опросом BSY. Без автоинкрементирования адреса OTPA */
    {
        /* OTPA[4:3] = 00b - основной массив OTP */
        uint8_t address_mask = 0b00111;

        for (uint32_t i = 0; i < DataLength; i++)
        {
            hotp->Instance->OTPA = (Address + i) & address_mask;
            HAL_OTP_WaitBSY(hotp);
            DataRead[i] = hotp->Instance->OTPDAT;
        }
    }
    else    /* Чтение в 2 этапа. Со вставкой тактов ожидания. Без опроса BSY. С автоинкрементированием адреса OTPA. */
    {
        /* OTPA[4:3] = 00b - основной массив OTP */
        hotp->Instance->OTPA = 0b00000 + (Address & 0b111); 
        DataRead[0] = hotp->Instance->OTPDAT;
        
        for (uint32_t i = 0; i < DataLength; i++)
        {
            DataRead[i] = hotp->Instance->OTPDAT;
        }
    }

}
