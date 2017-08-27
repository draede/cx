
#include "Struct1.hpp"
#include "CX/CBuffers/Struct.h"
#include <new>


namespace My
{

namespace Namespace
{

Struct1::Struct1()
{
	m_struct.pEnv          = NULL;
	m_struct.pBuffer       = NULL;
	m_struct.cbStartOffset = 0;
}

Struct1::~Struct1()
{
}

Struct1 *Struct1::Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset/* = 0*/)
{
	Struct1 *pStruct;
	CX::Status   status;

	if (NULL == (pStruct = new (std::nothrow) Struct1()))
	{
		return NULL;
	}
	if (!(status = CX_CB_Struct_Init(&pStruct->m_struct, pEnv, pBuffer, cbStartOffset)))
	{
		delete pStruct;

		return NULL;
	}
	if (cbStartOffset == pBuffer->cbUsedSize)
	{
		if (!(status = CX_CB_Buffer_AddSize(pBuffer, 552, pEnv)))
		{
			delete pStruct;

			return NULL;
		}
		pEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, 552);
	}

	return pStruct;
}

void Struct1::Destroy()
{
	delete this;
}

CX::Status Struct1::SetMyBool(CX_Bool value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 0, &value, sizeof(CX_Bool));
}

CX_Bool Struct1::GetMyBool()
{
	CX_Bool   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 0, (void **)&pValue, sizeof(CX_Bool))))
	{
		return (CX_Bool)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyInt8(CX_Int8 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 1, &value, sizeof(CX_Int8));
}

CX_Int8 Struct1::GetMyInt8()
{
	CX_Int8   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 1, (void **)&pValue, sizeof(CX_Int8))))
	{
		return (CX_Int8)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyUInt8(CX_UInt8 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 2, &value, sizeof(CX_UInt8));
}

CX_UInt8 Struct1::GetMyUInt8()
{
	CX_UInt8   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 2, (void **)&pValue, sizeof(CX_UInt8))))
	{
		return (CX_UInt8)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyInt16(CX_Int16 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 3, &value, sizeof(CX_Int16));
}

CX_Int16 Struct1::GetMyInt16()
{
	CX_Int16   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 3, (void **)&pValue, sizeof(CX_Int16))))
	{
		return (CX_Int16)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyUInt16(CX_UInt16 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 5, &value, sizeof(CX_UInt16));
}

CX_UInt16 Struct1::GetMyUInt16()
{
	CX_UInt16   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 5, (void **)&pValue, sizeof(CX_UInt16))))
	{
		return (CX_UInt16)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyInt32(CX_Int32 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 7, &value, sizeof(CX_Int32));
}

CX_Int32 Struct1::GetMyInt32()
{
	CX_Int32   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 7, (void **)&pValue, sizeof(CX_Int32))))
	{
		return (CX_Int32)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyUInt32(CX_UInt32 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 11, &value, sizeof(CX_UInt32));
}

CX_UInt32 Struct1::GetMyUInt32()
{
	CX_UInt32   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 11, (void **)&pValue, sizeof(CX_UInt32))))
	{
		return (CX_UInt32)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyInt64(CX_Int64 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 15, &value, sizeof(CX_Int64));
}

CX_Int64 Struct1::GetMyInt64()
{
	CX_Int64   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 15, (void **)&pValue, sizeof(CX_Int64))))
	{
		return (CX_Int64)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyUInt64(CX_UInt64 value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 23, &value, sizeof(CX_UInt64));
}

CX_UInt64 Struct1::GetMyUInt64()
{
	CX_UInt64   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 23, (void **)&pValue, sizeof(CX_UInt64))))
	{
		return (CX_UInt64)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyFloat(CX_Float value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 31, &value, sizeof(CX_Float));
}

CX_Float Struct1::GetMyFloat()
{
	CX_Float   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 31, (void **)&pValue, sizeof(CX_Float))))
	{
		return (CX_Float)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyDouble(CX_Double value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 35, &value, sizeof(CX_Double));
}

CX_Double Struct1::GetMyDouble()
{
	CX_Double   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 35, (void **)&pValue, sizeof(CX_Double))))
	{
		return (CX_Double)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyChar(CX_Char value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 43, &value, sizeof(CX_Char));
}

CX_Char Struct1::GetMyChar()
{
	CX_Char   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 43, (void **)&pValue, sizeof(CX_Char))))
	{
		return (CX_Char)0;
	}

	return *pValue;
}

CX::Status Struct1::SetMyWChar(CX_WChar value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 44, &value, sizeof(CX_WChar));
}

CX_WChar Struct1::GetMyWChar()
{
	CX_WChar   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 44, (void **)&pValue, sizeof(CX_WChar))))
	{
		return (CX_WChar)0;
	}

	return *pValue;
}

CX_CB_Size Struct1::GetMyStringCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 46, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyStringCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 46, sizeof(CX_Char), cCount);
}

CX_Char *Struct1::GetMyString()
{
	CX_Char   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 46, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyWStringCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 54, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyWStringCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 54, sizeof(CX_WChar), cCount);
}

CX_WChar *Struct1::GetMyWString()
{
	CX_WChar   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 54, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyBLOBCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 62, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyBLOBCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 62, sizeof(CX_UInt8), cCount);
}

CX_UInt8 *Struct1::GetMyBLOB()
{
	CX_UInt8   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 62, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyBoolStaticArrayCount()
{
	return 1;
}

CX_Bool *Struct1::GetMyBoolStaticArray()
{
	CX_Bool   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 70, (void **)&pValues, sizeof(CX_Bool))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt8StaticArrayCount()
{
	return 2;
}

CX_Int8 *Struct1::GetMyInt8StaticArray()
{
	CX_Int8   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 71, (void **)&pValues, sizeof(CX_Int8))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt8StaticArrayCount()
{
	return 3;
}

CX_UInt8 *Struct1::GetMyUInt8StaticArray()
{
	CX_UInt8   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 73, (void **)&pValues, sizeof(CX_UInt8))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt16StaticArrayCount()
{
	return 4;
}

CX_Int16 *Struct1::GetMyInt16StaticArray()
{
	CX_Int16   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 76, (void **)&pValues, sizeof(CX_Int16))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt16StaticArrayCount()
{
	return 5;
}

CX_UInt16 *Struct1::GetMyUInt16StaticArray()
{
	CX_UInt16   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 84, (void **)&pValues, sizeof(CX_UInt16))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt32StaticArrayCount()
{
	return 6;
}

CX_Int32 *Struct1::GetMyInt32StaticArray()
{
	CX_Int32   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 94, (void **)&pValues, sizeof(CX_Int32))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt32StaticArrayCount()
{
	return 7;
}

CX_UInt32 *Struct1::GetMyUInt32StaticArray()
{
	CX_UInt32   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 118, (void **)&pValues, sizeof(CX_UInt32))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt64StaticArrayCount()
{
	return 8;
}

CX_Int64 *Struct1::GetMyInt64StaticArray()
{
	CX_Int64   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 146, (void **)&pValues, sizeof(CX_Int64))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt64StaticArrayCount()
{
	return 9;
}

CX_UInt64 *Struct1::GetMyUInt64StaticArray()
{
	CX_UInt64   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 210, (void **)&pValues, sizeof(CX_UInt64))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyFloatStaticArrayCount()
{
	return 10;
}

CX_Float *Struct1::GetMyFloatStaticArray()
{
	CX_Float   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 282, (void **)&pValues, sizeof(CX_Float))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyDoubleStaticArrayCount()
{
	return 11;
}

CX_Double *Struct1::GetMyDoubleStaticArray()
{
	CX_Double   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 322, (void **)&pValues, sizeof(CX_Double))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyCharStaticArrayCount()
{
	return 12;
}

CX_Char *Struct1::GetMyCharStaticArray()
{
	CX_Char   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 410, (void **)&pValues, sizeof(CX_Char))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyWCharStaticArrayCount()
{
	return 13;
}

CX_WChar *Struct1::GetMyWCharStaticArray()
{
	CX_WChar   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 422, (void **)&pValues, sizeof(CX_WChar))))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyBoolArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 448, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyBoolArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 448, sizeof(CX_Bool), cCount);
}

CX_Bool *Struct1::GetMyBoolArray()
{
	CX_Bool   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 448, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt8ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 456, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyInt8ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 456, sizeof(CX_Int8), cCount);
}

CX_Int8 *Struct1::GetMyInt8Array()
{
	CX_Int8   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 456, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt8ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 464, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyUInt8ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 464, sizeof(CX_UInt8), cCount);
}

CX_UInt8 *Struct1::GetMyUInt8Array()
{
	CX_UInt8   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 464, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt16ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 472, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyInt16ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 472, sizeof(CX_Int16), cCount);
}

CX_Int16 *Struct1::GetMyInt16Array()
{
	CX_Int16   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 472, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt16ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 480, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyUInt16ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 480, sizeof(CX_UInt16), cCount);
}

CX_UInt16 *Struct1::GetMyUInt16Array()
{
	CX_UInt16   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 480, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt32ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 488, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyInt32ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 488, sizeof(CX_Int32), cCount);
}

CX_Int32 *Struct1::GetMyInt32Array()
{
	CX_Int32   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 488, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt32ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 496, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyUInt32ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 496, sizeof(CX_UInt32), cCount);
}

CX_UInt32 *Struct1::GetMyUInt32Array()
{
	CX_UInt32   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 496, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyInt64ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 504, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyInt64ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 504, sizeof(CX_Int64), cCount);
}

CX_Int64 *Struct1::GetMyInt64Array()
{
	CX_Int64   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 504, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyUInt64ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 512, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyUInt64ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 512, sizeof(CX_UInt64), cCount);
}

CX_UInt64 *Struct1::GetMyUInt64Array()
{
	CX_UInt64   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 512, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyFloatArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 520, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyFloatArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 520, sizeof(CX_Float), cCount);
}

CX_Float *Struct1::GetMyFloatArray()
{
	CX_Float   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 520, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyDoubleArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 528, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyDoubleArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 528, sizeof(CX_Double), cCount);
}

CX_Double *Struct1::GetMyDoubleArray()
{
	CX_Double   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 528, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyCharArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 536, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyCharArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 536, sizeof(CX_Char), cCount);
}

CX_Char *Struct1::GetMyCharArray()
{
	CX_Char   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 536, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

CX_CB_Size Struct1::GetMyWCharArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 544, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct1::SetMyWCharArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 544, sizeof(CX_WChar), cCount);
}

CX_WChar *Struct1::GetMyWCharArray()
{
	CX_WChar   *pValues;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 544, (void **)&pValues)))
	{
		return NULL;
	}

	return pValues;
}

}//namespace Namespace

}//namespace My

