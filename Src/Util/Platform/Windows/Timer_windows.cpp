
#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Util/Timer.h"
#include "CX/Status.h"


namespace CX
{

namespace Util
{

Timer::Timer()
{
	Status::Clear();

	QueryPerformanceFrequency(&m_liTimerResolution);
	QueryPerformanceCounter(&m_liStartTimer);
}

Timer::~Timer()
{
	Status::Clear();
}

StatusCode Timer::ResetTimer()
{
	Status::Clear();

	if (!QueryPerformanceCounter(&m_liStartTimer))
	{
		return Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());
	}

	return Status_OK;
}

double Timer::GetElapsedTime() const
{
	Status::Clear();

	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());

		return 0.0;
	}

	return (double)(lint.QuadPart - m_liStartTimer.QuadPart) / m_liTimerResolution.QuadPart;
}

UInt64 Timer::GetTimeStamp()
{
	Status::Clear();

	LARGE_INTEGER lint;

	if (!QueryPerformanceCounter(&lint))
	{
		Status::Set(Status_OperationFailed, "QueryPerformanceCounter failed with code {1}", GetLastError());

		return 0;
	}

	return (UInt64)lint.QuadPart;
}

UInt64 Timer::GetResolution()
{
	Status::Clear();

	return (UInt64)m_liTimerResolution.QuadPart;
}	

}//namespace Util

}//namespace CX


#endif

