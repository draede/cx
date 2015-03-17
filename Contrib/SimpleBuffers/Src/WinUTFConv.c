
#include "../Include/SimpleBuffers/WinUTFConv.h"
#include "CX/C/Platform/Windows/windows.h"


typedef struct _CX_SB_WinUTFConv
{
	CX_SB_Memory     *pMem;
}CX_SB_WinUTFConv;


//if cUTF8Len == CX_SIZE_MAX => use null terminator
//pcUTF16Len can be NULL
//free with CX_SB_WString_Uninit
static CX_StatusCode CX_SB_WinUTFConv_UTF8toUTF16(void *pContext, 
                                                  const CX_Char *szUTF8, CX_Size cUTF8Len,
                                                  CX_WChar **pwszUTF16, 
                                                  CX_Size *pcUTF16Len)
{
	CX_SB_WinUTFConv *pConv = (CX_SB_WinUTFConv *)pContext;
	int              nRes;

	pContext;

	if (0 == cUTF8Len || (CX_SIZE_MAX == cUTF8Len && 0 == szUTF8[0]))
	{
		if (NULL == (*pwszUTF16 = pConv->pMem->Alloc(pConv->pMem->pContext, sizeof(CX_WChar))))
		{
			return CX_Status_MemAllocFailed;
		}
		(*pwszUTF16)[0] = 0;
		if (NULL != pcUTF16Len)
		{
			*pcUTF16Len = 0;
		}

		return CX_Status_OK;
	}

	if (0 >= (nRes = MultiByteToWideChar(CP_UTF8, 0, szUTF8, 
	                                     CX_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, NULL, 0)))
	{
		return CX_Status_OperationFailed;
	}
	if (NULL == (*pwszUTF16 = pConv->pMem->Alloc(pConv->pMem->pContext, nRes * sizeof(CX_WChar))))
	{
		return CX_Status_MemAllocFailed;
	}
	if (0 >= (nRes = MultiByteToWideChar(CP_UTF8, 0, szUTF8, 
	                                     CX_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, 
	                                     *pwszUTF16, nRes)))
	{
		pConv->pMem->Free(pConv->pMem->pContext, *pwszUTF16);

		return CX_Status_OperationFailed;
	}
	if (NULL != pcUTF16Len)
	{
		if (CX_SIZE_MAX == cUTF8Len)
		{
			*pcUTF16Len = (CX_Size)(nRes - 1);
		}
		else
		{
			*pcUTF16Len = (CX_Size)nRes;
		}
	}

	return CX_Status_OK;
}

//if cUTF16Len == CX_SIZE_MAX => use null terminator
//pcUTF8Len can be NULL
//free with CX_SB_String_Uninit
static CX_StatusCode CX_SB_WinUTFConv_UTF16toUTF8(void *pContext, 
                                                  const CX_WChar *wszUTF16, 
                                                  CX_Size cUTF16Len,
                                                  CX_Char **pszUTF8, 
                                                  CX_Size *pcUTF8Len)
{
	CX_SB_WinUTFConv *pConv = (CX_SB_WinUTFConv *)pContext;
	int              nRes;

	pContext;

	if (0 == cUTF16Len || (CX_SIZE_MAX == cUTF16Len && 0 == wszUTF16[0]))
	{
		if (NULL == (*pszUTF8 = pConv->pMem->Alloc(pConv->pMem->pContext, sizeof(CX_Char))))
		{
			return CX_Status_MemAllocFailed;
		}
		(*pszUTF8)[0] = 0;
		if (NULL != pcUTF8Len)
		{
			*pcUTF8Len = 0;
		}

		return CX_Status_OK;
	}

	if (0 >= (nRes = WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
	                                     CX_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, 
	                                     NULL, 0, NULL, NULL)))
	{
		return CX_Status_OperationFailed;
	}
	if (NULL == (*pszUTF8 = pConv->pMem->Alloc(pConv->pMem->pContext, nRes * sizeof(CX_Char))))
	{
		return CX_Status_MemAllocFailed;
	}
	if (0 >= (nRes = WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
	                                     CX_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, 
	                                     *pszUTF8, nRes, NULL, NULL)))
	{
		pConv->pMem->Free(pConv->pMem->pContext, *pszUTF8);

		return CX_Status_OperationFailed;
	}
	if (NULL != pcUTF8Len)
	{
		if (CX_SIZE_MAX == cUTF16Len)
		{
			*pcUTF8Len = (CX_Size)(nRes - 1);
		}
		else
		{
			*pcUTF8Len = (CX_Size)nRes;
		}
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_WinUTFConv_Create(CX_SB_UTFConv *pUTFConv, CX_SB_Memory *pMem)
{
	CX_SB_WinUTFConv *pConv;

	if (NULL == (pConv = (CX_SB_WinUTFConv *)pMem->Alloc(pMem->pContext, sizeof(CX_SB_WinUTFConv))))
	{
		return CX_Status_MemAllocFailed;
	}
	pConv->pMem               = pMem;
	pUTFConv->pContext        = pConv;
	pUTFConv->UTF8toUTF16     = &CX_SB_WinUTFConv_UTF8toUTF16;
	pUTFConv->UTF16toUTF8     = &CX_SB_WinUTFConv_UTF16toUTF8;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_WinUTFConv_Destroy(CX_SB_UTFConv *pUTFConv)
{
	CX_SB_WinUTFConv *pConv = (CX_SB_WinUTFConv *)pUTFConv->pContext;
	CX_SB_Memory     *pMem  = pConv->pMem;

	pMem->Free(pMem->pContext, pUTFConv->pContext);
	pUTFConv->pContext        = NULL;
	pUTFConv->UTF8toUTF16     = NULL;
	pUTFConv->UTF16toUTF8     = NULL;

	return CX_Status_OK;
}
