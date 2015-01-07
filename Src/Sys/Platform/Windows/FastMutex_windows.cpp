
#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/FastMutex.h"
#include "CX/Status.h"


namespace CX
{

namespace Sys
{

FastMutex::FastMutex()
{
	Status::Clear();

	InitializeCriticalSection(&m_cs);
}

FastMutex::~FastMutex()
{
	Status::Clear();

	DeleteCriticalSection(&m_cs);
}

void FastMutex::Enter()
{
	Status::Clear();

	EnterCriticalSection(&m_cs);
}

void FastMutex::Leave()
{
	LeaveCriticalSection(&m_cs);
}

}//namespace Sys

}//namespace CX


#endif

