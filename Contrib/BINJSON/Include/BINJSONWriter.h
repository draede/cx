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
#include "BINJSON.h"


struct _CX_BINJSON_Writer;


typedef CX_StatusCode (* CX_BINJSON_Writer_Write_Func)(void *pUserContext, const void *pData, 
                                                       CX_Size cbSize);

CX_StatusCode CX_BINJSON_Writer_Init(struct _CX_BINJSON_Writer *pWriter, void *pUserContext, 
                                     CX_BINJSON_HelperAPI *pHelperAPI, 
                                     CX_BINJSON_Writer_Write_Func pfnWrite);

CX_StatusCode CX_BINJSON_Writer_BeginRootObject(struct _CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndRootObject(struct _CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_BeginRootArray(struct _CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndRootArray(struct _CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteNull(struct _CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteNull(struct _CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBool(struct _CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, CX_Bool bValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBool(struct _CX_BINJSON_Writer *pWriter, CX_Bool bValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteInt(struct _CX_BINJSON_Writer *pWriter, 
                                            const CX_Char *szName, CX_Int64 nValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteInt(struct _CX_BINJSON_Writer *pWriter, CX_Int64 nValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteReal(struct _CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, CX_Double lfValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteReal(struct _CX_BINJSON_Writer *pWriter, CX_Double lfValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteString(struct _CX_BINJSON_Writer *pWriter, 
                                               const CX_Char *szName, const CX_Char *szValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteString(struct _CX_BINJSON_Writer *pWriter, 
                                            const CX_Char *szValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteWString(struct _CX_BINJSON_Writer *pWriter, 
                                                const CX_Char *szName, const CX_WChar *wszValue);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteWString(struct _CX_BINJSON_Writer *pWriter, 
                                             const CX_WChar *wszValue);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBLOB(struct _CX_BINJSON_Writer *pWriter, 
                                             const CX_Char *szName, const void *pData, 
                                             CX_Size cbSize);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBLOB(struct _CX_BINJSON_Writer *pWriter, 
                                          const void *pData, CX_Size cbSize);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginObject(struct _CX_BINJSON_Writer *pWriter, 
                                               const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginObject(struct _CX_BINJSON_Writer *pWriter);

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginArray(struct _CX_BINJSON_Writer *pWriter, 
                                              const CX_Char *szName);

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginArray(struct _CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndObject(struct _CX_BINJSON_Writer *pWriter);

CX_StatusCode CX_BINJSON_Writer_EndArray(struct _CX_BINJSON_Writer *pWriter);


#ifdef __cplusplus
}
#endif

