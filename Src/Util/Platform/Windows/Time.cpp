/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Util/Time.hpp"
#include "CX/Status.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include <time.h>


namespace CX
{

namespace Util
{

Time::Time()
{
	GetLocalTime();
}

Time::Time(const Time &t)
{
	m_nYear        = t.m_nYear;
	m_nMonth       = t.m_nMonth;
	m_nDay         = t.m_nDay;
	m_nDayOfWeek   = t.m_nDayOfWeek;
	m_nHour        = t.m_nHour;
	m_nMinute      = t.m_nMinute;
	m_nSecond      = t.m_nSecond;
	m_nMillisecond	= t.m_nMillisecond;
}

Time::~Time()
{
}

Time &Time::operator=(const Time &t)
{
	m_nYear        = t.m_nYear;
	m_nMonth       = t.m_nMonth;
	m_nDay         = t.m_nDay;
	m_nDayOfWeek   = t.m_nDayOfWeek;
	m_nHour        = t.m_nHour;
	m_nMinute      = t.m_nMinute;
	m_nSecond      = t.m_nSecond;
	m_nMillisecond = t.m_nMillisecond;

	return *this;
}

Status Time::GetLocalTime()
{
	SYSTEMTIME st;

	::GetLocalTime(&st);	
	m_nYear        = (unsigned)st.wYear;
	m_nMonth       = (unsigned)st.wMonth;
	m_nDay         = (unsigned)st.wDay;
	m_nDayOfWeek   = (DayOfWeek)st.wDayOfWeek;
	m_nHour        = (unsigned)st.wHour;
	m_nMinute      = (unsigned)st.wMinute;
	m_nSecond      = (unsigned)st.wSecond;
	m_nMillisecond = (unsigned)st.wMilliseconds;

	return Status();
}

Status Time::GetUTCTime()
{
	SYSTEMTIME st;

	GetSystemTime(&st);
	m_nYear        = (unsigned)st.wYear;
	m_nMonth       = (unsigned)st.wMonth;
	m_nDay         = (unsigned)st.wDay;
	m_nDayOfWeek   = (DayOfWeek)st.wDayOfWeek;
	m_nHour        = (unsigned)st.wHour;
	m_nMinute      = (unsigned)st.wMinute;
	m_nSecond      = (unsigned)st.wSecond;
	m_nMillisecond = (unsigned)st.wMilliseconds;

	return Status();
}

Status Time::SetDate(unsigned nYear, unsigned nMonth, unsigned nDay)
{
	m_nYear  = nYear;
	m_nMonth = nMonth;
	m_nDay   = nDay;

	return Status();
}

Status Time::SetYear(unsigned nYear)
{
	m_nYear = nYear;

	return Status();
}

unsigned Time::GetYear() const
{
	return m_nYear;
}

Status Time::SetMonth(unsigned nMonth)
{
	m_nMonth = nMonth;

	return Status();
}

unsigned Time::GetMonth() const
{
	return m_nMonth;
}

Status Time::SetDay(unsigned nDay)
{
	m_nDay = nDay;

	return Status();
}

unsigned Time::GetDay() const
{
	return m_nDay;
}

Status Time::SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond)
{
	m_nHour   = nHour;
	m_nMinute = nMinute;
	m_nSecond = nSecond;

	return Status();
}

Status Time::SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond, unsigned nMillisecond)
{
	m_nHour        = nHour;
	m_nMinute      = nMinute;
	m_nSecond      = nSecond;
	m_nMillisecond = nMillisecond;

	return Status();
}

Status Time::SetHour(unsigned nHour)
{
	m_nHour = nHour;

	return Status();
}

unsigned Time::GetHour() const
{
	return m_nHour;
}

Status Time::SetMinute(unsigned nMinute)
{
	m_nMinute = nMinute;

	return Status();
}

unsigned Time::GetMinute() const
{
	return m_nMinute;
}

Status Time::SetSecond(unsigned nSecond)
{
	m_nSecond = nSecond;

	return Status();
}

unsigned Time::GetSecond() const
{
	return m_nSecond;
}

Status Time::SetMillisecond(unsigned nMillisecond)
{
	m_nMillisecond = nMillisecond;

	return Status();
}

unsigned Time::GetMillisecond() const
{
	return m_nMillisecond;
}

UInt64 Time::GetTimeStampMS()
{
	struct tm  tmstruct;

	tmstruct.tm_hour  = m_nHour;
	tmstruct.tm_min   = m_nMinute;
	tmstruct.tm_sec   = m_nSecond;
	tmstruct.tm_year  = m_nYear - 1900;
	tmstruct.tm_mon   = m_nMonth - 1;
	tmstruct.tm_mday  = m_nDay;
	tmstruct.tm_isdst = -1;

	return mktime(&tmstruct) * 1000 + m_nMillisecond;
}

}//namespace Util

}//namespace CX


#endif

