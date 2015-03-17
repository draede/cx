
#include "../Include/SimpleBuffers/SBAPI.h"


static CX_Char    CX_SB_String_Default[]  = "";
static CX_WChar   CX_SB_WString_Default[] = L"";


CX_StatusCode CX_SB_String_Init(CX_Char **pszStr, CX_SB_Memory *pMem)
{
	pMem;

	*pszStr = CX_SB_String_Default;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_String_Uninit(CX_Char **pszStr, CX_SB_Memory *pMem)
{
	if (NULL != *pszStr && CX_SB_String_Default != *pszStr)
	{
		pMem->Free(pMem->pContext, *pszStr);

		*pszStr = CX_SB_String_Default;
	}

	return CX_Status_OK;
}

CX_Size CX_SB_String_Len(const CX_Char *szStr)
{
	const CX_Char *pszPos;

	pszPos = szStr;
	while (0 != *pszPos)
	{
		pszPos++;
	}

	return (pszPos - szStr);
}

CX_StatusCode CX_SB_String_Set(CX_Char **pszStr, const CX_Char *szValue, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;
	CX_Char       *szStr;
	CX_Char       *pszStrPos;
	const CX_Char *pszValPos;
	CX_Size       cLen;

	cLen = CX_SB_String_Len(szValue);
	if (NULL == (szStr = (CX_Char *)pMem->Alloc(pMem->pContext, (cLen + 1) * sizeof(CX_Char))))
	{
		return CX_Status_MemAllocFailed;
	}
	pszValPos = szValue;
	pszStrPos = szStr;
	for (;;)
	{
		*pszStrPos = *pszValPos;
		if (0 == *pszValPos)
		{
			break;
		}
		pszValPos++;
		pszStrPos++;
	}
	if (CXNOK(nStatus = CX_SB_String_Uninit(pszStr, pMem)))
	{
		pMem->Free(pMem->pContext, szStr);

		return nStatus;
	}
	*pszStr = szStr;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WString_Init(CX_WChar **pwszStr, CX_SB_Memory *pMem)
{
	pMem;

	*pwszStr = CX_SB_WString_Default;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WString_Uninit(CX_WChar **pwszStr, CX_SB_Memory *pMem)
{
	if (NULL != *pwszStr && CX_SB_WString_Default != *pwszStr)
	{
		pMem->Free(pMem->pContext, *pwszStr);

		*pwszStr = CX_SB_WString_Default;
	}

	return CX_Status_OK;
}

CX_Size CX_SB_WString_Len(const CX_WChar *wszStr)
{
	const CX_WChar *pwszPos;

	pwszPos = wszStr;
	while (0 != *pwszPos)
	{
		pwszPos++;
	}

	return (pwszPos - wszStr);
}

CX_StatusCode CX_SB_WString_Set(CX_WChar **pwszStr, const CX_WChar *wszValue, CX_SB_Memory *pMem)
{
	CX_StatusCode  nStatus;
	CX_WChar       *wszStr;
	CX_WChar       *pwszStrPos;
	const CX_WChar *pwszValPos;
	CX_Size        cLen;

	cLen = CX_SB_WString_Len(wszValue);
	if (NULL == (wszStr = (CX_WChar *)pMem->Alloc(pMem->pContext, (cLen + 1) * sizeof(CX_WChar))))
	{
		return CX_Status_MemAllocFailed;
	}
	pwszValPos = wszValue;
	pwszStrPos = wszStr;
	for (;;)
	{
		*pwszStrPos = *pwszValPos;
		if (0 == *pwszValPos)
		{
			break;
		}
		pwszValPos++;
		pwszStrPos++;
	}
	if (CXNOK(nStatus = CX_SB_WString_Uninit(pwszStr, pMem)))
	{
		pMem->Free(pMem->pContext, wszStr);

		return nStatus;
	}
	*pwszStr = wszStr;

	return CX_Status_OK;
}


CX_StatusCode CX_SB_BoolArray_Init(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_BoolArray_Uninit(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_BoolArray_GetCount(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_BoolArray_Set(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Bool val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Bool CX_SB_BoolArray_Get(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Bool defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_BoolArray_Add(CX_SB_BoolArray *pArray, CX_Bool val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_BoolArray_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_BoolArray_Remove(CX_SB_BoolArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_BoolArray_FreeUnusedMem(CX_SB_BoolArray *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Bool) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_BoolArray_Reserve(CX_SB_BoolArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Bool) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Bool *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int8Array_Init(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int8Array_Uninit(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_Int8Array_GetCount(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_Int8Array_Set(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Int8 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Int8 CX_SB_Int8Array_Get(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Int8 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_Int8Array_Add(CX_SB_Int8Array *pArray, CX_Int8 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_Int8Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int8Array_Remove(CX_SB_Int8Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int8Array_FreeUnusedMem(CX_SB_Int8Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int8) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int8Array_Reserve(CX_SB_Int8Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int8) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Int8 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt8Array_Init(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt8Array_Uninit(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_UInt8Array_GetCount(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_UInt8Array_Set(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_UInt8 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_UInt8 CX_SB_UInt8Array_Get(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_UInt8 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_UInt8Array_Add(CX_SB_UInt8Array *pArray, CX_UInt8 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_UInt8Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt8Array_Remove(CX_SB_UInt8Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt8Array_FreeUnusedMem(CX_SB_UInt8Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt8) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt8Array_Reserve(CX_SB_UInt8Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt8) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_UInt8 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int16Array_Init(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int16Array_Uninit(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_Int16Array_GetCount(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_Int16Array_Set(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Int16 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Int16 CX_SB_Int16Array_Get(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Int16 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_Int16Array_Add(CX_SB_Int16Array *pArray, CX_Int16 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_Int16Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int16Array_Remove(CX_SB_Int16Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int16Array_FreeUnusedMem(CX_SB_Int16Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int16) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int16Array_Reserve(CX_SB_Int16Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int16) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Int16 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt16Array_Init(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt16Array_Uninit(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_UInt16Array_GetCount(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_UInt16Array_Set(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_UInt16 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_UInt16 CX_SB_UInt16Array_Get(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_UInt16 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_UInt16Array_Add(CX_SB_UInt16Array *pArray, CX_UInt16 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_UInt16Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt16Array_Remove(CX_SB_UInt16Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt16Array_FreeUnusedMem(CX_SB_UInt16Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt16) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt16Array_Reserve(CX_SB_UInt16Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt16) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_UInt16 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int32Array_Init(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int32Array_Uninit(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_Int32Array_GetCount(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_Int32Array_Set(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Int32 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Int32 CX_SB_Int32Array_Get(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Int32 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_Int32Array_Add(CX_SB_Int32Array *pArray, CX_Int32 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_Int32Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int32Array_Remove(CX_SB_Int32Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int32Array_FreeUnusedMem(CX_SB_Int32Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int32) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int32Array_Reserve(CX_SB_Int32Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int32) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Int32 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt32Array_Init(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt32Array_Uninit(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_UInt32Array_GetCount(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_UInt32Array_Set(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_UInt32 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_UInt32 CX_SB_UInt32Array_Get(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_UInt32 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_UInt32Array_Add(CX_SB_UInt32Array *pArray, CX_UInt32 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_UInt32Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt32Array_Remove(CX_SB_UInt32Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt32Array_FreeUnusedMem(CX_SB_UInt32Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt32) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt32Array_Reserve(CX_SB_UInt32Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt32) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_UInt32 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int64Array_Init(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int64Array_Uninit(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_Int64Array_GetCount(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_Int64Array_Set(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Int64 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Int64 CX_SB_Int64Array_Get(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Int64 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_Int64Array_Add(CX_SB_Int64Array *pArray, CX_Int64 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_Int64Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int64Array_Remove(CX_SB_Int64Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int64Array_FreeUnusedMem(CX_SB_Int64Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int64) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_Int64Array_Reserve(CX_SB_Int64Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Int64) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Int64 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt64Array_Init(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt64Array_Uninit(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_UInt64Array_GetCount(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_UInt64Array_Set(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_UInt64 val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_UInt64 CX_SB_UInt64Array_Get(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_UInt64 defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_UInt64Array_Add(CX_SB_UInt64Array *pArray, CX_UInt64 val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_UInt64Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt64Array_Remove(CX_SB_UInt64Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt64Array_FreeUnusedMem(CX_SB_UInt64Array *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt64) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_UInt64Array_Reserve(CX_SB_UInt64Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_UInt64) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_UInt64 *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_FloatArray_Init(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_FloatArray_Uninit(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_FloatArray_GetCount(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_FloatArray_Set(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Float val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Float CX_SB_FloatArray_Get(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Float defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_FloatArray_Add(CX_SB_FloatArray *pArray, CX_Float val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_FloatArray_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_FloatArray_Remove(CX_SB_FloatArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_FloatArray_FreeUnusedMem(CX_SB_FloatArray *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Float) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_FloatArray_Reserve(CX_SB_FloatArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Float) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Float *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_DoubleArray_Init(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_DoubleArray_Uninit(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem)
{
	if (NULL != pArray->values)
	{
		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount = 0;
	pArray->cTotalCount = 0;
	pArray->values = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_DoubleArray_GetCount(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_DoubleArray_Set(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Double val, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	pArray->values[cIndex] = val;

	return CX_Status_OK;
}

CX_Double CX_SB_DoubleArray_Get(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Double defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_DoubleArray_Add(CX_SB_DoubleArray *pArray, CX_Double val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_DoubleArray_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->values[pArray->cUsedCount] = val;
	pArray->cUsedCount++;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_DoubleArray_Remove(CX_SB_DoubleArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_DoubleArray_FreeUnusedMem(CX_SB_DoubleArray *pArray, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Double) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_DoubleArray_Reserve(CX_SB_DoubleArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	void *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Double) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_Double *)pPtr;
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}


CX_StatusCode CX_SB_StringArray_Init(CX_SB_StringArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount  = 0;
	pArray->cTotalCount = 0;
	pArray->values      = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_StringArray_Uninit(CX_SB_StringArray *pArray, CX_SB_Memory *pMem)
{
	CX_Size cIndex;

	if (NULL != pArray->values)
	{
		for (cIndex = 0; cIndex < pArray->cTotalCount; cIndex++)
		{
			CX_SB_String_Uninit(&pArray->values[cIndex], pMem);
		}

		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount  = 0;
	pArray->cTotalCount = 0;
	pArray->values      = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_StringArray_GetCount(CX_SB_StringArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_StringArray_Set(CX_SB_StringArray *pArray, CX_Size cIndex, const CX_Char *val, CX_SB_Memory *pMem)
{
	CX_Char       *szStr;
	CX_StatusCode nStatus;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (CXNOK(nStatus = CX_SB_String_Init(&szStr, pMem)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_String_Set(&szStr, val, pMem)))
	{
		CX_SB_String_Uninit(&szStr, pMem);

		return nStatus;
	}
	CX_SB_String_Uninit(&pArray->values[cIndex], pMem);
	pArray->values[cIndex] = szStr;

	return CX_Status_OK;
}

const CX_Char *CX_SB_StringArray_Get(CX_SB_StringArray *pArray, CX_Size cIndex, const CX_Char *defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_StringArray_Add(CX_SB_StringArray *pArray, const CX_Char *val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_StringArray_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->cUsedCount++;
	if (CXNOK(nStatus = CX_SB_StringArray_Set(pArray, pArray->cUsedCount - 1, val, pMem)))
	{
		pArray->cUsedCount--;

		return nStatus;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_SB_StringArray_Remove(CX_SB_StringArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	for (i = 0; i < cCount; i++)
	{
		CX_SB_String_Uninit(&pArray->values[cIndex + i], pMem);
		CX_SB_String_Init(&pArray->values[cIndex + i], pMem);
	}

	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
		CX_SB_String_Init(&pArray->values[cSrcIndex + i], pMem);
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_StringArray_FreeUnusedMem(CX_SB_StringArray *pArray, CX_SB_Memory *pMem)
{
	CX_Size i;
	void    *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	for (i = pArray->cUsedCount; i < pArray->cTotalCount; i++)
	{
		CX_SB_String_Uninit(&pArray->values[i], pMem);
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Char *) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_StringArray_Reserve(CX_SB_StringArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	void    *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_Char *) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values      = (CX_Char **)pPtr;
	for (i = pArray->cTotalCount; i < cTotalCount; i++)
	{
		CX_SB_String_Init(&pArray->values[i], pMem);
	}
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}


CX_StatusCode CX_SB_WStringArray_Init(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	pArray->cUsedCount  = 0;
	pArray->cTotalCount = 0;
	pArray->values      = NULL;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WStringArray_Uninit(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem)
{
	CX_Size cIndex;

	if (NULL != pArray->values)
	{
		for (cIndex = 0; cIndex < pArray->cTotalCount; cIndex++)
		{
			CX_SB_WString_Uninit(&pArray->values[cIndex], pMem);
		}

		pMem->Free(pMem->pContext, pArray->values);
	}
	pArray->cUsedCount  = 0;
	pArray->cTotalCount = 0;
	pArray->values      = NULL;

	return CX_Status_OK;
}

CX_Size CX_SB_WStringArray_GetCount(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem)
{
	pMem;

	return pArray->cUsedCount;
}

CX_StatusCode CX_SB_WStringArray_Set(CX_SB_WStringArray *pArray, CX_Size cIndex, const CX_WChar *val, CX_SB_Memory *pMem)
{
	CX_WChar      *wszStr;
	CX_StatusCode nStatus;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (CXNOK(nStatus = CX_SB_WString_Init(&wszStr, pMem)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_WString_Set(&wszStr, val, pMem)))
	{
		CX_SB_WString_Uninit(&wszStr, pMem);

		return nStatus;
	}
	CX_SB_WString_Uninit(&pArray->values[cIndex], pMem);
	pArray->values[cIndex] = wszStr;

	return CX_Status_OK;
}

const CX_WChar *CX_SB_WStringArray_Get(CX_SB_WStringArray *pArray, CX_Size cIndex, const CX_WChar *defval, CX_SB_Memory *pMem)
{
	pMem;

	if (cIndex >= pArray->cUsedCount)
	{
		return defval;
	}

	return pArray->values[cIndex];
}

CX_StatusCode CX_SB_WStringArray_Add(CX_SB_WStringArray *pArray, const CX_WChar *val, CX_SB_Memory *pMem)
{
	CX_StatusCode nStatus;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		if (CXNOK(nStatus = CX_SB_WStringArray_Reserve(pArray, pArray->cUsedCount + 1, pMem)))
		{
			return nStatus;
		}
	}
	pArray->cUsedCount++;
	if (CXNOK(nStatus = CX_SB_WStringArray_Set(pArray, pArray->cUsedCount - 1, val, pMem)))
	{
		pArray->cUsedCount--;

		return nStatus;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WStringArray_Remove(CX_SB_WStringArray *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	CX_Size cSrcIndex;
	CX_Size cSrcCount;

	if (cIndex >= pArray->cUsedCount)
	{
		return CX_Status_OutOfBounds;
	}
	if (cIndex + cCount > pArray->cUsedCount)
	{
		cCount = pArray->cUsedCount - cIndex;
	}
	for (i = 0; i < cCount; i++)
	{
		CX_SB_WString_Uninit(&pArray->values[cIndex + i], pMem);
		CX_SB_WString_Init(&pArray->values[cIndex + i], pMem);
	}

	cSrcIndex = cIndex + cCount;
	cSrcCount = cCount;
	if (cSrcIndex + cSrcCount > pArray->cUsedCount)
	{
		cSrcCount = pArray->cUsedCount - cSrcIndex;
	}
	for (i = 0; i < cSrcCount; i++)
	{
		pArray->values[cIndex + i] = pArray->values[cSrcIndex + i];
		CX_SB_WString_Init(&pArray->values[cSrcIndex + i], pMem);
	}
	pArray->cUsedCount -= cCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WStringArray_FreeUnusedMem(CX_SB_WStringArray *pArray, CX_SB_Memory *pMem)
{
	CX_Size i;
	void    *pPtr;

	if (pArray->cUsedCount == pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	for (i = pArray->cUsedCount; i < pArray->cTotalCount; i++)
	{
		CX_SB_WString_Uninit(&pArray->values[i], pMem);
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_WChar *) * pArray->cUsedCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->cTotalCount = pArray->cUsedCount;

	return CX_Status_OK;
}

CX_StatusCode CX_SB_WStringArray_Reserve(CX_SB_WStringArray *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)
{
	CX_Size i;
	void    *pPtr;

	if (cTotalCount <= pArray->cTotalCount)
	{
		return CX_Status_OK;
	}
	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_WChar *) * cTotalCount)))
	{
		return CX_Status_MemAllocFailed;
	}
	pArray->values = (CX_WChar **)pPtr;
	for (i = pArray->cTotalCount; i < cTotalCount; i++)
	{
		CX_SB_WString_Init(&pArray->values[i], pMem);
	}
	pArray->cTotalCount = cTotalCount;

	return CX_Status_OK;
}

