
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include "CX/Scope.h"
#include "CX/APIDefs.h"
#include <windows.h>


namespace CX
{

namespace Sys
{

class CX_API Lock
{
public:

	Lock();

	virtual ~Lock();

	void Enter();

	void Leave();

private:

	CRITICAL_SECTION m_cs;

};

typedef Scope<Lock, &Lock::Enter, &Lock::Leave>     Locker;

}//namespace Sys

}//namespace CX


#endif

