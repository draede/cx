
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Debug
{

class IProfilingHandler
{
public:

	static const Size   MAX_CALL_HOT_SPOTS      = 100;
	static const Size   MAX_DURATIONS_HOT_SPOTS = 100;

	~IProfilingHandler() { }

	enum Resolution
	{
		Resolution_Seconds,
		Resolution_MilliSeconds,
		Resolution_MicroSeconds,
		Resolution_NanoSeconds,
	};

	virtual Size GetMaxCallHotSpots()
	{
		return MAX_CALL_HOT_SPOTS;
	}

	virtual Size GetMaxDurationHotSpots()
	{
		return MAX_DURATIONS_HOT_SPOTS;
	}

	virtual Resolution GetResolution() = 0;

	virtual bool OnBeginProfiling() = 0;

	virtual bool OnBeginScope(const Char *szFileName, const Char *szScopeName, int cLineNo, UInt64 cMinCallDuration, 
	                          UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, 
	                          UInt64 cTotalCalls, bool bRootScope) = 0;

	virtual bool OnEndScope(bool bRootScope) = 0;

	virtual bool OnBeginCallHotSpots() = 0;

	virtual bool OnCallHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo,
	                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
	                           UInt64 cTotalCallDuration, UInt64 cTotalCalls) = 0;

	virtual bool OnEndCallHotSpots() = 0;

	virtual bool OnBeginDurationHotSpots() = 0;

	virtual bool OnDurationHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo, 
	                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
	                               UInt64 cTotalCallDuration, UInt64 cTotalCalls) = 0;

	virtual bool OnEndDurationHotSpots() = 0;

	virtual bool OnEndProfiling() = 0;

};

}//namespace Debug

}//namespace CX
