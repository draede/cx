/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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


#include "CX/Sys/Platform/Windows/SyncedWorkerThreads.hpp"


namespace CX
{

namespace Sys
{

SyncedWorkerThreads::SyncedWorkerThreads()
{
	m_cThreads = 0;
	m_bRunning = False;
}

SyncedWorkerThreads::~SyncedWorkerThreads()
{
	Stop();
}

Status SyncedWorkerThreads::Start(Size cThreads)
{
	if (m_bRunning)
	{
		return Status_Busy;
	}
	if (MIN_THREADS > cThreads || MAX_THREADS < cThreads)
	{
		return Status_InvalidArg;
	}

	Group    group;

	m_vectorGroups.push_back(group);

	DWORD    dwThreadID;
	HANDLE   hWaitEvent;
	HANDLE   hReadyEvent;
	HANDLE   hThread;
	Status   status;

	m_cThreads = cThreads;
	m_bRunning = True;

	for (;;)
	{
		for (Size i = 0; i < cThreads; i++)
		{
			Group       *pGroup = &m_vectorGroups[m_vectorGroups.size() - 1];

			if (NULL == (hWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
			{
				status = Status(Status_OperationFailed, "Failed to create event {1}", i);

				break;
			}

			if (NULL == (hReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
			{
				CloseHandle(hWaitEvent);
				status = Status(Status_OperationFailed, "Failed to create event {1}", i);

				break;
			}

			ThreadArg   *pArg   = new (std::nothrow) ThreadArg();

			if (NULL == pArg)
			{
				CloseHandle(hWaitEvent);
				CloseHandle(hReadyEvent);
				status = Status(Status_OperationFailed, "Failed to allocate thread arg {1}", i);

				break;
			}

			pArg->pThis       = this;
			pArg->hWaitEvent  = hWaitEvent;
			pArg->hReadyEvent = hReadyEvent;
			pArg->jobs        = NULL;
			pArg->cJobs       = 0;
			pArg->cbJobSize   = 0;
			pArg->pfnJob      = NULL;

			if (NULL == (hThread = CreateThread(NULL, 0, &Worker, pArg, 0, &dwThreadID)))
			{
				CloseHandle(hWaitEvent);
				CloseHandle(hReadyEvent);
				delete pArg;

				status = Status(Status_OperationFailed, "Failed to create thread {1}", i);

				break;
			}

			pGroup->vectorWaitEvents.push_back(hWaitEvent);
			pGroup->vectorReadyEvents.push_back(hReadyEvent);
			pGroup->vectorThreadArgs.push_back(pArg);
			pGroup->vectorThreads.push_back(hThread);

			if (pGroup->vectorThreads.size() == MAX_THREADS_PER_GROUP)
			{
				m_vectorGroups.push_back(group);
			}
		}
		if (!status)
		{
			break;
		}

		break;
	}
	if (!status)
	{
		Stop();
	}

	return status;
}

Status SyncedWorkerThreads::RunJobs(void *jobs, Size cJobs, Size cbJobSize, JobFunction pfnJobFunction)
{
	if (!m_bRunning)
	{
		return Status_NotInitialized;
	}
	if (0 == cJobs)
	{
		return Status(); // nothing to do
	}

	Size   cJobsPerThread = cJobs / m_cThreads;

	if (0 < (cJobs % m_cThreads))
	{
		cJobsPerThread++;
	}

	//update args
	for (auto iterGroups = m_vectorGroups.begin(); iterGroups != m_vectorGroups.end(); ++iterGroups)
	{
		for (auto iterArgs = iterGroups->vectorThreadArgs.begin(); 
		     iterArgs != iterGroups->vectorThreadArgs.end(); ++iterArgs)
		{
			if (0 == cJobs)
			{
				(*iterArgs)->pfnJob    = NULL;
				(*iterArgs)->cJobs     = 0;
				(*iterArgs)->jobs      = NULL;
				(*iterArgs)->cbJobSize = 0;
			}
			else
			{
				if (cJobs > cJobsPerThread)
				{
					(*iterArgs)->pfnJob    = pfnJobFunction;
					(*iterArgs)->cJobs     = cJobsPerThread;
					(*iterArgs)->jobs      = jobs;
					(*iterArgs)->cbJobSize = cbJobSize;

					jobs = (Byte *)jobs + cJobsPerThread * cbJobSize;
					cJobs -= cJobsPerThread;
				}
				else
				{
					(*iterArgs)->pfnJob    = pfnJobFunction;
					(*iterArgs)->cJobs     = cJobs;
					(*iterArgs)->jobs      = jobs;
					(*iterArgs)->cbJobSize = cbJobSize;

					jobs = (Byte *)jobs + cJobsPerThread * cJobs;
					cJobs = 0;
				}
			}
		}
	}

	//set events
	for (auto iterGroups = m_vectorGroups.begin(); iterGroups != m_vectorGroups.end(); ++iterGroups)
	{
		for (auto iterEvents = iterGroups->vectorWaitEvents.begin(); 
		     iterEvents != iterGroups->vectorWaitEvents.end(); ++iterEvents)
		{
			SetEvent(*iterEvents);
		}
	}
	
	//wait for threads to finish the jobs
	for (auto iterGroups = m_vectorGroups.begin(); iterGroups != m_vectorGroups.end(); ++iterGroups)
	{
		WaitForMultipleObjects((DWORD)iterGroups->vectorReadyEvents.size(), &iterGroups->vectorReadyEvents[0], TRUE, 
		                       INFINITE);
	}

	return Status();
}

Status SyncedWorkerThreads::Stop()
{
	if (!m_bRunning)
	{
		return Status_NotInitialized;
	}

	m_bRunning = False;
	for (auto iterGroups = m_vectorGroups.begin(); iterGroups != m_vectorGroups.end(); ++iterGroups)
	{
		Size   cThreads = iterGroups->vectorThreads.size();

		for (Size i = 0; i < cThreads; i++)
		{
			SetEvent(iterGroups->vectorWaitEvents[i]);
		}
		WaitForMultipleObjects((DWORD)cThreads, &iterGroups->vectorThreads[0], TRUE, INFINITE);

		for (auto iterThreads = iterGroups->vectorThreads.begin(); 
		     iterThreads != iterGroups->vectorThreads.end(); ++iterThreads)
		{
			CloseHandle(*iterThreads);
		}

		for (auto iterEvents = iterGroups->vectorReadyEvents.begin(); 
		     iterEvents != iterGroups->vectorReadyEvents.end(); ++iterEvents)
		{
			CloseHandle(*iterEvents);
		}

		for (auto iterEvents = iterGroups->vectorWaitEvents.begin(); 
		     iterEvents != iterGroups->vectorWaitEvents.end(); ++iterEvents)
		{
			CloseHandle(*iterEvents);
		}

		for (auto iterArgs = iterGroups->vectorThreadArgs.begin(); 
		     iterArgs != iterGroups->vectorThreadArgs.end(); ++iterArgs)
		{
			delete *iterArgs;
		}
	}
	m_vectorGroups.clear();
	m_cThreads = 0;

	return Status();
}

DWORD WINAPI SyncedWorkerThreads::Worker(void *pThreadArgs)
{
	ThreadArg   *pArg = (ThreadArg *)pThreadArgs;

	for (;;)
	{
		WaitForSingleObject(pArg->hWaitEvent, INFINITE);

		void   *jobs = pArg->jobs;

		for (Size i = 0; i < pArg->cJobs; i++)
		{
			if (!pArg->pfnJob(jobs, pArg->cbJobSize))
			{
				pArg->pThis->m_bRunning = False;

				break;
			}
			jobs = (Byte *)jobs + pArg->cbJobSize;
		}

		pArg->jobs      = NULL;
		pArg->cJobs     = 0;
		pArg->cbJobSize = 0;
		pArg->pfnJob    = NULL;

		SetEvent(pArg->hReadyEvent);

		if (!pArg->pThis->m_bRunning)
		{
			break;
		}
	}

	return 0;
}

}//namespace Sys

}//namespace CX


#endif

