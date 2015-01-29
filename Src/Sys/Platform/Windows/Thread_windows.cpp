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


#include "CX/Sys/Thread.h"
#include "CX/Status.h"


namespace CX
{

namespace Sys
{

Thread::Thread()
{
	Status::Clear();

	m_hThread = NULL;
	m_nID     = 0;
}

Thread::~Thread()
{
	Status::Clear();
}

StatusCode Thread::Wait()
{
	Status::Clear();

	if (NULL == m_hThread)
	{
		return Status::Set(Status_NotInitialized, "Thread not started");
	}
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, INFINITE))
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		return Status_OK;
	}

	return Status::Set(Status_OperationFailed, "WaitForSingleObject failed with error {1}", 
	                   GetLastError());
}

bool Thread::IsRunning()
{
	Status::Clear();

	return (NULL != m_hThread);
}

TID Thread::GetID()
{
	Status::Clear();

	if (NULL == m_hThread)
	{
		Status::Set(Status_NotInitialized, "Thread not started");

		return 0;
	}

	return m_nID;
}

PID Thread::GetCurrentProcessID()
{
	Status::Clear();

	return (PID)GetCurrentProcessId();
}

TID Thread::GetCurrentThreadID()
{
	Status::Clear();

	return (TID)GetCurrentThreadId();
}

void Thread::Sleep(Size cMilliseconds)
{
	Status::Clear();

	::Sleep((DWORD)cMilliseconds);
}

DWORD Thread::ThreadProc(void *pArg)
{
	IHelper *pHelper = (IHelper *)pArg;

	pHelper->Run();

	Delete(pHelper);

	return 0;
}

}//namespace Sys

}//namespace CX


#endif

