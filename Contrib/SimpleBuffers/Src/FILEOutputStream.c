
#include "../Include/SimpleBuffers/FILEOutputStream.h"
#include "CX/C/stdio.h"


static CX_StatusCode CX_SB_FILEOutputStream_Write(void *pContext, const void *pBuffer, CX_Size cbReqSize, CX_Size *pcbAckSize)
{
	FILE *pFile = (FILE *)pContext;

	*pcbAckSize = fwrite(pBuffer, 1, cbReqSize, pFile);
	if (*pcbAckSize != cbReqSize)
	{
		if (ferror(pFile))
		{
			return CX_Status_WriteFailed;
		}
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEOutputStream_Create(CX_SB_OutputStream *pOutputStream, const CX_Char *szPath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (pOutputStream->pContext = cx_fopen(szPath, "wb")))
#pragma warning(pop)
	{
		return CX_Status_OpenFailed;
	}
	pOutputStream->Write = &CX_SB_FILEOutputStream_Write;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEOutputStream_CreateW(CX_SB_OutputStream *pOutputStream, const CX_WChar *wszPath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (pOutputStream->pContext = cxw_fopen(wszPath, L"wb")))
#pragma warning(pop)
	{
		return CX_Status_CreateFailed;
	}
	pOutputStream->Write = &CX_SB_FILEOutputStream_Write;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_FILEOutputStream_Destroy(CX_SB_OutputStream *pOutputStream)
{
	fclose((FILE *)pOutputStream->pContext);
	pOutputStream->pContext     = NULL;
	pOutputStream->Write        = NULL;

	return CX_Status_OK;
}

