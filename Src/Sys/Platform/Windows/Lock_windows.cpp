
#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Lock.h"
#include "CX/Status.h"


namespace CX
{

namespace Sys
{

Lock::Lock()
{
	Status::Clear();

	InitializeCriticalSection(&m_cs);
}

Lock::~Lock()
{
	Status::Clear();

	DeleteCriticalSection(&m_cs);
}

void Lock::Enter()
{
	Status::Clear();

	EnterCriticalSection(&m_cs);
}

void Lock::Leave()
{
	LeaveCriticalSection(&m_cs);
}

}//namespace Sys

}//namespace CX


#endif

