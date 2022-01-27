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
#include "CX/Debug/Profiler.hpp"


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

	m_root.szScopeName              = NULL;

	m_root.timer.ResetTimer();

	m_root.pParent                  = NULL;
	m_root.pFirstChild              = NULL;
	m_root.pNextSibling             = NULL;

	m_pOnDestructProfilingHandler   = NULL;

	m_bEnabled                      = false;
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

void Profiler::SetEnabled(bool bEnabled/* = true*/)
{
	m_bEnabled = bEnabled;
}

bool Profiler::GetEnabled()
{
	return m_bEnabled;
}

void Profiler::SetOnExitProfilingHandler(IProfilingHandler *pProfilingHandler)
{
	Sys::FastWLocker locker(&m_rwlock);

	if (!m_bEnabled)
	{
		return;
	}

	m_pOnDestructProfilingHandler = pProfilingHandler;
}

void Profiler::AddScope(ThreadProfiler::Scope *pScope)
{
	Sys::FastWLocker locker(&m_rwlock);

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
	Sys::FastRLocker              locker(&m_rwlock);
	ThreadProfiler::Scope         *pScope;
	HotSpotsVector                vectorCalls;
	HotSpotsVector                vectorDurations;
	HotSpotsMap                   mapCalls;
	HotSpotsMap                   mapDurations;

	if (!m_bEnabled)
	{
		return false;
	}

	if (!pProfilingHandler->OnBeginProfiling())
	{
		return false;
	}

	Merge(&m_root);
	GetHotSpots(&m_root, vectorCalls, vectorDurations, mapCalls, mapDurations, pProfilingHandler->GetMaxCallHotSpots(), 
	            pProfilingHandler->GetMaxDurationHotSpots());

	pScope = m_root.pFirstChild;
	while (NULL != pScope)
	{
		if (!GetProfiling(pScope, pProfilingHandler, 0))
		{
			return false;
		}
		pScope = pScope->pNextSibling;
	}

	if (!pProfilingHandler->OnBeginCallHotSpots())
	{
		return false;
	}

	for (HotSpotsVector::iterator iter = vectorCalls.begin(); iter != vectorCalls.end(); ++iter)
	{
		if (!pProfilingHandler->OnCallHotSpot(iter->name.szScopeName, 
		                                      iter->cMinDuration, iter->cMaxDuration,
		                                      (UInt64)((double)iter->cTotalDuration / (double)iter->cCalls),
		                                      iter->cTotalDuration, iter->cCalls))
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

	for (HotSpotsVector::iterator iter = vectorDurations.begin(); iter != vectorDurations.end(); ++iter)
	{
		if (!pProfilingHandler->OnDurationHotSpot(iter->name.szScopeName, 
		                                          iter->cMinDuration, iter->cMaxDuration, 
		                                          (UInt64)((double)iter->cTotalDuration / (double)iter->cCalls), 
		                                          iter->cTotalDuration, iter->cCalls))
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

bool Profiler::GetProfiling(ThreadProfiler::Scope *pScope, IProfilingHandler *pProfilingHandler, Size cDepth)
{
	ThreadProfiler::Scope *pTmp;

	if (!pProfilingHandler->OnBeginScope(pScope->szScopeName, 
	                                     pScope->cMinDuration, pScope->cMaxDuration, 
	                                     (UInt64)((double)pScope->cTotalDuration / (double)pScope->cCalls),
	                                     pScope->cTotalDuration, pScope->cCalls, cDepth))
	{
		return false;
	}
	pTmp = pScope->pFirstChild;
	while (NULL != pTmp)
	{
		if (!GetProfiling(pTmp, pProfilingHandler, cDepth + 1))
		{
			return false;
		}
		pTmp = pTmp->pNextSibling;
	}
	if (!pProfilingHandler->OnEndScope(cDepth))
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
	ThreadProfiler::Scope *pTmp;
	Size                  cMerged;

	cInitialCount = 0;
	cMerged       = 0;

	pChild1 = pScope->pFirstChild;
	while (NULL != pChild1)
	{
		cInitialCount++;

		pChild2 = pChild1->pNextSibling;
		while (NULL != pChild2)
		{
			if (ThreadProfiler::MatchScope(pChild1, pChild2))
			{
				cMerged++;

				//merge stats
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

				//remove child2 from list
				if (NULL != pChild2->pPrevSibling)
				{
					pChild2->pPrevSibling->pNextSibling = pChild2->pNextSibling;
				}
				if (NULL != pChild2->pNextSibling)
				{
					pChild2->pNextSibling->pPrevSibling = pChild2->pPrevSibling;
				}

				//add child2 children to child1
				if (NULL != pChild2->pFirstChild)
				{
					if (NULL == pChild1->pFirstChild)
					{
						pChild1->pFirstChild = pChild2->pFirstChild;
					}
					else
					{
						pTmp = pChild1->pFirstChild;
						while (NULL != pTmp->pNextSibling)
						{
							pTmp = pTmp->pNextSibling;
						}
						pTmp->pNextSibling                 = pChild2->pFirstChild;
						pChild2->pFirstChild->pPrevSibling = pTmp;
					}
					pChild2->pFirstChild->pParent = pChild1;
				}

				pTmp  = pChild2->pNextSibling;

				ThreadProfiler::DestroyScope(pChild2);

				pChild2 = pTmp;
			}
			else
			{
				pChild2 = pChild2->pNextSibling;
			}
		}

		pChild1 = pChild1->pNextSibling;
	}

	cFinalCount = cInitialCount - cMerged;
}

void Profiler::GetHotSpots(ThreadProfiler::Scope *pScope, HotSpotsVector &vectorCalls, HotSpotsVector &vectorDurations, 
                           HotSpotsMap &mapCalls, HotSpotsMap &mapDurations, Size cMaxCallHotSpots, 
                           Size cMaxDurationHotSpots)
{
	if (&m_root != pScope)
	{
		HotSpotName hsn;

		hsn.szScopeName = pScope->szScopeName;

		{
			HotSpotsMap::iterator   iterMap = mapCalls.find(hsn);
			HotSpot                 hs;

			if (mapCalls.end() != iterMap && vectorCalls.size() > iterMap->second)
			{
				HotSpotsVector::iterator iterVector = vectorCalls.begin() + iterMap->second;

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
				hs.name           = hsn;
				hs.cCalls         = pScope->cCalls;
				hs.cTotalDuration = pScope->cTotalDuration;
				hs.cMaxDuration   = pScope->cMaxDuration;
				hs.cMinDuration   = pScope->cMinDuration;
			}

			bool bAdded = false;

			for (HotSpotsVector::iterator iter = vectorCalls.begin(); iter != vectorCalls.end(); ++iter)
			{
				if (iter->cCalls < hs.cCalls)
				{
					bAdded = true;
					if (mapCalls.end() != iterMap)
					{
						iterMap->second = iter - vectorCalls.begin();
					}
					else
					{
						mapCalls[hsn]   = iter - vectorCalls.begin();
					}
					vectorCalls.insert(iter, hs);

					break;
				}
			}
			if (!bAdded)
			{
				if (cMaxCallHotSpots > vectorCalls.size())
				{
					vectorCalls.push_back(hs);
					if (mapCalls.end() != iterMap)
					{
						iterMap->second = vectorCalls.size();
					}
					else
					{
						mapCalls[hsn]   = vectorCalls.size();
					}
				}
			}
			if (cMaxCallHotSpots < vectorCalls.size())
			{
				HotSpotsVector::iterator iterVectorCalls = vectorCalls.end();

				--iterVectorCalls;

				auto   iterMapCalls = mapCalls.find(iterVectorCalls->name);
				
				if (mapCalls.end() != iterMapCalls)
				{
					mapCalls.erase(iterMapCalls);
				}
				vectorCalls.erase(iterVectorCalls);
			}
		}

		{
			HotSpotsMap::iterator iterMap = mapDurations.find(hsn);
			HotSpot               hs;

			if (mapDurations.end() != iterMap && vectorDurations.size() > iterMap->second)
			{
				HotSpotsVector::iterator iterVector = vectorDurations.begin() + iterMap->second;

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
				hs.name           = hsn;
				hs.cCalls         = pScope->cCalls;
				hs.cTotalDuration = pScope->cTotalDuration;
				hs.cMaxDuration   = pScope->cMaxDuration;
				hs.cMinDuration   = pScope->cMinDuration;
			}

			bool bAdded = false;

			for (HotSpotsVector::iterator iter = vectorDurations.begin(); iter != vectorDurations.end(); ++iter)
			{
				if (iter->cTotalDuration < hs.cTotalDuration)
				{
					bAdded = true;
					if (mapDurations.end() != iterMap)
					{
						iterMap->second = iter - vectorDurations.begin();
					}
					else
					{
						mapDurations[hsn] = iter - vectorDurations.begin();
					}
					vectorDurations.insert(iter, hs);

					break;
				}
			}
			if (!bAdded)
			{
				if (cMaxDurationHotSpots > vectorDurations.size())
				{
					vectorDurations.push_back(hs);
					if (mapDurations.end() != iterMap)
					{
						iterMap->second   = vectorDurations.size();
					}
					else
					{
						mapDurations[hsn] = vectorDurations.size();
					}
				}
			}
			if (cMaxDurationHotSpots < vectorDurations.size())
			{
				HotSpotsVector::iterator iterVectorDurations = vectorDurations.end();

				--iterVectorDurations;

				auto iterMapDurations = mapDurations.find(iterVectorDurations->name);

				if (mapDurations.end() != iterMapDurations)
				{
					mapDurations.erase(iterMapDurations);
				}
				vectorDurations.erase(iterVectorDurations);
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
