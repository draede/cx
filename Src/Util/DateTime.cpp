/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */  

#include "CX/precomp.hpp"
#include "CX/Util/DateTime.hpp"
#include "CX/Platform.hpp"

#ifdef CX_OS_WINDOWS
#include "CX/C/Platform/Windows/windows.h"
#else
#include <time.h>
#include <sys/time.h>
#endif


namespace CX
{

namespace Util
{

Bool DateTime::IsLeapYear(UInt16 uYear)
{
	return (uYear % 4 == 0 && uYear % 100 != 0) || (uYear % 400 == 0);
}

UInt8 DateTime::GetDaysInMonth(UInt16 uYear, UInt8 uMonth)
{
	if (2 == uMonth)
	{
		if (IsLeapYear(uYear))
		{
			return 29;
		}
		else
		{
			return 28;
		}
	}
	else
	if (4 == uMonth || 6 == uMonth || 9 == uMonth || 11 == uMonth)
	{
		return 30;
	}
	else
	{
		return 31;
	}
}

DateTime::DayOfWeek DateTime::GetDayOfWeekFromDate(UInt16 uYear, UInt8 uMonth, UInt8 uDay)
{
	UInt32   y;
	UInt32   v;

	y = uYear;
	y -= uMonth <= 2;

	const UInt32 era = (y >= 0 ? y : y- 399) / 400;
	const UInt32 yoe = static_cast<UInt32>(y - era * 400);                           // [0, 399]
	const UInt32 doy = (153 * (uMonth + (uMonth > 2 ? -3 : 9)) + 2) / 5 + uDay - 1;  // [0, 365]
	const UInt32 doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                        // [0, 146096]

	v = era * 146097 + static_cast<UInt32>(doe) - 719468;
	v *= SECONDS_PER_DAY;

	return GetDayOfWeekFromSeconds(v);
}

DateTime::DayOfWeek DateTime::GetDayOfWeekFromSeconds(UInt32 uSeconds)
{
	return (DayOfWeek)((uSeconds / SECONDS_PER_DAY + 4) % 7);
}

DateTime::DayOfWeek DateTime::GetDayOfWeekFromMilliseconds(UInt64 uMilliseconds)
{
	return GetDayOfWeekFromSeconds((UInt32)(uMilliseconds / 1000));
}

DateTime::DayOfWeek DateTime::GetDayOfWeekFromDate() const
{
	return GetDayOfWeekFromDate(uYear, uMonth, uDay);
}

void DateTime::FromSeconds(UInt32 uSeconds)
{
	UInt64  cDays    = uSeconds / SECONDS_PER_DAY;

	cDays += 719468;
	const UInt64 era = (cDays >= 0 ? cDays : cDays - 146096) / 146097;
	const UInt64 doe = static_cast<UInt64>(cDays - era * 146097);              // [0, 146096]
	const UInt64 yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const UInt64 y   = static_cast<UInt64>(yoe) + era * 400;
	const UInt64 doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const UInt64 mp  = (5 * doy + 2) / 153;                                    // [0, 11]
	const UInt64 d   = doy - (153 * mp + 2)/5 + 1;                             // [1, 31]
	const UInt64 m   = mp + (mp < 10 ? 3 : -9);                                // [1, 12]

	uYear   = (UInt16)(y + (m <= 2));
	uMonth  = (UInt8)m;
	uDay    = (UInt8)d;

	uSeconds %= SECONDS_PER_DAY;

	uHour        = (UInt8)(uSeconds / SECONDS_PER_HOUR); uSeconds %= SECONDS_PER_HOUR;
	uMinute      = (UInt8)(uSeconds / SECONDS_PER_MINUTE); uSeconds %= SECONDS_PER_MINUTE;
	uSecond      = (UInt8)uSeconds;
	uMillisecond = 0;
}

UInt32 DateTime::ToSeconds() const
{
	UInt32   y;
	UInt32   v;

	y = uYear;
	y -= uMonth <= 2;

	const UInt32 era = (y >= 0 ? y : y- 399) / 400;
	const UInt32 yoe = static_cast<UInt32>(y - era * 400);                           // [0, 399]
	const UInt32 doy = (153 * (uMonth + (uMonth > 2 ? -3 : 9)) + 2) / 5 + uDay - 1;  // [0, 365]
	const UInt32 doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                        // [0, 146096]

	v = era * 146097 + static_cast<UInt32>(doe) - 719468;
	v *= SECONDS_PER_DAY;
	v += uHour * SECONDS_PER_HOUR;
	v += uMinute * SECONDS_PER_MINUTE;
	v += uSecond;

	return v;
}

void DateTime::FromMilliseconds(UInt64 uMilliseconds)
{
	UInt64  cDays    = uMilliseconds / MILLISECONDS_PER_DAY;

	cDays += 719468;
	const UInt64 era = (cDays >= 0 ? cDays : cDays - 146096) / 146097;
	const UInt64 doe = static_cast<UInt64>(cDays - era * 146097);              // [0, 146096]
	const UInt64 yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const UInt64 y   = static_cast<UInt64>(yoe) + era * 400;
	const UInt64 doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const UInt64 mp  = (5 * doy + 2) / 153;                                    // [0, 11]
	const UInt64 d   = doy - (153 * mp + 2)/5 + 1;                             // [1, 31]
	const UInt64 m   = mp + (mp < 10 ? 3 : -9);                                // [1, 12]

	uYear  = (UInt16)(y + (m <= 2));
	uMonth = (UInt8)m;
	uDay   = (UInt8)d;

	uMilliseconds %= MILLISECONDS_PER_DAY;
	uHour        = (UInt8)(uMilliseconds / MILLISECONDS_PER_HOUR); uMilliseconds %= MILLISECONDS_PER_HOUR;
	uMinute      = (UInt8)(uMilliseconds / MILLISECONDS_PER_MINUTE); uMilliseconds %= MILLISECONDS_PER_MINUTE;
	uSecond      = (UInt8)(uMilliseconds / MILLISECONDS_PER_SECOND); uMilliseconds %= MILLISECONDS_PER_SECOND;
	uMillisecond = (UInt16)uMilliseconds;
}

UInt64 DateTime::ToMilliseconds() const
{
	UInt64   y;
	UInt64   v;

	y = uYear;
	y -= uMonth <= 2;

	const UInt64 era = (y >= 0 ? y : y- 399) / 400;
	const UInt64 yoe = static_cast<UInt64>(y - era * 400);                                                 // [0, 399]
	const UInt64 doy = (153 * ((UInt64)uMonth + ((UInt64)uMonth > 2 ? -3 : 9)) + 2) / 5 + (UInt64)uDay - 1;// [0, 365]
	const UInt64 doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                                              // [0, 146096]

	v = era * 146097 + static_cast<UInt64>(doe) - 719468;
	v *= MILLISECONDS_PER_DAY;
	v += uHour * MILLISECONDS_PER_HOUR;
	v += uMinute * MILLISECONDS_PER_MINUTE;
	v += uSecond * MILLISECONDS_PER_SECOND;
	v += uMillisecond;

	return v;
}

void DateTime::Now()
{
#ifdef CX_OS_WINDOWS
	SYSTEMTIME st;

	::GetLocalTime(&st);
	uYear        = (UInt16)st.wYear;
	uMonth       = (UInt8)st.wMonth;
	uDay         = (UInt8)st.wDay;
	uHour        = (UInt8)st.wHour;
	uMinute      = (UInt8)st.wMinute;
	uSecond      = (UInt8)st.wSecond;
	uMillisecond = (UInt16)st.wMilliseconds;
#else
	struct tm        *pTime;
	struct timeval   tv;

	pTime   = localtime(&tv.tv_sec);

	uYear   = (UInt16)(pTime->tm_year + 1900);
	uMonth  = (UInt8)(pTime->tm_mon + 1);
	uDay    = (UInt8)(pTime->tm_mday);
	uHour   = (UInt8)(pTime->tm_hour);
	uMinute = (UInt8)(pTime->tm_min);
	uSecond = (UInt8)(pTime->tm_sec);

	if (0 != gettimeofday(&tv, NULL))
	{
		uMillisecond = (UInt16)(tv.tv_usec / 1000);
	}
	else
	{
		uMillisecond = 0;
	}
#endif
}

}//namespace Util

}//namespace CX

