/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/Sys/Lock.hpp"
#include "CX/Debug/ThreadProfiler.hpp"
#include "CX/Debug/IProfilingHandler.hpp"
#include "CX/Map.hpp"
#include "CX/Vector.hpp"


namespace CX
{

namespace Debug
{

class Profiler
{
public:

	static Profiler &Get();

	void SetEnabled(bool bEnabled = true);

	bool GetEnabled();

	//pProfilingHandler will be deleted at exit
	void SetOnExitProfilingHandler(IProfilingHandler *pProfilingHandler);

	bool GetProfiling(IProfilingHandler *pProfilingHandler);

protected:

	friend class ThreadProfiler;

	void AddScope(ThreadProfiler::Scope *pScope);

private:

	Sys::Lock                     m_lock;
	IProfilingHandler             *m_pOnDestructProfilingHandler;
	ThreadProfiler::Scope         m_root;
	bool                          m_bEnabled;

	struct HotSpotName
	{
		Char   *szFileName;
		Char   *szScopeName;
		int    cLineNo;
	};

	struct HotSpot
	{
		HotSpotName name;
		UInt64      cMinDuration;
		UInt64      cMaxDuration;
		UInt64      cTotalDuration;
		UInt64      cCalls;
	};

	struct HotSpotNameLess
	{
		bool operator()(const HotSpotName &hs1, const HotSpotName &hs2) const
		{
			int nRet;

#pragma warning(push)
#pragma warning(disable: 4996)
			nRet = cx_stricmp(hs1.szFileName, hs2.szFileName);
#pragma warning(pop)
			if (0 > nRet)
			{
				return true;
			}
			else
			if (0 < nRet)
			{
				return false;
			}
			else
			{
#pragma warning(push)
#pragma warning(disable: 4996)
				nRet = cx_stricmp(hs1.szScopeName, hs2.szScopeName);
#pragma warning(pop)
				if (0 > nRet)
				{
					return true;
				}
				else
				if (0 < nRet)
				{
					return false;
				}
				else
				{
					return (hs1.cLineNo < hs2.cLineNo);
				}
			}
		}
	};

	typedef Map<HotSpotName, Size, HotSpotNameLess>::Type   HotSpotsMap;
	typedef Vector<HotSpot>::Type                           HotSpotsVector;

	Profiler();

	~Profiler();

	void FreeScopeMem(ThreadProfiler::Scope *pScope);

	bool GetProfiling(ThreadProfiler::Scope *pScope, IProfilingHandler *pProfilingHandler, bool bRootScope);

	void Merge(ThreadProfiler::Scope *pScope);

	void MergeChildren(ThreadProfiler::Scope *pScope, Size &cInitialCount, Size &cFinalCount);

	void GetHotSpots(ThreadProfiler::Scope *pScope, HotSpotsVector &vectorCalls, HotSpotsVector &vectorDurations, 
	                 HotSpotsMap &mapCalls, HotSpotsMap &mapDurations, 
	                 Size cMaxCallHotSpots, Size cMaxDurationHotSpots);

};

}//namespace Sys

}//namespace CX
