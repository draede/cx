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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_ANDROID)


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

	m_hEvent = neosmart::CreateEvent(bManualReset, bInitialState);

	return Status();
}

Status Event::Destroy()
{
	if (NULL != m_hEvent)
	{
		int nRet;

		if (0 != (nRet = neosmart::DestroyEvent(m_hEvent)))
		{
			return Status(Status_OperationFailed, "DestroyEvent failed with error {1}", nRet);
		}
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

	int nRet;

	if (0 != (nRet = neosmart::SetEvent(m_hEvent)))
	{
		return Status(Status_OperationFailed, "SetEvent failed with error {1}", nRet);
	}

	return Status();
}

Status Event::Reset()
{
	if (NULL == m_hEvent)
	{
		return Status_NotInitialized;
	}

	int nRet;

	if (0 != (nRet = neosmart::ResetEvent(m_hEvent)))
	{
		return Status(Status_OperationFailed, "ResetEvent failed with error {1}", nRet);
	}

	return Status();
}

Event::WaitResult Event::Wait(UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	if (NULL == m_hEvent)
	{
		return Wait_Error;
	}
	
	int nRet;

	nRet = neosmart::WaitForEvent(m_hEvent, (UInt32)cTimeout);
	if (WAIT_TIMEOUT == nRet)
	{
		return Wait_Timeout;
	}
	else
	if (0 != nRet)
	{
		return Wait_Error;
	}
	else
	{
		return Wait_OK;
	}
}

Event::WaitResult Event::WaitForMultipleEvents(Event **events, Size cCount, bool bWaitAll, Size *pcIndex, 
                                               UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	if (0 == cCount || 64 < cCount)
	{
		return Wait_Error;
	}

	neosmart::neosmart_event_t   handles[64];
	WaitResult                   nRes;

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

		int cIndex;
		int nRet;

		nRet = neosmart::WaitForMultipleEvents(handles, (int)cCount, bWaitAll, (UInt64)cTimeout, cIndex);
		if (WAIT_TIMEOUT == nRet)
		{
			return Wait_Timeout;
		}
		else
		if (0 != nRet)
		{
			return Wait_Error;
		}
		else
		{
			*pcIndex = (Size)cIndex;
			nRes     = Wait_OK;
		}

		break;
	}

	return nRes;
}

}//namespace Sys

}//namespace CX


#endif

