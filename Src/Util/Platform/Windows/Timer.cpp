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


#include "CX/Util/Timer.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Util
{

Timer::Timer()
{
	QueryPerformanceFrequency(&m_liTimerResolution);
	m_lfTimerResolutionNS = (double)m_liTimerResolution.QuadPart / 1000000000.0;
	QueryPerformanceCounter(&m_liStartTimer);
}

Timer::~Timer()
{
}

Status Timer::ResetTimer()
{
	if (!QueryPerformanceCounter(&m_liStartTimer))
	{
		return Status(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());
	}

	return Status();
}

double Timer::GetElapsedTime() const
{
	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		return 0.0;
	}

	return (double)(lint.QuadPart - m_liStartTimer.QuadPart) / m_liTimerResolution.QuadPart;
}

UInt64 Timer::GetElapsedTimeInNS() const
{
	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		return 0;
	}

	return (UInt64)((lint.QuadPart - m_liStartTimer.QuadPart) / m_lfTimerResolutionNS);
}

}//namespace Util

}//namespace CX


#endif

