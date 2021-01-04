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

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Sys
{

class CX_API ThreadPool
{
public:

	static const Size MAX_THREADS            = 1024;
	static const Size MAX_WAITING_WORK_ITEMS = 0;

	ThreadPool();

	~ThreadPool();

	Status Start(PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfnCancelCallback = NULL, UInt32 cMinThreads = 1, 
	             UInt32 cMaxThreads = 1, UInt32 cWaitingWorkItems = MAX_WAITING_WORK_ITEMS);

	Status Stop(Bool bWaitForUnfinishedWork = True, void *pCancelCallbackContext = NULL);

	Status AddWork(PTP_WORK_CALLBACK pfnWorkCallback, void *pWorkArg, UInt32 cWaitTimeInMS = 0xFFFFFFFF);

private:

	struct WorkCallbackContext
	{
		ThreadPool                          *pThreadPool;
		PVOID                               pActualContext;
		PTP_WORK_CALLBACK                   pfnActualWorkCallback;
	};

	struct CancelCallbackContext
	{
		ThreadPool                          *pThreadPool;
		PVOID                               pActualContext;
		PTP_CLEANUP_GROUP_CANCEL_CALLBACK   pfnActualCancelCallback;
	};

	PTP_POOL                               m_pPool;
	PTP_CLEANUP_GROUP                      m_pCleanupGroup;
	TP_CALLBACK_ENVIRON                    m_cbe;
	PTP_CLEANUP_GROUP_CANCEL_CALLBACK      m_pfnCancelCallback;
	Bool                                   m_bCBEInitialized;
	HANDLE                                 m_hEventStop;
	HANDLE                                 m_hSemaphoreWorkItems;

	static void CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork);

	static void CALLBACK CancelCallback(void *pWorkContext, void *pCleanupContext);

};

}//namespace Sys

}//namespace CX


#endif
