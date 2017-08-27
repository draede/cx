
#include "My_Namespace_Struct1.h"
#include "CX/CBuffers/Struct.h"


typedef struct _My_Namespace_Struct1_Ex
{
	My_Namespace_Struct1    base;
	CX_CB_Struct   helper;
}My_Namespace_Struct1_Ex;

static CX_StatusCode My_Namespace_Struct1_SetMyBool(struct _My_Namespace_Struct1 *pThis, CX_Bool value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 0, &value, sizeof(CX_Bool));
}

static CX_Bool My_Namespace_Struct1_GetMyBool(struct _My_Namespace_Struct1 *pThis)
{
	CX_Bool   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 0, &pValue, sizeof(CX_Bool))))
	{
		return (CX_Bool)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt8(struct _My_Namespace_Struct1 *pThis, CX_Int8 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 1, &value, sizeof(CX_Int8));
}

static CX_Int8 My_Namespace_Struct1_GetMyInt8(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int8   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 1, &pValue, sizeof(CX_Int8))))
	{
		return (CX_Int8)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt8(struct _My_Namespace_Struct1 *pThis, CX_UInt8 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 2, &value, sizeof(CX_UInt8));
}

static CX_UInt8 My_Namespace_Struct1_GetMyUInt8(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt8   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 2, &pValue, sizeof(CX_UInt8))))
	{
		return (CX_UInt8)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt16(struct _My_Namespace_Struct1 *pThis, CX_Int16 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 3, &value, sizeof(CX_Int16));
}

static CX_Int16 My_Namespace_Struct1_GetMyInt16(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int16   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 3, &pValue, sizeof(CX_Int16))))
	{
		return (CX_Int16)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt16(struct _My_Namespace_Struct1 *pThis, CX_UInt16 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 5, &value, sizeof(CX_UInt16));
}

static CX_UInt16 My_Namespace_Struct1_GetMyUInt16(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt16   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 5, &pValue, sizeof(CX_UInt16))))
	{
		return (CX_UInt16)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt32(struct _My_Namespace_Struct1 *pThis, CX_Int32 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 7, &value, sizeof(CX_Int32));
}

static CX_Int32 My_Namespace_Struct1_GetMyInt32(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int32   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 7, &pValue, sizeof(CX_Int32))))
	{
		return (CX_Int32)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt32(struct _My_Namespace_Struct1 *pThis, CX_UInt32 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 11, &value, sizeof(CX_UInt32));
}

static CX_UInt32 My_Namespace_Struct1_GetMyUInt32(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt32   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 11, &pValue, sizeof(CX_UInt32))))
	{
		return (CX_UInt32)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt64(struct _My_Namespace_Struct1 *pThis, CX_Int64 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 15, &value, sizeof(CX_Int64));
}

static CX_Int64 My_Namespace_Struct1_GetMyInt64(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int64   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 15, &pValue, sizeof(CX_Int64))))
	{
		return (CX_Int64)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt64(struct _My_Namespace_Struct1 *pThis, CX_UInt64 value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 23, &value, sizeof(CX_UInt64));
}

static CX_UInt64 My_Namespace_Struct1_GetMyUInt64(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt64   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 23, &pValue, sizeof(CX_UInt64))))
	{
		return (CX_UInt64)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyFloat(struct _My_Namespace_Struct1 *pThis, CX_Float value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 31, &value, sizeof(CX_Float));
}

static CX_Float My_Namespace_Struct1_GetMyFloat(struct _My_Namespace_Struct1 *pThis)
{
	CX_Float   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 31, &pValue, sizeof(CX_Float))))
	{
		return (CX_Float)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyDouble(struct _My_Namespace_Struct1 *pThis, CX_Double value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 35, &value, sizeof(CX_Double));
}

static CX_Double My_Namespace_Struct1_GetMyDouble(struct _My_Namespace_Struct1 *pThis)
{
	CX_Double   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 35, &pValue, sizeof(CX_Double))))
	{
		return (CX_Double)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyChar(struct _My_Namespace_Struct1 *pThis, CX_Char value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 43, &value, sizeof(CX_Char));
}

static CX_Char My_Namespace_Struct1_GetMyChar(struct _My_Namespace_Struct1 *pThis)
{
	CX_Char   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 43, &pValue, sizeof(CX_Char))))
	{
		return (CX_Char)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct1_SetMyWChar(struct _My_Namespace_Struct1 *pThis, CX_WChar value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 44, &value, sizeof(CX_WChar));
}

static CX_WChar My_Namespace_Struct1_GetMyWChar(struct _My_Namespace_Struct1 *pThis)
{
	CX_WChar   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 44, &pValue, sizeof(CX_WChar))))
	{
		return (CX_WChar)0;
	}

	return *pValue;
}

static CX_CB_Size My_Namespace_Struct1_GetMyStringCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 46, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyStringCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 46, sizeof(CX_Char), cCount);
}

static CX_Char *My_Namespace_Struct1_GetMyString(struct _My_Namespace_Struct1 *pThis)
{
	CX_Char   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 46, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyWStringCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 54, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyWStringCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 54, sizeof(CX_WChar), cCount);
}

static CX_WChar *My_Namespace_Struct1_GetMyWString(struct _My_Namespace_Struct1 *pThis)
{
	CX_WChar   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 54, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyBLOBCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 62, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyBLOBCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 62, sizeof(CX_UInt8), cCount);
}

static CX_UInt8 *My_Namespace_Struct1_GetMyBLOB(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt8   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 62, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyBoolStaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 1;
}

static CX_Bool *My_Namespace_Struct1_GetMyBoolStaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Bool   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 70, &pValues, sizeof(CX_Bool))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyInt8StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 2;
}

static CX_Int8 *My_Namespace_Struct1_GetMyInt8StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int8   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 71, &pValues, sizeof(CX_Int8))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyUInt8StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 3;
}

static CX_UInt8 *My_Namespace_Struct1_GetMyUInt8StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt8   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 73, &pValues, sizeof(CX_UInt8))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyInt16StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 4;
}

static CX_Int16 *My_Namespace_Struct1_GetMyInt16StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int16   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 76, &pValues, sizeof(CX_Int16))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyUInt16StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 5;
}

static CX_UInt16 *My_Namespace_Struct1_GetMyUInt16StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt16   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 84, &pValues, sizeof(CX_UInt16))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyInt32StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 6;
}

static CX_Int32 *My_Namespace_Struct1_GetMyInt32StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int32   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 94, &pValues, sizeof(CX_Int32))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyUInt32StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 7;
}

static CX_UInt32 *My_Namespace_Struct1_GetMyUInt32StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt32   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 118, &pValues, sizeof(CX_UInt32))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyInt64StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 8;
}

static CX_Int64 *My_Namespace_Struct1_GetMyInt64StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int64   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 146, &pValues, sizeof(CX_Int64))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyUInt64StaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 9;
}

static CX_UInt64 *My_Namespace_Struct1_GetMyUInt64StaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt64   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 210, &pValues, sizeof(CX_UInt64))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyFloatStaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 10;
}

static CX_Float *My_Namespace_Struct1_GetMyFloatStaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Float   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 282, &pValues, sizeof(CX_Float))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyDoubleStaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 11;
}

static CX_Double *My_Namespace_Struct1_GetMyDoubleStaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Double   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 322, &pValues, sizeof(CX_Double))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyCharStaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 12;
}

static CX_Char *My_Namespace_Struct1_GetMyCharStaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Char   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 410, &pValues, sizeof(CX_Char))))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_GetMyWCharStaticArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_UNUSED(pThis);

	return 13;
}

static CX_WChar *My_Namespace_Struct1_GetMyWCharStaticArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_WChar   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 422, &pValues, sizeof(CX_WChar))))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyBoolArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 448, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyBoolArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 448, sizeof(CX_Bool), cCount);
}

static CX_Bool *My_Namespace_Struct1_GetMyBoolArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Bool   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 448, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyInt8ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 456, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt8ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 456, sizeof(CX_Int8), cCount);
}

static CX_Int8 *My_Namespace_Struct1_GetMyInt8Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int8   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 456, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyUInt8ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 464, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt8ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 464, sizeof(CX_UInt8), cCount);
}

static CX_UInt8 *My_Namespace_Struct1_GetMyUInt8Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt8   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 464, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyInt16ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 472, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt16ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 472, sizeof(CX_Int16), cCount);
}

static CX_Int16 *My_Namespace_Struct1_GetMyInt16Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int16   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 472, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyUInt16ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 480, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt16ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 480, sizeof(CX_UInt16), cCount);
}

static CX_UInt16 *My_Namespace_Struct1_GetMyUInt16Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt16   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 480, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyInt32ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 488, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt32ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 488, sizeof(CX_Int32), cCount);
}

static CX_Int32 *My_Namespace_Struct1_GetMyInt32Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int32   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 488, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyUInt32ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 496, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt32ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 496, sizeof(CX_UInt32), cCount);
}

static CX_UInt32 *My_Namespace_Struct1_GetMyUInt32Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt32   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 496, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyInt64ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 504, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyInt64ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 504, sizeof(CX_Int64), cCount);
}

static CX_Int64 *My_Namespace_Struct1_GetMyInt64Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_Int64   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 504, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyUInt64ArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 512, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyUInt64ArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 512, sizeof(CX_UInt64), cCount);
}

static CX_UInt64 *My_Namespace_Struct1_GetMyUInt64Array(struct _My_Namespace_Struct1 *pThis)
{
	CX_UInt64   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 512, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyFloatArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 520, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyFloatArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 520, sizeof(CX_Float), cCount);
}

static CX_Float *My_Namespace_Struct1_GetMyFloatArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Float   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 520, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyDoubleArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 528, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyDoubleArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 528, sizeof(CX_Double), cCount);
}

static CX_Double *My_Namespace_Struct1_GetMyDoubleArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Double   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 528, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyCharArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 536, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyCharArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 536, sizeof(CX_Char), cCount);
}

static CX_Char *My_Namespace_Struct1_GetMyCharArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_Char   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 536, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_CB_Size My_Namespace_Struct1_GetMyWCharArrayCount(struct _My_Namespace_Struct1 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 544, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct1_SetMyWCharArrayCount(struct _My_Namespace_Struct1 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 544, sizeof(CX_WChar), cCount);
}

static CX_WChar *My_Namespace_Struct1_GetMyWCharArray(struct _My_Namespace_Struct1 *pThis)
{
	CX_WChar   *pValues;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct1_Ex, helper), 544, &pValues)))
	{
		return NULL;
	}

	return pValues;
}

static CX_StatusCode My_Namespace_Struct1_Destroy(struct _My_Namespace_Struct1 *pThis)
{
	My_Namespace_Struct1_Ex   *pStruct;
	CX_CB_Environment   *pEnv;

	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	pStruct   = (My_Namespace_Struct1_Ex *)pThis;
	pEnv = pStruct->helper.pEnv;
	pEnv->MemFree(pEnv, pStruct);

	return CX_Status_OK;
}

CX_StatusCode My_Namespace_Struct1_Create(My_Namespace_Struct1 **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset)
{
	My_Namespace_Struct1_Ex   *pStruct;
	CX_StatusCode   nStatus;

	if (NULL == *ppStruct || NULL == pBuffer || NULL == pEnv)
	{
		return CX_Status_InvalidArg;
	}
	if (NULL == (pStruct = (My_Namespace_Struct1_Ex *)pEnv->MemAlloc(pEnv, sizeof(My_Namespace_Struct1_Ex))))
	{
		return CX_Status_MemAllocFailed;
	}

	if (CXNOK(nStatus = CX_CB_Struct_Init(&pStruct->helper, pEnv, pBuffer, cbStartOffset)))
	{
		pEnv->MemFree(pEnv, pStruct);
	
		return nStatus;
	}
	if (cbStartOffset == pBuffer->cbUsedSize)
	{
		if (CXNOK(nStatus = CX_CB_Buffer_AddSize(pBuffer, 552, pEnv)))
		{
			pEnv->MemFree(pEnv, pStruct);
		
			return nStatus;
		}
	pEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, 552);
	}
	pStruct->base.SetMyBool = My_Namespace_Struct1_SetMyBool;
	pStruct->base.GetMyBool = My_Namespace_Struct1_GetMyBool;
	pStruct->base.SetMyInt8 = My_Namespace_Struct1_SetMyInt8;
	pStruct->base.GetMyInt8 = My_Namespace_Struct1_GetMyInt8;
	pStruct->base.SetMyUInt8 = My_Namespace_Struct1_SetMyUInt8;
	pStruct->base.GetMyUInt8 = My_Namespace_Struct1_GetMyUInt8;
	pStruct->base.SetMyInt16 = My_Namespace_Struct1_SetMyInt16;
	pStruct->base.GetMyInt16 = My_Namespace_Struct1_GetMyInt16;
	pStruct->base.SetMyUInt16 = My_Namespace_Struct1_SetMyUInt16;
	pStruct->base.GetMyUInt16 = My_Namespace_Struct1_GetMyUInt16;
	pStruct->base.SetMyInt32 = My_Namespace_Struct1_SetMyInt32;
	pStruct->base.GetMyInt32 = My_Namespace_Struct1_GetMyInt32;
	pStruct->base.SetMyUInt32 = My_Namespace_Struct1_SetMyUInt32;
	pStruct->base.GetMyUInt32 = My_Namespace_Struct1_GetMyUInt32;
	pStruct->base.SetMyInt64 = My_Namespace_Struct1_SetMyInt64;
	pStruct->base.GetMyInt64 = My_Namespace_Struct1_GetMyInt64;
	pStruct->base.SetMyUInt64 = My_Namespace_Struct1_SetMyUInt64;
	pStruct->base.GetMyUInt64 = My_Namespace_Struct1_GetMyUInt64;
	pStruct->base.SetMyFloat = My_Namespace_Struct1_SetMyFloat;
	pStruct->base.GetMyFloat = My_Namespace_Struct1_GetMyFloat;
	pStruct->base.SetMyDouble = My_Namespace_Struct1_SetMyDouble;
	pStruct->base.GetMyDouble = My_Namespace_Struct1_GetMyDouble;
	pStruct->base.SetMyChar = My_Namespace_Struct1_SetMyChar;
	pStruct->base.GetMyChar = My_Namespace_Struct1_GetMyChar;
	pStruct->base.SetMyWChar = My_Namespace_Struct1_SetMyWChar;
	pStruct->base.GetMyWChar = My_Namespace_Struct1_GetMyWChar;
	pStruct->base.GetMyStringCount = My_Namespace_Struct1_GetMyStringCount;
	pStruct->base.SetMyStringCount = My_Namespace_Struct1_SetMyStringCount;
	pStruct->base.GetMyString = My_Namespace_Struct1_GetMyString;
	pStruct->base.GetMyWStringCount = My_Namespace_Struct1_GetMyWStringCount;
	pStruct->base.SetMyWStringCount = My_Namespace_Struct1_SetMyWStringCount;
	pStruct->base.GetMyWString = My_Namespace_Struct1_GetMyWString;
	pStruct->base.GetMyBLOBCount = My_Namespace_Struct1_GetMyBLOBCount;
	pStruct->base.SetMyBLOBCount = My_Namespace_Struct1_SetMyBLOBCount;
	pStruct->base.GetMyBLOB = My_Namespace_Struct1_GetMyBLOB;
	pStruct->base.GetMyBoolStaticArrayCount = My_Namespace_Struct1_GetMyBoolStaticArrayCount;
	pStruct->base.GetMyBoolStaticArray = My_Namespace_Struct1_GetMyBoolStaticArray;
	pStruct->base.GetMyInt8StaticArrayCount = My_Namespace_Struct1_GetMyInt8StaticArrayCount;
	pStruct->base.GetMyInt8StaticArray = My_Namespace_Struct1_GetMyInt8StaticArray;
	pStruct->base.GetMyUInt8StaticArrayCount = My_Namespace_Struct1_GetMyUInt8StaticArrayCount;
	pStruct->base.GetMyUInt8StaticArray = My_Namespace_Struct1_GetMyUInt8StaticArray;
	pStruct->base.GetMyInt16StaticArrayCount = My_Namespace_Struct1_GetMyInt16StaticArrayCount;
	pStruct->base.GetMyInt16StaticArray = My_Namespace_Struct1_GetMyInt16StaticArray;
	pStruct->base.GetMyUInt16StaticArrayCount = My_Namespace_Struct1_GetMyUInt16StaticArrayCount;
	pStruct->base.GetMyUInt16StaticArray = My_Namespace_Struct1_GetMyUInt16StaticArray;
	pStruct->base.GetMyInt32StaticArrayCount = My_Namespace_Struct1_GetMyInt32StaticArrayCount;
	pStruct->base.GetMyInt32StaticArray = My_Namespace_Struct1_GetMyInt32StaticArray;
	pStruct->base.GetMyUInt32StaticArrayCount = My_Namespace_Struct1_GetMyUInt32StaticArrayCount;
	pStruct->base.GetMyUInt32StaticArray = My_Namespace_Struct1_GetMyUInt32StaticArray;
	pStruct->base.GetMyInt64StaticArrayCount = My_Namespace_Struct1_GetMyInt64StaticArrayCount;
	pStruct->base.GetMyInt64StaticArray = My_Namespace_Struct1_GetMyInt64StaticArray;
	pStruct->base.GetMyUInt64StaticArrayCount = My_Namespace_Struct1_GetMyUInt64StaticArrayCount;
	pStruct->base.GetMyUInt64StaticArray = My_Namespace_Struct1_GetMyUInt64StaticArray;
	pStruct->base.GetMyFloatStaticArrayCount = My_Namespace_Struct1_GetMyFloatStaticArrayCount;
	pStruct->base.GetMyFloatStaticArray = My_Namespace_Struct1_GetMyFloatStaticArray;
	pStruct->base.GetMyDoubleStaticArrayCount = My_Namespace_Struct1_GetMyDoubleStaticArrayCount;
	pStruct->base.GetMyDoubleStaticArray = My_Namespace_Struct1_GetMyDoubleStaticArray;
	pStruct->base.GetMyCharStaticArrayCount = My_Namespace_Struct1_GetMyCharStaticArrayCount;
	pStruct->base.GetMyCharStaticArray = My_Namespace_Struct1_GetMyCharStaticArray;
	pStruct->base.GetMyWCharStaticArrayCount = My_Namespace_Struct1_GetMyWCharStaticArrayCount;
	pStruct->base.GetMyWCharStaticArray = My_Namespace_Struct1_GetMyWCharStaticArray;
	pStruct->base.GetMyBoolArrayCount = My_Namespace_Struct1_GetMyBoolArrayCount;
	pStruct->base.SetMyBoolArrayCount = My_Namespace_Struct1_SetMyBoolArrayCount;
	pStruct->base.GetMyBoolArray = My_Namespace_Struct1_GetMyBoolArray;
	pStruct->base.GetMyInt8ArrayCount = My_Namespace_Struct1_GetMyInt8ArrayCount;
	pStruct->base.SetMyInt8ArrayCount = My_Namespace_Struct1_SetMyInt8ArrayCount;
	pStruct->base.GetMyInt8Array = My_Namespace_Struct1_GetMyInt8Array;
	pStruct->base.GetMyUInt8ArrayCount = My_Namespace_Struct1_GetMyUInt8ArrayCount;
	pStruct->base.SetMyUInt8ArrayCount = My_Namespace_Struct1_SetMyUInt8ArrayCount;
	pStruct->base.GetMyUInt8Array = My_Namespace_Struct1_GetMyUInt8Array;
	pStruct->base.GetMyInt16ArrayCount = My_Namespace_Struct1_GetMyInt16ArrayCount;
	pStruct->base.SetMyInt16ArrayCount = My_Namespace_Struct1_SetMyInt16ArrayCount;
	pStruct->base.GetMyInt16Array = My_Namespace_Struct1_GetMyInt16Array;
	pStruct->base.GetMyUInt16ArrayCount = My_Namespace_Struct1_GetMyUInt16ArrayCount;
	pStruct->base.SetMyUInt16ArrayCount = My_Namespace_Struct1_SetMyUInt16ArrayCount;
	pStruct->base.GetMyUInt16Array = My_Namespace_Struct1_GetMyUInt16Array;
	pStruct->base.GetMyInt32ArrayCount = My_Namespace_Struct1_GetMyInt32ArrayCount;
	pStruct->base.SetMyInt32ArrayCount = My_Namespace_Struct1_SetMyInt32ArrayCount;
	pStruct->base.GetMyInt32Array = My_Namespace_Struct1_GetMyInt32Array;
	pStruct->base.GetMyUInt32ArrayCount = My_Namespace_Struct1_GetMyUInt32ArrayCount;
	pStruct->base.SetMyUInt32ArrayCount = My_Namespace_Struct1_SetMyUInt32ArrayCount;
	pStruct->base.GetMyUInt32Array = My_Namespace_Struct1_GetMyUInt32Array;
	pStruct->base.GetMyInt64ArrayCount = My_Namespace_Struct1_GetMyInt64ArrayCount;
	pStruct->base.SetMyInt64ArrayCount = My_Namespace_Struct1_SetMyInt64ArrayCount;
	pStruct->base.GetMyInt64Array = My_Namespace_Struct1_GetMyInt64Array;
	pStruct->base.GetMyUInt64ArrayCount = My_Namespace_Struct1_GetMyUInt64ArrayCount;
	pStruct->base.SetMyUInt64ArrayCount = My_Namespace_Struct1_SetMyUInt64ArrayCount;
	pStruct->base.GetMyUInt64Array = My_Namespace_Struct1_GetMyUInt64Array;
	pStruct->base.GetMyFloatArrayCount = My_Namespace_Struct1_GetMyFloatArrayCount;
	pStruct->base.SetMyFloatArrayCount = My_Namespace_Struct1_SetMyFloatArrayCount;
	pStruct->base.GetMyFloatArray = My_Namespace_Struct1_GetMyFloatArray;
	pStruct->base.GetMyDoubleArrayCount = My_Namespace_Struct1_GetMyDoubleArrayCount;
	pStruct->base.SetMyDoubleArrayCount = My_Namespace_Struct1_SetMyDoubleArrayCount;
	pStruct->base.GetMyDoubleArray = My_Namespace_Struct1_GetMyDoubleArray;
	pStruct->base.GetMyCharArrayCount = My_Namespace_Struct1_GetMyCharArrayCount;
	pStruct->base.SetMyCharArrayCount = My_Namespace_Struct1_SetMyCharArrayCount;
	pStruct->base.GetMyCharArray = My_Namespace_Struct1_GetMyCharArray;
	pStruct->base.GetMyWCharArrayCount = My_Namespace_Struct1_GetMyWCharArrayCount;
	pStruct->base.SetMyWCharArrayCount = My_Namespace_Struct1_SetMyWCharArrayCount;
	pStruct->base.GetMyWCharArray = My_Namespace_Struct1_GetMyWCharArray;
	pStruct->base.Destroy = &My_Namespace_Struct1_Destroy;

	*ppStruct = (My_Namespace_Struct1 *)pStruct;

	return CX_Status_OK;
}

