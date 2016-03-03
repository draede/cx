/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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


#include "CX/Sys/TaskExecutor.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Sys
{

TaskExecutor::TaskExecutor()
{
	m_nPriority          = Priority_Normal;
	m_cThreads           = 0;
	m_cRunDelayInMS      = 0;
	m_cFirstRunDelayInMS = 0;
	m_threads            = NULL;
	m_pQueue             = NULL;
	m_pStopSemaphore     = NULL;
	m_pCustomExecutor    = NULL;
}

TaskExecutor::~TaskExecutor()
{
}

Status TaskExecutor::Add(TaskQueue::ITask *pTask)
{
	if (NULL == m_threads)
	{
		return Status_NotInitialized;
	}

	return m_pQueue->Push(pTask);
}

Status TaskExecutor::Start(Size cThreads, Priority nPriority, Size cFirstRunDelayInMS/* = 0*/, 
                          Size cRunDelayInMS/* = 0*/, ICustomExecutor *pCustomExecutor/* = NULL*/)
{
	if (NULL != m_threads)
	{
		return Status_NotInitialized;
	}
	if (MIN_THREADS > cThreads || MAX_THREADS < cThreads)
	{
		return Status_InvalidArg;
	}

	if (NULL == (m_threads = new HANDLE[cThreads]))
	{
		return Status_MemAllocFailed;
	}
	m_cThreads           = cThreads;
	m_nPriority          = nPriority;
	m_cRunDelayInMS      = cRunDelayInMS;
	m_cFirstRunDelayInMS = cFirstRunDelayInMS;
	m_pCustomExecutor    = pCustomExecutor;
	m_pQueue             = new TaskQueue(cThreads);

	if (0 < m_cFirstRunDelayInMS || 0 < m_cRunDelayInMS)
	{
		m_pStopSemaphore = CreateSemaphore(NULL, 0, (LONG)m_cThreads, NULL);
	}

	DWORD dwID;

	for (Size i = 0; i < cThreads; i++)
	{
		m_threads[i] = CreateThread(NULL, 0, &TaskExecutor::ThreadProc, this, 0, &dwID);
	}

	return Status();
}

Status TaskExecutor::Stop(bool bWaitForTasks)
{
	if (NULL == m_threads)
	{
		return Status_NotInitialized;
	}
	if (bWaitForTasks)
	{
		m_pQueue->Wait();
	}
	if (NULL != m_pStopSemaphore)
	{
		ReleaseSemaphore(m_pStopSemaphore, (LONG)m_cThreads, NULL);
	}
	m_pQueue->Shutdown();
	WaitForMultipleObjects((DWORD)m_cThreads, m_threads, TRUE, INFINITE);
	for (Size i = 0; i < m_cThreads; i++)
	{
		CloseHandle(m_threads[i]);
	}
	if (NULL != m_pStopSemaphore)
	{
		CloseHandle(m_pStopSemaphore);
		m_pStopSemaphore = NULL;
	}
	delete [] m_threads;
	delete m_pQueue;
	m_nPriority          = Priority_Normal;
	m_cThreads           = 0;
	m_cRunDelayInMS      = 0;
	m_cFirstRunDelayInMS = 0;
	m_threads            = NULL;
	m_pQueue             = NULL;
	m_pStopSemaphore     = NULL;
	m_pCustomExecutor    = NULL;

	return Status();
}

unsigned long __stdcall TaskExecutor::ThreadProc(void *pArg)
{
	TaskExecutor *pThis  = (TaskExecutor *)pArg;
	HANDLE       hThread = GetCurrentThread();
	Status       status;

	switch (pThis->m_cThreads)
	{
		case Priority_Background: SetThreadPriority(hThread, THREAD_MODE_BACKGROUND_BEGIN); break;
		case Priority_Normal: SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL); break;
		case Priority_Idle: SetThreadPriority(hThread, THREAD_PRIORITY_IDLE); break;
		case Priority_TimeCritical: SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL); break;
		case Priority_AboveNormal: SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL); break;
		case Priority_BellowNormal: SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL); break;
		case Priority_Highest: SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST); break;
		case Priority_Lowest: SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST); break;
	}

	Size cRun = 0;

	for (;;)
	{
		if (0 == cRun)
		{
			if (0 < pThis->m_cFirstRunDelayInMS)
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_pStopSemaphore, (DWORD)pThis->m_cFirstRunDelayInMS))
				{
					status = Status_Cancelled;

					break;
				}
			}
		}
		else
		{
			if (0 < pThis->m_cRunDelayInMS)
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_pStopSemaphore, (DWORD)pThis->m_cRunDelayInMS))
				{
					status = Status_Cancelled;

					break;
				}
			}
		}

		status.Clear();
		if (NULL != pThis->m_pCustomExecutor)
		{
			Size cCount = pThis->m_pCustomExecutor->GetTasksCount();

			TaskQueue::TasksVector vectorTasks;

			if ((status = pThis->m_pQueue->Pop(&vectorTasks, cCount)).IsOK())
			{
				if (!vectorTasks.empty())
				{
					pThis->m_pCustomExecutor->ExecuteTasks(vectorTasks);
					for (auto iter = vectorTasks.begin(); iter != vectorTasks.end(); ++iter)
					{
						(*iter)->Release();
					}
				}
			}
		}
		else
		{
			TaskQueue::ITask *pTask;

			if ((status = pThis->m_pQueue->Pop(&pTask)).IsOK())
			{
				pTask->Run();
				pTask->Release();
			}
		}
		if (Status_Cancelled == status.GetCode())
		{
			break;
		}

		cRun++;
	}

	return 0;
}

bool TaskExecutor::IsRunning()
{
	return (NULL != m_threads);
}

Size TaskExecutor::GetThreadCount() const
{
	return m_cThreads;
}

Size TaskExecutor::GetPriority() const
{
	return m_nPriority;
}

Size TaskExecutor::GetFirstRunDelay() const
{
	return m_cFirstRunDelayInMS;
}

Size TaskExecutor::GetRunDelay() const
{
	return m_cRunDelayInMS;
}

}//namespace Sys

}//namespace CX


#endif

