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

#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Util/Time.h"
#include "CX/Status.h"
#include <windows.h>


namespace CX
{

namespace Util
{

Time::Time()
{
	Status::Clear();

	GetLocalTime();
}

Time::Time(const Time &t)
{
	Status::Clear();

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
	Status::Clear();
}

Time &Time::operator=(const Time &t)
{
	Status::Clear();

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

StatusCode Time::GetLocalTime()
{
	Status::Clear();

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

	return Status_OK;
}

StatusCode Time::GetUTCTime()
{
	Status::Clear();

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

	return Status_OK;
}

StatusCode Time::SetDate(unsigned nYear, unsigned nMonth, unsigned nDay)
{
	Status::Clear();

	m_nYear  = nYear;
	m_nMonth = nMonth;
	m_nDay   = nDay;

	return Status_OK;
}

StatusCode Time::SetYear(unsigned nYear)
{
	Status::Clear();

	m_nYear = nYear;

	return Status_OK;
}

unsigned Time::GetYear() const
{
	Status::Clear();

	return m_nYear;
}

StatusCode Time::SetMonth(unsigned nMonth)
{
	Status::Clear();

	m_nMonth = nMonth;

	return Status_OK;
}

unsigned Time::GetMonth() const
{
	Status::Clear();

	return m_nMonth;
}

StatusCode Time::SetDay(unsigned nDay)
{
	Status::Clear();

	m_nDay = nDay;

	return Status_OK;
}

unsigned Time::GetDay() const
{
	Status::Clear();

	return m_nDay;
}

StatusCode Time::SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond)
{
	Status::Clear();

	m_nHour   = nHour;
	m_nMinute = nMinute;
	m_nSecond = nSecond;

	return Status_OK;
}

StatusCode Time::SetTime(unsigned nHour, unsigned nMinute, unsigned nSecond, unsigned nMillisecond)
{
	Status::Clear();

	m_nHour        = nHour;
	m_nMinute      = nMinute;
	m_nSecond      = nSecond;
	m_nMillisecond = nMillisecond;

	return Status_OK;
}

StatusCode Time::SetHour(unsigned nHour)
{
	Status::Clear();

	m_nHour = nHour;

	return Status_OK;
}

unsigned Time::GetHour() const
{
	Status::Clear();

	return m_nHour;
}

StatusCode Time::SetMinute(unsigned nMinute)
{
	Status::Clear();

	m_nMinute = nMinute;

	return Status_OK;
}

unsigned Time::GetMinute() const
{
	Status::Clear();

	return m_nMinute;
}

StatusCode Time::SetSecond(unsigned nSecond)
{
	Status::Clear();

	m_nSecond = nSecond;

	return Status_OK;
}

unsigned Time::GetSecond() const
{
	Status::Clear();

	return m_nSecond;
}

StatusCode Time::SetMillisecond(unsigned nMillisecond)
{
	Status::Clear();

	m_nMillisecond = nMillisecond;

	return Status_OK;
}

unsigned Time::GetMillisecond() const
{
	Status::Clear();

	return m_nMillisecond;
}

}//namespace Util

}//namespace CX


#endif

