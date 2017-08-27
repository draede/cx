
#pragma once


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/Environment.h"
#include "CX/CBuffers/Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _My_Namespace_Struct1
{
	CX_StatusCode (* SetMyBool)(struct _My_Namespace_Struct1 *pThis, CX_Bool value);
	CX_Bool (* GetMyBool)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt8)(struct _My_Namespace_Struct1 *pThis, CX_Int8 value);
	CX_Int8 (* GetMyInt8)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt8)(struct _My_Namespace_Struct1 *pThis, CX_UInt8 value);
	CX_UInt8 (* GetMyUInt8)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt16)(struct _My_Namespace_Struct1 *pThis, CX_Int16 value);
	CX_Int16 (* GetMyInt16)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt16)(struct _My_Namespace_Struct1 *pThis, CX_UInt16 value);
	CX_UInt16 (* GetMyUInt16)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt32)(struct _My_Namespace_Struct1 *pThis, CX_Int32 value);
	CX_Int32 (* GetMyInt32)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt32)(struct _My_Namespace_Struct1 *pThis, CX_UInt32 value);
	CX_UInt32 (* GetMyUInt32)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt64)(struct _My_Namespace_Struct1 *pThis, CX_Int64 value);
	CX_Int64 (* GetMyInt64)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt64)(struct _My_Namespace_Struct1 *pThis, CX_UInt64 value);
	CX_UInt64 (* GetMyUInt64)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyFloat)(struct _My_Namespace_Struct1 *pThis, CX_Float value);
	CX_Float (* GetMyFloat)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyDouble)(struct _My_Namespace_Struct1 *pThis, CX_Double value);
	CX_Double (* GetMyDouble)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyChar)(struct _My_Namespace_Struct1 *pThis, CX_Char value);
	CX_Char (* GetMyChar)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyWChar)(struct _My_Namespace_Struct1 *pThis, CX_WChar value);
	CX_WChar (* GetMyWChar)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyStringCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyStringCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Char *(* GetMyString)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyWStringCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyWStringCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_WChar *(* GetMyWString)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyBLOBCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyBLOBCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_UInt8 *(* GetMyBLOB)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyBoolStaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Bool *(* GetMyBoolStaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt8StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Int8 *(* GetMyInt8StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt8StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_UInt8 *(* GetMyUInt8StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt16StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Int16 *(* GetMyInt16StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt16StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_UInt16 *(* GetMyUInt16StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt32StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Int32 *(* GetMyInt32StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt32StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_UInt32 *(* GetMyUInt32StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt64StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Int64 *(* GetMyInt64StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt64StaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_UInt64 *(* GetMyUInt64StaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyFloatStaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Float *(* GetMyFloatStaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyDoubleStaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Double *(* GetMyDoubleStaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyCharStaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_Char *(* GetMyCharStaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyWCharStaticArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_WChar *(* GetMyWCharStaticArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyBoolArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyBoolArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Bool *(* GetMyBoolArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt8ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt8ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Int8 *(* GetMyInt8Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt8ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt8ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_UInt8 *(* GetMyUInt8Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt16ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt16ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Int16 *(* GetMyInt16Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt16ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt16ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_UInt16 *(* GetMyUInt16Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt32ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt32ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Int32 *(* GetMyInt32Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt32ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt32ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_UInt32 *(* GetMyUInt32Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyInt64ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyInt64ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Int64 *(* GetMyInt64Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyUInt64ArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyUInt64ArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_UInt64 *(* GetMyUInt64Array)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyFloatArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyFloatArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Float *(* GetMyFloatArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyDoubleArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyDoubleArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Double *(* GetMyDoubleArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyCharArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyCharArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_Char *(* GetMyCharArray)(struct _My_Namespace_Struct1 *pThis);
	CX_CB_Size (* GetMyWCharArrayCount)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* SetMyWCharArrayCount)(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount);
	CX_WChar *(* GetMyWCharArray)(struct _My_Namespace_Struct1 *pThis);
	CX_StatusCode (* Destroy)(struct _My_Namespace_Struct1 *pThis);
}My_Namespace_Struct1;

CX_StatusCode My_Namespace_Struct1_Create(My_Namespace_Struct1 **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset);

#ifdef __cplusplus
}
#endif

