
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "Memory.h"
#include "DataWriter.h"
#include "DataReader.h"


CX_StatusCode CX_SB_String_Init(CX_Char **pszStr, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_String_Uninit(CX_Char **pszStr, CX_SB_Memory *pMem);

CX_Size CX_SB_String_Len(const CX_Char *szStr);

CX_StatusCode CX_SB_String_Set(CX_Char **pszStr, const CX_Char *szValue, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WString_Init(CX_WChar **pwszStr, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WString_Uninit(CX_WChar **pszStr, CX_SB_Memory *pMem);

CX_Size CX_SB_WString_Len(const CX_WChar *wszStr);

CX_StatusCode CX_SB_WString_Set(CX_WChar **pwszStr, const CX_WChar *wszValue, CX_SB_Memory *pMem);


typedef struct _CX_SB_BoolArray
{
	CX_Bool         *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_BoolArray;

CX_StatusCode CX_SB_BoolArray_Init(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_Uninit(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_BoolArray_GetCount(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_Set(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Bool val, CX_SB_Memory *pMem);

CX_Bool CX_SB_BoolArray_Get(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Bool defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_Add(CX_SB_BoolArray *pArray, CX_Bool val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_Remove(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_FreeUnusedMem(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_BoolArray_Reserve(CX_SB_BoolArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_Int8Array
{
	CX_Int8         *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_Int8Array;

CX_StatusCode CX_SB_Int8Array_Init(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_Uninit(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_Int8Array_GetCount(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_Set(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Int8 val, CX_SB_Memory *pMem);

CX_Int8 CX_SB_Int8Array_Get(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Int8 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_Add(CX_SB_Int8Array *pArray, CX_Int8 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_Remove(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_FreeUnusedMem(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int8Array_Reserve(CX_SB_Int8Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_UInt8Array
{
	CX_UInt8        *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_UInt8Array;

CX_StatusCode CX_SB_UInt8Array_Init(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_Uninit(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_UInt8Array_GetCount(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_Set(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_UInt8 val, CX_SB_Memory *pMem);

CX_UInt8 CX_SB_UInt8Array_Get(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_UInt8 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_Add(CX_SB_UInt8Array *pArray, CX_UInt8 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_Remove(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_FreeUnusedMem(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt8Array_Reserve(CX_SB_UInt8Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_Int16Array
{
	CX_Int16        *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_Int16Array;

CX_StatusCode CX_SB_Int16Array_Init(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_Uninit(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_Int16Array_GetCount(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_Set(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Int16 val, CX_SB_Memory *pMem);

CX_Int16 CX_SB_Int16Array_Get(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Int16 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_Add(CX_SB_Int16Array *pArray, CX_Int16 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_Remove(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_FreeUnusedMem(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int16Array_Reserve(CX_SB_Int16Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_UInt16Array
{
	CX_UInt16       *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_UInt16Array;

CX_StatusCode CX_SB_UInt16Array_Init(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_Uninit(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_UInt16Array_GetCount(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_Set(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_UInt16 val, CX_SB_Memory *pMem);

CX_UInt16 CX_SB_UInt16Array_Get(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_UInt16 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_Add(CX_SB_UInt16Array *pArray, CX_UInt16 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_Remove(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_FreeUnusedMem(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt16Array_Reserve(CX_SB_UInt16Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_Int32Array
{
	CX_Int32        *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_Int32Array;

CX_StatusCode CX_SB_Int32Array_Init(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_Uninit(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_Int32Array_GetCount(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_Set(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Int32 val, CX_SB_Memory *pMem);

CX_Int32 CX_SB_Int32Array_Get(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Int32 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_Add(CX_SB_Int32Array *pArray, CX_Int32 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_Remove(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_FreeUnusedMem(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int32Array_Reserve(CX_SB_Int32Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_UInt32Array
{
	CX_UInt32       *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_UInt32Array;

CX_StatusCode CX_SB_UInt32Array_Init(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_Uninit(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_UInt32Array_GetCount(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_Set(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_UInt32 val, CX_SB_Memory *pMem);

CX_UInt32 CX_SB_UInt32Array_Get(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_UInt32 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_Add(CX_SB_UInt32Array *pArray, CX_UInt32 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_Remove(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_FreeUnusedMem(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt32Array_Reserve(CX_SB_UInt32Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_Int64Array
{
	CX_Int64        *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_Int64Array;

CX_StatusCode CX_SB_Int64Array_Init(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_Uninit(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_Int64Array_GetCount(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_Set(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Int64 val, CX_SB_Memory *pMem);

CX_Int64 CX_SB_Int64Array_Get(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Int64 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_Add(CX_SB_Int64Array *pArray, CX_Int64 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_Remove(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_FreeUnusedMem(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_Int64Array_Reserve(CX_SB_Int64Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_UInt64Array
{
	CX_UInt64       *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_UInt64Array;

CX_StatusCode CX_SB_UInt64Array_Init(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_Uninit(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_UInt64Array_GetCount(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_Set(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_UInt64 val, CX_SB_Memory *pMem);

CX_UInt64 CX_SB_UInt64Array_Get(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_UInt64 defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_Add(CX_SB_UInt64Array *pArray, CX_UInt64 val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_Remove(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_FreeUnusedMem(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_UInt64Array_Reserve(CX_SB_UInt64Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_FloatArray
{
	CX_Float        *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_FloatArray;

CX_StatusCode CX_SB_FloatArray_Init(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_Uninit(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_FloatArray_GetCount(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_Set(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Float val, CX_SB_Memory *pMem);

CX_Float CX_SB_FloatArray_Get(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Float defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_Add(CX_SB_FloatArray *pArray, CX_Float val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_Remove(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_FreeUnusedMem(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_FloatArray_Reserve(CX_SB_FloatArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_DoubleArray
{
	CX_Double       *values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_DoubleArray;

CX_StatusCode CX_SB_DoubleArray_Init(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_Uninit(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_DoubleArray_GetCount(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_Set(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Double val, CX_SB_Memory *pMem);

CX_Double CX_SB_DoubleArray_Get(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Double defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_Add(CX_SB_DoubleArray *pArray, CX_Double val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_Remove(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_FreeUnusedMem(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_DoubleArray_Reserve(CX_SB_DoubleArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_StringArray
{
	CX_Char         **values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_StringArray;

CX_StatusCode CX_SB_StringArray_Init(CX_SB_StringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_Uninit(CX_SB_StringArray *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_StringArray_GetCount(CX_SB_StringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_Set(CX_SB_StringArray *pArray, CX_Size cIndex, const CX_Char *val, CX_SB_Memory *pMem);

const CX_Char *CX_SB_StringArray_Get(CX_SB_StringArray *pArray, CX_Size cIndex, const CX_Char *defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_Add(CX_SB_StringArray *pArray, const CX_Char *val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_Remove(CX_SB_StringArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_FreeUnusedMem(CX_SB_StringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_StringArray_Reserve(CX_SB_StringArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


typedef struct _CX_SB_WStringArray
{
	CX_WChar        **values;
	CX_Size         cUsedCount;
	CX_Size         cTotalCount;
}CX_SB_WStringArray;

CX_StatusCode CX_SB_WStringArray_Init(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_Uninit(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem);

CX_Size CX_SB_WStringArray_GetCount(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_Set(CX_SB_WStringArray *pArray, CX_Size cIndex, const CX_WChar *val, CX_SB_Memory *pMem);

const CX_WChar *CX_SB_WStringArray_Get(CX_SB_WStringArray *pArray, CX_Size cIndex, const CX_WChar *defval, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_Add(CX_SB_WStringArray *pArray, const CX_WChar *val, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_Remove(CX_SB_WStringArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_FreeUnusedMem(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WStringArray_Reserve(CX_SB_WStringArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);


#ifdef __cplusplus
}
#endif

