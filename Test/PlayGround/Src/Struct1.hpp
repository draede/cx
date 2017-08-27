
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/Environment.h"
#include "CX/CBuffers/Types.h"
#include "CX/CBuffers/Struct.h"


namespace My
{

namespace Namespace
{

class Struct1
{
public:

	Struct1();

	~Struct1();

	static Struct1 *Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset = 0);

	void Destroy();

	CX::Status SetMyBool(CX_Bool value);

	CX_Bool GetMyBool();

	CX::Status SetMyInt8(CX_Int8 value);

	CX_Int8 GetMyInt8();

	CX::Status SetMyUInt8(CX_UInt8 value);

	CX_UInt8 GetMyUInt8();

	CX::Status SetMyInt16(CX_Int16 value);

	CX_Int16 GetMyInt16();

	CX::Status SetMyUInt16(CX_UInt16 value);

	CX_UInt16 GetMyUInt16();

	CX::Status SetMyInt32(CX_Int32 value);

	CX_Int32 GetMyInt32();

	CX::Status SetMyUInt32(CX_UInt32 value);

	CX_UInt32 GetMyUInt32();

	CX::Status SetMyInt64(CX_Int64 value);

	CX_Int64 GetMyInt64();

	CX::Status SetMyUInt64(CX_UInt64 value);

	CX_UInt64 GetMyUInt64();

	CX::Status SetMyFloat(CX_Float value);

	CX_Float GetMyFloat();

	CX::Status SetMyDouble(CX_Double value);

	CX_Double GetMyDouble();

	CX::Status SetMyChar(CX_Char value);

	CX_Char GetMyChar();

	CX::Status SetMyWChar(CX_WChar value);

	CX_WChar GetMyWChar();

	CX_CB_Size GetMyStringCount();

	CX::Status SetMyStringCount(CX_CB_Size cCount);

	CX_Char *GetMyString();

	CX_CB_Size GetMyWStringCount();

	CX::Status SetMyWStringCount(CX_CB_Size cCount);

	CX_WChar *GetMyWString();

	CX_CB_Size GetMyBLOBCount();

	CX::Status SetMyBLOBCount(CX_CB_Size cCount);

	CX_UInt8 *GetMyBLOB();

	CX_CB_Size GetMyBoolStaticArrayCount();

	CX_Bool *GetMyBoolStaticArray();

	CX_CB_Size GetMyInt8StaticArrayCount();

	CX_Int8 *GetMyInt8StaticArray();

	CX_CB_Size GetMyUInt8StaticArrayCount();

	CX_UInt8 *GetMyUInt8StaticArray();

	CX_CB_Size GetMyInt16StaticArrayCount();

	CX_Int16 *GetMyInt16StaticArray();

	CX_CB_Size GetMyUInt16StaticArrayCount();

	CX_UInt16 *GetMyUInt16StaticArray();

	CX_CB_Size GetMyInt32StaticArrayCount();

	CX_Int32 *GetMyInt32StaticArray();

	CX_CB_Size GetMyUInt32StaticArrayCount();

	CX_UInt32 *GetMyUInt32StaticArray();

	CX_CB_Size GetMyInt64StaticArrayCount();

	CX_Int64 *GetMyInt64StaticArray();

	CX_CB_Size GetMyUInt64StaticArrayCount();

	CX_UInt64 *GetMyUInt64StaticArray();

	CX_CB_Size GetMyFloatStaticArrayCount();

	CX_Float *GetMyFloatStaticArray();

	CX_CB_Size GetMyDoubleStaticArrayCount();

	CX_Double *GetMyDoubleStaticArray();

	CX_CB_Size GetMyCharStaticArrayCount();

	CX_Char *GetMyCharStaticArray();

	CX_CB_Size GetMyWCharStaticArrayCount();

	CX_WChar *GetMyWCharStaticArray();

	CX_CB_Size GetMyBoolArrayCount();

	CX::Status SetMyBoolArrayCount(CX_CB_Size cCount);

	CX_Bool *GetMyBoolArray();

	CX_CB_Size GetMyInt8ArrayCount();

	CX::Status SetMyInt8ArrayCount(CX_CB_Size cCount);

	CX_Int8 *GetMyInt8Array();

	CX_CB_Size GetMyUInt8ArrayCount();

	CX::Status SetMyUInt8ArrayCount(CX_CB_Size cCount);

	CX_UInt8 *GetMyUInt8Array();

	CX_CB_Size GetMyInt16ArrayCount();

	CX::Status SetMyInt16ArrayCount(CX_CB_Size cCount);

	CX_Int16 *GetMyInt16Array();

	CX_CB_Size GetMyUInt16ArrayCount();

	CX::Status SetMyUInt16ArrayCount(CX_CB_Size cCount);

	CX_UInt16 *GetMyUInt16Array();

	CX_CB_Size GetMyInt32ArrayCount();

	CX::Status SetMyInt32ArrayCount(CX_CB_Size cCount);

	CX_Int32 *GetMyInt32Array();

	CX_CB_Size GetMyUInt32ArrayCount();

	CX::Status SetMyUInt32ArrayCount(CX_CB_Size cCount);

	CX_UInt32 *GetMyUInt32Array();

	CX_CB_Size GetMyInt64ArrayCount();

	CX::Status SetMyInt64ArrayCount(CX_CB_Size cCount);

	CX_Int64 *GetMyInt64Array();

	CX_CB_Size GetMyUInt64ArrayCount();

	CX::Status SetMyUInt64ArrayCount(CX_CB_Size cCount);

	CX_UInt64 *GetMyUInt64Array();

	CX_CB_Size GetMyFloatArrayCount();

	CX::Status SetMyFloatArrayCount(CX_CB_Size cCount);

	CX_Float *GetMyFloatArray();

	CX_CB_Size GetMyDoubleArrayCount();

	CX::Status SetMyDoubleArrayCount(CX_CB_Size cCount);

	CX_Double *GetMyDoubleArray();

	CX_CB_Size GetMyCharArrayCount();

	CX::Status SetMyCharArrayCount(CX_CB_Size cCount);

	CX_Char *GetMyCharArray();

	CX_CB_Size GetMyWCharArrayCount();

	CX::Status SetMyWCharArrayCount(CX_CB_Size cCount);

	CX_WChar *GetMyWCharArray();

private:

	CX_CB_Struct   m_struct;

	template <typename STRUCT>
	static CX::Status CreateHelper(STRUCT **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset/* = 0*/)
	{
		STRUCT *pStruct;

		if (NULL == (pStruct = STRUCT::Create(pEnv, pBuffer, cbStartOffset)))
		{
			return CX::Status_MemAllocFailed;
		}
	*ppStruct = pStruct;

	return CX::Status();
	}

};

}//namespace Namespace

}//namespace My

