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
#include "CX/C/StatusCodes.h"
#include "CX/C/Limits.h"
#include "CX/C/string.h"
#include "CX/C/stdlib.h"
#include "CX/C/stdio.h"
#include "CX/C/Platform/Windows/windows.h"
#include <dbghelp.h>


#define CX_MEMSTATS_MAX_CALLS_COUNT   63


typedef CX_UIntPtr        CX_MemFrame;

INIT_ONCE g_cx_mem_init_once    = INIT_ONCE_STATIC_INIT;
INIT_ONCE g_cx_mem_initsym_once = INIT_ONCE_STATIC_INIT;
HANDLE    g_cx_mem_heap         = NULL;
CX_Bool   g_cx_mem_sym          = 0;
#ifdef CX_MEM_FORCE_TRACK
CX_Bool   g_cx_mem_track        = 1;
#else
CX_Bool   g_cx_mem_track        = 0;
#endif
#ifdef CX_MEM_FORCE_DUMP
CX_Bool   g_cx_mem_dump         = 1;
#else
CX_Bool   g_cx_mem_dump         = 0;
#endif

typedef struct _CX_MemCallStack
{
	CX_MemFrame *callstack;
	CX_Size     cFrameIndex;
	CX_Size     cFramesCount;
}CX_MemCallStack;

#define CX_MEMSTATS_MAX_FUNCTION_NAME_LEN    256


void CX_Mem_Uninit(void)
{
	if (g_cx_mem_dump)
	{
		CX_MemEnumAllocs(NULL);
	}
	if (g_cx_mem_sym)
	{
		SymCleanup(GetCurrentProcess());
		g_cx_mem_sym = CX_False;
	}
	HeapDestroy(g_cx_mem_heap);
	g_cx_mem_heap = NULL;
}

BOOL CALLBACK CX_Mem_InitHandler(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *lpContext)
{
	CX_UNUSED(InitOnce);
	CX_UNUSED(Parameter);
	CX_UNUSED(lpContext);

	g_cx_mem_heap = HeapCreate(0, 0, 0);
	atexit(&CX_Mem_Uninit);

	return TRUE;
}

BOOL CALLBACK CX_Mem_InitSymHandler(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *lpContext)
{
	CX_UNUSED(InitOnce);
	CX_UNUSED(Parameter);
	CX_UNUSED(lpContext);

	SymInitialize(GetCurrentProcess(), NULL, TRUE);
	g_cx_mem_sym = CX_True;

	return TRUE;
}

void CX_Mem_Init()
{
	PVOID lpContext;

	InitOnceExecuteOnce(&g_cx_mem_init_once, &CX_Mem_InitHandler, NULL, &lpContext);
}

void CX_Mem_SymInit()
{
	PVOID lpContext;

	InitOnceExecuteOnce(&g_cx_mem_initsym_once, &CX_Mem_InitSymHandler, NULL, &lpContext);
}

void CX_MemSetTrack(CX_Bool bTrackMem)
{
	g_cx_mem_track = bTrackMem;
}

CX_Bool CX_MemGetTrack()
{
	return g_cx_mem_track;
}

void CX_MemSetDumpAllocs(CX_Bool bDumpAllocs)
{
	g_cx_mem_dump = bDumpAllocs;
}

CX_Bool CX_MemGetDumpAllocs()
{
	return g_cx_mem_dump;
}

void CX_MemGetFrames(CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT], CX_Byte *pcFrames)
{
	USHORT cFrames;

	cFrames = CaptureStackBackTrace(0, CX_MEMSTATS_MAX_CALLS_COUNT, (PVOID *)frames, NULL);
	if ((USHORT)CX_MEMSTATS_MAX_CALLS_COUNT < cFrames)
	{
		cFrames = CX_MEMSTATS_MAX_CALLS_COUNT;
	}
	*pcFrames = (CX_Byte)cFrames;
}

/*

Memory block:

sizeof(CX_Size) = alloc_size
1 byte = frames_count
... user data ...
sizeof(CX_UIntPtr) * (frames count) = frames (only if 0 < frames_count)

*/

CX_Size CX_MemGetTotalSize(CX_Bool bTrack, CX_Size cbSize, CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT], CX_Byte *pcFrames)
{
	CX_Size     cbTotalSize;

	cbTotalSize = sizeof(CX_Size); //alloc size
	cbTotalSize += sizeof(CX_Byte); //frames count
	cbTotalSize += cbSize;
	if (bTrack)
	{
		CX_MemGetFrames(frames, pcFrames);
	}
	else
	{
		*pcFrames = 0;
	}
	if (0 < *pcFrames)
	{
		cbTotalSize += (*pcFrames) * sizeof(CX_MemFrame); //frames
	}

	return cbTotalSize;
}

void *CX_MemSetBlockData(CX_Byte *pMem, CX_Size cbSize, CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT], CX_Byte cFrames)
{
	void *pUsrMem;

	memcpy(pMem, &cbSize, sizeof(CX_Size)); //alloc size
	pMem += sizeof(CX_Size);
	memcpy(pMem, &cFrames, sizeof(CX_Byte)); //frames count
	pMem += sizeof(CX_Byte);
	pUsrMem = pMem;
	if (0 < cFrames)
	{
		pMem += cbSize;
		memcpy(pMem, frames, cFrames * sizeof(CX_MemFrame)); //frames
	}

	return pUsrMem;
}

void *CX_MemAllocHelper(CX_Bool bTrack, CX_Size cbSize)
{
	CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT];
	CX_Size     cbTotalSize;
	CX_Byte     cFrames;
	CX_Byte     *pMem;

	if (NULL == g_cx_mem_heap)
	{
		CX_Mem_Init();
	}
	cbTotalSize = CX_MemGetTotalSize(bTrack, cbSize, frames, &cFrames);
#pragma warning(suppress: 6387)
	if (NULL == (pMem = (CX_Byte *)HeapAlloc(g_cx_mem_heap, 0, cbTotalSize)))
	{
		return NULL;
	}

	return CX_MemSetBlockData(pMem, cbSize, frames, cFrames);
}

void CX_MemFreeHelper(CX_Bool bTrack, void *pPtr)
{
	CX_Byte *pMem;

	CX_UNUSED(bTrack);

	//get base address
	pMem = (CX_Byte *)pPtr;
	pMem -= sizeof(CX_Byte);
	pMem -= sizeof(CX_Size);
	HeapFree(g_cx_mem_heap, 0, pMem);
}

void *CX_MemReallocHelper(CX_Bool bTrack, void *pPtr, CX_Size cbSize)
{
	CX_MemFrame frames[CX_MEMSTATS_MAX_CALLS_COUNT];
	CX_Size     cbTotalSize;
	CX_Byte     cFrames;
	CX_Byte     *pMem;
	CX_Byte     *pOldMem;

	if (NULL == pPtr)
	{
		return CX_MemAllocHelper(bTrack, cbSize);
	}
	pOldMem = (CX_Byte *)pPtr;
	pOldMem -= sizeof(CX_Byte);
	pOldMem -= sizeof(CX_Size);
	cbTotalSize = CX_MemGetTotalSize(bTrack, cbSize, frames, &cFrames);
	if (NULL == (pMem = (CX_Byte *)HeapReAlloc(g_cx_mem_heap, 0, pOldMem, cbTotalSize)))
	{
		return NULL;
	}

	return CX_MemSetBlockData(pMem, cbSize, frames, cFrames);
}

void *CX_MemOptAlloc(CX_Size cbSize)
{
	return CX_MemAllocHelper(false, cbSize);
}

void *CX_MemOptRealloc(void *pPtr, CX_Size cbSize)
{
	return CX_MemReallocHelper(false, pPtr, cbSize);
}

void CX_MemOptFree(void *pPtr)
{
	CX_MemFreeHelper(false, pPtr);
}

void *CX_MemDbgAlloc(CX_Size cbSize)
{
	return CX_MemAllocHelper(true, cbSize);
}

void *CX_MemDbgRealloc(void *pPtr, CX_Size cbSize)
{
	return CX_MemReallocHelper(true, pPtr, cbSize);
}

void CX_MemDbgFree(void *pPtr)
{
	CX_MemFreeHelper(true, pPtr);
}

void *CX_MemAlloc(CX_Size cbSize)
{
	if (g_cx_mem_track)
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
	if (g_cx_mem_track)
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
	if (g_cx_mem_track)
	{
		CX_MemDbgFree(pPtr);
	}
	else
	{
		CX_MemOptFree(pPtr);
	}
}

CX_Bool CX_MemGetFrameInfo(void *pCallStack, const CX_Char **pszFunctionName, const CX_Char **pszFileName, CX_Size *pcLine)
{
	CX_MemCallStack   *pCS = (CX_MemCallStack *)pCallStack;
	unsigned char     symbuf[sizeof(SYMBOL_INFO) + CX_MEMSTATS_MAX_FUNCTION_NAME_LEN - 1];
	SYMBOL_INFO       *pSym = (SYMBOL_INFO *)symbuf;
	DWORD             dwDisplacement;
	IMAGEHLP_LINE64   line;

	if (NULL == pCS)
	{
		return CX_False;
	}
	if (pCS->cFrameIndex >= pCS->cFramesCount)
	{
		return CX_False;
	}
	pSym->MaxNameLen   = sizeof(symbuf) - sizeof(sizeof(SYMBOL_INFO));
	pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
	if (0 == pCS->cFrameIndex)
	{
		CX_Mem_SymInit();
	}
	while (pCS->cFrameIndex < pCS->cFramesCount)
	{
		*pszFunctionName = "";
		*pszFileName = "";
		*pcLine = 0;
		if (SymFromAddr(GetCurrentProcess(), (DWORD64)(pCS->callstack[pCS->cFrameIndex]), 0, pSym))
		{
			*pszFunctionName = pSym->Name;
			if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)(pCS->callstack[pCS->cFrameIndex]), &dwDisplacement, &line))
			{
				*pszFileName = line.FileName;
				*pcLine      = line.LineNumber;
			}
		}
		pCS->cFrameIndex++;
		if (0 == cx_strcmp(*pszFunctionName, "CX_MemGetFrames") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemGetTotalSize") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemAllocHelper") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemReallocHelper") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemFreeHelper") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemDbgAlloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemDbgRealloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemDbgFree") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemOptAlloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemOptRealloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemOptFree") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemAlloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemRealloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX_MemFree") || 
		    0 == cx_strcmp(*pszFunctionName, "CX::Mem::Alloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX::Mem::Realloc") || 
		    0 == cx_strcmp(*pszFunctionName, "CX::Mem::Free") || 
		    0 == cx_strncmp(*pszFunctionName, "CX::Mem::New<", 13) || 
		    0 == cx_strncmp(*pszFunctionName, "CX::Mem::NewArr<", 16) || 
		    0 == cx_strncmp(*pszFunctionName, "CX::Mem::Delete<", 16) || 
		    0 == cx_strncmp(*pszFunctionName, "CX::Mem::DeleteArr<", 19))
		{
			continue;
		}

		break;
	}

	return CX_True;
}

void CX_SysLogMemAllocHandler(void *pMem, CX_Size cbSize, void *pCallStack)
{
	CX_Char       szBuffer[4096];
	CX_Size       cBufferLen = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const CX_Char *szFunctionName;
	const CX_Char *szFileName;
	CX_Size       cLine;

	cx_snprintf(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0]), "\n%u bytes at 0x%p\n", cbSize, pMem);
	szBuffer[cBufferLen - 1] = 0;
	OutputDebugStringA(szBuffer);
	if (NULL != pCallStack)
	{
		while (CX_MemGetFrameInfo(pCallStack, &szFunctionName, &szFileName, &cLine))
		{
			cx_snprintf(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0]), " - %s (%s:%u)\n", szFunctionName, szFileName, cLine);
			szBuffer[cBufferLen - 1] = 0;
			OutputDebugStringA(szBuffer);
		}
	}
	else
	{
		OutputDebugStringA(" - no call stack available\n");
	}
}

void CX_StdOutMemAllocHandler(void *pMem, CX_Size cbSize, void *pCallStack)
{
	CX_Char       szBuffer[4096];
	CX_Size       cBufferLen = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const CX_Char *szFunctionName;
	const CX_Char *szFileName;
	CX_Size       cLine;

	cx_snprintf(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0]), "\n%u bytes at 0x%p\n", cbSize, pMem);
	szBuffer[cBufferLen - 1] = 0;
	printf(szBuffer);
	if (NULL != pCallStack)
	{
		while (CX_MemGetFrameInfo(pCallStack, &szFunctionName, &szFileName, &cLine))
		{
			cx_snprintf(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0]), " - %s (%s:%u)\n", szFunctionName, szFileName, cLine);
			szBuffer[cBufferLen - 1] = 0;
			printf(szBuffer);
		}
	}
	else
	{
		printf(" - no call stack available\n");
	}
}

void CX_MemEnumAllocs(CX_MemAllocHandler pfnMemAllocHandler)
{
	CX_Byte            *pMem;
	CX_Size            cbSize;
	CX_Byte            cFrames;
	CX_MemCallStack    callstack;
	PROCESS_HEAP_ENTRY entry;
	CX_Bool            bFirst;

	if (NULL == g_cx_mem_heap)
	{
		CX_Mem_Init();
	}
	if (NULL == pfnMemAllocHandler)
	{
		pfnMemAllocHandler = &CX_SysLogMemAllocHandler;
	}
#pragma warning(suppress: 6387)
	if (HeapLock(g_cx_mem_heap))
	{
		bFirst       = CX_True;
		entry.lpData = NULL;
#pragma warning(suppress: 6387)
		while (HeapWalk(g_cx_mem_heap, &entry))
		{
			if ((PROCESS_HEAP_ENTRY_BUSY & entry.wFlags))
			{
				if (bFirst)
				{
					bFirst = CX_False;
					OutputDebugStringA("\nAllocated memory blocks:\n");
				}
				pMem = (CX_Byte *)entry.lpData;
#pragma warning(suppress: 6387)
				memcpy(&cbSize, pMem, sizeof(CX_Size));
				pMem += sizeof(CX_Size);
				memcpy(&cFrames, pMem, sizeof(CX_Byte));
				pMem += sizeof(CX_Byte);
				pMem += cbSize;
				if (0 < cFrames)
				{
					callstack.callstack    = (CX_MemFrame *)pMem;
					callstack.cFrameIndex  = 0;
					callstack.cFramesCount = cFrames;
					pfnMemAllocHandler(pMem, cbSize, &callstack);
				}
				else
				{
					pfnMemAllocHandler(pMem, cbSize, NULL);
				}
			}
		}
#pragma warning(suppress: 6387)
		HeapUnlock(g_cx_mem_heap);
		if (!bFirst)
		{
			OutputDebugStringA("\n");
		}
	}
}

#endif

