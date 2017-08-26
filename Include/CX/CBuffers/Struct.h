/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/CBuffers/Types.h"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/Environment.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _CX_CB_Struct
{
	CX_CB_Environment   *pEnv;
	CX_CB_Buffer        *pBuffer;
	CX_CB_Size          cbStartOffset;
}CX_CB_Struct;


CX_StatusCode CX_CB_Struct_Init(CX_CB_Struct *pStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, 
                                CX_CB_Size cbStartOffset);

CX_CB_Environment *CX_CB_Struct_GetEnvironment_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset);

CX_CB_Buffer *CX_CB_Struct_GetBuffer_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset);

CX_CB_Size CX_CB_Struct_GetStartOffset_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset);

CX_StatusCode CX_CB_Struct_GetMember(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size cbSize, 
                                     const void **ppMember);

CX_StatusCode CX_CB_Struct_GetMember_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        CX_CB_Size cbSize, const void **ppMember);

CX_StatusCode CX_CB_Struct_SetStatic(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, const void *pValue, CX_CB_Size cbSize);

CX_StatusCode CX_CB_Struct_SetStatic_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        const void *pValue, CX_CB_Size cbSize);

CX_StatusCode CX_CB_Struct_GetStatic(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, void **ppValue, CX_CB_Size cbSize);

CX_StatusCode CX_CB_Struct_GetStatic_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                        void **ppValue, CX_CB_Size cbSize);

CX_StatusCode CX_CB_Struct_GetArrayItemCount(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size *pcItemCount);

CX_StatusCode CX_CB_Struct_GetArrayItemCount_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                                CX_CB_Size *pcItemCount);

CX_StatusCode CX_CB_Struct_SetArrayItemCount(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, CX_CB_Size cbItemSize, 
                                             CX_CB_Size cItemCount);

CX_StatusCode CX_CB_Struct_SetArrayItemCount_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                                CX_CB_Size cbItemSize, CX_CB_Size cItemCount);

CX_StatusCode CX_CB_Struct_GetArrayItems(CX_CB_Struct *pStruct, CX_CB_Size cbOffset, void **ppValues);

CX_StatusCode CX_CB_Struct_GetArrayItems_Ex(void *pBaseStruct, CX_CB_Size cbStructOffset, CX_CB_Size cbOffset, 
                                            void **ppValues);

#ifdef __cplusplus
}
#endif
