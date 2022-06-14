/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
 
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/MemPack/Storage.hpp"
#include "CX/MemPack/Allocator.hpp"
#include "CX/MemPack/Debug.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace MemPack
{

class Allocator
{
public:

	typedef UInt32        Ptr;

	static const Ptr      NullPtr            = Block::INVALID_OFFSET;

	static const UInt32   MAX_ALLOC_SIZE;

	static const UInt32   MIN_TOTAL_SIZE;
	static const UInt32   MAX_TOTAL_SIZE;

	static const UInt32   DEFAULT_INITIAL_SIZE;
	static const UInt32   DEFAULT_MAX_SIZE;

	Allocator()
	{
		m_pStorage     = NULL;
		m_cbMaxSize    = 0;
		m_pExtAlloc    = NULL;
		m_pExtAllocCtx = NULL;
	}

	~Allocator()
	{
		Destroy();
	}

	inline Status Create(ExternalAllocator *pExtAlloc, void *pExtAllocCtx, 
	                     UInt32 cbInitialSize = DEFAULT_INITIAL_SIZE, UInt32 cbMaxSize = DEFAULT_MAX_SIZE)
	{
		if (cbInitialSize < MIN_TOTAL_SIZE)
		{
			return Status(Status_TooSmall, "cbInitialSize must be >= {1} (was {2})", MIN_TOTAL_SIZE, cbInitialSize);
		}
		if (cbInitialSize > MAX_TOTAL_SIZE)
		{
			return Status(Status_TooBig, "cbInitialSize must be <= {1} (was {2})", MAX_TOTAL_SIZE, cbInitialSize);
		}
		if (cbMaxSize < MIN_TOTAL_SIZE)
		{
			return Status(Status_TooSmall, "cbInitialSize must be >= {1} (was {2})", MIN_TOTAL_SIZE, cbMaxSize);
		}
		if (cbMaxSize > MAX_TOTAL_SIZE)
		{
			return Status(Status_TooBig, "cbInitialSize must be <= {1} (was {2})", MAX_TOTAL_SIZE, cbMaxSize);
		}
		if (cbMaxSize < cbInitialSize)
		{
			return Status(Status_InvalidArg, "cbInitialSize must be <= cbMaxSize ({1} vs {2})", cbInitialSize, cbMaxSize);
		}

		Status   status;

		for (;;)
		{
			m_cbMaxSize    = cbMaxSize;
			m_pExtAlloc    = pExtAlloc;
			m_pExtAllocCtx = pExtAllocCtx;
			if (NULL == (m_pStorage = Storage::Create(cbInitialSize, pExtAlloc, pExtAllocCtx)))
			{
				status = Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cbInitialSize);

				break;
			}

			break;
		}
		if (!status)
		{
			Destroy();
		}

		return status;
	}

	inline Status Open(ExternalAllocator *pExtAlloc, void *pExtAllocCtx, 
	                   void *pStorage, Size cbStorageSize, UInt32 cbMaxSize = DEFAULT_MAX_SIZE)
	{
		if (MIN_TOTAL_SIZE > cbStorageSize)
		{
			return Status(Status_InvalidArg, "Storage size too small");
		}

		Storage   *pTmpStorage = (Storage *)pStorage;

		if (pTmpStorage->cbSize != cbStorageSize)
		{
			return Status(Status_InvalidArg, "Invalid storage size");
		}

		if (pTmpStorage->cbSize < MIN_TOTAL_SIZE)
		{
			return Status(Status_TooSmall, "cbInitialSize must be >= {1} (was {2})", MIN_TOTAL_SIZE, pTmpStorage->cbSize);
		}
		if (pTmpStorage->cbSize > MAX_TOTAL_SIZE)
		{
			return Status(Status_TooBig, "cbInitialSize must be <= {1} (was {2})", MAX_TOTAL_SIZE, pTmpStorage->cbSize);
		}
		if (cbMaxSize < MIN_TOTAL_SIZE)
		{
			return Status(Status_TooSmall, "cbInitialSize must be >= {1} (was {2})", MIN_TOTAL_SIZE, cbMaxSize);
		}
		if (cbMaxSize > MAX_TOTAL_SIZE)
		{
			return Status(Status_TooBig, "cbInitialSize must be <= {1} (was {2})", MAX_TOTAL_SIZE, cbMaxSize);
		}
		if (cbMaxSize < pTmpStorage->cbSize)
		{
			return Status(Status_InvalidArg, "cbInitialSize must be <= cbMaxSize ({1} vs {2})", 
			              pTmpStorage->cbSize, cbMaxSize);
		}

		m_pStorage     = pTmpStorage;
		m_cbMaxSize    = cbMaxSize;
		m_pExtAlloc    = pExtAlloc;
		m_pExtAllocCtx = pExtAllocCtx;

		return Status();
	}

	inline Status Close()
	{
		if (NULL != m_pStorage)
		{
			m_pStorage     = NULL;
			m_cbMaxSize    = 0;
			m_pExtAlloc    = NULL;
			m_pExtAllocCtx = NULL;
		}

		return Status();
	}
	inline Status Destroy()
	{
		if (NULL != m_pStorage)
		{
			m_pStorage->Destroy(m_pExtAlloc, m_pExtAllocCtx);
			m_pStorage     = NULL;
			m_cbMaxSize    = 0;
			m_pExtAlloc    = NULL;
			m_pExtAllocCtx = NULL;
		}

		return Status();
	}

	inline Ptr Alloc(Size cbSize)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Alloc);

		if (NULL == m_pStorage)
		{
			return NullPtr;
		}
		if (!CheckSize(cbSize))
		{
			return NullPtr;
		}

		cbSize += Block::USED_MIN_SIZE;

		if (Block::UNUSED_MIN_SIZE > cbSize)
		{
			cbSize = Block::UNUSED_MIN_SIZE;
		}

		Block   *pBlock = m_pStorage->RemoveBestFitBlock((UInt32)cbSize);

		if (NULL == pBlock)
		{
			Status   status;

			if (!(status = Extend(cbSize)))
			{
				return NullPtr;
			}
			if (NULL == (pBlock = m_pStorage->RemoveBestFitBlock((UInt32)cbSize)))
			{
				return NullPtr;
			}
		}

		return (UInt32)((Byte *)pBlock + sizeof(UInt32) - (Byte *)m_pStorage);
	}

	inline Ptr Realloc(Ptr cbPtr, Size cbSize)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Realloc);

		if (NULL == m_pStorage)
		{
			return NullPtr;
		}
		if (!CheckPtr(cbPtr))
		{
			return NullPtr;
		}
		if (!CheckSize(cbSize))
		{
			return NullPtr;
		}

		UInt32   cbInitialSize = (UInt32)cbSize;

		cbSize += Block::USED_MIN_SIZE;

		if (Block::UNUSED_MIN_SIZE > cbSize)
		{
			cbSize = Block::UNUSED_MIN_SIZE;
		}

		Block    *pBlock       = m_pStorage->GetBlockFromOffset(cbPtr - sizeof(UInt32));
		UInt32   cbCurrentSize = pBlock->GetSize();

		if (cbCurrentSize >= cbSize)
		{
			if (NULL != m_pStorage->ShrinkBlock(pBlock, (UInt32)cbSize))
			{
				m_pStorage->cUsedBlocksCount++;
			}

			return (UInt32)((Byte *)pBlock + sizeof(UInt32) - (Byte *)m_pStorage);
		}

		//1. try to expand into the next block if the next block is unused and the combined size is enough
		//...

		//2. try to expand into the prev & next blocks if the prev & next blocks are unused and the combined size is enough
		//...

		//3. 

		UInt32   cbNewPtr = Alloc(cbInitialSize);

		if (NullPtr == cbNewPtr)
		{
			return NullPtr;
		}

		memcpy((Byte *)m_pStorage + cbNewPtr, (Byte *)m_pStorage + cbPtr, cbCurrentSize - Block::USED_MIN_SIZE);

		Free(cbPtr);

		return cbNewPtr;
	}

	inline void Free(Ptr cbPtr)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Free);

		if (NULL == m_pStorage)
		{
			return;
		}
		if (!CheckPtr(cbPtr))
		{
			return;
		}

		Block   *pBlock = m_pStorage->GetBlockFromOffset(cbPtr - sizeof(UInt32));

		m_pStorage->AddBlock(pBlock);
	}

	inline Bool CheckPtr(Ptr cbPtr)
	{
		return (sizeof(Storage) < cbPtr && cbPtr <= m_pStorage->cbSize - Block::UNUSED_MIN_SIZE + Block::USED_MIN_SIZE);
	}

	inline Bool CheckSize(Size cbSize)
	{
		return (0 < cbSize && MAX_ALLOC_SIZE - Block::UNUSED_MIN_SIZE > cbSize && 
		        cbSize + Block::UNUSED_MIN_SIZE < m_cbMaxSize);
	}

	inline void *GetPtr(Ptr cbPtr)
	{
		return (Byte *)m_pStorage + cbPtr;
	}

	inline Storage *GetStorage()
	{
		return m_pStorage;
	}

	inline Size GetMaxSize()
	{
		return m_cbMaxSize;
	}

	//==

	template <typename OUTPUT>
	inline void Print(OUTPUT output, const Char *szMessage = NULL, const Char *szIndent = "")
	{
		if (NULL != szMessage && 0 != *szMessage)
		{
			CX::Print(output, "{1}\n", szMessage);
		}
		CX::Print(output, "{1}Max size           = {2}\n", szIndent, m_cbMaxSize);
		m_pStorage->Print(output, "", szIndent);
	}

private:

	Storage             *m_pStorage;
	Size                m_cbMaxSize;
	ExternalAllocator   *m_pExtAlloc;
	void                *m_pExtAllocCtx;

	inline Status Extend(Size cbNeededSize)
	{
		UInt32   cbSize                  = m_pStorage->cbSize;
		UInt32   cbOldSize               = m_pStorage->cbSize;
		UInt32   cbLastBlockSizeIfUnused = m_pStorage->GetLastBlockSizeIfUnused();

		if (cbLastBlockSizeIfUnused > cbNeededSize) // ?!?
		{
			return Status();
		}
		cbNeededSize -= cbLastBlockSizeIfUnused;

		while (cbSize < m_cbMaxSize && cbSize - m_pStorage->cbSize < cbNeededSize)
		{
			if ((m_cbMaxSize >> 1) >= cbSize)
			{
				cbSize <<= 1;
			}
			else
			{
				cbSize = (UInt32)m_cbMaxSize;
			}
		}
		if (cbSize - m_pStorage->cbSize < cbNeededSize)
		{
			return Status(Status_TooBig, "Can't grow memory from {1} bytes with {2} bytes", m_pStorage->cbSize, 
			              cbNeededSize);
		}

		Storage   *pStorage = (Storage *)m_pExtAlloc->Realloc(m_pExtAllocCtx, m_pStorage, cbSize);

		if (NULL == pStorage)
		{
			return Status(Status_MemAllocFailed, "Failed to extend from {1} bytes to {2} bytes", m_pStorage->cbSize, 
			              cbSize);
		}

		m_pStorage         = pStorage;
		m_pStorage->cbSize = (UInt32)cbSize;

		m_pStorage->ProcessStorageExpand(cbOldSize);

		return Status();
	}

};

}//namespace MemPack

}//namespace CX
