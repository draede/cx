
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include <windows.h>


namespace CX
{

namespace Util
{

class Timer
{
public:

	Timer();

	virtual ~Timer();

	StatusCode ResetTimer();

	double GetElapsedTime() const;

	UInt64 GetTimeStamp();

	UInt64 GetResolution();

private:

	LARGE_INTEGER m_liTimerResolution;
	LARGE_INTEGER m_liStartTimer;

};

}//namespace Util

}//namespace CX


#endif

