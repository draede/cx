
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include "CX/APIDefs.h"
#include <windows.h>


namespace CX
{

namespace Sys
{

typedef UInt64       TID;
typedef UInt64       PID;

class CX_API Thread
{
public:

	Thread();

	virtual ~Thread();



	StatusCode Wait();

	bool IsRunning();

	TID GetID();

	static PID GetCurrentProcessID();

	static TID GetCurrentThreadID();

	static void Sleep(Size cMilliseconds);

private:

	static DWORD ThreadProc(void *pArg);

	HANDLE   m_hThread;
	TID      m_nID;

};

}//namespace Sys

}//namespace CX


#endif

