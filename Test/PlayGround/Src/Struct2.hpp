
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/Environment.h"
#include "CX/CBuffers/Types.h"
#include "CX/CBuffers/Struct.h"
#include "Struct1.hpp"


namespace My
{

namespace Namespace
{

class Struct2
{
public:

	Struct2();

	~Struct2();

	static Struct2 *Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset = 0);

	void Destroy();

	CX::Status SetMyBool(CX_Bool value);

	CX_Bool GetMyBool();

	//You must call Destroy on ppStruct when done
	CX::Status GetMyStruct1(My::Namespace::Struct1 **ppStruct);

	CX_CB_Size GetMyStruct1StaticArrayCount();

	//You must call Destroy on ppStruct when done
	CX::Status GetMyStruct1StaticArrayItem(CX_CB_Size cIndex, My::Namespace::Struct1 **ppStruct);

	CX_CB_Size GetMyStruct1ArrayCount();

	CX::Status SetMyStruct1ArrayCount(CX_CB_Size cCount);

	//You must call Destroy on ppStruct when done
	CX::Status GetMyStruct1ArrayItem(CX_CB_Size cIndex, My::Namespace::Struct1 **ppStruct);

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

