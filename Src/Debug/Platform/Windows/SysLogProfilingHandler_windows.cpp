
#include "CX/Debug/SysLogProfilingHandler.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Debug
{

SysLogProfilingHandler::SysLogProfilingHandler(Resolution nResolution/* = Resolution_NanoSeconds*/)
{
	m_nResolution = nResolution;
	m_cDepth      = 0;
}

SysLogProfilingHandler::Resolution SysLogProfilingHandler::GetResolution()
{
	return m_nResolution;
}

bool SysLogProfilingHandler::OnBeginProfiling()
{
	if (IProfilingHandler::Resolution_Seconds == m_nResolution)
	{
		OutputDebugStringA("\nBEGIN PROFILING (seconds)\n\n");
		OutputDebugStringA("\nBEGIN SCOPE\n\n");
	}
	else
	if (IProfilingHandler::Resolution_MilliSeconds == m_nResolution)
	{
		OutputDebugStringA("\nBEGIN PROFILING (milliseconds)\n\n");
	}
	else
	if (IProfilingHandler::Resolution_MicroSeconds == m_nResolution)
	{
		OutputDebugStringA("\nBEGIN PROFILING (microseconds)\n\n");
	}
	else
	{
		OutputDebugStringA("\nBEGIN PROFILING (nanoseconds)\n\n");
	}

	return true;
}

bool SysLogProfilingHandler::OnBeginScope(const Char *szFileName, const Char *szScopeName, int cLineNo, 
                                          UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
                                          UInt64 cTotalCallDuration, UInt64 cTotalCalls, bool bRootScope)
{
	bRootScope;

	String sIndent(m_cDepth * 3, ' ');
	String sOutput;

	Print(&sOutput, "{1}{2} @ {3}:{4}, min={5}, max={6}, avg={7}, total={8}, calls={9}\n",
	      sIndent, szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
	      cTotalCallDuration, cTotalCalls);

	OutputDebugStringA(sOutput.c_str());

	m_cDepth++;

	return true;
}

bool SysLogProfilingHandler::OnEndScope(bool bRootScope)
{
	if (bRootScope)
	{
		OutputDebugStringA("\n");
	}
	m_cDepth--;

	return true;
}

bool SysLogProfilingHandler::OnBeginCallHotSpots()
{
	OutputDebugStringA("CALL HOT SPOTS\n\n");

	return true;
}

bool SysLogProfilingHandler::OnCallHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo,
                                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
                                           UInt64 cTotalCallDuration, UInt64 cTotalCalls)
{
	String sOutput;

	Print(&sOutput, "{1} @ {2}:{3}, min={4}, max={5}, avg={6}, total={7}, calls={8}\n",
	      szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
	      cTotalCallDuration, cTotalCalls);

	OutputDebugStringA(sOutput.c_str());

	return true;
}

bool SysLogProfilingHandler::OnEndCallHotSpots()
{
	OutputDebugStringA("\n");

	return true;
}

bool SysLogProfilingHandler::OnBeginDurationHotSpots()
{
	OutputDebugStringA("DURATION HOT SPOTS\n\n");

	return true;
}

bool SysLogProfilingHandler::OnDurationHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo, 
                                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, 
                                               UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, UInt64 cTotalCalls)
{
	String sOutput;

	Print(&sOutput, "{1} @ {2}:{3}, min={4}, max={5}, avg={6}, total={7}, calls={8}\n",
	      szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
	      cTotalCallDuration, cTotalCalls);

	OutputDebugStringA(sOutput.c_str());

	return true;
}

bool SysLogProfilingHandler::OnEndDurationHotSpots()
{
	OutputDebugStringA("\n");

	return true;
}

bool SysLogProfilingHandler::OnEndProfiling()
{
	OutputDebugStringA("END PROFILING\n\n");

	return true;
}

}//namespace Debug

}//namespace CX
