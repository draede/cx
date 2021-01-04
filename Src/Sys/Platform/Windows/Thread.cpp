/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#include "CX/Sys/Thread.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

Thread::Thread()
{
	m_hThread = NULL;
	m_nID     = 0;
}

Thread::~Thread()
{
}

Status Thread::Wait()
{
	if (NULL == m_hThread)
	{
		return Status(Status_NotInitialized, "Thread not started");
	}
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, INFINITE))
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		return Status();
	}

	return Status(Status_OperationFailed, "WaitForSingleObject failed with error {1}", 
	                   GetLastError());
}

Bool Thread::IsRunning()
{
	return (NULL != m_hThread);
}

Thread::ID Thread::GetID()
{
	if (NULL == m_hThread)
	{
		return 0;
	}

	return m_nID;
}

Thread::ID Thread::GetCurrentThreadID()
{
	return (Thread::ID)GetCurrentThreadId();
}

void Thread::Sleep(Size cMilliseconds)
{
	::Sleep((DWORD)cMilliseconds);
}

Status Thread::Run(const std::function<void()> &func)
{
	if (NULL != m_hThread)
	{
		return Status(Status_Busy, "Thread already started");
	}

	std::function<void()> *pFunc = new (std::nothrow) std::function<void()>(func);

	if (NULL == pFunc)
	{
		return Status(Status_MemAllocFailed, "Memory allocation error");
	}

	DWORD dwID;

	if (NULL != (m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, pFunc, 0, &dwID)))
	{
		m_nID = (ID)dwID;

		return Status();
	}
	else
	{
		delete(pFunc);

		return Status(Status_OpenFailed, "CreateThread failed with error {1}", GetLastError());
	}
}

DWORD Thread::ThreadProc(void *pArg)
{
	std::function<void()> *pFunc = (std::function<void()> *)pArg;

	(*pFunc)();

	delete pFunc;

	return 0;
}

}//namespace Sys

}//namespace CX


#endif

