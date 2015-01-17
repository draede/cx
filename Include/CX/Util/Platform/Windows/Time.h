
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

