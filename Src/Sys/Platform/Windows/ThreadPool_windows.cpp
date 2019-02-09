
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
	m_pPool           = NULL;
	m_pCleanupGroup   = NULL;
	m_bCBEInitialized = False;
	memset(&m_cbe, 0, sizeof(m_cbe));
}

ThreadPool::~ThreadPool()
{
	Stop();
}

Status ThreadPool::Start(Size cMaxThreads/* = MAX_THREADS*/)
{
	if (NULL != m_pPool)
	{
		return Status_InvalidCall;
	}

	Status   status;

	for (;;)
	{
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

	return Status();
}

Status ThreadPool::AddWork(PTP_WORK_CALLBACK pfnWorkCallback, void *pWorkArg)
{
	if (NULL == m_pPool)
	{
		return Status_InvalidCall;
	}

	PTP_WORK pWork;
	
	if (NULL == (pWork = CreateThreadpoolWork(pfnWorkCallback, pWorkArg, &m_cbe)))
	{
		return Status(Status_OperationFailed, "CreateThreadpoolWork failed with error {1}", GetLastError());
	}
	SubmitThreadpoolWork(pWork);

	return Status();
}

}//namespace Sys

}//namespace CX


#endif

