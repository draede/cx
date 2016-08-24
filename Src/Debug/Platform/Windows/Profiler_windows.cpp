
#include "CX/Debug/Profiler.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Debug
{

Profiler::Profiler()
{
	m_root.cCalls                   = 0;
	m_root.cMinDuration             = 0;
	m_root.cMaxDuration             = 0;
	m_root.cTotalDuration           = 0;

	m_root.szFileName               = "";
	m_root.szScopeName              = "";
	m_root.cLineNo                  = 0;

	m_root.nStart                   = 0;

	m_root.pParent                  = NULL;
	m_root.pFirstChild              = NULL;
	m_root.pNextSibling             = NULL;

	m_pOnDestructProfilingHandler   = NULL;
}

Profiler::~Profiler()
{
	if (NULL != m_pOnDestructProfilingHandler)
	{
		GetProfiling(m_pOnDestructProfilingHandler);
		delete m_pOnDestructProfilingHandler;
	}

	ThreadProfiler::Scope *pScope = m_root.pFirstChild;
	ThreadProfiler::Scope *pTmp;

	while (NULL != pScope)
	{
		pTmp   = pScope->pNextSibling;
		FreeScopeMem(pScope);
		pScope = pTmp;
	}
	m_root.pFirstChild = NULL;
}

void Profiler::FreeScopeMem(ThreadProfiler::Scope *pScope)
{
	ThreadProfiler::Scope *pTmp;
	ThreadProfiler::Scope *pTmp2;

	pTmp = pScope->pFirstChild;
	while (NULL != pTmp)
	{
		pTmp2 = pTmp->pNextSibling;
		FreeScopeMem(pTmp);
		pTmp  = pTmp2;
	}
	ThreadProfiler::DestroyScope(pScope);
}

Profiler &Profiler::Get()
{
	static Profiler profiler;

	return profiler;
}

void Profiler::SetOnExitProfilingHandler(IProfilingHandler *pProfilingHandler)
{
	Sys::Locker locker(&m_lock);

	m_pOnDestructProfilingHandler = pProfilingHandler;
}

void Profiler::AddScope(ThreadProfiler::Scope *pScope)
{
	Sys::Locker locker(&m_lock);

	pScope->pParent      = &m_root;
	pScope->pNextSibling = NULL;
	if (NULL != m_root.pFirstChild)
	{
		ThreadProfiler::Scope *pTmp = m_root.pFirstChild;

		while (NULL != pTmp->pNextSibling)
		{
			pTmp = pTmp->pNextSibling;
		}
		pTmp->pNextSibling   = pScope;
		pScope->pPrevSibling = pTmp;
	}
	else
	{
		m_root.pFirstChild = pScope;
	}
}

bool Profiler::GetProfiling(IProfilingHandler *pProfilingHandler)
{
	Sys::Locker                   locker(&m_lock);
	ThreadProfiler::Scope         *pScope;
	LARGE_INTEGER                 li;
	double                        lfRes;
	HotSpotsVector                vectorCalls;
	HotSpotsVector                vectorDurations;
	HotSpotsMap                   mapCalls;
	HotSpotsMap                   mapDurations;
	IProfilingHandler::Resolution nRes = pProfilingHandler->GetResolution();

	if (!pProfilingHandler->OnBeginProfiling())
	{
		return false;
	}

	Merge(&m_root);
	GetHotSpots(&m_root, vectorCalls, vectorDurations, mapCalls, mapDurations, pProfilingHandler->GetMaxCallHotSpots(), 
	            pProfilingHandler->GetMaxDurationHotSpots());

	QueryPerformanceFrequency(&li);
	if (IProfilingHandler::Resolution_Seconds == nRes)
	{
		lfRes = (double)li.QuadPart;
	}
	else
	if (IProfilingHandler::Resolution_MilliSeconds == nRes)
	{
		lfRes = li.QuadPart / 1000.0;
	}
	else
	if (IProfilingHandler::Resolution_MicroSeconds == nRes)
	{
		lfRes = li.QuadPart / 1000000.0;
	}
	else
	{
		lfRes = li.QuadPart / 1000000000.0;
	}

	pScope = m_root.pFirstChild;
	while (NULL != pScope)
	{
		if (!GetProfiling(pScope, pProfilingHandler, lfRes, true))
		{
			return false;
		}
		pScope = pScope->pNextSibling;
	}

	if (!pProfilingHandler->OnBeginCallHotSpots())
	{
		return false;
	}

	for (auto iter = vectorCalls.begin(); iter != vectorCalls.end(); ++iter)
	{
		UInt64 cMin   = (UInt64)(iter->cMinDuration / lfRes);
		UInt64 cMax   = (UInt64)(iter->cMaxDuration / lfRes);
		UInt64 cTotal = (UInt64)(iter->cTotalDuration / lfRes);
		UInt64 cAvg   = (UInt64)(((double)iter->cTotalDuration / (double)iter->cCalls) / lfRes);

		if (!pProfilingHandler->OnCallHotSpot(iter->name.szFileName, iter->name.szScopeName, iter->name.cLineNo, cMin, 
		                                      cMax, cAvg, cTotal, iter->cCalls))
		{

			return false;
		}
	}

	if (!pProfilingHandler->OnEndCallHotSpots())
	{
		return false;
	}

	if (!pProfilingHandler->OnBeginDurationHotSpots())
	{
		return false;
	}

	for (auto iter = vectorDurations.begin(); iter != vectorDurations.end(); ++iter)
	{
		UInt64 cMin   = (UInt64)(iter->cMinDuration / lfRes);
		UInt64 cMax   = (UInt64)(iter->cMaxDuration / lfRes);
		UInt64 cTotal = (UInt64)(iter->cTotalDuration / lfRes);
		UInt64 cAvg   = (UInt64)(((double)iter->cTotalDuration / (double)iter->cCalls) / lfRes);

		if (!pProfilingHandler->OnDurationHotSpot(iter->name.szFileName, iter->name.szScopeName, iter->name.cLineNo, 
		                                          cMin, cMax, cAvg, cTotal, iter->cCalls))
		{

			return false;
		}
	}

	if (!pProfilingHandler->OnEndDurationHotSpots())
	{
		return false;
	}

	if (!pProfilingHandler->OnEndProfiling())
	{
		return false;
	}

	return true;
}

bool Profiler::GetProfiling(ThreadProfiler::Scope *pScope, IProfilingHandler *pProfilingHandler, double lfRes, 
                            bool bRootScope)
{
	UInt64                cMin   = (UInt64)(pScope->cMinDuration / lfRes);
	UInt64                cMax   = (UInt64)(pScope->cMaxDuration / lfRes);
	UInt64                cTotal = (UInt64)(pScope->cTotalDuration / lfRes);
	UInt64                cAvg   = (UInt64)(((double)pScope->cTotalDuration / (double)pScope->cCalls) / lfRes);
	ThreadProfiler::Scope *pTmp;

	if (!pProfilingHandler->OnBeginScope(pScope->szFileName, pScope->szScopeName, pScope->cLineNo, cMin, cMax, cAvg, 
	                                     cTotal, pScope->cCalls, bRootScope))
	{
		return false;
	}
	pTmp = pScope->pFirstChild;
	while (NULL != pTmp)
	{
		if (!GetProfiling(pTmp, pProfilingHandler, lfRes, false))
		{
			return false;
		}
		pTmp = pTmp->pNextSibling;
	}
	if (!pProfilingHandler->OnEndScope(bRootScope))
	{
		return false;
	}

	return true;
}

void Profiler::Merge(ThreadProfiler::Scope *pScope)
{
	Size cInitialCount;
	Size cFinalCount;

	MergeChildren(pScope, cInitialCount, cFinalCount);
	if (cInitialCount != cFinalCount)
	{
		ThreadProfiler::Scope *pTmp;

		pTmp = pScope->pFirstChild;
		while (NULL != pTmp)
		{
			Merge(pTmp);
			pTmp = pTmp->pNextSibling;
		}
	}
}

void Profiler::MergeChildren(ThreadProfiler::Scope *pScope, Size &cInitialCount, Size &cFinalCount)
{
	ThreadProfiler::Scope *pChild1;
	ThreadProfiler::Scope *pChild2;
	ThreadProfiler::Scope *pLastChild;
	ThreadProfiler::Scope *pTmp;

	cInitialCount = 0;
	cFinalCount   = 0;
	pChild1       = pScope->pFirstChild;
	while (NULL != pChild1)
	{
		cInitialCount++;
		pLastChild = pChild1->pFirstChild;
		if (NULL != pLastChild)
		{
			while (NULL != pLastChild->pNextSibling)
			{
				pLastChild = pLastChild->pNextSibling;
			}
		}

		pChild2 = pChild1->pNextSibling;
		while (NULL != pChild2)
		{
			pTmp = pChild2->pNextSibling;

			if (ThreadProfiler::MatchScope(pChild1, pChild2))
			{
				cFinalCount++;

				pChild1->cCalls += pChild2->cCalls;
				pChild1->cTotalDuration += pChild2->cTotalDuration;
				if (pChild1->cMinDuration > pChild2->cMinDuration)
				{
					pChild1->cMinDuration = pChild2->cMinDuration;
				}
				if (pChild1->cMaxDuration < pChild2->cMaxDuration)
				{
					pChild1->cMaxDuration = pChild2->cMaxDuration;
				}
				
				if (NULL != pChild2->pPrevSibling)
				{
					pChild2->pPrevSibling->pNextSibling = pChild2->pNextSibling;
				}
				if (NULL != pChild2->pNextSibling)
				{
					pChild2->pNextSibling->pPrevSibling = pChild2->pPrevSibling;
				}

				pTmp = pChild2->pFirstChild;
				while (NULL != pTmp)
				{
					if (NULL == pLastChild)
					{
						pChild1->pFirstChild = pTmp;
						pTmp->pPrevSibling   = NULL;
					}
					else
					{
						pLastChild->pNextSibling = pTmp;
						pTmp->pPrevSibling       = pLastChild;
					}
					pLastChild         = pTmp;
					pTmp->pNextSibling = NULL;
					pTmp->pParent      = pTmp;
					pTmp               = pTmp->pNextSibling;
				}

				ThreadProfiler::DestroyScope(pChild2);
			}

			pChild2 = pTmp;
		}

		pChild1 = pChild1->pNextSibling;
	}
	cFinalCount = cInitialCount - cFinalCount;
}

void Profiler::GetHotSpots(ThreadProfiler::Scope *pScope, HotSpotsVector &vectorCalls, HotSpotsVector &vectorDurations, 
                           HotSpotsMap &mapCalls, HotSpotsMap &mapDurations, Size cMaxCallHotSpots, 
                           Size cMaxDurationHotSpots)
{
	if (&m_root != pScope)
	{
		HotSpotName hsn;

		hsn.szFileName = pScope->szFileName;
		hsn.szScopeName = pScope->szScopeName;
		hsn.cLineNo = pScope->cLineNo;

		{
			auto    iterMap = mapCalls.find(hsn);
			HotSpot hs;

			if (mapCalls.end() != iterMap)
			{
				auto iterVector = vectorCalls.begin() + iterMap->second;

				if (iterVector->cMinDuration > pScope->cMinDuration)
				{
					iterVector->cMinDuration = pScope->cMinDuration;
				}
				if (iterVector->cMaxDuration < pScope->cMaxDuration)
				{
					iterVector->cMaxDuration = pScope->cMaxDuration;
				}
				iterVector->cCalls += pScope->cCalls;
				iterVector->cTotalDuration += pScope->cTotalDuration;

				hs = *iterVector;

				vectorCalls.erase(iterVector);
			}
			else
			{
				hs.name = hsn;
				hs.cCalls = pScope->cCalls;
				hs.cTotalDuration = pScope->cTotalDuration;
				hs.cMaxDuration = pScope->cMaxDuration;
				hs.cMinDuration = pScope->cMinDuration;
			}

			bool bAdded = false;

			for (auto iter = vectorCalls.begin(); iter != vectorCalls.end(); ++iter)
			{
				if (iter->cCalls < hs.cCalls)
				{
					bAdded = true;
					vectorCalls.insert(iter, hs);

					break;
				}
			}
			if (bAdded)
			{
				if (cMaxCallHotSpots < vectorCalls.size())
				{
					auto iter = vectorCalls.end();

					iter--;
					vectorCalls.erase(iter);
				}
			}
			else
			{
				if (cMaxCallHotSpots > vectorCalls.size())
				{
					vectorCalls.push_back(hs);
				}
			}
		}

		{
			auto    iterMap = mapDurations.find(hsn);
			HotSpot hs;

			if (mapDurations.end() != iterMap)
			{
				auto iterVector = vectorDurations.begin() + iterMap->second;

				if (iterVector->cMinDuration > pScope->cMinDuration)
				{
					iterVector->cMinDuration = pScope->cMinDuration;
				}
				if (iterVector->cMaxDuration < pScope->cMaxDuration)
				{
					iterVector->cMaxDuration = pScope->cMaxDuration;
				}
				iterVector->cCalls += pScope->cCalls;
				iterVector->cTotalDuration += pScope->cTotalDuration;

				hs = *iterVector;

				vectorDurations.erase(iterVector);
			}
			else
			{
				hs.name = hsn;
				hs.cCalls = pScope->cCalls;
				hs.cTotalDuration = pScope->cTotalDuration;
				hs.cMaxDuration = pScope->cMaxDuration;
				hs.cMinDuration = pScope->cMinDuration;
			}

			bool bAdded = false;

			for (auto iter = vectorDurations.begin(); iter != vectorDurations.end(); ++iter)
			{
				if (iter->cTotalDuration < hs.cTotalDuration)
				{
					bAdded = true;
					vectorDurations.insert(iter, hs);

					break;
				}
			}
			if (bAdded)
			{
				if (cMaxDurationHotSpots < vectorDurations.size())
				{
					auto iter = vectorDurations.end();

					iter--;
					vectorDurations.erase(iter);
				}
			}
			else
			{
				if (cMaxDurationHotSpots > vectorDurations.size())
				{
					vectorDurations.push_back(hs);
				}
			}
		}
	}

	ThreadProfiler::Scope *pTmp;

	pTmp = pScope->pFirstChild;
	while (NULL != pTmp)
	{
		GetHotSpots(pTmp, vectorCalls, vectorDurations, mapCalls, mapDurations, cMaxCallHotSpots, cMaxDurationHotSpots);
		pTmp = pTmp->pNextSibling;
	}
}

}//namespace Debug

}//namespace CX
