
#pragma once


#include "CX/C/Types.h"
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif


//min Int8 value
static const CX_Int8          CX_INT8_MIN    = -127;

//max Int8 value
static const CX_Int8          CX_INT8_MAX    = 127;

//max UInt8 value
static const CX_UInt8         CX_UINT8_MAX   = 255;

//min Int16 value
static const CX_Int16         CX_INT16_MIN   = -32767;

//max Int16 value
static const CX_Int16         CX_INT16_MAX   = 32767;

//max UInt16 value
static const CX_UInt16        CX_UINT16_MAX  = 65535;

//min Int32 value
static const CX_Int32         CX_INT32_MIN   = -2147483647;

//max Int32 value
static const CX_Int32         CX_INT32_MAX   = 2147483647;

//max UInt32 value
static const CX_UInt32        CX_UINT32_MAX   = 4294967295;

//min Int64 value
static const CX_Int64         CX_INT64_MIN   = -9223372036854775807;

//max Int64 value
static const CX_Int64         CX_INT64_MAX   = 9223372036854775807;

//max UInt64 value
static const CX_UInt64        CX_UINT64_MAX  = 18446744073709551615;

//max Byte value
static const CX_Byte          CX_BYTE_MAX    = 255;

#ifdef CX_64BIT_ARCH
static const CX_Size          CX_SIZE_MAX    = 18446744073709551615;
#else
static const CX_Size          CX_SIZE_MAX    = 4294967295;
#endif

#ifdef __cplusplus
}
#endif

