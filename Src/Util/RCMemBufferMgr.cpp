/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Util/RCMemBufferMgr.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Util
{

RCMemBufferMgr::RCMemBufferMgr()
{
	m_cbAllocsSize = 0;
	m_mapMemBuffers.set_deleted_key(NULL);
}

RCMemBufferMgr::~RCMemBufferMgr()
{
	Cleanup();
}

RCMemBufferMgr &RCMemBufferMgr::Get()
{
	static RCMemBufferMgr mgr;

	return mgr;
}

void *RCMemBufferMgr::Alloc(Size cbSize)
{
	void      *pMem;
	MemBuffer mb;

	if (NULL == (pMem = Mem::Alloc(cbSize)))
	{
		return NULL;
	}
	mb.cbSize             = cbSize;
	mb.cRefCount          = 1;
	m_mapMemBuffers[pMem] = mb;
	m_cbAllocsSize += cbSize;

	return pMem;
}

void RCMemBufferMgr::Retain(void *pMem)
{
	if (NULL == pMem)
	{
		return;
	}

	MemBuffersMap::iterator iter;

	if (m_mapMemBuffers.end() == (iter = m_mapMemBuffers.find(pMem)))
	{
		return;
	}
	iter->second.cRefCount++;
}

void RCMemBufferMgr::Release(void *pMem)
{
	if (NULL == pMem)
	{
		return;
	}

	MemBuffersMap::iterator iter;

	if (m_mapMemBuffers.end() == (iter = m_mapMemBuffers.find(pMem)))
	{
		return;
	}
	iter->second.cRefCount--;
	if (0 == iter->second.cRefCount)
	{
		m_cbAllocsSize -= iter->second.cbSize;
		Mem::Free(iter->first);
		m_mapMemBuffers.erase(iter);
	}
}

//will free all mem (even if used)
void RCMemBufferMgr::Cleanup()
{
	for (MemBuffersMap::const_iterator iter = m_mapMemBuffers.begin(); iter != m_mapMemBuffers.end(); ++iter)
	{
		Mem::Free(iter->first);
	}
	m_mapMemBuffers.clear();
}

Size RCMemBufferMgr::GetAllocsSize() const
{
	return m_cbAllocsSize;
}

}//namespace Util

}//namespace CX
