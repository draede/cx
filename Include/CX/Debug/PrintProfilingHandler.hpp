
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Print.hpp"
#include "CX/Debug/IProfilingHandler.hpp"


namespace CX
{

namespace Debug
{

template <typename OUTPUT>
class PrintProfilingHandler : public IProfilingHandler
{
public:

	PrintProfilingHandler(OUTPUT output, Resolution nResolution = Resolution_NanoSeconds)
	{
		m_output      = output;
		m_nResolution = nResolution;
		m_cDepth      = 0;
	}

	virtual Resolution GetResolution()
	{
		return m_nResolution;
	}

	virtual bool OnBeginProfiling()
	{
		if (IProfilingHandler::Resolution_Seconds == m_nResolution)
		{
			Print(m_output, "\nBEGIN PROFILING (seconds)\n\n");
			Print(m_output, "\nBEGIN SCOPE (seconds)\n\n");
		}
		else
		if (IProfilingHandler::Resolution_MilliSeconds == m_nResolution)
		{
			Print(m_output, "\nBEGIN PROFILING (milliseconds)\n\n");
		}
		else
		if (IProfilingHandler::Resolution_MicroSeconds == m_nResolution)
		{
			Print(m_output, "\nBEGIN PROFILING (microseconds)\n\n");
		}
		else
		{
			Print(m_output, "\nBEGIN PROFILING (nanoseconds)\n\n");
		}

		return true;
	}

	virtual bool OnBeginScope(const Char *szFileName, const Char *szScopeName, int cLineNo, UInt64 cMinCallDuration, 
	                          UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, 
	                          UInt64 cTotalCalls, bool bRootScope)
	{
		bRootScope;

		String sIndent(m_cDepth * 3, ' ');

		Print(m_output, "{1}{2} @ {3}:{4}, min={5}, max={6}, avg={7}, total={8}, calls={9}\n",
		      sIndent, szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
		      cTotalCallDuration, cTotalCalls);

		m_cDepth++;

		return true;
	}

	virtual bool OnEndScope(bool bRootScope)
	{
		if (bRootScope)
		{
			Print(m_output, "\n");
		}

		m_cDepth--;

		return true;
	}

	virtual bool OnBeginCallHotSpots()
	{
		Print(m_output, "CALL HOT SPOTS\n\n");

		return true;
	}

	virtual bool OnCallHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo,
	                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
	                           UInt64 cTotalCallDuration, UInt64 cTotalCalls)
	{
		Print(m_output, "{1} @ {2}:{3}, min={4}, max={5}, avg={6}, total={7}, calls={8}\n",
		      szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
		      cTotalCallDuration, cTotalCalls);

		return true;
	}

	virtual bool OnEndCallHotSpots()
	{
		Print(m_output, "\n");

		return true;
	}

	virtual bool OnBeginDurationHotSpots()
	{
		Print(m_output, "DURATION HOT SPOTS\n\n");

		return true;
	}

	virtual bool OnDurationHotSpot(const Char *szFileName, const Char *szScopeName, int cLineNo, 
	                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
	                               UInt64 cTotalCallDuration, UInt64 cTotalCalls)
	{
		Print(m_output, "{1} @ {2}:{3}, min={4}, max={5}, avg={6}, total={7}, calls={8}\n",
		      szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
		      cTotalCallDuration, cTotalCalls);

		return true;
	}

	virtual bool OnEndDurationHotSpots()
	{
		Print(m_output, "\n");

		return true;
	}

	virtual bool OnEndProfiling()
	{
		Print(m_output, "END PROFILING\n\n");

		return true;
	}

private:

	OUTPUT     m_output;
	Resolution m_nResolution;
	Size       m_cDepth;

};

}//namespace Debug

}//namespace CX
