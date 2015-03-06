/* 
 * CX - C++ framework for general purpose developement
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
#include "CX/C/StatusCodes.h"
#include "../../xxHash/Include/xxhash.h"
#include "BINJSON.h"


#define CX_BINJSON_WRITER_MAX_DEPTH 1024

typedef CX_StatusCode (* CX_BINJSON_Writer_Write_Func)(void *pUserContext, const void *pData, 
                                                       CX_Size cbSize);

typedef struct _CX_BINJSON_Writer
{
	CX_Byte                        stack[CX_BINJSON_WRITER_MAX_DEPTH];
	CX_Size                        cDepth;
	void                           *pUserContext;
	CX_BINJSON_Writer_Write_Func   pfnWrite;
	CX_BINJSON_HelperAPI           api;
	XXH32_state_t                  hash;
}CX_BINJSON_Writer;

CX_StatusCode CX_BINJSON_Writer_Init(CX_BINJSON_Writer *pWriter, void *pUserContext, 
                                     CX_BINJSON_HelperAPI *pHelperAPI, 
                                     CX_BINJSON_Writer_Write_Func pfnWrite);

CX_StatusCode CX_BINJSON_Writer_BeginRootObject(CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndRootObject(CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_BeginRootArray(CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndRootArray(CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteNull(CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteNull(CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBool(CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, CX_Bool bValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBool(CX_BINJSON_Writer *pWriter, CX_Bool bValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteInt(CX_BINJSON_Writer *pWriter, 
                                            const CX_Char *szName, CX_Int64 nValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteInt(CX_BINJSON_Writer *pWriter, CX_Int64 nValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteReal(CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, CX_Double lfValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteReal(CX_BINJSON_Writer *pWriter, CX_Double lfValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteString(CX_BINJSON_Writer *pWriter, 
                                               const CX_Char *szName, const CX_Char *szValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteString(CX_BINJSON_Writer *pWriter, 
                                            const CX_Char *szValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteWString(CX_BINJSON_Writer *pWriter, 
                                                const CX_Char *szName, const CX_WChar *wszValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteWString(CX_BINJSON_Writer *pWriter, 
                                             const CX_WChar *wszValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBLOB(CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, const void *pData, 
                                             CX_Size cbSize);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBLOB(CX_BINJSON_Writer *pWriter, 
                                          const void *pData, CX_Size cbSize);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginObject(CX_BINJSON_Writer *pWriter, 
                                               const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginObject(CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginArray(CX_BINJSON_Writer *pWriter, 
                                              const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginArray(CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndObject(CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndArray(CX_BINJSON_Writer *pWriter);


#ifdef __cplusplus
}
#endif

