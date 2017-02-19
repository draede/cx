/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Event.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

Event::Event()
{
	m_hEvent = NULL;
}

Event::~Event()
{
	Destroy();
}

Status Event::Create(bool bManualReset/* = false*/, bool bInitialState/* = false*/)
{
	Destroy();

	if (NULL == (m_hEvent = CreateEventW(NULL, (BOOL)bManualReset, (BOOL)bInitialState, NULL)))
	{
		return Status(Status_OperationFailed, "CreateEventW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status Event::Destroy()
{
	if (NULL != m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

	return Status();
}

bool Event::IsOK()
{
	return (NULL != m_hEvent);
}

Status Event::Set()
{
	if (NULL == m_hEvent)
	{
		return Status_NotInitialized;
	}
	if (!SetEvent(m_hEvent))
	{
		return Status(Status_OperationFailed, "SetEvent failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status Event::Reset()
{
	if (NULL == m_hEvent)
	{
		return Status_NotInitialized;
	}
	if (!ResetEvent(m_hEvent))
	{
		return Status(Status_OperationFailed, "ResetEvent failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Event::WaitResult Event::Wait(UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	if (NULL == m_hEvent)
	{
		return Wait_Error;
	}

	DWORD dwRet;

	dwRet = WaitForSingleObject(m_hEvent, (DWORD)cTimeout);

	if (WAIT_FAILED == dwRet || WAIT_ABANDONED == dwRet)
	{
		return Wait_Error;
	}
	if (WAIT_TIMEOUT == dwRet)
	{
		return Wait_Timeout;
	}

	return Wait_OK;
}

Event::WaitResult Event::WaitForMultipleEvents(Event **events, Size cCount, bool bWaitAll, Size *pcIndex, 
                                               UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	if (0 == cCount || 64 < cCount)
	{
		return Wait_Error;
	}

	HANDLE     handles[64];
	WaitResult nRes;
	DWORD      dwRet;

	nRes = Wait_OK;
	for (;;)
	{
		for (Size i = 0; i < cCount; i++)
		{
			if (NULL == events[i]->m_hEvent)
			{
				nRes = Wait_Error;

				break;
			}
			handles[i] = events[i]->m_hEvent;
		}
		if (Wait_OK != nRes)
		{
			break;
		}
		dwRet = WaitForMultipleObjects((DWORD)cCount, handles, (BOOL)bWaitAll, (DWORD)cTimeout);
		if (WAIT_TIMEOUT == dwRet)
		{
			dwRet = Wait_Timeout;
		}
		else
		if (dwRet >= WAIT_ABANDONED_0)
		{
			return Wait_Error;
		}
		else
		{
			*pcIndex = dwRet - WAIT_OBJECT_0; 
		}

		break;
	}

	return nRes;
}

}//namespace Sys

}//namespace CX


#endif

