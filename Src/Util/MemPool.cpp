
#include "CX/Util/MemPool.h"
#include "CX/Status.h"


namespace CX
{

namespace Util
{

DynMemPool::DynMemPool(Size cbMaxSize/* = SIZET_MAX*/)
{
	Status::Clear();

	m_pMem			= NULL;
	m_cbSize			= 0;
	m_cbTotalSize	= 0;
	m_cbMaxSize		= cbMaxSize;
}

DynMemPool::~DynMemPool()
{
	Status::Clear();

	if (NULL != m_pMem)
	{
		Free(m_pMem);
	}
}

void *DynMemPool::GetMem()
{
	Status::Clear();

	return m_pMem;
}

const void *DynMemPool::GetMem() const
{
	Status::Clear();

	return m_pMem;
}

Size DynMemPool::GetSize() const
{
	Status::Clear();

	return m_cbSize;
}

Size DynMemPool::GetTotalSize() const
{
	Status::Clear();

	return m_cbTotalSize;
}

Size DynMemPool::GetMaxSize() const
{
	Status::Clear();

	return m_cbMaxSize;
}

StatusCode DynMemPool::SetSize(Size cbSize)
{
	Status::Clear();

	if (cbSize <= m_cbTotalSize)
	{
		m_cbSize = cbSize;

		return Status_OK;
	}

	if (cbSize > m_cbMaxSize)
	{
		return Status::Set(Status_TooBig, "Dynamic mem pool max size will be exceeded");
	}

	Size cbFinalSize;

	cbFinalSize = (cbSize / 4096) * 4096;

	if (0 < cbSize % 4096)
	{
		cbFinalSize += 4096;
	}

	void *pFinalMem;

	if (NULL == (pFinalMem = Realloc(m_pMem, cbFinalSize)))
	{
		return Status::Set(Status_MemAllocFailed, "Failed to allocate memory");
	}

	m_pMem			= pFinalMem;
	m_cbSize			= cbSize;
	m_cbTotalSize	= cbFinalSize;

	return Status_OK;
}

StatusCode DynMemPool::Add(Size cbSize)
{
	return SetSize(m_cbSize + cbSize);
}

StatusCode DynMemPool::Add(const void *pMem, Size cbSize)
{
	Status::Clear();

	Size cbOldSize = m_cbSize;

	if (CXNOK(Add(cbSize)))
	{
		return Status::GetCode();
	}
	memcpy((Byte *)m_pMem + cbOldSize, pMem, cbSize);

	return Status_OK;
}

StaticMemPool::StaticMemPool(void *pMem, Size cbSize)
{
	Status::Clear();

	m_pMem			= pMem;
	m_cbSize			= 0;
	m_cbTotalSize	= cbSize;
}

StaticMemPool::~StaticMemPool()
{
	Status::Clear();
}

void *StaticMemPool::GetMem()
{
	Status::Clear();

	return m_pMem;
}

const void *StaticMemPool::GetMem() const
{
	Status::Clear();

	return m_pMem;
}

Size StaticMemPool::GetSize() const
{
	Status::Clear();

	return m_cbSize;
}

Size StaticMemPool::GetTotalSize() const
{
	Status::Clear();

	return m_cbTotalSize;
}

Size StaticMemPool::GetMaxSize() const
{
	Status::Clear();

	return m_cbTotalSize;
}

StatusCode StaticMemPool::SetSize(Size cbSize)
{
	Status::Clear();

	if (cbSize > m_cbTotalSize)
	{
		return Status::Set(Status_TooBig, "Static mem pool max size will be exceeded");
	}
	m_cbSize = cbSize;

	return Status_OK;
}

StatusCode StaticMemPool::Add(Size cbSize)
{
	return SetSize(m_cbSize + cbSize);
}

StatusCode StaticMemPool::Add(const void *pMem, Size cbSize)
{
	Status::Clear();

	Size cbOldSize = m_cbSize;

	if (CXNOK(Add(cbSize)))
	{
		return Status::GetCode();
	}
	memcpy((Byte *)m_pMem + cbOldSize, pMem, cbSize);
	
	return Status_OK;
}

}//namespace Util

}//namespace CX
