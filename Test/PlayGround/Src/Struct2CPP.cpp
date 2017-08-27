
#include "Struct2.hpp"
#include "CX/CBuffers/Struct.h"
#include <new>


namespace My
{

namespace Namespace
{

Struct2::Struct2()
{
	m_struct.pEnv          = NULL;
	m_struct.pBuffer       = NULL;
	m_struct.cbStartOffset = 0;
}

Struct2::~Struct2()
{
}

Struct2 *Struct2::Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset/* = 0*/)
{
	Struct2 *pStruct;
	CX::Status   status;

	if (NULL == (pStruct = new (std::nothrow) Struct2()))
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
		if (!(status = CX_CB_Buffer_AddSize(pBuffer, 1665, pEnv)))
		{
			delete pStruct;

			return NULL;
		}
		pEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, 1665);
	}

	return pStruct;
}

void Struct2::Destroy()
{
	delete this;
}

CX::Status Struct2::SetMyBool(CX_Bool value)
{
	return CX_CB_Struct_SetStatic(&m_struct, 0, &value, sizeof(CX_Bool));
}

CX_Bool Struct2::GetMyBool()
{
	CX_Bool   *pValue;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 0, (void **)&pValue, sizeof(CX_Bool))))
	{
		return (CX_Bool)0;
	}

	return *pValue;
}

CX::Status Struct2::GetMyStruct1(My::Namespace::Struct1 **ppStruct)
{
	CX::Byte      *pValue;
	CX::Status    status;

	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 1, (void **)&pValue, 552)))
	{
		return status;
	}

	return CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValue - (CX_Byte *)m_struct.pBuffer->pData);
}

CX_CB_Size Struct2::GetMyStruct1StaticArrayCount()
{
	return 2;
}

CX::Status Struct2::GetMyStruct1StaticArrayItem(CX_CB_Size cIndex, My::Namespace::Struct1 **ppStruct)
{
	CX::Byte     *pValues;
	CX::Status   status;

	if (2 <= cIndex)
	{
		return CX::Status_InvalidArg;
	}
	if (!(status = CX_CB_Struct_GetStatic(&m_struct, 553, (void **)&pValues, 1104)))
	{
		return status;
	}

	return CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValues + cIndex * 552 - (CX_Byte *)m_struct.pBuffer->pData);
}

CX_CB_Size Struct2::GetMyStruct1ArrayCount()
{
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 1657, &cCount)))
	{
		return 0;
	}

	return cCount;
}

CX::Status Struct2::SetMyStruct1ArrayCount(CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount(&m_struct, 1657, 552, cCount);
}

CX::Status Struct2::GetMyStruct1ArrayItem(CX_CB_Size cIndex, My::Namespace::Struct1 **ppStruct)
{
	CX::Byte     *pValues;
	CX_CB_Size   cCount;
	CX::Status   status;

	if (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, 1657, &cCount)))
	{
		return status;
	}
	if (cCount <= cIndex)
	{
		return CX::Status_OutOfBounds;
	}
	if (!(status = CX_CB_Struct_GetArrayItems(&m_struct, 1657, (void **)&pValues)))
	{
		return status;
	}

	return CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValues + cIndex * 552 - (CX_Byte *)m_struct.pBuffer->pData);
}

}//namespace Namespace

}//namespace My

