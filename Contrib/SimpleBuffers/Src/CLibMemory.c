
#include "../Include/SimpleBuffers/CLibMemory.h"
#include "CX/C/stdlib.h"


typedef struct _CX_SB_CLibMemory
{
	CX_Size cbMaxMemUsage;
	CX_Size cbMemUsage;
}CX_SB_CLibMemory;


static void *CX_SB_CLibMemory_Alloc(void *pContext, CX_Size cbSize)
{
	CX_SB_CLibMemory *pMemCtx = (CX_SB_CLibMemory *)pContext;
	void             *pMem;

	if (NULL != (pMem = malloc(sizeof(CX_Size) + cbSize)))
	{
		memcpy(pMem, &cbSize, sizeof(CX_Size));
		pMem = (CX_Byte *)pMem + sizeof(CX_Size);
		pMemCtx->cbMemUsage += cbSize;
		if (pMemCtx->cbMaxMemUsage < pMemCtx->cbMemUsage)
		{
			pMemCtx->cbMaxMemUsage = pMemCtx->cbMemUsage;
		}
	}

	return pMem;
}

static void *CX_SB_CLibMemory_Realloc(void *pContext, void *pPtr, CX_Size cbSize)
{
	CX_SB_CLibMemory *pMemCtx = (CX_SB_CLibMemory *)pContext;
	void             *pMem;
	CX_Size          cbAllocSize;

	if (NULL == pPtr)
	{
		return CX_SB_CLibMemory_Alloc(pContext, cbSize);
	}
	pMem = (CX_Byte *)pPtr - sizeof(CX_Size);
	memcpy(&cbAllocSize, pMem, sizeof(CX_Size));
	if (NULL != (pMem = realloc(pMem, sizeof(CX_Size) + cbSize)))
	{
		memcpy(pMem, &cbSize, sizeof(CX_Size));

		pMem = (CX_Byte *)pMem + sizeof(CX_Size);

		pMemCtx->cbMemUsage -= cbAllocSize;
		pMemCtx->cbMemUsage += cbSize;
		if (pMemCtx->cbMaxMemUsage < pMemCtx->cbMemUsage)
		{
			pMemCtx->cbMaxMemUsage = pMemCtx->cbMemUsage;
		}
	}

	return pMem;
}

static void CX_SB_CLibMemory_Free(void *pContext, void *pPtr)
{
	CX_SB_CLibMemory *pMemCtx = (CX_SB_CLibMemory *)pContext;
	void             *pMem;
	CX_Size          cbAllocSize;

	pMem = (CX_Byte *)pPtr - sizeof(CX_Size);
	memcpy(&cbAllocSize, pMem, sizeof(CX_Size));
	pMemCtx->cbMemUsage -= cbAllocSize;
	free(pMem);
}

static CX_Bool CX_SB_CLibMemory_IsUsageCounted(void *pContext)
{
	CX_SB_CLibMemory *pMem = (CX_SB_CLibMemory *)pContext;

	pMem;

	return CX_True;
}

static CX_Size CX_SB_CLibMemory_GetUsage(void *pContext)
{
	CX_SB_CLibMemory *pMem = (CX_SB_CLibMemory *)pContext;

	return pMem->cbMemUsage;
}

static CX_Size CX_SB_CLibMemory_GetMaxUsage(void *pContext)
{
	CX_SB_CLibMemory *pMem = (CX_SB_CLibMemory *)pContext;

	return pMem->cbMaxMemUsage;
}

CX_StatusCode  CX_SB_CLibMemory_Create(CX_SB_Memory *pMemory)
{
	CX_SB_CLibMemory *pMem;

	if (NULL == (pMem = (CX_SB_CLibMemory *)malloc(sizeof(CX_SB_CLibMemory))))
	{
		return CX_Status_MemAllocFailed;
	}
	pMem->cbMemUsage          = 0;
	pMem->cbMaxMemUsage       = 0;
	pMemory->pContext         = pMem;
	pMemory->Alloc            = &CX_SB_CLibMemory_Alloc;
	pMemory->Realloc          = &CX_SB_CLibMemory_Realloc;
	pMemory->Free             = &CX_SB_CLibMemory_Free;
	pMemory->IsUsageCounted   = &CX_SB_CLibMemory_IsUsageCounted;
	pMemory->GetUsage         = &CX_SB_CLibMemory_GetUsage;
	pMemory->GetMaxUsage      = &CX_SB_CLibMemory_GetMaxUsage;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_CLibMemory_Destroy(CX_SB_Memory *pMemory)
{
	free(pMemory->pContext);
	pMemory->pContext     = NULL;
	pMemory->Alloc        = NULL;
	pMemory->Realloc      = NULL;
	pMemory->Free         = NULL;

	return CX_Status_OK;
}

