
#pragma once


#include "CX/C/Types.h"


namespace CX
{

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

// wchar_t type
typedef CX_WChar               WChar;

//platform specific mem buffer size type (4 bytes on 32 bit, 8 bytes on 64 bit)
typedef CX_Size                Size;

// pointer diff
typedef CX_PtrDiff             PtrDiff;

// status code
typedef CX_StatusCode          StatusCode;

}//namespace CX

