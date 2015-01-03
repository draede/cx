
#pragma once


#include "CX/Types.h"
#include "CX/Limits.h"
#include "CX/String.h"


namespace CX
{

namespace Util
{

class IMemPool
{
public:

	virtual ~IMemPool() { }

	virtual void *GetMem() = 0;

	virtual const void *GetMem() const = 0;

	//pool used size
	virtual Size GetSize() const = 0;

	//pool total size
	virtual Size GetTotalSize() const = 0;

	//pool max size
	virtual Size GetMaxSize() const = 0;

	virtual StatusCode SetSize(Size cbSize) = 0;
		
	virtual StatusCode Add(Size cbSize) = 0;

	virtual StatusCode Add(const void *pMem, Size cbSize) = 0;

};

class DynMemPool : IMemPool
{
public:

	DynMemPool(Size cbMaxSize = SIZET_MAX);

	~DynMemPool();

	virtual void *GetMem();

	virtual const void *GetMem() const;

	//pool used size
	virtual Size GetSize() const;

	//pool total size
	virtual Size GetTotalSize() const;

	//pool max size
	virtual Size GetMaxSize() const;

	virtual StatusCode SetSize(Size cbSize);

	virtual StatusCode Add(Size cbSize);

	virtual StatusCode Add(const void *pMem, Size cbSize);

private:

	void *m_pMem;
	Size m_cbSize;
	Size m_cbTotalSize;
	Size m_cbMaxSize;

};

class StaticMemPool : IMemPool
{
public:

	StaticMemPool(void *pMem, Size cbSize);

	~StaticMemPool();

	virtual void *GetMem();

	virtual const void *GetMem() const;

	//pool used size
	virtual Size GetSize() const;

	//pool total size
	virtual Size GetTotalSize() const;

	//pool max size
	virtual Size GetMaxSize() const;

	virtual StatusCode SetSize(Size cbSize);

	virtual StatusCode Add(Size cbSize);

	virtual StatusCode Add(const void *pMem, Size cbSize);

private:

	void *m_pMem;
	Size m_cbSize;
	Size m_cbTotalSize;

};

}//namespace Util

}//namespace CX

