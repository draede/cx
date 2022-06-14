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


namespace CX
{

namespace Debug
{

class IProfilingHandler
{
public:

	static const Size       MAX_CALL_HOT_SPOTS      = 100;
	static const Size       MAX_DURATIONS_HOT_SPOTS = 100;

	virtual ~IProfilingHandler() { }

	virtual Size GetMaxCallHotSpots()
	{
		return MAX_CALL_HOT_SPOTS;
	}

	virtual Size GetMaxDurationHotSpots()
	{
		return MAX_DURATIONS_HOT_SPOTS;
	}

	virtual bool OnBeginProfiling() = 0;

	virtual bool OnBeginScope(const Char *szScopeName, UInt64 cMinCallDuration, 
	                          UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, UInt64 cTotalCallDuration, 
	                          UInt64 cTotalCalls, Size cDepth) = 0;

	virtual bool OnEndScope(Size cDepth) = 0;

	virtual bool OnBeginCallHotSpots() = 0;

	virtual bool OnCallHotSpot(const Char *szScopeName,
	                           UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration,
	                           UInt64 cTotalCallDuration, UInt64 cTotalCalls) = 0;

	virtual bool OnEndCallHotSpots() = 0;

	virtual bool OnBeginDurationHotSpots() = 0;

	virtual bool OnDurationHotSpot(const Char *szScopeName, 
	                               UInt64 cMinCallDuration, UInt64 cMaxCallDuration, UInt64 cAvgCallDuration, 
	                               UInt64 cTotalCallDuration, UInt64 cTotalCalls) = 0;

	virtual bool OnEndDurationHotSpots() = 0;

	virtual bool OnEndProfiling() = 0;

};

}//namespace Debug

}//namespace CX
