
#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Platform/Windows/ThreadPool.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

ThreadPool::ThreadPool()
{
	m_pPool               = NULL;
	m_pCleanupGroup       = NULL;
	m_bCBEInitialized     = False;
	m_hSemaphoreWorkItems = NULL;
	m_hEventStop          = NULL;
	memset(&m_cbe, 0, sizeof(m_cbe));
}

ThreadPool::~ThreadPool()
{
	Stop();
}

Status ThreadPool::Start(Size cMaxThreads/* = MAX_THREADS*/, Size cWaitingWorkItems/* = MAX_WAITING_WORK_ITEMS*/)
{
	if (NULL != m_pPool)
	{
		return Status_InvalidCall;
	}

	Status   status;

	for (;;)
	{
		if (0 < cWaitingWorkItems)
		{
			m_hEventStop          = CreateEventA(NULL, FALSE, FALSE, NULL);
			m_hSemaphoreWorkItems = CreateSemaphoreA(NULL, (LONG)cWaitingWorkItems, (LONG)cWaitingWorkItems, NULL);
		}
		if (NULL == (m_pPool = CreateThreadpool(NULL)))
		{
			status = Status(Status_OperationFailed, "CreateThreadpool failed with error {1}", GetLastError());

			break;
		}

		InitializeThreadpoolEnvironment(&m_cbe);
		m_bCBEInitialized = True;

		SetThreadpoolThreadMaximum(m_pPool, (DWORD)cMaxThreads);

		SetThreadpoolCallbackPool(&m_cbe, m_pPool);

		if (NULL == (m_pCleanupGroup = CreateThreadpoolCleanupGroup()))
		{
			status = Status(Status_OperationFailed, "CreateThreadpoolCleanupGroup failed with error {1}", GetLastError());

			break;
		}
		SetThreadpoolCallbackCleanupGroup(&m_cbe, m_pCleanupGroup, NULL);

		break;
	}
	if (!status)
	{
		Stop();
	}

	return status;
}

Status ThreadPool::Stop()
{
	if (NULL != m_hEventStop)
	{
		SetEvent(m_hEventStop);
	}
	if (NULL != m_pCleanupGroup)
	{
		CloseThreadpoolCleanupGroupMembers(m_pCleanupGroup, FALSE, NULL);
		CloseThreadpoolCleanupGroup(m_pCleanupGroup);
		m_pCleanupGroup = NULL;
	}
	if (m_bCBEInitialized)
	{
		DestroyThreadpoolEnvironment(&m_cbe);
		m_bCBEInitialized = False;
	}
	if (NULL != m_pPool)
	{
		CloseThreadpool(m_pPool);
		m_pPool = NULL;
	}
	if (NULL != m_hSemaphoreWorkItems)
	{
		CloseHandle(m_hSemaphoreWorkItems);
		m_hSemaphoreWorkItems = NULL;
	}
	if (NULL != m_hEventStop)
	{
		CloseHandle(m_hEventStop);
		m_hEventStop = NULL;
	}

	return Status();
}

Status ThreadPool::AddWork(PTP_WORK_CALLBACK pfnWorkCallback, void *pWorkArg)
{
	if (NULL == m_pPool)
	{
		return Status_InvalidCall;
	}

	if (NULL != m_hSemaphoreWorkItems)
	{
		HANDLE   handles[2] = { m_hEventStop, m_hSemaphoreWorkItems };
		DWORD    dwRet;

		dwRet = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dwRet)
		{
			SetEvent(m_hEventStop);

			return Status_Cancelled;
		}
		else
		if (WAIT_OBJECT_0 + 1 != dwRet)
		{
			return Status_OperationFailed;
		}
	}

	PTP_WORK              pWork;
	WorkCallbackContext   *pContext;
	
	if (NULL == (pContext = new WorkCallbackContext()))
	{
		return Status_MemAllocFailed;
	}
	pContext->pThreadPool           = this;
	pContext->pfnActualWorkCallback = pfnWorkCallback;
	pContext->pActualContext        = pWorkArg;
	if (NULL == (pWork = CreateThreadpoolWork(&ThreadPool::WorkCallback, pContext, &m_cbe)))
	{
		delete pContext;

		return Status(Status_OperationFailed, "CreateThreadpoolWork failed with error {1}", GetLastError());
	}
	SubmitThreadpoolWork(pWork);

	return Status();
}

VOID CALLBACK ThreadPool::WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork)
{
	WorkCallbackContext *pCTX = (WorkCallbackContext *)pContext;

	pCTX->pfnActualWorkCallback(pInstance, pCTX->pActualContext, pWork);

	if (NULL != pCTX->pThreadPool->m_hSemaphoreWorkItems)
	{
		ReleaseSemaphore(pCTX->pThreadPool->m_hSemaphoreWorkItems, 1, NULL);
	}

	delete pCTX;
}

}//namespace Sys

}//namespace CX


#endif

