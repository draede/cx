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
 
#pragma once


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "CX/CBuffers/Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _CX_CB_Environment
{
	void *(* MemAlloc)(struct _CX_CB_Environment *pThis, CX_CB_Size cbSize);
	void *(* MemRealloc)(struct _CX_CB_Environment *pThis, void *pPtr, CX_CB_Size cbSize);
	CX_StatusCode (* MemFree)(struct _CX_CB_Environment *pThis, void *pPtr);
	void (* MemSet)(struct _CX_CB_Environment *pThis, void *pPtr, CX_UInt8 value, CX_CB_Size cbSize);
	void (* MemCpy)(struct _CX_CB_Environment *pThis, void *pDest, const void *pSrc, CX_CB_Size cbSize);
	CX_CB_Size (* StrLen)(struct _CX_CB_Environment *pThis, const CX_Char *szStr);
	CX_CB_Size (* WStrLen)(struct _CX_CB_Environment *pThis, const CX_WChar *wszStr);
	CX_StatusCode (*Destroy)(struct _CX_CB_Environment *pThis);
}CX_CB_Environment;


#ifdef __cplusplus
}
#endif
