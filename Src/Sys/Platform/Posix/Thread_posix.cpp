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


#include "CX/Sys/Thread.hpp"
#include "CX/Status.hpp"
#include <unistd.h>


namespace CX
{

namespace Sys
{

Thread::Thread()
{
	m_bThreadIsOK = false;
}

Thread::~Thread()
{
}

Status Thread::Wait()
{
	if (m_bThreadIsOK)
	{
		return Status(Status_NotInitialized);
	}

	int nRes;

	if (0 == (nRes = pthread_join(m_hThread, NULL)))
	{
		m_bThreadIsOK = false;

		return Status();
	}
	else
	{
		return Status(Status_OperationFailed, "pthread_join failed with error code %d", nRes);
	}
}

Bool Thread::IsRunning()
{
	return m_bThreadIsOK;
}

Thread::ID Thread::GetID()
{
	if (!m_bThreadIsOK)
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

void *Thread::ThreadProc(void *pArg)
{
	IHelper *pHelper = (IHelper *)pArg;

	pHelper->Run();

	delete pHelper;

	return NULL;
}

}//namespace Sys

}//namespace CX


#endif

