
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

class CX_API FastMutex
{
public:

	FastMutex();

	virtual ~FastMutex();

	void Enter();

	void Leave();

private:

	CRITICAL_SECTION m_cs;

};

}//namespace Sys

}//namespace CX


#endif

