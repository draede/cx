
#include "../Include/SimpleBuffers/MemOutputStream.h"


typedef struct _CX_SB_MemOutputStream
{
	CX_SB_Memory   *pMemory;
	CX_Byte        *pBuffer;
	CX_Size        cbUsedSize;
	CX_Size        cbTotalSize;
	CX_Size        cbInitialSize;
	CX_Size        cbChunkSize;
	CX_Size        cbMaxSize;
}CX_SB_MemOutputStream;


static CX_StatusCode CX_SB_MemOutputStream_Write(void *pContext, const void *pBuffer, CX_Size cbReqSize, CX_Size *pcbAckSize)
{
	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pContext;
	CX_Size               cbSize;
	CX_Byte               *pTmp;
	const CX_Byte         *pSrc;

	if (0 == cbReqSize)
	{
		*pcbAckSize = 0;

		return CX_Status_OK;
	}
	if (cbReqSize + pOS->cbUsedSize > pOS->cbTotalSize)
	{
		cbSize = pOS->cbChunkSize * ((cbReqSize + pOS->cbUsedSize) / pOS->cbChunkSize);
		if (0 < (cbReqSize + pOS->cbUsedSize) % pOS->cbChunkSize)
		{
			cbSize += pOS->cbChunkSize;
		}
		if (cbSize > pOS->cbMaxSize)
		{
			return CX_Status_OutOfBounds;
		}
		if (NULL == (pTmp = (CX_Byte *)pOS->pMemory->Realloc(pOS->pMemory->pContext, pOS->pBuffer, cbSize)))
		{
			return CX_Status_MemAllocFailed;
		}
		pOS->pBuffer     = pTmp;
		pOS->cbTotalSize = cbSize;
	}

	*pcbAckSize = cbReqSize;
	cbSize      = cbReqSize;
	pSrc        = (const CX_Byte *)pBuffer;
	while (0 < cbSize)
	{
		*(pOS->pBuffer + pOS->cbUsedSize) = *pSrc;
		pSrc++;
		pOS->cbUsedSize++;
		cbSize++;
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_MemOutputStream_Create(CX_SB_OutputStream *pOutputStream, CX_Size cbInitialSize, CX_Size cbChunkSize, 
                                            CX_Size cbMaxSize, CX_SB_Memory *pMemory)
{
	if (0 == cbChunkSize)
	{
		return CX_Status_InvalidArg;
	}

	if (NULL == (pOutputStream->pContext = ( CX_SB_MemOutputStream *)pMemory->Alloc(pMemory->pContext, 
	                                                                                sizeof(CX_SB_MemOutputStream))))
	{
		return CX_Status_OpenFailed;
	}

	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pOutputStream->pContext;

	pOS->pMemory        = pMemory;
	pOS->pBuffer        = NULL;
	pOS->cbUsedSize     = 0;
	pOS->cbTotalSize    = 0;
	pOS->cbInitialSize  = cbInitialSize;
	pOS->cbChunkSize    = cbChunkSize;
	pOS->cbMaxSize      = cbMaxSize;

	if (0 < cbInitialSize)
	{
		if (NULL == (pOS->pBuffer = (CX_Byte *)pMemory->Alloc(pMemory->pContext, cbInitialSize)))
		{
			pOS->pMemory->Free(pOS->pMemory->pContext, pOS->pBuffer);

			return CX_Status_MemAllocFailed;
		}
		pOS->cbTotalSize = cbInitialSize;
	}

	pOutputStream->Write = &CX_SB_MemOutputStream_Write;

	return CX_Status_OK;
}

void *CX_SB_MemOutputStream_GetBuffer(CX_SB_OutputStream *pOutputStream)
{
	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pOutputStream->pContext;

	return pOS->pBuffer;
}

CX_Size  CX_SB_MemOutputStream_GetUsedSize(CX_SB_OutputStream *pOutputStream)
{
	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pOutputStream->pContext;

	return pOS->cbUsedSize;
}

CX_Size  CX_SB_MemOutputStream_GetTotalSize(CX_SB_OutputStream *pOutputStream)
{
	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pOutputStream->pContext;

	return pOS->cbTotalSize;
}

CX_StatusCode  CX_SB_MemOutputStream_Destroy(CX_SB_OutputStream *pOutputStream, CX_Bool bDetachBuffer)
{
	CX_SB_MemOutputStream *pOS = (CX_SB_MemOutputStream *)pOutputStream->pContext;

	if (!bDetachBuffer)
	{
		pOS->pMemory->Free(pOS->pMemory->pContext, pOS->pBuffer);
	}
	pOS->pMemory->Free(pOS->pMemory->pContext, pOutputStream->pContext);
	pOutputStream->pContext     = NULL;
	pOutputStream->Write        = NULL;

	return CX_Status_OK;
}

