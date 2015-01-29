/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include "CX/APIDefs.h"
#include <windows.h>


namespace CX
{

namespace Util
{

class CX_API Time
{
public:

	enum DayOfWeek
	{
		Sunday      = 0,
		Monday      = 1,
		Tuesday     = 2,
		Wednesday   = 3,
		Thursday    = 4,
		Friday      = 5,
		Saturday    = 6,
	};

	Time();

	Time(const Time &t);

	virtual ~Time();

	Time &operator=(const Time &t);

	StatusCode GetLocalTime();

	StatusCode GetUTCTime();

	StatusCode SetDate(unsigned nYear, unsigned nMonth, unsigned nDay);

	StatusCode SetYear(unsigned nYear);

	unsigned GetYear() const;

	StatusCode SetMonth(unsigned nMonth);

	unsigned GetMonth() const;

	StatusCode SetDay(unsigned nDay);

	unsigned GetDay() const;

	StatusCode SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond);

	StatusCode SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond, unsigned nMillisecond);

	StatusCode SetHour(unsigned nHour);

	unsigned GetHour() const;

	StatusCode SetMinute(unsigned nMinute);

	unsigned GetMinute() const;

	StatusCode SetSecond(unsigned nSecond);

	unsigned GetSecond() const;

	StatusCode SetMillisecond(unsigned nMillisecond);

	unsigned GetMillisecond() const;

private:

	unsigned    m_nYear;
	unsigned    m_nMonth;
	unsigned    m_nDay;

	unsigned    m_nHour;
	unsigned    m_nMinute;
	unsigned    m_nSecond;
	unsigned    m_nMillisecond;

	DayOfWeek   m_nDayOfWeek;

};

}//namespace Util

}//namespace CX


#endif

