/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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

	PrintProfilingHandler(OUTPUT output)
	{
		m_output = output;
	}

	virtual bool OnBeginProfiling()
	{
		Print(m_output, "\nBEGIN PROFILING\n");

		Print(m_output, "\nCALL STACKS\n\n");

		return true;
	}

	virtual bool OnBeginScope(const Char *szScopeName, UInt64 cMinCallDuration, 
	                          UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, 
	                          UInt64 cTotalCalls, Size cDepth)
	{
		String sIndent(cDepth * 3, ' ');

		Print(m_output, "{1}{2}, min={3}, max={4}, avg={5}, total={6}, calls={7}\n",
		      sIndent, szScopeName, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
		      cTotalCallDuration, cTotalCalls);

		return true;
	}

	virtual bool OnEndScope(Size cDepth)
	{
		if (0 == cDepth)
		{
			Print(m_output, "\n");
		}

		return true;
	}

	virtual bool OnBeginCallHotSpots()
	{
		Print(m_output, "CALL HOT SPOTS\n\n");

		return true;
	}

	virtual bool OnCallHotSpot(const Char *szScopeName,
	                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
	                           UInt64 cTotalCallDuration, UInt64 cTotalCalls)
	{
		Print(m_output, "{1}, min={2}, max={3}, avg={4}, total={5}, calls={6}\n",
		      szScopeName, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
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

	virtual bool OnDurationHotSpot(const Char *szScopeName, 
	                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
	                               UInt64 cTotalCallDuration, UInt64 cTotalCalls)
	{
		Print(m_output, "{1}, min={2}, max={3}, avg={4}, total={5}, calls={6}\n",
		      szScopeName, cMinCallDuration, cMaxCallDuration, cAvgCallDuration,
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

	OUTPUT   m_output;

};

}//namespace Debug

}//namespace CX
