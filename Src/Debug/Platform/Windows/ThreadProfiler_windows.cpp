
#include "CX/Debug/ThreadProfiler.hpp"
#include "CX/Debug/Profiler.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Mem.hpp"


namespace CX
{

namespace Debug
{

_declspec(thread) ThreadProfiler::Scope *g_pTHScope = NULL;

ThreadProfiler::Scope *ThreadProfiler::CreateScope(const Char *szFileName, const Char *szScopeName, int cLineNo)
{
	Size  cLen;
	Size  cbSize;
	Scope *pScope;

	if (NULL == (pScope = (Scope *)Mem::Alloc(sizeof(Scope))))
	{
		return NULL;
	}
	cLen   = cx_strlen(szFileName);
	cbSize = sizeof(Char) * (cLen + 1);
	if (NULL == (pScope->szFileName = (Char *)Mem::Alloc(cbSize)))
	{
		Mem::Free(pScope);

		return NULL;
	}
	memcpy(pScope->szFileName, szFileName, cbSize);
	cLen  = cx_strlen(szScopeName);
	cbSize = sizeof(Char) * (cLen + 1);
	if (NULL == (pScope->szScopeName = (Char *)Mem::Alloc(cbSize)))
	{
		Mem::Free(pScope->szFileName);
		Mem::Free(pScope);

		return NULL;
	}
	memcpy(pScope->szScopeName, szScopeName, cbSize);

	pScope->cLineNo        = cLineNo;

	pScope->cMinDuration   = 0;
	pScope->cMaxDuration   = 0;
	pScope->cTotalDuration = 0;
	pScope->cCalls         = 0;

	pScope->nStart         = 0;

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
		if (NULL != pScope->szFileName)
		{
			Mem::Free(pScope->szFileName);
		}
		if (NULL != pScope->szScopeName)
		{
			Mem::Free(pScope->szScopeName);
		}
		Mem::Free(pScope);
	}
}

bool ThreadProfiler::MatchScope(Scope *pScope, const Char *szFileName, const Char *szScopeName, int cLineNo)
{
	if (pScope->cLineNo != cLineNo)
	{
		return false;
	}
	if (0 != cx_strcmp(pScope->szFileName, szFileName))
	{
		return false;
	}
	if (0 != cx_strcmp(pScope->szScopeName, szScopeName))
	{
		return false;
	}

	return true;
}

bool ThreadProfiler::MatchScope(Scope *pScope1, Scope *pScope2)
{
	return MatchScope(pScope1, pScope2->szFileName, pScope2->szScopeName, pScope2->cLineNo);
}

UInt64 ThreadProfiler::GetTimeStamp()
{
	LARGE_INTEGER li;

	QueryPerformanceCounter(&li);

	return (UInt64)li.QuadPart;
}

void ThreadProfiler::EnterScope(const Char *szFileName, const Char *szScopeName, int cLineNo)
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
				if (MatchScope(pScope, szFileName, szScopeName, cLineNo))
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
		if (NULL == (pScope = CreateScope(szFileName, szScopeName, cLineNo)))
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
	pScope->nStart = GetTimeStamp();
}

void ThreadProfiler::LeaveScope()
{
	if (!Profiler::Get().GetEnabled())
	{
		return;
	}

	UInt64 nTimestamp = GetTimeStamp();
	UInt64 nDuration;

	if (NULL == g_pTHScope)
	{
		return;
	}

	nDuration = nTimestamp - g_pTHScope->nStart;

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
