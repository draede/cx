/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
 
#include "CX/precomp.hpp"
#include "CX/Debug/ThreadProfiler.hpp"
#include "CX/Debug/Profiler.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Debug
{

#if defined(CX_OS_WINDOWS)
_declspec(thread) ThreadProfiler::Scope   *g_pTHScope = NULL;
#else
__thread ThreadProfiler::Scope            *g_pTHScope = NULL;
#endif

ThreadProfiler::Scope *ThreadProfiler::CreateScope(const Char *szScopeName)
{
	Size  cLen;
	Size  cbSize;
	Scope *pScope;

	if (NULL == (pScope = new (std::nothrow) Scope()))
	{
		return NULL;
	}
	cLen  = cx_strlen(szScopeName);
	cbSize = sizeof(Char) * (cLen + 1);
	if (NULL == (pScope->szScopeName = (Char *)Mem::Alloc(cbSize)))
	{
		delete pScope;

		return NULL;
	}
	memcpy(pScope->szScopeName, szScopeName, cbSize);

	pScope->cMinDuration   = 0;
	pScope->cMaxDuration   = 0;
	pScope->cTotalDuration = 0;
	pScope->cCalls         = 0;

	pScope->timer.ResetTimer();

	pScope->pParent        = NULL;
	pScope->pFirstChild    = NULL;
	pScope->pPrevSibling   = NULL;
	pScope->pNextSibling   = NULL;

	return pScope;
}

void ThreadProfiler::DestroyScope(Scope *pScope)
{
	if (NULL != pScope)
	{
		if (NULL != pScope->szScopeName)
		{
			Mem::Free(pScope->szScopeName);
		}
		delete pScope;
	}
}

bool ThreadProfiler::MatchScope(Scope *pScope, const Char *szScopeName)
{
	if (0 != cx_strcmp(pScope->szScopeName, szScopeName))
	{
		return false;
	}

	return true;
}

bool ThreadProfiler::MatchScope(Scope *pScope1, Scope *pScope2)
{
	return MatchScope(pScope1, pScope2->szScopeName);
}

void ThreadProfiler::EnterScope(const Char *szScopeName)
{
	if (!Profiler::Get().GetEnabled())
	{
		return;
	}

	Scope *pPrev  = NULL;
	Scope *pScope = NULL;

	if (NULL != g_pTHScope)
	{
		if (NULL != g_pTHScope->pFirstChild)
		{
			pScope = g_pTHScope->pFirstChild;
			
			pPrev = g_pTHScope->pFirstChild;
			while (NULL != pScope)
			{
				if (MatchScope(pScope, szScopeName))
				{
					break;
				}
				pPrev  = pScope;
				pScope = pScope->pNextSibling;
			}
		}
	}
	if (NULL == pScope)
	{
		if (NULL == (pScope = CreateScope(szScopeName)))
		{
			return;
		}
		if (NULL != pPrev)
		{
			pPrev->pNextSibling  = pScope;
			pScope->pPrevSibling = pPrev;
		}
		else
		if (NULL != g_pTHScope)
		{
			pScope->pPrevSibling    = NULL;
			g_pTHScope->pFirstChild = pScope;
		}
		pScope->pParent = g_pTHScope;
	}
	g_pTHScope     = pScope;
	pScope->timer.ResetTimer();
}

void ThreadProfiler::LeaveScope()
{
	if (!Profiler::Get().GetEnabled())
	{
		return;
	}

	UInt64 nDuration;

	if (NULL == g_pTHScope)
	{
		return;
	}

	nDuration = (UInt64)(g_pTHScope->timer.GetElapsedTime() * 1000.0);

	if (0 == g_pTHScope->cCalls || g_pTHScope->cMinDuration > nDuration)
	{
		g_pTHScope->cMinDuration = nDuration;
	}
	if (0 == g_pTHScope->cCalls || g_pTHScope->cMaxDuration < nDuration)
	{
		g_pTHScope->cMaxDuration = nDuration;
	}
	g_pTHScope->cTotalDuration += nDuration;
	g_pTHScope->cCalls++;

	if (NULL != g_pTHScope->pParent)
	{
		g_pTHScope = g_pTHScope->pParent;
	}
	else
	{
		Profiler::Get().AddScope(g_pTHScope);
		g_pTHScope = NULL;
	}
}

}//namespace Sys

}//namespace CX
