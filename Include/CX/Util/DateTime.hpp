/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Util
{

//using https://howardhinnant.github.io/date_algorithms.html
class CX_API DateTime
{
public:

	static const UInt64     DAYS_PER_WEEK           = 7;

	static const UInt64     HOURS_PER_DAY           = 24;
	static const UInt64     HOURS_PER_WEEK          = HOURS_PER_DAY * DAYS_PER_WEEK;

	static const UInt64     MINUTES_PER_HOUR        = 60;
	static const UInt64     MINUTES_PER_DAY         = MINUTES_PER_HOUR * HOURS_PER_DAY;
	static const UInt64     MINUTES_PER_WEEK        = MINUTES_PER_DAY * HOURS_PER_WEEK;

	static const UInt64     SECONDS_PER_MINUTE      = 60;
	static const UInt64     SECONDS_PER_HOUR        = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
	static const UInt64     SECONDS_PER_DAY         = SECONDS_PER_MINUTE * MINUTES_PER_DAY;
	static const UInt64     SECONDS_PER_WEEK        = SECONDS_PER_MINUTE * MINUTES_PER_WEEK;

	static const UInt64     MILLISECONDS_PER_SECOND = 1000;
	static const UInt64     MILLISECONDS_PER_MINUTE = MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE;
	static const UInt64     MILLISECONDS_PER_HOUR   = MILLISECONDS_PER_SECOND * SECONDS_PER_HOUR;
	static const UInt64     MILLISECONDS_PER_DAY    = MILLISECONDS_PER_SECOND * SECONDS_PER_DAY;
	static const UInt64     MILLISECONDS_PER_WEEK   = MILLISECONDS_PER_SECOND * SECONDS_PER_WEEK;

	enum DayOfWeek
	{
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
	};

	UInt16   uYear;
	UInt8    uMonth;
	UInt8    uDay;
	UInt8    uHour;
	UInt8    uMinute;
	UInt8    uSecond;
	UInt16   uMillisecond;

	static Bool IsLeapYear(UInt16 uYear);

	static UInt8 GetDaysInMonth(UInt16 uYear, UInt8 uMonth);

	static DayOfWeek GetDayOfWeekFromDate(UInt16 uYear, UInt8 uMonth, UInt8 uDay);

	static DayOfWeek GetDayOfWeekFromSeconds(UInt32 uSeconds);

	static DayOfWeek GetDayOfWeekFromMilliseconds(UInt64 uMilliseconds);

	DayOfWeek GetDayOfWeekFromDate() const;

	void FromSeconds(UInt32 uSeconds);

	UInt32 ToSeconds() const;

	void FromMilliseconds(UInt64 uMilliseconds);

	UInt64 ToMilliseconds() const;

	void Now();

};

}//namespace Util

}//namespace CX
