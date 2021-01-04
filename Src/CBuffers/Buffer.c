/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
 
#include "CX/CBuffers/Buffer.h"


CX_StatusCode CX_CB_Buffer_Init(CX_CB_Buffer *pBuffer)
{
	pBuffer->pData       = NULL;
	pBuffer->cbUsedSize  = 0;
	pBuffer->cbTotalSize = 0;

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Buffer_SetSize(CX_CB_Buffer *pBuffer, CX_CB_Size cbSize, CX_CB_Environment *pEnv)
{
	CX_CB_Size   cbTotalSize;
	void         *pData;

	if (NULL == pBuffer)
	{
		return CX_Status_InvalidArg;
	}
	if (cbSize < pBuffer->cbTotalSize)
	{
		pBuffer->cbUsedSize = cbSize;

		return CX_Status_OK;
	}
	if (NULL == pEnv)
	{
		return CX_Status_InvalidArg;
	}
	if (0 == pBuffer->cbTotalSize)
	{
		cbTotalSize = CX_CB_INITIAL_BUFFER_SIZE;
	}
	else
	{
		cbTotalSize = pBuffer->cbTotalSize;
	}
	while (cbTotalSize < cbSize)
	{
		cbTotalSize *= 2;
	}
	if (NULL == pBuffer->pData)
	{
		if (NULL == (pData = pEnv->MemAlloc(pEnv, cbTotalSize)))
		{
			return CX_Status_MemAllocFailed;
		}
	}
	else
	{
		if (NULL == (pData = pEnv->MemRealloc(pEnv, pBuffer->pData, cbTotalSize)))
		{
			return CX_Status_MemAllocFailed;
		}
	}
	pBuffer->pData       = pData;
	pBuffer->cbUsedSize  = cbSize;
	pBuffer->cbTotalSize = cbTotalSize;

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Buffer_AddSize(CX_CB_Buffer *pBuffer, CX_CB_Size cbSize, CX_CB_Environment *pEnv)
{
	if (NULL == pBuffer)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Buffer_SetSize(pBuffer, pBuffer->cbUsedSize + cbSize, pEnv);
}

CX_StatusCode CX_CB_Buffer_Uninit(CX_CB_Buffer *pBuffer, CX_CB_Environment *pEnv)
{
	if (NULL == pBuffer)
	{
		return CX_Status_InvalidArg;
	}
	if (NULL != pBuffer->pData)
	{
		if (NULL == pEnv)
		{
			return CX_Status_InvalidArg;
		}
		pEnv->MemFree(pEnv, pBuffer->pData);
	}
	pBuffer->pData       = NULL;
	pBuffer->cbUsedSize  = 0;
	pBuffer->cbTotalSize = 0;

	return CX_Status_OK;
}
