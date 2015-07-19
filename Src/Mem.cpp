/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/Mem.hpp"
#include "CX/C/Alloc.h"
#include "CX/Util/StackTrace.hpp"
#include "CX/Platform.hpp"
#include "CX/Print.hpp"
#include "CX/Sys/Lock.hpp"


namespace CX
{

#ifdef CX_DEBUG
unsigned int Mem::m_nFlags = Mem::Flag_SimpleMemTrack;
#else
unsigned int Mem::m_nFlags = Mem::Flag_None;
#endif

Mem::IgnoredAllocsSet   Mem::m_setIgnoredAllocs;
bool                    Mem::m_bInitIgnoredAllocs = false;

class MemAllocInfo
{
public:

	Size                            cbSize;
	Util::StackTrace::CallsVector   vectorCalls;

};

typedef std::map<void *, MemAllocInfo>   AllocsMap;

Mem::Mem()
{
}

Mem::~Mem()
{
}

Sys::Lock *Mem::GetLock()
{
	static Sys::Lock   lockAllocs;

	return &lockAllocs;
}

std::map<void *, MemAllocInfo> *Mem::GetAllocs()
{
	static AllocsMap mapAllocs;

	return &mapAllocs;
}

void *Mem::Alloc(Size cbSize)
{
	void *pPtr = CX_Alloc(cbSize);

	if (NULL == pPtr)
	{
		return NULL;
	}

	if ((Flag_SourceMemTrack == m_nFlags))
	{
		Sys::Locker                locker(GetLock());
		MemAllocInfo               info;

		info.cbSize = cbSize;
		Util::StackTrace::GetStackTrace(info.vectorCalls);
		
		(*GetAllocs())[pPtr] = info;
	}
	else
	if ((Flag_SimpleMemTrack == m_nFlags))
	{
		Sys::Locker    locker(GetLock());
		MemAllocInfo   info;
		
		info.cbSize = cbSize;
		
		(*GetAllocs())[pPtr] = info;
	}

	return pPtr;
}

void *Mem::Realloc(void *pPtr, Size cbSize)
{
	if (NULL == pPtr)
	{
		return Alloc(cbSize);
	}

	void *pNewPtr = CX_Realloc(pPtr, cbSize);

	if (NULL == pNewPtr)
	{
		return NULL;
	}

	if ((Flag_SimpleMemTrack == m_nFlags) || (Flag_SourceMemTrack == m_nFlags))
	{
		Sys::Locker         locker(GetLock());
		AllocsMap::iterator iter = (*GetAllocs()).find(pPtr);

		if ((*GetAllocs()).end() != iter)
		{
			(*GetAllocs()).erase(iter);
		}
	}

	if ((Flag_SourceMemTrack == m_nFlags))
	{
		Sys::Locker    locker(GetLock());
		MemAllocInfo   info;
		
		info.cbSize = cbSize;
		Util::StackTrace::GetStackTrace(info.vectorCalls);
		
		(*GetAllocs())[pNewPtr] = info;
	}
	else
	if ((Flag_SimpleMemTrack == m_nFlags))
	{
		Sys::Locker    locker(GetLock());
		MemAllocInfo   info;
		
		info.cbSize = cbSize;
		
		(*GetAllocs())[pNewPtr] = info;
	}

	return pNewPtr;
}

void Mem::Free(void *pPtr)
{
	if (NULL == pPtr)
	{
		return;
	}

	if ((Flag_SimpleMemTrack == m_nFlags) || (Flag_SourceMemTrack == m_nFlags))
	{
		Sys::Locker         locker(GetLock());
		AllocsMap::iterator iter = (*GetAllocs()).find(pPtr);

		if ((*GetAllocs()).end() != iter)
		{
			(*GetAllocs()).erase(iter);
		}
	}

	CX_Free(pPtr);
}

void Mem::SetFlags(unsigned int nFlags)
{
	m_nFlags = nFlags;
}

unsigned int Mem::GetFlags()
{
	return m_nFlags;
}

void Mem::IgnoreAllocsFromFunc(const Char *szFunction)
{
	Sys::Locker locker(GetLock());

	m_setIgnoredAllocs.insert(szFunction);
}

void Mem::AddIgnoredAllocs()
{
	IgnoreAllocsFromFunc("CX::Log::Logger::GetDefaultLogger");
	IgnoreAllocsFromFunc("CX::Log::Logger::AddOutput");
}

void Mem::GetCurrentAllocs(AllocsVector &vectorAllocs)
{
	Sys::Locker locker(GetLock());

	if (!m_bInitIgnoredAllocs)
	{
		m_bInitIgnoredAllocs = true;
		AddIgnoredAllocs();
	}

	vectorAllocs.clear();

	for (AllocsMap::const_iterator iter = (*GetAllocs()).begin(); iter != (*GetAllocs()).end(); ++iter)
	{
		AllocData data;

		data.pMem        = iter->first;
		data.cbSize      = iter->second.cbSize;

		bool bAdd = true;

		for (Util::StackTrace::CallsVector::const_iterator iterCalls = iter->second.vectorCalls.begin(); 
		     iterCalls != iter->second.vectorCalls.end(); ++iterCalls)
		{
			IgnoredAllocsSet::iterator iterIgnoredAllocs;

			if (m_setIgnoredAllocs.end() != (iterIgnoredAllocs = m_setIgnoredAllocs.find(iterCalls->sFunction)))
			{
				bAdd = false;
				
				break;
			}

			Call call;

			call.sFunction = iterCalls->sFunction;
			call.sFile     = iterCalls->sFile;
			call.cLine     = iterCalls->cLine;

			data.vectorCalls.push_back(call);
		}

		if (bAdd)
		{
			vectorAllocs.push_back(data);
		}
	}
}

void Mem::PrintAllocs(std::string &sOut, const AllocsVector &vectorAllocs)
{
	if (vectorAllocs.empty())
	{
		Print(&sOut, "No allocations found at this moment!\n");
		return;
	}

	bool bFirst = true;

	for (AllocsVector::const_iterator iter = vectorAllocs.begin(); iter != vectorAllocs.end(); ++iter)
	{
		static const Char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		if (!bFirst)
		{
			Print(&sOut, "\n");
		}
		else
		{
			bFirst = false;
		}

#ifdef CX_64BIT_ARCH
		Print(&sOut, "Detected {1} bytes at 0x{2:'0'16}\n", iter->cbSize, (void *)iter->pMem);
#else
		Print(&sOut, "Detected {1} bytes at 0x{2:'0'8}\n", iter->cbSize, (void *)iter->pMem);
#endif

		std::string   sContent;
		Size          cIndex = 0;
		const Byte    *pPos = (const Byte *)iter->pMem;

		while (cIndex < iter->cbSize)
		{
			if (!sContent.empty())
			{
				sContent += "\n";
			}

			sContent += "[";

			for (Size i = 0; i < 16; i++)
			{
				if (cIndex + i < iter->cbSize)
				{
					if (32 > *(pPos + i) || 127 < *(pPos + i))
					{
						sContent += ".";
					}
					else
					{
						sContent += *(pPos + i);
					}
				}
				else
				{
					sContent += " ";
				}
			}

			sContent += "] [";

			for (Size i = 0; i < 16; i++)
			{
				if (cIndex + i < iter->cbSize)
				{
					if (0 < i)
					{
						sContent += " ";
					}
					sContent += hexdigits[(*(pPos + i)) / 16];
					sContent += hexdigits[(*(pPos + i)) % 16];
				}
				else
				{
					if (0 < i)
					{
						sContent += " ";
					}
					sContent += "  ";
				}
			}

			sContent += "]";

			cIndex += 16;
			pPos += 16;
		}

		Print(&sOut, "{1}\n", sContent);

		Size cCount = iter->vectorCalls.size();

		if (0 == cCount)
		{
			Print(&sOut, "<no stack trace available>\n");

			return;
		}
		for (Size i = 0; i < cCount; i++)
		{
			Print(&sOut, "[{1}]: {2} - {3}:{4}\n", cCount - i - 1,
			      iter->vectorCalls[i].sFunction, iter->vectorCalls[i].sFile, iter->vectorCalls[i].cLine);
		}
	}
}

}//namespace CX

