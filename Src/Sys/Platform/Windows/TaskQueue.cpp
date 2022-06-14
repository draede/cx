/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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


#include "CX/Sys/TaskQueue.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Sys
{

struct Sync
{
	CRITICAL_SECTION cs;
	HANDLE           hShutdownSemaphore;
	HANDLE           hTasksSemaphore;
	HANDLE           hEmptyEvent;
};

TaskQueue::TaskQueue(Size cConsumers/* = 1*/)
{
	m_cConsumers = cConsumers;
	m_pSync      = new (std::nothrow) Sync();
	if (NULL != m_pSync)
	{
		InitializeCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
		static_cast<Sync *>(m_pSync)->hEmptyEvent        = CreateEventA(NULL, FALSE, TRUE, NULL);
		static_cast<Sync *>(m_pSync)->hShutdownSemaphore = CreateSemaphoreA(NULL, 0, (LONG)m_cConsumers, NULL);
		static_cast<Sync *>(m_pSync)->hTasksSemaphore    = CreateSemaphoreA(NULL, 0, LONG_MAX, NULL);
	}
}

TaskQueue::~TaskQueue()
{
	Clear();

	if (NULL != m_pSync)
	{
		CloseHandle(static_cast<Sync *>(m_pSync)->hTasksSemaphore);
		CloseHandle(static_cast<Sync *>(m_pSync)->hShutdownSemaphore);
		CloseHandle(static_cast<Sync *>(m_pSync)->hEmptyEvent);
		DeleteCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
		delete static_cast<Sync *>(m_pSync);
		m_pSync = NULL;
	}
}

Status TaskQueue::Push(ITask *pTask)
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	Status status;

	EnterCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
	m_queueTasks.push(pTask);
	ReleaseSemaphore(static_cast<Sync *>(m_pSync)->hTasksSemaphore, 1, NULL);
	ResetEvent(static_cast<Sync *>(m_pSync)->hEmptyEvent);
	LeaveCriticalSection(&static_cast<Sync *>(m_pSync)->cs);

	return status;
}

Status TaskQueue::Pop(ITask **ppTask)
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	HANDLE handles[2] = 
	{ 
		static_cast<Sync *>(m_pSync)->hShutdownSemaphore, 
		static_cast<Sync *>(m_pSync)->hTasksSemaphore 
	};
	DWORD  dwRet;
	Status status;

	dwRet = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	if (WAIT_OBJECT_0 == dwRet)
	{
		status = Status_Cancelled;
	}
	else
	if (WAIT_OBJECT_0 + 1 == dwRet)
	{
		EnterCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
		if (!m_queueTasks.empty())
		{
			*ppTask = m_queueTasks.front();
			m_queueTasks.pop();
			if (m_queueTasks.empty())
			{
				SetEvent(static_cast<Sync *>(m_pSync)->hEmptyEvent);
			}
		}
		LeaveCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
	}
	else
	{
		status = Status(Status_OperationFailed, "WaitForMultipleObjects failed with error {1}", GetLastError());
	}

	return status;
}

Status TaskQueue::Pop(TasksVector *pVectorTasks, Size cCount/* = 0*/)
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	HANDLE handles[2] = 
	{ 
		static_cast<Sync *>(m_pSync)->hShutdownSemaphore, 
		static_cast<Sync *>(m_pSync)->hTasksSemaphore 
	};
	DWORD  dwRet;
	Status status;

	pVectorTasks->clear();
	dwRet = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	if (WAIT_OBJECT_0 == dwRet)
	{
		status = Status_Cancelled;
	}
	else
	if (WAIT_OBJECT_0 + 1 == dwRet)
	{
		EnterCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
		if (!m_queueTasks.empty())
		{
			while (!m_queueTasks.empty() && (0 == cCount || pVectorTasks->size() < cCount))
			{
				pVectorTasks->push_back(m_queueTasks.front());
				m_queueTasks.pop();
			}
			if (m_queueTasks.empty())
			{
				SetEvent(static_cast<Sync *>(m_pSync)->hEmptyEvent);
			}
		}
		LeaveCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
	}
	else
	{
		status = Status(Status_OperationFailed, "WaitForMultipleObjects failed with error {1}", GetLastError());
	}

	return status;
}

Status TaskQueue::Shutdown()
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	ReleaseSemaphore(static_cast<Sync *>(m_pSync)->hShutdownSemaphore, (LONG)m_cConsumers, NULL);

	return Status();
}

Status TaskQueue::Wait()
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	WaitForSingleObject(static_cast<Sync *>(m_pSync)->hEmptyEvent, INFINITE);

	return Status();
}

Status TaskQueue::Clear()
{
	if (NULL == m_pSync)
	{
		return Status_NotInitialized;
	}

	EnterCriticalSection(&static_cast<Sync *>(m_pSync)->cs);
	while (!m_queueTasks.empty())
	{
		m_queueTasks.front()->Release();
		m_queueTasks.pop();
	}
	LeaveCriticalSection(&static_cast<Sync *>(m_pSync)->cs);

	return Status();
}

}//namespace Sys

}//namespace CX


#endif

