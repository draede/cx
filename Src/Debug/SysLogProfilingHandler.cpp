/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 
 
#include "CX/Debug/SysLogProfilingHandler.hpp"


namespace CX
{

namespace Debug
{

SysLogProfilingHandler::SysLogProfilingHandler()
{
	m_cDepth = 0;
}

bool SysLogProfilingHandler::OnBeginProfiling()
{
	String sOutput = "\nBEGIN PROFILING (nanoseconds)\n\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

bool SysLogProfilingHandler::OnBeginScope(const Char *szFileName, const Char *szScopeName, int cLineNo, 
                                          UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
                                          UInt64 cTotalCallDuration, UInt64 cTotalCalls, bool bRootScope)
{
	CX_UNUSED(bRootScope);

	String sIndent(m_cDepth * 3, ' ');
	String sOutput;

	Print(&sOutput, "{1}{2} @ {3}:{4}, min={5}, max={6}, avg={7}, total={8}, calls={9}\n",
	      sIndent, szScopeName, szFileName, cLineNo, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
	      cTotalCallDuration, cTotalCalls);

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	m_cDepth++;

	return true;
}

bool SysLogProfilingHandler::OnEndScope(bool bRootScope)
{
	if (bRootScope)
	{
		String sOutput = "\n";

		m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());
	}
	m_cDepth--;

	return true;
}

bool SysLogProfilingHandler::OnBeginCallHotSpots()
{
	String sOutput = "CALL HOT SPOTS\n\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

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

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

bool SysLogProfilingHandler::OnEndCallHotSpots()
{
	String sOutput = "\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

bool SysLogProfilingHandler::OnBeginDurationHotSpots()
{
	String sOutput = "DURATION HOT SPOTS\n\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

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

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

bool SysLogProfilingHandler::OnEndDurationHotSpots()
{
	String sOutput = "\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

bool SysLogProfilingHandler::OnEndProfiling()
{
	String sOutput = "END PROFILING\n\n";

	m_sysout.Write(Log::Level_Debug, "", sOutput.c_str(), sOutput.size());

	return true;
}

}//namespace Debug

}//namespace CX
