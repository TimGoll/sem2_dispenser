#include "rtc.hpp"

RTC_DS1307* RTC = new RTC_DS1307();

extern char DAYS[7][4] = {
	"MON",
	"TUE",
	"WED",
	"THU",
	"FRI",
	"SAT",
	"SUN"
};
