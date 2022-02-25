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
 
#include "CX/CBuffers/CLibEnvironment.h"
#include <stdlib.h>
#include <string.h>


typedef struct _CX_CB_CLibEnvironment
{
	CX_CB_Environment   base;
}CX_CB_CLibEnvironment;


static void *CX_CB_CLibEnvironment_MemAlloc(struct _CX_CB_Environment *pThis, CX_CB_Size cbSize)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return NULL;
	}

	return malloc(cbSize);
}

static void *CX_CB_CLibEnvironment_MemRealloc(struct _CX_CB_Environment *pThis, void *pPtr, CX_CB_Size cbSize)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return NULL;
	}

	return realloc(pPtr, cbSize);
}

static CX_StatusCode CX_CB_CLibEnvironment_MemFree(struct _CX_CB_Environment *pThis, void *pPtr)
{
	CX_UNUSED(pThis);
	
	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	free(pPtr);

	return CX_Status_OK;
}

static CX_StatusCode CX_CB_CLibEnvironment_MemSet(struct _CX_CB_Environment *pThis, void *pPtr, CX_UInt8 value, 
                                                  CX_CB_Size cbSize)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	memset(pPtr, value, cbSize);

	return CX_Status_OK;
}

static CX_StatusCode CX_CB_CLibEnvironment_MemCpy(struct _CX_CB_Environment *pThis, void *pDest, const void *pSrc, 
                                                  CX_CB_Size cbSize)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	memcpy(pDest, pSrc, cbSize);

	return CX_Status_OK;
}

static CX_CB_Size CX_CB_CLibEnvironment_StrLen(struct _CX_CB_Environment *pThis, const CX_Char *szStr)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return 0;
	}
	
	return (CX_CB_Size)strlen(szStr);
}

static CX_CB_Size CX_CB_CLibEnvironment_WStrLen(struct _CX_CB_Environment *pThis, const CX_WChar *wszStr)
{
	CX_UNUSED(pThis);

	if (NULL == pThis)
	{
		return 0;
	}

	return (CX_CB_Size)wcslen(wszStr);
}

static CX_StatusCode CX_CB_CLibEnvironment_Destroy(struct _CX_CB_Environment *pThis)
{
	CX_CB_CLibEnvironment *pEnv;

	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	pEnv = (CX_CB_CLibEnvironment *)pThis;
	free(pEnv);

	return CX_Status_OK;
}

CX_StatusCode CX_CB_CLibEnvironment_Create(CX_CB_Environment **ppEnv)
{
	CX_CB_CLibEnvironment *pEnv;

	if (NULL == (pEnv = (CX_CB_CLibEnvironment *)malloc(sizeof(CX_CB_CLibEnvironment))))
	{
		return CX_Status_MemAllocFailed;
	}
	pEnv->base.MemAlloc   = &CX_CB_CLibEnvironment_MemAlloc;
	pEnv->base.MemRealloc = &CX_CB_CLibEnvironment_MemRealloc;
	pEnv->base.MemFree    = &CX_CB_CLibEnvironment_MemFree;
	pEnv->base.MemSet     = &CX_CB_CLibEnvironment_MemSet;
	pEnv->base.MemCpy     = &CX_CB_CLibEnvironment_MemCpy;
	pEnv->base.StrLen     = &CX_CB_CLibEnvironment_StrLen;
	pEnv->base.WStrLen    = &CX_CB_CLibEnvironment_WStrLen;
	pEnv->base.Destroy    = &CX_CB_CLibEnvironment_Destroy;

	*ppEnv = (CX_CB_Environment *)pEnv;

	return CX_Status_OK;
}

