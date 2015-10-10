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

#pragma once


#include "CX/C/Platform.h"


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/APIDefs.h"


#define CX_MEMSTATS_MAX_CALLS_COUNT   60
#define CX_MEMSTATS_MAX_SYMBOL_LEN    256


typedef struct _CX_MemAllocCall
{
	CX_Char szFunction[CX_MEMSTATS_MAX_SYMBOL_LEN];
	CX_Char szFile[CX_MEMSTATS_MAX_SYMBOL_LEN];
	CX_Size cLineNumber;
}CX_MemAllocCall;

typedef struct _CX_MemAllocInfo
{
	void            *pPtr;
	CX_Size         cbSize;
	CX_Size         cCallsCount;
	CX_MemAllocCall *calls;
}CX_MemAllocInfo;

typedef void (* CX_MemStats_AllocStatsHandler)(const CX_MemAllocInfo *pInfo, void *pUsrCtx);

CX_API CX_StatusCode CX_MemStats_Activate();

CX_API CX_Bool CX_MemStats_IsActive();

CX_API CX_Size CX_MemStats_GetCurrentAllocsSize();

CX_API CX_Size CX_MemStats_GetCurrentAllocsCount();

CX_API CX_Size CX_MemStats_GetMaxAllocsSize();

CX_API CX_Size CX_MemStats_GetMaxAllocsCount();

CX_API void CX_MemStats_GetAllocs(CX_MemStats_AllocStatsHandler pfnHandler, void *pUsrCtx, 
                                  const CX_Char *exceptions[], CX_Size cExceptions,
                                  const CX_Char *ignoresUpper[], CX_Size cIgnoresUpper, 
                                  const CX_Char *ignoresLower[], CX_Size cIgnoresLower);

#ifdef __cplusplus
}
#endif

