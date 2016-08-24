/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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

#include "CX/Util/MemPool.hpp"
#include "CX/Status.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Util
{

DynMemPool::DynMemPool(Size cbMaxSize/* = SIZET_MAX*/)
{
	m_pMem         = NULL;
	m_cbSize       = 0;
	m_cbTotalSize  = 0;
	m_cbMaxSize    = cbMaxSize;
}

DynMemPool::~DynMemPool()
{
	if (NULL != m_pMem)
	{
		Mem::Free(m_pMem);
	}
}

void *DynMemPool::GetMem()
{
	return m_pMem;
}

const void *DynMemPool::GetMem() const
{
	return m_pMem;
}

Size DynMemPool::GetSize() const
{
	return m_cbSize;
}

Size DynMemPool::GetTotalSize() const
{
	return m_cbTotalSize;
}

Size DynMemPool::GetMaxSize() const
{
	return m_cbMaxSize;
}

Status DynMemPool::SetSize(Size cbSize)
{
	if (cbSize <= m_cbTotalSize)
	{
		m_cbSize = cbSize;

		return Status();
	}

	if (cbSize > m_cbMaxSize)
	{
		return Status(Status_TooBig, "Dynamic mem pool max size will be exceeded");
	}

	Size cbFinalSize;

	cbFinalSize = (cbSize / 4096) * 4096;

	if (0 < cbSize % 4096)
	{
		cbFinalSize += 4096;
	}

	void *pFinalMem;

	if (NULL == (pFinalMem = Mem::Realloc(m_pMem, cbFinalSize)))
	{
		return Status(Status_MemAllocFailed, "Failed to allocate memory");
	}

	m_pMem         = pFinalMem;
	m_cbSize       = cbSize;
	m_cbTotalSize  = cbFinalSize;

	return Status();
}

Status DynMemPool::Add(Size cbSize)
{
	return SetSize(m_cbSize + cbSize);
}

Status DynMemPool::Add(const void *pMem, Size cbSize)
{
	Size   cbOldSize = m_cbSize;
	Status status;

	status = Add(cbSize);
	if (status.IsNOK())
	{
		return status;
	}
	memcpy((Byte *)m_pMem + cbOldSize, pMem, cbSize);

	return Status();
}

StaticMemPool::StaticMemPool(void *pMem, Size cbSize)
{
	m_pMem         = pMem;
	m_cbSize       = 0;
	m_cbTotalSize  = cbSize;
}

StaticMemPool::~StaticMemPool()
{
}

void *StaticMemPool::GetMem()
{
	return m_pMem;
}

const void *StaticMemPool::GetMem() const
{
	return m_pMem;
}

Size StaticMemPool::GetSize() const
{
	return m_cbSize;
}

Size StaticMemPool::GetTotalSize() const
{
	return m_cbTotalSize;
}

Size StaticMemPool::GetMaxSize() const
{
	return m_cbTotalSize;
}

Status StaticMemPool::SetSize(Size cbSize)
{
	if (cbSize > m_cbTotalSize)
	{
		return Status(Status_TooBig, "Static mem pool max size will be exceeded");
	}
	m_cbSize = cbSize;

	return Status();
}

Status StaticMemPool::Add(Size cbSize)
{
	return SetSize(m_cbSize + cbSize);
}

Status StaticMemPool::Add(const void *pMem, Size cbSize)
{
	Size   cbOldSize = m_cbSize;
	Status status;

	status = Add(cbSize);
	if (status.IsNOK())
	{
		return status;
	}
	memcpy((Byte *)m_pMem + cbOldSize, pMem, cbSize);
	
	return Status();
}

}//namespace Util

}//namespace CX
