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


#include "CX/Util/Timer.h"
#include "CX/Status.h"


namespace CX
{

namespace Util
{

Timer::Timer()
{
	Status::Clear();

	QueryPerformanceFrequency(&m_liTimerResolution);
	QueryPerformanceCounter(&m_liStartTimer);
}

Timer::~Timer()
{
	Status::Clear();
}

StatusCode Timer::ResetTimer()
{
	Status::Clear();

	if (!QueryPerformanceCounter(&m_liStartTimer))
	{
		return Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());
	}

	return Status_OK;
}

double Timer::GetElapsedTime() const
{
	Status::Clear();

	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());

		return 0.0;
	}

	return (double)(lint.QuadPart - m_liStartTimer.QuadPart) / m_liTimerResolution.QuadPart;
}

UInt64 Timer::GetTimeStamp()
{
	Status::Clear();

	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());

		return 0;
	}

	return (UInt64)lint.QuadPart;
}

UInt64 Timer::GetResolution()
{
	Status::Clear();

	return (UInt64)m_liTimerResolution.QuadPart;
}	

}//namespace Util

}//namespace CX


#endif

