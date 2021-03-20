#include "../mcuduino.h"

#include "include/mcu32_memory_map.h"
#include "libs/rtc_lib.h"

int main()
{

	RTC_Disable(RTC);
	RTC_LoadDate(RTC, 19, 3, 2021);
	RTC_LoadTime(RTC, 17, 3, 0, 0, 5);
	RTC_LoadDateAlarm(RTC, 19, 3, 2021);
	RTC_LoadTimeAlarm(RTC, 17, 3, 2, 0, 5);
	RTC_Enable(RTC);

	while (1) {
		RTC_TypeDef* rtc = RTC;
		uint32_t date = RTC->DATE;
		uint32_t s = RTC->S;
		uint32_t m = RTC->M;
		uint32_t alarm = RTC->CTRL & RTC_CTRL_ALRM_M;
		delay(1000);
	}
}


