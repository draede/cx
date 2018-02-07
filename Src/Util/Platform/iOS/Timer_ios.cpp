/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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


#if defined(CX_OS_IOS)


#include "CX/Util/Timer.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Util
{

Timer::Timer()
{
	ResetTimer();
}

Timer::~Timer()
{
}

Status Timer::ResetTimer()
{
	clock_gettime(CLOCK_REALTIME, &m_start);

	return Status();
}

double Timer::GetElapsedTime() const
{
	timespec stop;
	timespec diff;

	clock_gettime(CLOCK_REALTIME, &stop);
	GetTimeSpecDiff(&m_start, &stop, &diff);

	return (double)diff.tv_sec + (double)diff.tv_nsec / 1000000000.0;
}

UInt64 Timer::GetElapsedTimeInNS() const
{
	timespec stop;
	timespec diff;

	clock_gettime(CLOCK_REALTIME, &stop);
	GetTimeSpecDiff(&m_start, &stop, &diff);

	return diff.tv_sec * 1000000000 + diff.tv_nsec;
}

void Timer::GetTimeSpecDiff(const struct timespec *pStart, const struct timespec *pStop, struct timespec *pDiff)
{
	if ((pStop->tv_nsec - pStart->tv_nsec) < 0) 
	{
		pDiff->tv_sec = pStop->tv_sec - pStart->tv_sec - 1;
		pDiff->tv_nsec = pStop->tv_nsec - pStart->tv_nsec + 1000000000;
	}
	else 
	{
		pDiff->tv_sec = pStop->tv_sec - pStart->tv_sec;
		pDiff->tv_nsec = pStop->tv_nsec - pStart->tv_nsec;
	}
}

}//namespace Util

}//namespace CX


#endif

