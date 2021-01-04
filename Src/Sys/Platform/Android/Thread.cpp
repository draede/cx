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


#if defined(CX_OS_ANDROID)


#include "CX/Sys/Thread.hpp"
#include "CX/Status.hpp"
#include <unistd.h>
#include <pthread.h>


namespace CX
{

namespace Sys
{

Thread::Thread()
{
	m_bRunning = false;
}

Thread::~Thread()
{
}

Status Thread::Wait()
{
	if (!m_bRunning)
	{
		return Status_NotInitialized;
	}

	int nRet;

	if (0 == (nRet = pthread_join(m_hThread, NULL)))
	{
		m_bRunning = false;

		return Status();
	}
	else
	{
		return Status(Status_OperationFailed, "pthread_join failed with error {1}", nRet);
	}
}

Bool Thread::IsRunning()
{
	return m_bRunning;
}

Thread::ID Thread::GetID()
{
	if (!m_bRunning)
	{
		return 0;
	}

	return (Thread::ID)m_hThread;
}

Thread::ID Thread::GetCurrentThreadID()
{
	return (Thread::ID)pthread_self();
}

void Thread::Sleep(Size cMilliseconds)
{
	usleep(cMilliseconds * 1000);
}

Status Thread::Run(const std::function<void()> &func)
{
	if (m_bRunning)
	{
		return Status(Status_Busy, "Thread already started");
	}

	ThreadData *pData = new ThreadData();

	if (NULL == pData)
	{
		return Status(Status_MemAllocFailed, "Memory allocation error");
	}

	int nRet;

	if (0 == (nRet = pthread_create(&m_hThread, NULL, &Thread::ThreadProc, pData)))
	{
		return Status();
	}
	else
	{
		delete pData;

		return Status(Status_OperationFailed, "pthread_create failed with error {1}", nRet);
	}
}

void *Thread::ThreadProc(void *pArg)
{
	ThreadData *pData = (ThreadData *)pArg;

	pData->pThis->m_bRunning = true;

	pData->func();

	pData->pThis->m_bRunning = false;

	delete pData;

	return NULL;
}

}//namespace Sys

}//namespace CX


#endif

