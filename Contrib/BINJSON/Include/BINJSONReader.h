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


typedef enum _CX_BINJSON_EntryType
{
	CX_BINJSON_EntryType_Invalid, // == ERROR
	CX_BINJSON_EntryType_EOG,     // == End of group
	CX_BINJSON_EntryType_Null,
	CX_BINJSON_EntryType_Bool,
	CX_BINJSON_EntryType_Int,
	CX_BINJSON_EntryType_Real,
	CX_BINJSON_EntryType_String,
	CX_BINJSON_EntryType_WString,
	CX_BINJSON_EntryType_BLOB,
	CX_BINJSON_EntryType_Object,
	CX_BINJSON_EntryType_Array,
}CX_BINJSON_EntryType;

#define CX_BINJSON_READER_MAX_DEPTH 1024

typedef CX_StatusCode (* CX_BINJSON_Reader_Read_Func)(void *pUserContext, void *pData, 
                                                      CX_Size cbSize);

typedef struct _CX_BINJSON_Reader
{
	CX_Byte                        stack[CX_BINJSON_READER_MAX_DEPTH];
	CX_Size                        cDepth;
	CX_Byte                        nRootEntryType;
	CX_Byte                        nCrEntryType;
	void                           *pUserContext;
	CX_BINJSON_Reader_Read_Func    pfnRead;
	CX_BINJSON_HelperAPI           api;
	XXH32_state_t                  hash;
}CX_BINJSON_Reader;

#define CX_BINJSON_READER_STATIC_STRING_MAXLEN    64

typedef struct _CX_BINJSON_Reader_String
{
	CX_Char buffer[CX_BINJSON_READER_STATIC_STRING_MAXLEN + 1]; //do not use
	CX_Char *pString;
}CX_BINJSON_Reader_String;

typedef struct _CX_BINJSON_Reader_WString
{
	CX_WChar buffer[CX_BINJSON_READER_STATIC_STRING_MAXLEN + 1]; //do not use
	CX_WChar *pWString;
}CX_BINJSON_Reader_WString;


CX_StatusCode CX_BINJSON_Reader_Init(CX_BINJSON_Reader *pReader, void *pUserContext, 
                                     CX_BINJSON_HelperAPI *pHelperAPI, 
                                     CX_BINJSON_Reader_Read_Func pfnRead);

CX_StatusCode CX_BINJSON_Reader_FreeString(CX_BINJSON_Reader *pReader,
                                           CX_BINJSON_Reader_String *pString);

CX_StatusCode CX_BINJSON_Reader_FreeWString(CX_BINJSON_Reader *pReader,
                                            CX_BINJSON_Reader_WString *pWString);

CX_StatusCode CX_BINJSON_Reader_FreeBLOB(CX_BINJSON_Reader *pReader, void *pPtr);

CX_BINJSON_EntryType CX_BINJSON_Reader_GetRootEntryType(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_BeginRootObject(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_EndRootObject(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_BeginRootArray(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_EndRootArray(CX_BINJSON_Reader *pReader);

CX_BINJSON_EntryType CX_BINJSON_Reader_GetEntryType(CX_BINJSON_Reader *pReader);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadNull(CX_BINJSON_Reader *pReader, 
                                           CX_BINJSON_Reader_String *psName);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadNull(CX_BINJSON_Reader *pReader);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadBool(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, CX_Bool *pbValue);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadBool(CX_BINJSON_Reader *pReader, CX_Bool *pbValue);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadInt(CX_BINJSON_Reader *pReader, 
                                           CX_BINJSON_Reader_String *psName, CX_Int64 *pnValue);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadInt(CX_BINJSON_Reader *pReader, CX_Int64 *pnValue);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadReal(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, CX_Double *plfValue);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadReal(CX_BINJSON_Reader *pReader, CX_Double *plfValue);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadString(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psName, 
                                              CX_BINJSON_Reader_String *psValue);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadString(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psValue);

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadWString(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psName, 
                                              CX_BINJSON_Reader_WString *pwsValue);

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadWString(CX_BINJSON_Reader *pReader, 
                                               CX_BINJSON_Reader_WString *pwsValue);

//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
CX_StatusCode CX_BINJSON_Reader_ObjReadBLOB(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, 
                                            void **ppData, CX_Size *pcbSize);

//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
CX_StatusCode CX_BINJSON_Reader_ArrReadBLOB(CX_BINJSON_Reader *pReader, 
                                            void **ppData, CX_Size *pcbSize);

//object member
CX_StatusCode CX_BINJSON_Reader_ObjBeginObject(CX_BINJSON_Reader *pReader, 
                                               CX_BINJSON_Reader_String *psName);

//array item
CX_StatusCode CX_BINJSON_Reader_ArrBeginObject(CX_BINJSON_Reader *pReader);

//object member
CX_StatusCode CX_BINJSON_Reader_ObjBeginArray(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psName);

//array item
CX_StatusCode CX_BINJSON_Reader_ArrBeginArray(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_EndObject(CX_BINJSON_Reader *pReader);

CX_StatusCode CX_BINJSON_Reader_EndArray(CX_BINJSON_Reader *pReader);


#ifdef __cplusplus
}
#endif

