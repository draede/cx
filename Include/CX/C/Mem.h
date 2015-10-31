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


CX_API void *CX_MemAlloc(CX_Size cbSize);

CX_API void *CX_MemRealloc(void *pPtr, CX_Size cbSize);

CX_API void CX_MemFree(void *pPtr);

CX_API void *CX_MemOptAlloc(CX_Size cbSize);

CX_API void *CX_MemOptRealloc(void *pPtr, CX_Size cbSize);

CX_API void CX_MemOptFree(void *pPtr);

CX_API void CX_MemSetTrack(CX_Bool bTrackMem);

CX_API CX_Bool CX_MemGetTrack();

CX_API void CX_MemSetDumpAllocs(CX_Bool bDumpAllocs);

CX_API CX_Bool CX_MemGetDumpAllocs();

typedef void (* CX_MemAllocHandler)(void *pMem, CX_Size cbSize, void *pCallStack);

CX_API void CX_SysLogMemAllocHandler(void *pMem, CX_Size cbSize, void *pCallStack);

CX_API void CX_StdOutMemAllocHandler(void *pMem, CX_Size cbSize, void *pCallStack);

CX_API void CX_MemEnumAllocs(CX_MemAllocHandler pfnMemAllocHandler);

//call this from your CX_MemAllocHandler
CX_API CX_Bool CX_MemGetFrameInfo(void *pCallStack, const CX_Char **pszFunctionName, const CX_Char **pszFileName, 
                                  CX_Size *pcLine);

#ifdef __cplusplus
}
#endif

