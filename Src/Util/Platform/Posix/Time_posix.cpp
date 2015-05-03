/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/Platform.hpp"


#if defined(CX_OS_POSIX)


#include "CX/Util/Time.hpp"
#include "CX/Status.hpp"
#include <time.h>
#include <sys/time.h>


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
	struct tm        *pTime;
	struct timeval   tv;

	if (0 != gettimeofday(&tv, NULL))
	{
		return Status(Status_OperationFailed, "gettimeofday failed");
	}
	pTime          = localtime(&tv.tv_sec);
	m_nYear        = (unsigned)pTime->tm_year + 1900;
	m_nMonth       = (unsigned)pTime->tm_mon + 1;
	m_nDay         = (unsigned)pTime->tm_mday;
	m_nDayOfWeek   = (DayOfWeek)pTime->tm_wday;
	m_nHour        = (unsigned)pTime->tm_hour;
	m_nMinute      = (unsigned)pTime->tm_min;
	m_nSecond      = (unsigned)pTime->tm_sec;
	m_nMillisecond = tv.tv_usec / 1000;

	return Status();
}

Status Time::GetUTCTime()
{
	struct tm        *pTime;
	struct timeval   tv;

	if (0 != gettimeofday(&tv, NULL))
	{
		return Status(Status_OperationFailed, "gettimeofday failed");
	}
	pTime          = gmtime(&tv.tv_sec);
	m_nYear        = (unsigned)pTime->tm_year + 1900;
	m_nMonth       = (unsigned)pTime->tm_mon + 1;
	m_nDay         = (unsigned)pTime->tm_mday;
	m_nDayOfWeek   = (DayOfWeek)pTime->tm_wday;
	m_nHour        = (unsigned)pTime->tm_hour;
	m_nMinute      = (unsigned)pTime->tm_min;
	m_nSecond      = (unsigned)pTime->tm_sec;
	m_nMillisecond = tv.tv_usec / 1000;

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
	UInt64 nTSMS;

	nTSMS = 0;
	nTSMS += (UInt64)m_nMillisecond;
	nTSMS += ((UInt64)m_nSecond << 16);
	nTSMS += ((UInt64)m_nMinute << 24);
	nTSMS += ((UInt64)m_nHour << 32);
	nTSMS += ((UInt64)m_nDay << 40);
	nTSMS += ((UInt64)m_nMonth << 48);
	nTSMS += ((UInt64)(m_nYear - 1900) << 56);

	return nTSMS;
}

}//namespace Util

}//namespace CX


#endif
