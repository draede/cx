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

#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/C/Mem.h"
#include "CX/C/MemStats.h"
#include "CX/C/StatusCodes.h"
#include "CX/C/Limits.h"
#include "CX/C/string.h"
#include "CX/C/stdlib.h"
#include "CX/C/stdio.h"
#include "CX/C/Platform/Windows/windows.h"
#include <dbghelp.h>


typedef CX_UIntPtr    CX_MemFrame;

typedef struct _CX_MemBlock
{
	struct _CX_MemBlock *pPrev;
	struct _CX_MemBlock *pNext;
	CX_Size             cbSize;
	CX_UInt8            cFrames;
	CX_MemFrame         frames[1];
}CX_MemBlock;

typedef struct _CX_MemStats
{
	CX_Bool          bActive;
	CX_Bool          bSynInitialized;
	CX_Size          cbCrAllocsSize;
	CX_Size          cCrAllocsCount;
	CX_Size          cbMaxAllocsSize;
	CX_Size          cMaxAllocsCount;
	CX_MemBlock      *pFirstBlock;
	CX_MemBlock      *pLastBlock;
	CRITICAL_SECTION cs;
}CX_MemStats;

static const CX_Char *CX_MEMSTATS_IGNORES_UPPER[] =
{
	"CX_MemAlloc",
	"CX_MemRealloc",
	"CX_MemFree",
	"CX_MemOptAlloc",
	"CX_MemOptRealloc",
	"CX_MemOptFree",
	"CX_MemDbgAlloc",
	"CX_MemDbgRealloc",
	"CX_MemDbgFree",
	"CX_MemGetFrames",
	"CX::Mem::Alloc",
	"CX::Mem::New",
	"CX::Mem::NewArr",
	"CX::IObject::operator new",
};

static const CX_Size CX_MEMSTATS_IGNORES_UPPER_COUNT = sizeof(CX_MEMSTATS_IGNORES_UPPER) / sizeof(CX_MEMSTATS_IGNORES_UPPER[0]);

static const CX_Char *CX_MEMSTATS_IGNORES_LOWER[] =
{
	"__tmainCRTStartup",
	"mainCRTStartup",
};

static const CX_Size CX_MEMSTATS_IGNORES_LOWER_COUNT = sizeof(CX_MEMSTATS_IGNORES_LOWER) / sizeof(CX_MEMSTATS_IGNORES_LOWER[0]);

static const CX_Char *CX_MEMSTATS_EXCEPTIONS[] =
{
	"",//dummy
};

static const CX_Size CX_MEMSTATS_EXCEPTIONS_COUNT = 0;//sizeof(CX_MEMSTATS_EXCEPTIONS) / sizeof(CX_MEMSTATS_EXCEPTIONS[0]);

static CX_MemStats g_cx_memstats = { 0 };


void *CX_MemOptAlloc(CX_Size cbSize)
{
	return HeapAlloc(GetProcessHeap(), 0, cbSize);
}

void *CX_MemOptRealloc(void *pPtr, CX_Size cbSize)
{
	if (NULL == pPtr)
	{
		return CX_MemOptAlloc(cbSize);
	}
	else
	{
		return HeapReAlloc(GetProcessHeap(), 0, pPtr, cbSize);
	}
}

void CX_MemOptFree(void *pPtr)
{
	if (NULL != pPtr)
	{
		HeapFree(GetProcessHeap(), 0, pPtr);
	}
}

CX_StatusCode CX_MemGetFrames(CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT], CX_UInt8 *pcFrames)
{
	USHORT cFrames;

	cFrames = CaptureStackBackTrace(0, CX_MEMSTATS_MAX_CALLS_COUNT, (PVOID *)frames, NULL);
	if ((USHORT)CX_UINT8_MAX < cFrames)
	{
		cFrames = CX_UINT8_MAX;
	}
	*pcFrames = (CX_UInt8)cFrames;

	return CX_Status_OK;
}

CX_UInt16 CX_MemGetBlockSize(CX_UInt8 cFrames)
{
	CX_UInt16 cbBlockSize;
	
	cbBlockSize = (CX_UInt16)sizeof(CX_MemBlock);
	if (0 < cFrames)
	{
		cbBlockSize += (CX_UInt16)((cFrames - 1) * sizeof(CX_MemFrame));
	}

	return cbBlockSize;
}

CX_MemBlock *CX_MemGetBlockFromPtr(void *pPtr)
{
	CX_UInt16 cbBlockSize;
	CX_Byte   *pRealPtr;

	pRealPtr = (CX_Byte *)pPtr;
	pRealPtr -= sizeof(CX_UInt16);
	memcpy(&cbBlockSize, pRealPtr, sizeof(CX_UInt16));
	pRealPtr -= cbBlockSize;

	return (CX_MemBlock *)pRealPtr;
}

void *CX_MemDbgAlloc(CX_Size cbSize)
{
	CX_MemFrame   frames[CX_MEMSTATS_MAX_CALLS_COUNT];
	CX_UInt8      cFrames;
	CX_UInt16     cbBlockSize;
	CX_Size       cbTotalSize;
	CX_Byte       *pBlockPtr;
	CX_Byte       *pBlockSizePtr;
	CX_Byte       *pUserPtr;
	CX_MemBlock   *pBlock;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_MemGetFrames(frames, &cFrames)))
	{
		cFrames = 0;
	}
	cbBlockSize = CX_MemGetBlockSize(cFrames);
	cbTotalSize = cbBlockSize + sizeof(CX_UInt16) + cbSize;
	if (NULL == (pBlockPtr = (CX_Byte *)CX_MemOptAlloc(cbTotalSize)))
	{
		return NULL;
	}
	pBlockSizePtr   = pBlockPtr + cbBlockSize;
	pUserPtr        = pBlockSizePtr + sizeof(CX_UInt16);
	memcpy(pBlockSizePtr, &cbBlockSize, sizeof(CX_UInt16));

	pBlock          = (CX_MemBlock *)pBlockPtr;
	pBlock->pNext   = NULL;
	pBlock->cbSize  = cbSize;
	pBlock->cFrames = cFrames;
	if (0 < cFrames)
	{
		memcpy(pBlock->frames, frames, cFrames * sizeof(CX_MemFrame));
	}

	EnterCriticalSection(&g_cx_memstats.cs);

	g_cx_memstats.cbCrAllocsSize += cbSize;
	g_cx_memstats.cCrAllocsCount++;
	if (g_cx_memstats.cbCrAllocsSize > g_cx_memstats.cbMaxAllocsSize)
	{
		g_cx_memstats.cbMaxAllocsSize = g_cx_memstats.cbCrAllocsSize;
	}
	if (g_cx_memstats.cCrAllocsCount > g_cx_memstats.cMaxAllocsCount)
	{
		g_cx_memstats.cMaxAllocsCount = g_cx_memstats.cCrAllocsCount;
	}
	if (NULL == g_cx_memstats.pFirstBlock)
	{
		pBlock->pPrev             = NULL;
		g_cx_memstats.pFirstBlock = g_cx_memstats.pLastBlock = pBlock;
	}
	else
	{
		pBlock->pPrev                   = g_cx_memstats.pLastBlock;
		g_cx_memstats.pLastBlock->pNext = pBlock;
		g_cx_memstats.pLastBlock        = pBlock;
	}

	LeaveCriticalSection(&g_cx_memstats.cs);

	return pUserPtr;
}

void *CX_MemDbgRealloc(void *pOldPtr, CX_Size cbSize)
{
	if (NULL == pOldPtr)
	{
		return CX_MemDbgAlloc(cbSize);
	}

	CX_Byte       *pOldBlockPtr;
	CX_Byte       *pOldBlockSizePtr;
	CX_Byte       *pOldUserPtr = (CX_Byte *)pOldPtr;
	CX_UInt16     cbOldBlockSize;
	CX_MemBlock   *pOldBlock;
	CX_MemFrame   frames[CX_MEMSTATS_MAX_CALLS_COUNT];
	CX_UInt8      cFrames;
	CX_UInt16     cbBlockSize;
	CX_Size       cbTotalSize;
	CX_Byte       *pBlockPtr;
	CX_Byte       *pBlockSizePtr;
	CX_Byte       *pUserPtr;
	CX_MemBlock   *pBlock;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_MemGetFrames(frames, &cFrames)))
	{
		cFrames = 0;
	}
	cbBlockSize = CX_MemGetBlockSize(cFrames);
	cbTotalSize = cbBlockSize + sizeof(CX_UInt16) + cbSize;
	if (NULL == (pBlockPtr = (CX_Byte *)CX_MemOptAlloc(cbTotalSize)))
	{

		return NULL;
	}

	EnterCriticalSection(&g_cx_memstats.cs);

	pOldBlockSizePtr = pOldUserPtr - sizeof(CX_UInt16);
	memcpy(&cbOldBlockSize, pOldBlockSizePtr, sizeof(CX_UInt16));
	pOldBlockPtr     = pOldBlockSizePtr - cbOldBlockSize;
	pOldBlock        = (CX_MemBlock *)pOldBlockPtr;

	if (NULL != pOldBlock->pPrev)
	{
		pOldBlock->pPrev->pNext = pOldBlock->pNext;
	}
	if (NULL != pOldBlock->pNext)
	{
		pOldBlock->pNext->pPrev = pOldBlock->pPrev;
	}
	if (pOldBlock == g_cx_memstats.pFirstBlock)
	{
		g_cx_memstats.pFirstBlock = g_cx_memstats.pFirstBlock->pNext;
	}
	else
	if (pOldBlock == g_cx_memstats.pLastBlock)
	{
		g_cx_memstats.pLastBlock = g_cx_memstats.pLastBlock->pPrev;
	}
	g_cx_memstats.cbCrAllocsSize -= pOldBlock->cbSize;
	g_cx_memstats.cCrAllocsCount--;

	pBlockSizePtr = pBlockPtr + cbBlockSize;
	pUserPtr = pBlockSizePtr + sizeof(CX_UInt16);
	memcpy(pBlockSizePtr, &cbBlockSize, sizeof(CX_UInt16));

	pBlock = (CX_MemBlock *)pBlockPtr;
	pBlock->pNext = NULL;
	pBlock->cbSize = cbSize;
	pBlock->cFrames = cFrames;
	if (0 < cFrames)
	{
		memcpy(pBlock->frames, frames, cFrames * sizeof(CX_MemFrame));
	}

	g_cx_memstats.cbCrAllocsSize += cbSize;
	g_cx_memstats.cCrAllocsCount++;
	if (g_cx_memstats.cbCrAllocsSize > g_cx_memstats.cbMaxAllocsSize)
	{
		g_cx_memstats.cbMaxAllocsSize = g_cx_memstats.cbCrAllocsSize;
	}
	if (g_cx_memstats.cCrAllocsCount > g_cx_memstats.cMaxAllocsCount)
	{
		g_cx_memstats.cMaxAllocsCount = g_cx_memstats.cCrAllocsCount;
	}
	if (NULL == g_cx_memstats.pFirstBlock)
	{
		pBlock->pPrev = NULL;
		g_cx_memstats.pFirstBlock = g_cx_memstats.pLastBlock = pBlock;
	}
	else
	{
		pBlock->pPrev = g_cx_memstats.pLastBlock;
		g_cx_memstats.pLastBlock->pNext = pBlock;
		g_cx_memstats.pLastBlock = pBlock;
	}

	if (cbSize > pOldBlock->cbSize)
	{
		cbSize = pOldBlock->cbSize;
	}
	memcpy(pUserPtr, pOldUserPtr, cbSize);

	CX_MemOptFree(pOldBlockPtr);

	LeaveCriticalSection(&g_cx_memstats.cs);

	return pUserPtr;
}

void CX_MemDbgFree(void *pPtr)
{
	EnterCriticalSection(&g_cx_memstats.cs);

	CX_Byte       *pBlockPtr;
	CX_Byte       *pBlockSizePtr;
	CX_Byte       *pUserPtr = (CX_Byte *)pPtr;
	CX_UInt16     cbBlockSize;
	CX_MemBlock   *pBlock;

	pBlockSizePtr = pUserPtr - sizeof(CX_UInt16);
	memcpy(&cbBlockSize, pBlockSizePtr, sizeof(CX_UInt16));
	pBlockPtr     = pBlockSizePtr - cbBlockSize;
	pBlock        = (CX_MemBlock *)pBlockPtr;

	if (NULL != pBlock->pPrev)
	{
		pBlock->pPrev->pNext = pBlock->pNext;
	}
	if (NULL != pBlock->pNext)
	{
		pBlock->pNext->pPrev = pBlock->pPrev;
	}
	if (pBlock == g_cx_memstats.pFirstBlock)
	{
		g_cx_memstats.pFirstBlock = g_cx_memstats.pFirstBlock->pNext;
	}
	else
	if (pBlock == g_cx_memstats.pLastBlock)
	{
		g_cx_memstats.pLastBlock = g_cx_memstats.pLastBlock->pPrev;
	}
	g_cx_memstats.cbCrAllocsSize -= pBlock->cbSize;
	g_cx_memstats.cCrAllocsCount--;

	CX_MemOptFree(pBlockPtr);

	LeaveCriticalSection(&g_cx_memstats.cs);
}

void *CX_MemAlloc(CX_Size cbSize)
{
	if (g_cx_memstats.bActive)
	{
		return CX_MemDbgAlloc(cbSize);
	}
	else
	{
		return CX_MemOptAlloc(cbSize);
	}
}

void *CX_MemRealloc(void *pPtr, CX_Size cbSize)
{
	if (g_cx_memstats.bActive)
	{
		return CX_MemDbgRealloc(pPtr, cbSize);
	}
	else
	{
		return CX_MemOptRealloc(pPtr, cbSize);
	}
}

void CX_MemFree(void *pPtr)
{
	if (g_cx_memstats.bActive)
	{
		CX_MemDbgFree(pPtr);
	}
	else
	{
		CX_MemOptFree(pPtr);
	}
}

static void CX_MemStats_AtExit(void)
{
	if (g_cx_memstats.bActive)
	{
		DeleteCriticalSection(&g_cx_memstats.cs);
		g_cx_memstats.bActive = CX_False;
	}
	if (g_cx_memstats.bSynInitialized)
	{
		SymCleanup(GetCurrentProcess());
		g_cx_memstats.bSynInitialized = CX_False;
	}
}

CX_StatusCode CX_MemStats_Activate()
{
	if (g_cx_memstats.bActive)
	{
		return CX_Status_OK;
	}
	g_cx_memstats.bSynInitialized = false;
	g_cx_memstats.cbCrAllocsSize  = 0;
	g_cx_memstats.cCrAllocsCount  = 0;
	g_cx_memstats.cbMaxAllocsSize = 0;
	g_cx_memstats.cMaxAllocsCount = 0;
	g_cx_memstats.pFirstBlock     = NULL;
	g_cx_memstats.pLastBlock      = NULL;
	InitializeCriticalSection(&g_cx_memstats.cs);
	atexit(&CX_MemStats_AtExit);
	g_cx_memstats.bActive = true;

	return CX_Status_OK;
}

CX_Bool CX_MemStats_IsActive()
{
	return g_cx_memstats.bActive;
}

CX_Size CX_MemStats_GetCurrentAllocsSize()
{
	return g_cx_memstats.cbCrAllocsSize;
}

CX_Size CX_MemStats_GetCurrentAllocsCount()
{
	return g_cx_memstats.cCrAllocsCount;
}

CX_Size CX_MemStats_GetMaxAllocsSize()
{
	return g_cx_memstats.cbMaxAllocsSize;
}

CX_Size CX_MemStats_GetMaxAllocsCount()
{
	return g_cx_memstats.cMaxAllocsCount;
}

bool CX_MemStatsIsString(const CX_Char *szString, const CX_Char *strings[], CX_Size cStrings)
{
	CX_Size cIndex;

	for (cIndex = 0; cIndex < cStrings; cIndex++)
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp(szString, strings[cIndex]))
#pragma warning(pop)
		{
			return true;
		}
	}

	return false;
}

void CX_MemStats_GetAllocs(CX_MemStats_AllocStatsHandler pfnHandler, void *pUsrCtx,
                           const CX_Char *exceptions[], CX_Size cExceptions,
                           const CX_Char *ignoresUpper[], CX_Size cIgnoresUpper,
                           const CX_Char *ignoresLower[], CX_Size cIgnoresLower)
{
	CX_MemAllocInfo   info;
	CX_MemAllocCall   *calls;
	CX_MemBlock       *pBlock;
	CX_Byte           *pPos;
	CX_UInt8          cIndex;
	unsigned char     symbuf[sizeof(SYMBOL_INFO) + CX_MEMSTATS_MAX_SYMBOL_LEN - 1];
	SYMBOL_INFO       *pSym = (SYMBOL_INFO *)symbuf;
	IMAGEHLP_LINE64   line;
	DWORD             dwDisplacement;
	CX_Bool           bException;
	HANDLE            hProcess;

	if (NULL == (calls = (CX_MemAllocCall *)CX_MemOptAlloc(sizeof(CX_MemAllocCall) * CX_MEMSTATS_MAX_CALLS_COUNT)))
	{
		return;
	}
	
	hProcess = GetCurrentProcess();
	pSym->MaxNameLen = sizeof(symbuf) - sizeof(sizeof(SYMBOL_INFO));
	pSym->SizeOfStruct = sizeof(SYMBOL_INFO);

	EnterCriticalSection(&g_cx_memstats.cs);

	if (!g_cx_memstats.bSynInitialized)
	{
		g_cx_memstats.bSynInitialized = CX_True;
		SymInitialize(GetCurrentProcess(), NULL, TRUE);
	}

	pBlock = g_cx_memstats.pFirstBlock;
	while (NULL != pBlock)
	{
		pPos = (CX_Byte *)pBlock;
		pPos += sizeof(CX_MemBlock);
		if (0 < pBlock->cFrames)
		{
			pPos += (pBlock->cFrames - 1) * sizeof(CX_UIntPtr);
		}
		pPos += sizeof(CX_UInt16);

		info.pPtr = pPos;
		info.cbSize = pBlock->cbSize;
		info.cCallsCount = 0;
		info.calls = calls;
		bException = CX_False;
		for (cIndex = 0; cIndex < pBlock->cFrames; cIndex++)
		{
			calls[info.cCallsCount].szFunction[0] = 0;
			calls[info.cCallsCount].szFile[0] = 0;
			calls[info.cCallsCount].cLineNumber = 1;
			if (SymFromAddr(hProcess, (DWORD64)(pBlock->frames[cIndex]), 0, pSym))
			{
				if (CX_MemStatsIsString(pSym->Name, ignoresUpper, cIgnoresUpper) ||
					CX_MemStatsIsString(pSym->Name, CX_MEMSTATS_IGNORES_UPPER, CX_MEMSTATS_IGNORES_UPPER_COUNT))
				{
					continue;
				}
				if (CX_MemStatsIsString(pSym->Name, ignoresLower, cIgnoresLower) ||
					CX_MemStatsIsString(pSym->Name, CX_MEMSTATS_IGNORES_LOWER, CX_MEMSTATS_IGNORES_LOWER_COUNT))
				{
					break;
				}
				if (CX_MemStatsIsString(pSym->Name, exceptions, cExceptions) ||
					CX_MemStatsIsString(pSym->Name, CX_MEMSTATS_EXCEPTIONS, CX_MEMSTATS_EXCEPTIONS_COUNT))
				{
					bException = CX_True;

					break;
				}
#pragma warning(push)
#pragma warning(disable: 4996)
				cx_strcpy(calls[info.cCallsCount].szFunction, pSym->Name);
#pragma warning(pop)
				if (SymGetLineFromAddr64(hProcess, (DWORD64)(pBlock->frames[cIndex]), &dwDisplacement, &line))
				{
#pragma warning(push)
#pragma warning(disable: 4996)
					cx_strncpy(calls[info.cCallsCount].szFile, line.FileName, CX_MEMSTATS_MAX_SYMBOL_LEN);
#pragma warning(pop)
					calls[info.cCallsCount].szFile[CX_MEMSTATS_MAX_SYMBOL_LEN - 1] = 0;
					calls[info.cCallsCount].cLineNumber = (CX_Size)line.LineNumber;
				}
			}
			info.cCallsCount++;
		}
		if (!bException)
		{
			pfnHandler(&info, pUsrCtx);
		}

		pBlock = pBlock->pNext;
	}

	LeaveCriticalSection(&g_cx_memstats.cs);

	CX_MemOptFree(calls);
}

#endif

