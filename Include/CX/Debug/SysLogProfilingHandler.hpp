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
 
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Print.hpp"
#include "CX/Debug/IProfilingHandler.hpp"
#include "CX/Log/SystemOutput.hpp"


namespace CX
{

namespace Debug
{

class SysLogProfilingHandler : public IProfilingHandler
{
public:

	SysLogProfilingHandler();

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

	Size                m_cDepth;
	Log::SystemOutput   m_sysout;

};

}//namespace Debug

}//namespace CX
