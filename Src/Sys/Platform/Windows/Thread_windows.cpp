
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

	::Sleep(cMilliseconds);
}

DWORD Thread::ThreadProc(void *pArg)
{
	return 0;
}

}//namespace Sys

}//namespace CX


#endif

