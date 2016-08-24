
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Print.hpp"
#include "CX/Debug/IProfilingHandler.hpp"


namespace CX
{

namespace Debug
{

class SysLogProfilingHandler : public IProfilingHandler
{
public:

	SysLogProfilingHandler(Resolution nResolution = Resolution_NanoSeconds);

	virtual Resolution GetResolution();

	virtual bool OnBeginProfiling();

	virtual bool OnBeginScope(const Char *szFileName, const Char *szScopeName, int cLineNo, UInt64 cMinCallDuration, 
	                          UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, 
	                          UInt64 cTotalCalls, bool bRootScope);

	virtual bool OnEndScope(bool bRootScope);

	virtual bool OnBeginCallHotSpots();

	virtual bool OnCallHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo,
	                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
	                           UInt64 cTotalCallDuration, UInt64 cTotalCalls);

	virtual bool OnEndCallHotSpots();

	virtual bool OnBeginDurationHotSpots();

	virtual bool OnDurationHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo, 
	                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
	                               UInt64 cTotalCallDuration, UInt64 cTotalCalls);

	virtual bool OnEndDurationHotSpots();

	virtual bool OnEndProfiling();

private:

	Resolution m_nResolution;
	Size       m_cDepth;

};

}//namespace Debug

}//namespace CX
