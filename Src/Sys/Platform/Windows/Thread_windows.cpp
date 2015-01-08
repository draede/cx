
#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Thread.h"
#include "CX/Status.h"


namespace CX
{

namespace Sys
{

Thread::Thread()
{
	Status::Clear();

	m_hThread = NULL;
	m_nID     = 0;
}

Thread::~Thread()
{
	Status::Clear();
}

StatusCode Thread::Wait()
{
	Status::Clear();

	if (NULL == m_hThread)
	{
		return Status::Set(Status_NotInitialized, "Thread not started");
	}
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, INFINITE))
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		return Status_OK;
	}

	return Status::Set(Status_OperationFailed, "WaitForSingleObject failed with error {1}", 
	                   GetLastError());
}

bool Thread::IsRunning()
{
	Status::Clear();

	return (NULL != m_hThread);
}

TID Thread::GetID()
{
	Status::Clear();

	if (NULL == m_hThread)
	{
		Status::Set(Status_NotInitialized, "Thread not started");

		return 0;
	}

	return m_nID;
}

PID Thread::GetCurrentProcessID()
{
	Status::Clear();

	return (PID)GetCurrentProcessId();
}

TID Thread::GetCurrentThreadID()
{
	Status::Clear();

	return (TID)GetCurrentThreadId();
}

void Thread::Sleep(Size cMilliseconds)
{
	Status::Clear();

	::Sleep((DWORD)cMilliseconds);
}

DWORD Thread::ThreadProc(void *pArg)
{
	IHelper *pHelper = (IHelper *)pArg;

	pHelper->Run();

	Delete(pHelper);

	return 0;
}

}//namespace Sys

}//namespace CX


#endif

