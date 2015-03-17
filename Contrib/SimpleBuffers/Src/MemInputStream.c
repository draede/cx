
#include "../Include/SimpleBuffers/MemInputStream.h"


typedef struct _CX_SB_MemInputStream
{
	CX_SB_Memory    *pMemory;
	const CX_Byte   *pBuffer;
	CX_Size         cbOffset;
	CX_Size         cbSize;
}CX_SB_MemInputStream;


static CX_StatusCode CX_SB_MemInputStream_Read(void *pContext, void *pBuffer, 
                                               CX_Size cbReqSize, CX_Size *pcbAckSize)
{
	CX_SB_MemInputStream *pIS = (CX_SB_MemInputStream *)pContext;
	CX_Byte              *pDst;
	CX_Size              cbSize;

	cbSize = pIS->cbSize - pIS->cbOffset;
	if (cbSize >= cbReqSize)
	{
		cbSize = cbReqSize;
	}
	*pcbAckSize = cbSize;
	pDst = (CX_Byte *)pBuffer;
	while (0 < cbSize)
	{
		*pDst = *(pIS->pBuffer + pIS->cbOffset);
		pIS->cbOffset++;
		cbSize--;
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_MemInputStream_Create(CX_SB_InputStream *pInputStream, const void *pBuffer, CX_Size cbSize, CX_SB_Memory *pMemory)
{
	if (NULL == (pInputStream->pContext = ( CX_SB_MemInputStream *)pMemory->Alloc(pMemory->pContext, sizeof(CX_SB_MemInputStream))))
	{
		return CX_Status_OpenFailed;
	}

	CX_SB_MemInputStream *pIS = (CX_SB_MemInputStream *)pInputStream->pContext;

	pIS->pMemory  = pMemory;
	pIS->pBuffer  = (const CX_Byte *)pBuffer;
	pIS->cbSize   = cbSize;
	pIS->cbOffset = 0;

	pInputStream->Read = &CX_SB_MemInputStream_Read;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_MemInputStream_Destroy(CX_SB_InputStream *pInputStream)
{
	CX_SB_MemInputStream *pIS = (CX_SB_MemInputStream *)pInputStream->pContext;

	pIS->pMemory->Free(pIS->pMemory->pContext, pInputStream->pContext);
	pInputStream->pContext     = NULL;
	pInputStream->Read         = NULL;

	return CX_Status_OK;
}

