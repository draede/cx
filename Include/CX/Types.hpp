/* 
 * CX - C++ framework for general purpose development
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


#include "CX/C/Types.h"


namespace CX
{

// bool
typedef CX_Bool                Bool;

static const Bool False = CX_False;
static const Bool True  = CX_True;

// signed 1 byte int type
typedef CX_Int8                Int8;

// unsigned 1 byte int type
typedef CX_UInt8               UInt8;

// signed 2 bytes int type
typedef CX_Int16               Int16;

// unsigned 2 bytes int type
typedef CX_UInt16              UInt16;

// signed 4 bytes int type
typedef CX_Int32               Int32;

// unsigned 4 bytes int type
typedef CX_UInt32              UInt32;

// signed 8 bytes int type
typedef CX_Int64               Int64;

// unsigned 8 bytes int type
typedef CX_UInt64              UInt64;

//single precision floating-point type (4 bytes)
typedef CX_Float               Float;

//double precision floating-point type (8 bytes)
typedef CX_Double              Double;

// byte type (unsigned 1 byte int type)
typedef CX_Byte                Byte;

// char type
typedef CX_Char                Char;

// unsigned char type
typedef CX_UChar               UChar;

// wchar_t type
typedef CX_WChar               WChar;

//platform specific mem buffer size type (4 bytes on 32 bit, 8 bytes on 64 bit)
typedef CX_Size                Size;

// intptr
typedef CX_IntPtr              IntPtr;

// intptr
typedef CX_UIntPtr             UIntPtr;

// pointer diff
typedef CX_PtrDiff             PtrDiff;

// status code
typedef CX_StatusCode          StatusCode;

}//namespace CX

