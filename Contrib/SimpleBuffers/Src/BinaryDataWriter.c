
#include "../Include/SimpleBuffers/BinaryDataWriter.h"
#include "../Include/SimpleBuffers/SBAPI.h"
#include "CX/C/Limits.h"


#define CX_SB_BINARYDATAWRITER_MAX_DEPTH      1024


static const CX_Byte CX_SB_BinaryDataWriter_ArrayItem = 0x0;
static const CX_Byte CX_SB_BinaryDataWriter_ArrayEnd  = 0xFF;


typedef enum _StateType
{
	StateType_Struct,
	StateType_Array,
}StateType;

typedef struct _State
{
	StateType nType;
	CX_Size   cIndex;
}State;

typedef struct _CX_SB_BinaryDataWriter
{
	CX_SB_OutputStream   *pOutputStream;
	CX_SB_Memory         *pMem;
	State                stack[CX_SB_BINARYDATAWRITER_MAX_DEPTH];
	CX_Size              cDepth;
}CX_SB_BinaryDataWriter;



static CX_StatusCode CX_SB_BinaryDataWriter_Begin(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (0 != pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Struct;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_End(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (1 != pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	pWriter->cDepth--;
	
	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructBeginStruct(void *pContext, const CX_Char *szName)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	szName;

	if (0 == pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_BINARYDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;
	pWriter->stack[pWriter->cDepth].nType  = StateType_Struct;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructEndStruct(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructBeginArray(void *pContext, const CX_Char *szName)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	szName;

	if (0 == pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_BINARYDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;
	pWriter->stack[pWriter->cDepth].nType  = StateType_Array;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;
	
	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructEndArray(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->cDepth--;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayEnd, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayEnd), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayEnd) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructWriteBool(void *pContext, const CX_Char *szName, CX_Bool bValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	szName;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &bValue, sizeof(bValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(bValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructWriteInt(void *pContext, const CX_Char *szName, CX_Int64 nValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	szName;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &nValue, sizeof(nValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(nValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructWriteReal(void *pContext, const CX_Char *szName, CX_Double lfValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	szName;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &lfValue, sizeof(lfValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(lfValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructWriteString(void *pContext, const CX_Char *szName, const CX_Char *szValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cLen;
	CX_UInt32                cTmpLen;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	szName;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	cLen = CX_SB_String_Len(szValue);
	if ((CX_Size)CX_UINT32_MAX < cLen)
	{
		return CX_Status_InvalidArg;
	}
	cTmpLen = (CX_UInt32)cLen;
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &cTmpLen, sizeof(cTmpLen), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(cTmpLen) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, szValue, sizeof(CX_Char) * cLen, 
	                                                  &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_Char) * cLen != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_StructWriteWString(void *pContext, const CX_Char *szName, const CX_WChar *wszValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cLen;
	CX_UInt32                cTmpLen;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	szName;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	cLen = CX_SB_WString_Len(wszValue);
	if ((CX_Size)CX_UINT32_MAX < cLen)
	{
		return CX_Status_InvalidArg;
	}
	cTmpLen = (CX_UInt32)cLen;
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &cTmpLen, sizeof(cTmpLen), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(cTmpLen) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, wszValue, sizeof(CX_WChar) * cLen, 
	                                                  &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_WChar) * cLen != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayBeginStruct(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_BINARYDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}
	
	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayEndStruct(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}
	
	pWriter->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayBeginArray(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_BINARYDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayEndArray(void *pContext)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->cDepth--;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayEnd, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayEnd), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayEnd) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}


	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayWriteBool(void *pContext, CX_Bool bValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayItem, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayItem), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayItem) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &bValue, sizeof(bValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(bValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayWriteInt(void *pContext, CX_Int64 nValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayItem, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayItem), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayItem) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &nValue, sizeof(nValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(nValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayWriteReal(void *pContext, CX_Double lfValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayItem, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayItem), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayItem) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &lfValue, sizeof(lfValue), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(lfValue) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayWriteString(void *pContext, const CX_Char *szValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cLen;
	CX_UInt32                cTmpLen;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayItem, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayItem), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayItem) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	cLen = CX_SB_String_Len(szValue);
	if ((CX_Size)CX_UINT32_MAX < cLen)
	{
		return CX_Status_InvalidArg;
	}
	cTmpLen = (CX_UInt32)cLen;
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &cTmpLen, sizeof(cTmpLen), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(cTmpLen) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, szValue, sizeof(CX_Char) * cLen, 
	                                                  &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_Char) * cLen != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataWriter_ArrayWriteWString(void *pContext, const CX_WChar *wszValue)
{
	CX_SB_BinaryDataWriter   *pWriter = (CX_SB_BinaryDataWriter *)pContext;
	CX_Size                  cLen;
	CX_UInt32                cTmpLen;
	CX_Size                  cbAckSize;
	CX_StatusCode            nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &CX_SB_BinaryDataWriter_ArrayItem, 
	                                                  sizeof(CX_SB_BinaryDataWriter_ArrayItem), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_SB_BinaryDataWriter_ArrayItem) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	cLen = CX_SB_WString_Len(wszValue);
	if ((CX_Size)CX_UINT32_MAX < cLen)
	{
		return CX_Status_InvalidArg;
	}
	cTmpLen = (CX_UInt32)cLen;
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, &cTmpLen, sizeof(cTmpLen), &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(cTmpLen) != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}
	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, wszValue, sizeof(CX_WChar) * cLen, 
	                                                  &cbAckSize)))
	{
		return nStatus;
	}
	if (sizeof(CX_WChar) * cLen != cbAckSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_BinaryDataWriter_Create(CX_SB_DataWriter *pDataWriter, CX_SB_OutputStream *pOutputStream, CX_SB_Memory *pMem)
{
	CX_SB_BinaryDataWriter *pWriter;

	if (NULL == (pWriter = (CX_SB_BinaryDataWriter *)pMem->Alloc(pMem->pContext, sizeof(CX_SB_BinaryDataWriter))))
	{
		return CX_Status_MemAllocFailed;
	}
	pWriter->cDepth                   = 0;
	pWriter->pMem                     = pMem;
	pWriter->pOutputStream            = pOutputStream;
	pDataWriter->pContext             = pWriter;
	pDataWriter->Begin                = CX_SB_BinaryDataWriter_Begin;
	pDataWriter->End                  = CX_SB_BinaryDataWriter_End;
	pDataWriter->StructBeginStruct    = &CX_SB_BinaryDataWriter_StructBeginStruct;
	pDataWriter->StructEndStruct      = &CX_SB_BinaryDataWriter_StructEndStruct;
	pDataWriter->StructBeginArray     = &CX_SB_BinaryDataWriter_StructBeginArray;
	pDataWriter->StructEndArray       = &CX_SB_BinaryDataWriter_StructEndArray;
	pDataWriter->StructWriteBool      = &CX_SB_BinaryDataWriter_StructWriteBool;
	pDataWriter->StructWriteInt       = &CX_SB_BinaryDataWriter_StructWriteInt;
	pDataWriter->StructWriteReal      = &CX_SB_BinaryDataWriter_StructWriteReal;
	pDataWriter->StructWriteString    = &CX_SB_BinaryDataWriter_StructWriteString;
	pDataWriter->StructWriteWString   = &CX_SB_BinaryDataWriter_StructWriteWString;
	pDataWriter->ArrayBeginStruct     = &CX_SB_BinaryDataWriter_ArrayBeginStruct;
	pDataWriter->ArrayEndStruct       = &CX_SB_BinaryDataWriter_ArrayEndStruct;
	pDataWriter->ArrayBeginArray      = &CX_SB_BinaryDataWriter_ArrayBeginArray;
	pDataWriter->ArrayEndArray        = &CX_SB_BinaryDataWriter_ArrayEndArray;
	pDataWriter->ArrayWriteBool       = &CX_SB_BinaryDataWriter_ArrayWriteBool;
	pDataWriter->ArrayWriteInt        = &CX_SB_BinaryDataWriter_ArrayWriteInt;
	pDataWriter->ArrayWriteReal       = &CX_SB_BinaryDataWriter_ArrayWriteReal;
	pDataWriter->ArrayWriteString     = &CX_SB_BinaryDataWriter_ArrayWriteString;
	pDataWriter->ArrayWriteWString    = &CX_SB_BinaryDataWriter_ArrayWriteWString;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_BinaryDataWriter_Destroy(CX_SB_DataWriter *pDataWriter)
{
	CX_SB_BinaryDataWriter *pWriter = (CX_SB_BinaryDataWriter *)pDataWriter->pContext;
	CX_SB_Memory         *pMem    = pWriter->pMem;

	pMem->Free(pMem->pContext, pDataWriter->pContext);
	pDataWriter->pContext             = NULL;
	pDataWriter->Begin                = NULL;
	pDataWriter->End                  = NULL;
	pDataWriter->StructBeginStruct    = NULL;
	pDataWriter->StructEndStruct      = NULL;
	pDataWriter->StructBeginArray     = NULL;
	pDataWriter->StructEndArray       = NULL;
	pDataWriter->StructWriteBool      = NULL;
	pDataWriter->StructWriteInt       = NULL;
	pDataWriter->StructWriteReal      = NULL;
	pDataWriter->StructWriteString    = NULL;
	pDataWriter->StructWriteWString   = NULL;
	pDataWriter->ArrayBeginStruct     = NULL;
	pDataWriter->ArrayEndStruct       = NULL;
	pDataWriter->ArrayBeginArray      = NULL;
	pDataWriter->ArrayEndArray        = NULL;
	pDataWriter->ArrayWriteBool       = NULL;
	pDataWriter->ArrayWriteInt        = NULL;
	pDataWriter->ArrayWriteReal       = NULL;
	pDataWriter->ArrayWriteString     = NULL;
	pDataWriter->ArrayWriteWString    = NULL;

	return CX_Status_OK;
}

