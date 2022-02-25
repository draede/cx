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


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)


#ifdef __cplusplus
extern "C" {
#endif


#include <wchar.h>
#include <stddef.h>


// EXAMPLE for no default bool type
/*
#define CX_CUSTOM_BOOL_TYPE    unsigned char
#define CX_CUSTOM_BOOL_FALSE   0
#define CX_CUSTOM_BOOL_TRUE    1
*/

#ifdef CX_CUSTOM_BOOL_TYPE
typedef CX_CUSTOM_BOOL_TYPE     CX_Bool;
static const CX_Bool            CX_False = CX_CUSTOM_BOOL_FALSE;
static const CX_Bool            CX_True  = CX_CUSTOM_BOOL_TRUE;
#else
#include <stdbool.h>
typedef bool CX_Bool;
static const CX_Bool            CX_False = false;
static const CX_Bool            CX_True  = true;
#endif


// signed 1 byte int type
typedef signed __int8         CX_Int8;

// unsigned 1 byte int type
typedef unsigned __int8       CX_UInt8;

// signed 2 bytes int type
typedef __int16               CX_Int16;

// unsigned 2 bytes int type
typedef unsigned __int16      CX_UInt16;

// signed 4 bytes int type
typedef __int32               CX_Int32;

// unsigned 4 bytes int type
typedef unsigned __int32      CX_UInt32;

// signed 8 bytes int type
typedef __int64               CX_Int64;

// unsigned 8 bytes int type
typedef unsigned __int64      CX_UInt64;

//single precision floating-point type (4 bytes)
typedef float                 CX_Float;

//double precision floating-point type (8 bytes)
typedef double                CX_Double;

// byte type (unsigned 1 byte int type)
typedef unsigned __int8       CX_Byte;

// char type
typedef char                  CX_Char;

// unsigned char type
typedef unsigned char         CX_UChar;

// wchar_t type
typedef wchar_t               CX_WChar;

// platform specific mem buffer size type (4 bytes on 32 bit, 8 bytes on 64 bit)
typedef size_t                CX_Size;

//intptr
typedef intptr_t              CX_IntPtr;

//uintptr
typedef uintptr_t             CX_UIntPtr;

// pointer diff
typedef ptrdiff_t             CX_PtrDiff;

// status code
typedef CX_UInt32             CX_StatusCode;


#ifdef __cplusplus
}
#endif


#endif

