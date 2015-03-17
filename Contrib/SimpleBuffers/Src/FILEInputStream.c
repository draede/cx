
#include "../Include/SimpleBuffers/FILEInputStream.h"
#include "CX/C/stdio.h"


static CX_StatusCode CX_SB_FILEInputStream_Read(void *pContext, void *pBuffer, 
                                                CX_Size cbReqSize, CX_Size *pcbAckSize)
{
	FILE *pFile = (FILE *)pContext;

	*pcbAckSize = fread(pBuffer, 1, cbReqSize, pFile);
	if (*pcbAckSize != cbReqSize)
	{
		if (ferror(pFile))
		{
			return CX_Status_ReadFailed;
		}
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEInputStream_Create(CX_SB_InputStream *pInputStream, const CX_Char *szPath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (pInputStream->pContext = cx_fopen(szPath, "rb")))
#pragma warning(pop)
	{
		return CX_Status_OpenFailed;
	}
	pInputStream->Read = &CX_SB_FILEInputStream_Read;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEInputStream_CreateW(CX_SB_InputStream *pInputStream, 
                                             const CX_WChar *wszPath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (pInputStream->pContext = cxw_fopen(wszPath, L"rb")))
#pragma warning(pop)
	{
		return CX_Status_OpenFailed;
	}
	pInputStream->Read = &CX_SB_FILEInputStream_Read;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEInputStream_Destroy(CX_SB_InputStream *pInputStream)
{
	fclose((FILE *)pInputStream->pContext);
	pInputStream->pContext     = NULL;
	pInputStream->Read         = NULL;

	return CX_Status_OK;
}

