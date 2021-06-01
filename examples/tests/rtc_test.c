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

	while (1) { }
}


