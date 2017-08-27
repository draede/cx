
#include "My_Namespace_Struct2.h"
#include "CX/CBuffers/Struct.h"


typedef struct _My_Namespace_Struct2_Ex
{
	My_Namespace_Struct2    base;
	CX_CB_Struct   helper;
}My_Namespace_Struct2_Ex;

static CX_StatusCode My_Namespace_Struct2_SetMyBool(struct _My_Namespace_Struct2 *pThis, CX_Bool value)
{
	return CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 0, &value, sizeof(CX_Bool));
}

static CX_Bool My_Namespace_Struct2_GetMyBool(struct _My_Namespace_Struct2 *pThis)
{
	CX_Bool   *pValue;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 0, &pValue, sizeof(CX_Bool))))
	{
		return (CX_Bool)0;
	}

	return *pValue;
}

static CX_StatusCode My_Namespace_Struct2_GetMyStruct1(struct _My_Namespace_Struct2 *pThis, My_Namespace_Struct1 **ppStruct)
{
	CX_Byte             *pValue;
	CX_CB_Environment   *pEnv;
	CX_CB_Buffer        *pBuffer;
	CX_StatusCode       nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 1, &pValue, 552)))
	{
		return nStatus;
	}
	pEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));
	pBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));

	return My_Namespace_Struct1_Create(ppStruct, pEnv, pBuffer, pValue - (CX_Byte *)pBuffer->pData);
}

static CX_StatusCode My_Namespace_Struct2_GetMyStruct1StaticArrayCount(struct _My_Namespace_Struct2 *pThis)
{
	CX_UNUSED(pThis);

	return 2;
}

static CX_StatusCode My_Namespace_Struct2_GetMyStruct1StaticArrayItem(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cIndex, My_Namespace_Struct1 **ppStruct)
{
	CX_Byte             *pValues;
	CX_CB_Environment   *pEnv;
	CX_CB_Buffer        *pBuffer;
	CX_StatusCode       nStatus;

	if (2 <= cIndex)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 553, &pValues, 1104)))
	{
		return nStatus;
	}
	pEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));
	pBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));

	return My_Namespace_Struct1_Create(ppStruct, pEnv, pBuffer, pValues + cIndex * 552 - (CX_Byte *)pBuffer->pData);
}

static CX_CB_Size My_Namespace_Struct2_GetMyStruct1ArrayCount(struct _My_Namespace_Struct2 *pThis)
{
	CX_CB_Size      cCount;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 1657, &cCount)))
	{
		return 0;
	}

	return cCount;
}

static CX_StatusCode My_Namespace_Struct2_SetMyStruct1ArrayCount(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cCount)
{
	return CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 1657, 552, cCount);
}

static CX_StatusCode My_Namespace_Struct2_GetMyStruct1ArrayItem(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cIndex, My_Namespace_Struct1 **ppStruct)
{
	CX_Byte             *pValues;
	CX_CB_Environment   *pEnv;
	CX_CB_Buffer        *pBuffer;
	CX_CB_Size          cCount;
	CX_StatusCode       nStatus;

	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 1657, &cCount)))
	{
		return nStatus;
	}
	if (cCount <= cIndex)
	{
		return CX_Status_OutOfBounds;
	}
	if (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper), 1657, &pValues)))
	{
		return nStatus;
	}
	pEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));
	pBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF(My_Namespace_Struct2_Ex, helper));

	return My_Namespace_Struct1_Create(ppStruct, pEnv, pBuffer, pValues + cIndex * 552 - (CX_Byte *)pBuffer->pData);
}

static CX_StatusCode My_Namespace_Struct2_Destroy(struct _My_Namespace_Struct2 *pThis)
{
	My_Namespace_Struct2_Ex   *pStruct;
	CX_CB_Environment   *pEnv;

	if (NULL == pThis)
	{
		return CX_Status_InvalidArg;
	}
	pStruct   = (My_Namespace_Struct2_Ex *)pThis;
	pEnv = pStruct->helper.pEnv;
	pEnv->MemFree(pEnv, pStruct);

	return CX_Status_OK;
}

CX_StatusCode My_Namespace_Struct2_Create(My_Namespace_Struct2 **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset)
{
	My_Namespace_Struct2_Ex   *pStruct;
	CX_StatusCode   nStatus;

	if (NULL == *ppStruct || NULL == pBuffer || NULL == pEnv)
	{
		return CX_Status_InvalidArg;
	}
	if (NULL == (pStruct = (My_Namespace_Struct2_Ex *)pEnv->MemAlloc(pEnv, sizeof(My_Namespace_Struct2_Ex))))
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
		if (CXNOK(nStatus = CX_CB_Buffer_AddSize(pBuffer, 1665, pEnv)))
		{
			pEnv->MemFree(pEnv, pStruct);
		
			return nStatus;
		}
	pEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, 1665);
	}
	pStruct->base.SetMyBool = My_Namespace_Struct2_SetMyBool;
	pStruct->base.GetMyBool = My_Namespace_Struct2_GetMyBool;
	pStruct->base.GetMyStruct1 = My_Namespace_Struct2_GetMyStruct1;
	pStruct->base.GetMyStruct1StaticArrayCount = My_Namespace_Struct2_GetMyStruct1StaticArrayCount;
	pStruct->base.GetMyStruct1StaticArrayItem = My_Namespace_Struct2_GetMyStruct1StaticArrayItem;
	pStruct->base.GetMyStruct1ArrayCount = My_Namespace_Struct2_GetMyStruct1ArrayCount;
	pStruct->base.SetMyStruct1ArrayCount = My_Namespace_Struct2_SetMyStruct1ArrayCount;
	pStruct->base.GetMyStruct1ArrayItem = My_Namespace_Struct2_GetMyStruct1ArrayItem;
	pStruct->base.Destroy = &My_Namespace_Struct2_Destroy;

	*ppStruct = (My_Namespace_Struct2 *)pStruct;

	return CX_Status_OK;
}

