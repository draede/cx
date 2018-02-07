/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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
 
#include "CX/CBuffers/Struct.h"


CX_StatusCode CX_CB_Struct_Init(CX_CB_Struct *pStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, 
                                CX_CB_Size cbStartOffset)
{
	pStruct->pEnv          = pEnv;
	pStruct->pBuffer       = pBuffer;
	pStruct->cbStartOffset = cbStartOffset;

	return CX_Status_OK;
}

CX_CB_Environment *CX_CB_Struct_GetEnvironment_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset)
{
	CX_CB_Struct *pStruct;

	if (NULL == pBaseStruct)
	{
		return NULL;
	}
	pStruct = (CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset);

	return pStruct->pEnv;
}

CX_CB_Buffer *CX_CB_Struct_GetBuffer_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset)
{
	CX_CB_Struct *pStruct;

	if (NULL == pBaseStruct)
	{
		return NULL;
	}
	pStruct = (CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset);

	return pStruct->pBuffer;
}

CX_CB_Size CX_CB_Struct_GetStartOffset_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset)
{
	CX_CB_Struct *pStruct;

	if (NULL == pBaseStruct)
	{
		return 0;
	}
	pStruct = (CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset);

	return pStruct->cbStartOffset;
}


CX_StatusCode CX_CB_Struct_GetMember(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size cbSize, 
                                     const void **ppMember)
{
	if (pStruct->cbStartOffset + cbOffset + cbSize > pStruct->pBuffer->cbUsedSize)
	{
		return CX_Status_OutOfBounds;
	}
	*ppMember = (CX_Byte *)pStruct->pBuffer->pData + pStruct->cbStartOffset + cbOffset;

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_GetMember_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        CX_CB_Size cbSize, const void **ppMember)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_GetMember((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset,
	                              cbSize, ppMember);
}

CX_StatusCode CX_CB_Struct_SetStatic(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, const void *pValue, CX_CB_Size cbSize)
{
	CX_Byte         *pMember;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, cbSize, &pMember)))
	{
		return nStatus;
	}
	pStruct->pEnv->MemCpy(pStruct->pEnv, pMember, pValue, cbSize);

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_SetStatic_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        const void *pValue, CX_CB_Size cbSize)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_SetStatic((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset, pValue, cbSize);
}

CX_StatusCode CX_CB_Struct_GetStatic(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, void **ppValue, CX_CB_Size cbSize)
{
	CX_Byte         *pMember;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, cbSize, &pMember)))
	{
		return nStatus;
	}
	*ppValue = pMember;

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_GetStatic_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        void **ppValue, CX_CB_Size cbSize)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_GetStatic((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset, ppValue, cbSize);
}


CX_StatusCode CX_CB_Struct_GetArrayItemCount(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size *pcItemCount)
{
	CX_Byte         *pMember;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, sizeof(CX_CB_Size) * 2, &pMember)))
	{
		return nStatus;
	}
	if (0 == ((CX_CB_Size *)pMember)[0])
	{
		return CX_Status_InvalidCall;
	}
	*pcItemCount = ((CX_CB_Size *)pMember)[1];

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_GetArrayItemCount_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                                CX_CB_Size *pcItemCount)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_GetArrayItemCount((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset, 
	                                      pcItemCount);
}

CX_StatusCode CX_CB_Struct_SetArrayItemCount(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size cbItemSize, 
                                             CX_CB_Size cItemCount)
{
	CX_Byte         *pMember;
	CX_CB_Size      cbArrayOffset;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, sizeof(CX_CB_Size) * 2, &pMember)))
	{
		return nStatus;
	}
	if (0 != ((CX_CB_Size *)pMember)[0])
	{
		return CX_Status_InvalidCall;
	}
	cbArrayOffset = pStruct->pBuffer->cbUsedSize;
	if (CXNOK(nStatus = CX_CB_Buffer_AddSize(pStruct->pBuffer, cbItemSize * cItemCount, pStruct->pEnv)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, sizeof(CX_CB_Size) * 2, &pMember)))
	{
		return nStatus;
	}
	pStruct->pEnv->MemSet(pStruct->pEnv, (CX_Byte *)pStruct->pBuffer->pData + cbArrayOffset, 0, cbItemSize * cItemCount);
	((CX_CB_Size *)pMember)[0] = cbArrayOffset;
	((CX_CB_Size *)pMember)[1] = cItemCount;

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_SetArrayItemCount_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                                CX_CB_Size cbItemSize, CX_CB_Size cItemCount)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_SetArrayItemCount((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset, 
	                                      cbItemSize, cItemCount);
}

CX_StatusCode CX_CB_Struct_GetArrayItems(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, void **ppValues)
{
	CX_Byte         *pMember;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetMember(pStruct, cbOffset, sizeof(CX_CB_Size) * 2, &pMember)))
	{
		return nStatus;
	}
	if (0 == ((CX_CB_Size *)pMember)[0])
	{
		return CX_Status_InvalidCall;
	}
	*ppValues = (CX_Byte *)pStruct->pBuffer->pData + ((CX_CB_Size *)pMember)[0];

	return CX_Status_OK;
}

CX_StatusCode CX_CB_Struct_GetArrayItems_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                            void **ppValues)
{
	if (NULL == pBaseStruct)
	{
		return CX_Status_InvalidArg;
	}

	return CX_CB_Struct_GetArrayItems((CX_CB_Struct *)((CX_Byte *)pBaseStruct + cbStructOffset), cbOffset, ppValues);
}

