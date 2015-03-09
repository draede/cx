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


typedef enum _CX_BINJSON_State
{
	CX_BINJSON_State_None,
	CX_BINJSON_State_RootObject,
	CX_BINJSON_State_RootArray,
	CX_BINJSON_State_Object,
	CX_BINJSON_State_Array,
}CX_BINJSON_State;

typedef void * (* CX_BINJSON_Alloc_Func)(void *pUserContext, CX_Size cbSize);

typedef void * (* CX_BINJSON_Realloc_Func)(void *pUserContext, void *pPtr, CX_Size cbSize);

typedef void ( *CX_BINJSON_Free_Func)(void *pUserContext, void *pPtr);

typedef struct _CX_BINJSON_HelperAPI
{
	CX_BINJSON_Alloc_Func         Alloc;
	CX_BINJSON_Realloc_Func       Realloc;
	CX_BINJSON_Free_Func          Free;
}CX_BINJSON_HelperAPI;

static const CX_UInt64    CX_BINJSON_MAGIC                = 0x314E4F534A4E4942; //BINJSON1

static const CX_UInt32    CX_BINJSON_ENCODING_NONE        = 0x0; //no encoding

static const CX_Byte      CX_BINJSON_TYPE_BEGINROOTOBJECT = '<';
static const CX_Byte      CX_BINJSON_TYPE_ENDROOTOBJECT   = '>';
static const CX_Byte      CX_BINJSON_TYPE_BEGINROOTARRAY  = '(';
static const CX_Byte      CX_BINJSON_TYPE_ENDROOTARRAY    = ')';
static const CX_Byte      CX_BINJSON_TYPE_BEGINOBJECT     = '{';
static const CX_Byte      CX_BINJSON_TYPE_ENDOBJECT       = '}';
static const CX_Byte      CX_BINJSON_TYPE_BEGINARRAY      = '[';
static const CX_Byte      CX_BINJSON_TYPE_ENDARRAY        = ']';
static const CX_Byte      CX_BINJSON_TYPE_NULL            = 'N';
static const CX_Byte      CX_BINJSON_TYPE_FALSE           = 'F';
static const CX_Byte      CX_BINJSON_TYPE_TRUE            = 'T';
static const CX_Byte      CX_BINJSON_TYPE_INT             = 'I';
static const CX_Byte      CX_BINJSON_TYPE_REAL            = 'R';
static const CX_Byte      CX_BINJSON_TYPE_STRING          = 'S';
static const CX_Byte      CX_BINJSON_TYPE_WSTRING         = 'W';
static const CX_Byte      CX_BINJSON_TYPE_BLOB            = 'O';


#ifdef __cplusplus
}
#endif

