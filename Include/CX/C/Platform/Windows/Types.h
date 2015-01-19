
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#ifdef __cplusplus
extern "C" {
#endif


#include <wchar.h>
#include <stddef.h>
#include <stdbool.h>


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

// wchar_t type
typedef wchar_t               CX_WChar;

// platform specific mem buffer size type (4 bytes on 32 bit, 8 bytes on 64 bit)
typedef size_t                CX_Size;

// pointer diff
typedef ptrdiff_t             CX_PtrDiff;

// status code
typedef CX_UInt32             CX_StatusCode;

#ifdef __cplusplus
}
#endif


#endif

