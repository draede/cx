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
#include "CX/MemPack/ExternalAllocator.hpp"
#include "CX/MemPack/Block.hpp"
#include "CX/MemPack/Debug.hpp"
#include "CX/Hash/CRC32Hash.hpp"
#include "CX/Print.hpp"
#include <intrin.h>


namespace CX
{

namespace MemPack
{

#pragma pack(push)
#pragma pack(1)
struct Storage
{

	static const UInt32     MIN_SIZE;
	static const UInt32     MAX_SIZE;

	static const UInt32     CUSTOM_DATA_COUNT    = 8; //8 x 4

	static const UInt32     LISTS_SINGLE_COUNT   = 117; // [   12 -   128]
	static const UInt32     LISTS_MULTIPLE_COUNT = 112; // [  129 - 16384]
	static const UInt32     LISTS_BIG_COUNT      = 1;   // [16385 - .....]
	static const UInt32     LISTS_BITMASK_COUNT  = 8; //8 x 4
	static const UInt32     LISTS_ALL_COUNT      = LISTS_SINGLE_COUNT + LISTS_MULTIPLE_COUNT + LISTS_BIG_COUNT;

	static const UInt32     MAGIC                = 0x504D5843; //CXMP

	static const UInt32     VERSION              = 1;

	UInt32   uMagic;
	UInt32   uVersion;
	UInt32   uChecksum;
	UInt32   cbSize;
	UInt32   cUsedBlocksCount;
	UInt32   cUnusedBlocksCount;
	UInt32   customData[CUSTOM_DATA_COUNT];
	UInt32   cbMostRecentAddedBlockOffset;
	UInt32   cbMostRecentAddedBlockSize;
	UInt32   cbLastBlockOffset;
	UInt32   cbLastBlockSize;
	UInt32   unusedBlocksBitmask[LISTS_BITMASK_COUNT];
	UInt32   unusedBlocksLists[LISTS_ALL_COUNT];

	//=== begin create/destroy methods ==================================================================================

	static Storage *Create(UInt32 cbInitialSize, ExternalAllocator *pExternalAllocator, 
	                       void *pExternalAllocatorCtx = NULL)
	{
		if (cbInitialSize < MIN_SIZE)
		{
			return NULL;
		}
		if (cbInitialSize > MAX_SIZE)
		{
			return NULL;
		}

		Storage   *pStorage = (Storage *)pExternalAllocator->Alloc(pExternalAllocatorCtx, cbInitialSize);

		if (NULL == pStorage)
		{
			return NULL;
		}

		memset(pStorage, 0x00, cbInitialSize);

		pStorage->uMagic            = MAGIC;
		pStorage->uVersion          = VERSION;
		pStorage->cbSize            = cbInitialSize;
		pStorage->cbLastBlockOffset = sizeof(Storage);
		pStorage->cbLastBlockSize   = cbInitialSize - sizeof(Storage);

		return pStorage;
	}

	inline void Destroy(ExternalAllocator *pExternalAllocator, void *pExternalAllocatorCtx = NULL)
	{
		pExternalAllocator->Free(pExternalAllocatorCtx, this);
	}

	//=== end create/destroy methods ====================================================================================

	//=== begin hash methods ============================================================================================

	inline UInt32 ComputeHash()
	{
		Hash::CRC32Hash   hash;
		UInt32            uCRC32;

		hash.Init();

		//magic + version
		hash.Update(this, 2 * sizeof(UInt32));
		//skip checksum
		hash.Update((Byte *)this + 3 * sizeof(UInt32), cbSize - 3 * sizeof(UInt32));
		hash.Done(&uCRC32);

		return uCRC32;
	}

	inline UInt32 UpdateHash()
	{
		uChecksum = ComputeHash();

		return uChecksum;
	}

	inline Bool ValidateHash()
	{
		UInt32   uCRC32 = ComputeHash();

		return (uCRC32 == uChecksum);
	}

	//=== end hash methods ==============================================================================================

	//=== begin unused blocks container methods =========================================================================

	inline void AddBlock(Block *pBlock)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Unused_Add);

		pBlock = ExpandBlock(pBlock);

		UInt32   cbBlockSize   = pBlock->GetSize();
		UInt32   cbBlockOffset = GetOffsetFromBlock(pBlock);

		if (cbBlockOffset + cbBlockSize == cbSize) //last block
		{
			cbLastBlockOffset = cbBlockOffset;
			cbLastBlockSize   = cbBlockSize;
			cUsedBlocksCount--;
		}
		else
		if (Block::INVALID_OFFSET != cbLastBlockOffset && cbBlockOffset + cbBlockSize == cbLastBlockOffset) //merge with last block
		{
			cbLastBlockOffset = cbBlockOffset;
			cbLastBlockSize += cbBlockSize;
			cUsedBlocksCount--;
		}
		else
		{
			UInt32   cIndex = GetListIndex(cbBlockSize);

			ListInsert(cIndex, pBlock);
			cUsedBlocksCount--;
			cUnusedBlocksCount++;
		}
	}

	inline Bool RemoveBlock(Block *pBlock)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Unused_Remove);

		UInt32   cbBlockSize   = pBlock->GetSize();
		UInt32   cbBlockOffset = GetOffsetFromBlock(pBlock);
		Bool     bRet;

		if (cbBlockOffset + cbBlockSize == cbSize) //last block
		{
			cbLastBlockOffset = Block::INVALID_OFFSET;
			cbLastBlockSize   = 0;
			bRet              = True;
			cUnusedBlocksCount--;
		}
		else
		{
			UInt32   cIndex = GetListIndex(cbBlockSize);

			if ((bRet = ListRemove(cIndex, cbBlockOffset)))
			{
				cUsedBlocksCount++;
				cUnusedBlocksCount--;
			}
		}

		return bRet;
	}

#define TRY_LASTBLOCK_FIRST_ALWAYS

	//cbBlockSize must be >= Block::UNUSED_MIN_SIZE
	inline Block *RemoveBestFitBlock(UInt32 cbBlockSize)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Unused_RemoveBestFit);

		Block   *pBlock;

		if (Block::INVALID_OFFSET != cbMostRecentAddedBlockOffset && cbBlockSize == cbMostRecentAddedBlockSize)
		{
			Block    *pBlock = GetBlockFromOffset(cbMostRecentAddedBlockOffset);
			UInt32   cIndex  = GetListIndex(cbBlockSize);

			if (ListRemove(cIndex, cbMostRecentAddedBlockOffset))
			{
				pBlock->SetUsed();

				Block   *pNextBlock = GetNextBlock(pBlock);

				if (NULL != pNextBlock)
				{
					pNextBlock->SetPrevBlockUsed();
				}

				cUsedBlocksCount++;
				cUnusedBlocksCount--;

				return pBlock;
			}
		}

#ifdef TRY_LASTBLOCK_FIRST_ALWAYS
		if (NULL != (pBlock = RemoveBestFitBlockFromLastBlock(cbBlockSize)))
		{
			return pBlock;
		}
#else
		if (0 == cUnusedBlocksCount)
		{
			return RemoveBestFitBlockFromLastBlock(cbBlockSize);
		}
#endif

		UInt32          cIndex = GetListIndex(cbBlockSize);
		unsigned long   cIndexEx;

		if (GetFirstBitSetBitmask(cIndex, &cIndexEx))
		{
			cIndex = cIndexEx;
			while (cIndex < LISTS_ALL_COUNT)
			{
				if (NULL != (pBlock = ListRemoveBestFit(cIndex, cbBlockSize)))
				{
					cUsedBlocksCount++;
					if (NULL != (ShrinkBlock(pBlock, cbBlockSize)))
					{
						cUsedBlocksCount++;
					}
					cUnusedBlocksCount--;

					return pBlock;
				}
				cIndex++;
			}
		}

#ifdef TRY_LASTBLOCK_FIRST_ALWAYS
		return NULL;
#else
		return RemoveBestFitBlockFromLastBlock(cbBlockSize);
#endif
	}

	inline Block *RemoveBestFitBlockFromLastBlock(UInt32 cbBlockSize)
	{
		CXMP_PROF_SCOPE(Profiler::Slot_Unused_RemoveBestFitFromLastBlock);

		if (Block::INVALID_OFFSET == cbLastBlockOffset)
		{
			return NULL;
		}
		if (cbBlockSize > cbLastBlockSize)
		{
			return NULL;
		}

		Block   *pBlock = GetBlockFromOffset(cbLastBlockOffset);

		cbLastBlockSize -= cbBlockSize;
		cbLastBlockOffset += cbBlockSize;

		if (Block::UNUSED_MIN_SIZE > cbLastBlockSize)
		{
			cbBlockSize += cbLastBlockSize;
			cbLastBlockSize = 0;
			cbLastBlockOffset = Block::INVALID_OFFSET;
		}

		pBlock->SetUsedPrevBlockUsedSize(cbBlockSize);

		cUsedBlocksCount++;

		return pBlock;
	}

	//=== end unused blocks container methods ===========================================================================

	//=== begin block methods ===========================================================================================

	inline UInt32 GetOffsetFromBlock(Block *pBlock)
	{
		return (UInt32)((Byte *)pBlock - (Byte *)this);
	}

	inline Block *GetBlockFromOffset(UInt32 cbOffset)
	{
		return (Block *)((Byte *)this + cbOffset);
	}

	inline Block *GetFirstBlock()
	{
		if (sizeof(Storage) == cbLastBlockOffset)
		{
			return NULL;
		}

		return (Block *)((Byte *)this + sizeof(Storage));
	}

	inline Block *GetPrevBlock(Block *pBlock)
	{
		if (pBlock->IsPrevBlockUsed())
		{
			return NULL;
		}

		if ((Byte *)this + sizeof(Storage) == (Byte *)pBlock)
		{
			return NULL;
		}

		UInt32   cbPrevSize = *(UInt32 *)((Byte *)pBlock - sizeof(UInt32));

		return (Block *)((Byte *)pBlock - cbPrevSize);
	}

	inline Block *GetNextBlock(Block *pBlock)
	{
		Block   *pNextBlock = (Block *)((Byte *)pBlock + pBlock->GetSize());

		if (Block::INVALID_OFFSET != cbLastBlockOffset)
		{
			if ((Byte *)this + cbLastBlockOffset <= (Byte *)pNextBlock)
			{
				return NULL;
			}
		}
		else
		{
			if ((Byte *)this + cbSize <= (Byte *)pNextBlock)
			{
				return NULL;
			}
		}

		return pNextBlock;
	}

	inline Block *GetNextUnusedBlock(Block *pBlock)
	{
		if (Block::INVALID_OFFSET == pBlock->cbNextOffset)
		{
			return NULL;
		}

		return GetBlockFromOffset(pBlock->cbNextOffset);
	}

	//split a block in order to use the first resulted block and return the second block

	inline Block *ShrinkBlock(Block *pBlock, UInt32 cbNewSize)
	{
		UInt32    cbBlockOffset     = GetOffsetFromBlock(pBlock);
		UInt32    cbBlockSize       = pBlock->GetSize();
		UInt32    cbNextSize;
		Block     *pNextBlock       = NULL;
		Block     *pActualNextBlock = NULL;

		pActualNextBlock = GetNextBlock(pBlock);

		if (Block::UNUSED_MIN_SIZE <= cbNewSize && cbNewSize < cbBlockSize)
		{
			cbNextSize = cbBlockSize - cbNewSize;
			if (Block::UNUSED_MIN_SIZE <= cbNextSize)
			{
				pBlock->SetUsedSize(cbNewSize);
				pNextBlock = (Block *)((Byte *)pBlock + cbNewSize);
				pNextBlock->SetUnusedPrevBlockUsedSize(cbNextSize);
				pNextBlock->SetFooterSize(cbNextSize);
				if (NULL != pActualNextBlock)
				{
					pActualNextBlock->SetPrevBlockUnused();
				}
			}
		}

		if (NULL == pNextBlock)
		{
			pBlock->SetUsed();
			if (NULL != pActualNextBlock)
			{
				pActualNextBlock->SetPrevBlockUsed();
			}
		}
		else
		{
			AddBlock(pNextBlock);
		}

		return pNextBlock;
	}

	//extend an unused block with prev and next blocks if they are unused and return the resulted block 

	inline Block *ExpandBlock(Block *pBlock)
	{
		UInt32   cbBlockSize   = pBlock->GetSize();
		UInt32   cbBlockOffset = GetOffsetFromBlock(pBlock);
		Bool     bExtended     = False;
		Block    *pTmpBlock;
		UInt32   cbTmpSize;

		if (NULL != (pTmpBlock = GetPrevBlock(pBlock)))
		{
			cbTmpSize = pTmpBlock->GetSize();
			RemoveBlock(pTmpBlock);
			cUsedBlocksCount--;

			cbBlockOffset -= cbTmpSize;
			cbBlockSize += cbTmpSize;
			bExtended = True;
		}

		if (NULL != (pTmpBlock = GetNextBlock(pBlock)))
		{
			if (pTmpBlock->IsUnused())
			{
				cbTmpSize = pTmpBlock->GetSize();
				RemoveBlock(pTmpBlock);
				cUsedBlocksCount--;

				cbBlockSize += cbTmpSize;
				bExtended = True;
			}
		}

		if (bExtended)
		{
			pBlock = GetBlockFromOffset(cbBlockOffset);
			pBlock->SetUnusedPrevBlockUsedSize(cbBlockSize);
			pBlock->SetFooterSize(cbBlockSize);
		}
		else
		{
			pBlock->SetUnused();
			pBlock->SetFooterSize(cbBlockSize);
		}

		if (NULL != (pTmpBlock = GetNextBlock(pBlock)))
		{
			pTmpBlock->SetPrevBlockUnused();
		}

		return pBlock;
	}

	//=== end block methods =============================================================================================

	//=== begin list methods ============================================================================================

	inline void ListInsert(UInt32 cIndex, Block *pBlock)
	{
		UInt32   cbBlockOffset = GetOffsetFromBlock(pBlock);
		UInt32   cbBlockSize   = pBlock->GetSize();

		cbMostRecentAddedBlockOffset = cbBlockOffset;
		cbMostRecentAddedBlockSize   = cbBlockSize;

		if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
		{
			unusedBlocksLists[cIndex] = cbBlockOffset;
			pBlock->cbNextOffset      = Block::INVALID_OFFSET;
			SetBitmask(unusedBlocksBitmask, cIndex);

			return;
		}

		Block    *pIterBlock       = GetBlockFromOffset(unusedBlocksLists[cIndex]);
		Block    *pIterParentBlock = NULL;

		while (NULL != pIterBlock && cbBlockSize > pIterBlock->GetSize())
		{
			pIterParentBlock = pIterBlock;
			pIterBlock       = GetNextUnusedBlock(pIterBlock);
		}

		if (NULL != pIterBlock)
		{
			pBlock->cbNextOffset = GetOffsetFromBlock(pIterBlock);
		}
		else
		{
			pBlock->cbNextOffset = Block::INVALID_OFFSET;
		}
		
		if (NULL != pIterParentBlock)
		{
			pIterParentBlock->cbNextOffset = cbBlockOffset;
		}
		else
		{
			unusedBlocksLists[cIndex] = cbBlockOffset;
		}
	}

	inline Bool ListRemove(UInt32 cIndex, UInt32 cbBlockOffset)
	{
		if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
		{
			return False;
		}

		UInt32   cbIterOffset      = unusedBlocksLists[cIndex];
		Block    *pIterBlock       = GetBlockFromOffset(cbIterOffset);
		Block    *pIterParentBlock = NULL;

		while (NULL != pIterBlock && cbBlockOffset != cbIterOffset)
		{
			pIterParentBlock = pIterBlock;
			cbIterOffset     = pIterBlock->cbNextOffset;
			pIterBlock       = GetNextUnusedBlock(pIterBlock);
		}

		if (cbBlockOffset != cbIterOffset)
		{
			return False;
		}

		cbMostRecentAddedBlockOffset = Block::INVALID_OFFSET;
		cbMostRecentAddedBlockSize   = 0;

		if (NULL != pIterParentBlock)
		{
			pIterParentBlock->cbNextOffset = pIterBlock->cbNextOffset;
		}
		else
		{
			unusedBlocksLists[cIndex] = pIterBlock->cbNextOffset;
			if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
			{
				UnsetBitmask(unusedBlocksBitmask, cIndex);
			}
		}

		return True;
	}

	inline Block *ListRemoveBestFit(UInt32 cIndex, UInt32 cbBlockSize)
	{
		if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
		{
			return NULL;
		}

		Block    *pIterBlock       = GetBlockFromOffset(unusedBlocksLists[cIndex]);
		Block    *pIterParentBlock = NULL;

		while (NULL != pIterBlock && cbBlockSize > pIterBlock->GetSize())
		{
			pIterParentBlock = pIterBlock;
			pIterBlock       = GetNextUnusedBlock(pIterBlock);
		}

		if (NULL == pIterBlock)
		{
			return NULL;
		}

		cbMostRecentAddedBlockOffset = Block::INVALID_OFFSET;
		cbMostRecentAddedBlockSize   = 0;

		if (NULL != pIterParentBlock)
		{
			pIterParentBlock->cbNextOffset = pIterBlock->cbNextOffset;
		}
		else
		{
			unusedBlocksLists[cIndex] = pIterBlock->cbNextOffset;
			if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
			{
				UnsetBitmask(unusedBlocksBitmask, cIndex);
			}
		}

		return pIterBlock;
	}

	template <typename OUTPUT>
	inline void ListPrint(OUTPUT output, UInt32 cIndex, const Char *szMessage = NULL, const Char *szIndent = "")
	{
		if (NULL != szMessage)
		{
			CX::Print(output, "{1}\n", szMessage);
		}
		if (Block::INVALID_OFFSET == unusedBlocksLists[cIndex])
		{
			CX::Print(output, "{1}<EMPTY>\n", szIndent);

			return;
		}

		Block   *pIterBlock = GetBlockFromOffset(unusedBlocksLists[cIndex]);
		Size    cPrintIndex = 0;

		while (NULL != pIterBlock)
		{
			CX::Print(output, "{1}   {2:' '3} : STATE = {3}, PSTATE = {4}, SIZE = {5}, OFFFSET = {6}\n", 
			         szIndent, cPrintIndex, 
			         (int)pIterBlock->IsUsed(), (int)pIterBlock->IsPrevBlockUsed(), pIterBlock->GetSize(), 
			         (Byte *)pIterBlock - (Byte *)this);
			pIterBlock = GetNextUnusedBlock(pIterBlock);
			cPrintIndex++;
		}
	}

	//=== end list methods ==============================================================================================

	//=== begin list indexing methods ===================================================================================

	static inline UInt32 CLZ(UInt32 uValue)
	{
		DWORD   dwIndex = 0;

		_BitScanReverse(&dwIndex, uValue);

		return 31 - dwIndex;
	}

	static inline UInt32 GetListIndexMethod1Helper(UInt32 cbBlockSize)
	{
		UInt32   c = CLZ(cbBlockSize - 1);
		UInt32   r = 117 + ((24 - c) << 4) + ((cbBlockSize - 1 - (1 << (31 - c))) >> (27 - c));

		return 229 < r ? 229 : r;
	}

	static inline UInt32 GetListIndex(UInt32 cbBlockSize)
	{
		return 12 >= cbBlockSize ? 0 : (128 >= cbBlockSize ? cbBlockSize - 12 : GetListIndexMethod1Helper(cbBlockSize));
	}

	//cIndex = [0 ... 229]
	static inline void SetBitmask(UInt32 bitmask[8], UInt32 cIndex)
	{
		static const Byte BYTE_POWS[8] = 
		{
			1, 2, 4, 8, 16, 32, 64, 128
		};

		((Byte *)bitmask)[cIndex >> 3] |= BYTE_POWS[cIndex & 0x07];
	}

	//cIndex = [0 ... 229]
	static inline void UnsetBitmask(UInt32 bitmask[8], UInt32 cIndex)
	{
		static const Byte BYTE_POWS_NEGATED[8] = 
		{
			254, 253, 251, 247, 239, 223, 191, 127
		};

		((Byte *)bitmask)[cIndex >> 3] &= BYTE_POWS_NEGATED[cIndex & 0x07];
	}

	inline Bool GetFirstBitSetBitmask(UInt32 cStartIndex, unsigned long *pcIndex)
	{
		static const UInt32   BASE_OFFSET[8] = { 0, 32, 64, 96, 128, 160, 192, 224 };

		UInt32   cStartDWORDIndex  = cStartIndex >> 5;
		UInt32   cOffsetDWORDIndex = cStartIndex & 31;
		UInt32   uFirstValue       = unusedBlocksBitmask[cStartDWORDIndex] >> cOffsetDWORDIndex;

		if (_BitScanForward(pcIndex, uFirstValue))
		{
			*pcIndex += BASE_OFFSET[cStartDWORDIndex] + cOffsetDWORDIndex;

			return True;
		}
		for (UInt32 cDWORDIndex = cStartDWORDIndex + 1; cDWORDIndex < 8; cDWORDIndex++)
		{
			if (_BitScanForward(pcIndex, unusedBlocksBitmask[cDWORDIndex]))
			{
				*pcIndex += BASE_OFFSET[cDWORDIndex];

				return True;;
			}
		}

		return False;
	}

	//=== end list indexing methods =====================================================================================

	//=== begin helper methods ==========================================================================================

	inline UInt32 GetLastBlockSizeIfUnused()
	{
		if (Block::INVALID_OFFSET != cbLastBlockOffset)
		{
			return cbLastBlockSize;
		}
		else
		{
			return 0;
		}
	}

	inline void ProcessStorageExpand(UInt32 cbOldSize)
	{
		if (Block::INVALID_OFFSET != cbLastBlockOffset)
		{
			cbLastBlockSize += cbSize - cbOldSize;
		}
		else
		{
			cbLastBlockOffset = cbOldSize;
			cbLastBlockSize   = cbSize - cbOldSize;
		}
	}

	template <typename OUTPUT>
	inline void Print(OUTPUT output, const Char *szMessage = NULL, const Char *szIndent = "")
	{
		Block    *pBlock      = GetFirstBlock();
		UInt32   cBlocksCount = 0;

		if (NULL != szMessage && 0 != *szMessage)
		{
			CX::Print(output, "{1}\n", szMessage);
		}
		CX::Print(output, "{1}Header size        = {2}\n", szIndent, sizeof(Storage));
		CX::Print(output, "{1}Total size         = {2}\n", szIndent, cbSize);
		CX::Print(output, "{1}Used blocks        = {2}\n", szIndent, cUsedBlocksCount);
		CX::Print(output, "{1}Unused blocks      = {2}\n", szIndent, cUnusedBlocksCount);
		if (Block::INVALID_OFFSET != cbLastBlockOffset)
		{
			CX::Print(stdout, "{1}Last block         = UNUSED ({2} bytes at offset {3})\n", szIndent, 
			          cbLastBlockSize, cbLastBlockOffset);
		}
		else
		{
			CX::Print(stdout, "{1}Last block         = USED\n", szIndent);
		}

		CX::Print(output, "{1}All blocks         = {2}\n", szIndent, cUsedBlocksCount + cUnusedBlocksCount);

		cBlocksCount = 0;
		while (NULL != pBlock)
		{
			CX::Print(output, "{1}{2:' '3} : STATE = {3}, PSTATE = {4}, SIZE = {5}, OFFFSET = {6}\n", 
			          szIndent, cBlocksCount, (int)pBlock->IsUsed(), (int)pBlock->IsPrevBlockUsed(), pBlock->GetSize(), 
			          GetOffsetFromBlock(pBlock));
			pBlock = GetNextBlock(pBlock);
			cBlocksCount++;
		}
		CX::Print(output, "{1}All blocks (found) = {2} (expected {3})\n", szIndent, 
		          cBlocksCount, cUsedBlocksCount + cUnusedBlocksCount);

		String   sIndent;

		sIndent = szIndent;
		sIndent += "   ";

		UInt32   cListsCount = 0;

		for (UInt32 i = 0; i < LISTS_ALL_COUNT; i++)
		{
			if (Block::INVALID_OFFSET != unusedBlocksLists[i])
			{
				if (0 == cListsCount)
				{
					CX::Print(stdout, "{1}Lists              =\n", szIndent);
				}

				String   sMsg;

				CX::Print(&sMsg, "{1}{2} : {3} - {4}:", sIndent, i, 
				          VAR_BLOCKS_SIZES[i].cbBeginSize, VAR_BLOCKS_SIZES[i].cbEndSize);

				ListPrint(output, i, sMsg.c_str(), sIndent.c_str());

				cListsCount++;
			}
		}

		if (0 == cListsCount)
		{
			CX::Print(stdout, "{1}Lists              = NONE\n", szIndent);
		}
	}

	struct ListEntryInfo
	{
		UInt32   cbBeginSize;
		UInt32   cbEndSize;
	};

	static const ListEntryInfo   VAR_BLOCKS_SIZES[LISTS_ALL_COUNT];

	//=== end helper methods ============================================================================================

};
#pragma pack(pop)

}//namespace MemPack

}//namespace CX
