/* 
 * CX - C++ framework for general purpose developement
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
#include "CX/Sys/Lock.hpp"


namespace CX
{

#ifdef CX_DEBUG
unsigned int Mem::m_nFlags = Mem::Flag_SimpleMemTrack;
#else
unsigned int Mem::m_nFlags = Mem::Flag_None;
#endif

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
		Sys::Locker    locker(GetLock());
		MemAllocInfo   info;
		
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

void Mem::GetCurrentAllocs(AllocsVector &vectorAllocs)
{
	Sys::Locker locker(GetLock());

	vectorAllocs.clear();

	for (AllocsMap::const_iterator iter = (*GetAllocs()).begin(); iter != (*GetAllocs()).end(); ++iter)
	{
		AllocData data;

		data.pMem        = iter->first;
		data.cbSize      = iter->second.cbSize;

		for (Util::StackTrace::CallsVector::const_iterator iterCalls = iter->second.vectorCalls.begin(); 
		     iterCalls != iter->second.vectorCalls.end(); ++iterCalls)
		{
			Call call;

			call.sFunction = iterCalls->sFunction;
			call.sFile     = iterCalls->sFile;
			call.cLine     = iterCalls->cLine;

			data.vectorCalls.push_back(call);
		}
		vectorAllocs.push_back(data);
	}
}

}//namespace CX

