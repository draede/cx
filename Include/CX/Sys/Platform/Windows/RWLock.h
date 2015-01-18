//http://msdn.microsoft.com/en-us/magazine/cc163405.aspx
//http://www.codeproject.com/Articles/32685/Testing-reader-writer-locks
//RWLockFavorWriters

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

class CX_API RWLock
{
public:

	RWLock();

	virtual ~RWLock();

	void EnterRead();

	void LeaveRead();

	void EnterWrite();

	void LeaveWrite();

private:

	LONG               m_cWritersWaiting;
	LONG               m_cReadersWaiting;

	DWORD              m_cActiveWriterReaders;

	HANDLE             m_hReadyToRead;
	HANDLE             m_hReadyToWrite;

	CRITICAL_SECTION   m_cs;

};

typedef Scope<RWLock, &RWLock::EnterRead, &RWLock::LeaveRead>     RLocker;
typedef Scope<RWLock, &RWLock::EnterWrite, &RWLock::LeaveWrite>   WLocker;

}//namespace Sys

}//namespace CX


#endif

