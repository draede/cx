
#include "../Include/SimpleBuffers/BinaryDataReader.h"
#include "../Include/SimpleBuffers/SBAPI.h"
#include "CX/C/ctype.h"
#include "CX/C/string.h"


#define CX_SB_BINARYDATAREADER_MAX_DEPTH        1024
#define CX_SB_BINARYDATAREADER_BUFFER_SIZE      16384
#define CX_SB_BINARYDATAREADER_MAX_STR_LEN      1024


static const CX_Byte CX_SB_BinaryDataReader_ArrayItem = 0x0;
static const CX_Byte CX_SB_BinaryDataReader_ArrayEnd = 0xFF;

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

typedef struct _CX_SB_BinaryDataReader
{
	CX_SB_InputStream    *pInputStream;
	CX_SB_Memory         *pMem;
	State                stack[CX_SB_BINARYDATAREADER_MAX_DEPTH];
	CX_Size              cDepth;
	CX_Byte              buffer[CX_SB_BINARYDATAREADER_MAX_DEPTH];
	CX_Size              cbBufSize;
	CX_Size              cbBufOffset;
}CX_SB_BinaryDataReader;


static CX_Bool CXSBBDREOF(CX_SB_BinaryDataReader *pReader)
{
	CX_StatusCode   nStatus;

	if (0 == pReader->cbBufSize || pReader->cbBufOffset >= pReader->cbBufSize)
	{
		if (CXNOK(nStatus = pReader->pInputStream->Read(pReader->pInputStream->pContext, pReader->buffer, sizeof(pReader->buffer), &pReader->cbBufSize)))
		{
			return CX_True;
		}
		pReader->cbBufOffset = 0;
	}

	return CX_False;
}

static int CXSBBDRGET(CX_SB_BinaryDataReader *pReader)
{
	CX_StatusCode   nStatus;

	if (0 == pReader->cbBufSize || pReader->cbBufOffset >= pReader->cbBufSize)
	{
		if (CXNOK(nStatus = pReader->pInputStream->Read(pReader->pInputStream->pContext, pReader->buffer, sizeof(pReader->buffer), &pReader->cbBufSize)))
		{
			return -1;
		}
		pReader->cbBufOffset = 0;
	}

	return pReader->buffer[pReader->cbBufOffset];
}

static void CXSBBDRNEXT(CX_SB_BinaryDataReader *pReader)
{
	CX_StatusCode   nStatus;

	if (pReader->cbBufOffset + 1 < pReader->cbBufSize)
	{
		pReader->cbBufOffset++;
	}
	else
	{
		if (CXNOK(nStatus = pReader->pInputStream->Read(pReader->pInputStream->pContext, pReader->buffer, sizeof(pReader->buffer), &pReader->cbBufSize)))
		{
			return;
		}
		pReader->cbBufOffset = 0;
	}
}

static CX_StatusCode CX_SB_BinaryDataReader_ReadData(CX_SB_BinaryDataReader *pReader, void *pData, CX_Size cbSize)
{
	CX_Byte         *pPos;

	pPos = (CX_Byte *)pData;
	while (0 < cbSize)
	{
		if (CXSBBDREOF(pReader))
		{
			return CX_Status_ParseFailed;
		}
		*pPos = (CX_Byte)CXSBBDRGET(pReader);
		CXSBBDRNEXT(pReader);
		pPos++;
		cbSize--;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ReadString(CX_SB_BinaryDataReader *pReader, CX_Char **pszStr)
{
	CX_UInt32       cLen;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, &cLen, sizeof(cLen))))
	{
		return nStatus;
	}
	if (NULL == (*pszStr = (CX_Char *)pReader->pMem->Alloc(pReader->pMem->pContext, (cLen + 1) * sizeof(CX_Char))))
	{
		return CX_Status_MemAllocFailed;
	}
	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, *pszStr, cLen * sizeof(CX_Char))))
	{
		pReader->pMem->Free(pReader->pMem->pContext, *pszStr);

		return nStatus;
	}
	(*pszStr)[cLen] = 0;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ReadWString(CX_SB_BinaryDataReader *pReader, CX_WChar **pwszStr)
{
	CX_UInt32       cLen;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, &cLen, sizeof(cLen))))
	{
		return nStatus;
	}
	if (NULL == (*pwszStr = (CX_WChar *)pReader->pMem->Alloc(pReader->pMem->pContext, (cLen + 1) * sizeof(CX_WChar))))
	{
		return CX_Status_MemAllocFailed;
	}
	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, *pwszStr, cLen * sizeof(CX_WChar))))
	{
		pReader->pMem->Free(pReader->pMem->pContext, *pwszStr);

		return nStatus;
	}
	(*pwszStr)[cLen] = 0;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_Begin(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_End(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructBeginStruct(void *pContext, const CX_Char *szName)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_BINARYDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructEndStruct(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructBeginArray(void *pContext, const CX_Char *szName)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_BINARYDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Array;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructEndArray(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructReadBool(void *pContext, const CX_Char *szName, CX_Bool *pbValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode            nStatus;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, pbValue, sizeof(*pbValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructReadInt(void *pContext, const CX_Char *szName, CX_Int64 *pnValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, pnValue, sizeof(*pnValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_StructReadReal(void *pContext, const CX_Char *szName, CX_Double *plfValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, plfValue, sizeof(*plfValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_String_Uninit
static CX_StatusCode CX_SB_BinaryDataReader_StructReadString(void *pContext, const CX_Char *szName, CX_Char **pszValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadString(pReader, pszValue)))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_UTFConv.FreeUTF16Mem
static CX_StatusCode CX_SB_BinaryDataReader_StructReadWString(void *pContext, const CX_Char *szName, CX_WChar **pwszValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	szName;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadWString(pReader, pwszValue)))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayBeginStruct(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_BINARYDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayEndStruct(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayBeginArray(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_BINARYDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Array;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayEndArray(void *pContext)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayReadBool(void *pContext, CX_Bool *pbValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode            nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd == CXSBBDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	else
	if (CX_SB_BinaryDataReader_ArrayItem != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, pbValue, sizeof(*pbValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayReadInt(void *pContext, CX_Int64 *pnValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd == CXSBBDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	else
	if (CX_SB_BinaryDataReader_ArrayItem != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, pnValue, sizeof(*pnValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_ArrayReadReal(void *pContext, CX_Double *plfValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd == CXSBBDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	else
	if (CX_SB_BinaryDataReader_ArrayItem != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadData(pReader, plfValue, sizeof(*plfValue))))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_String_Uninit
static CX_StatusCode CX_SB_BinaryDataReader_ArrayReadString(void *pContext, CX_Char **pszValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}

	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd == CXSBBDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	else
	if (CX_SB_BinaryDataReader_ArrayItem != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadString(pReader, pszValue)))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_UTFConv.FreeUTF16Mem
static CX_StatusCode CX_SB_BinaryDataReader_ArrayReadWString(void *pContext, CX_WChar **pwszValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;
	CX_StatusCode            nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	
	if (CXSBBDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (CX_SB_BinaryDataReader_ArrayEnd == CXSBBDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	else
	if (CX_SB_BinaryDataReader_ArrayItem != CXSBBDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBBDRNEXT(pReader);

	if (CXNOK(nStatus = CX_SB_BinaryDataReader_ReadWString(pReader, pwszValue)))
	{
		return nStatus;
	}

	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_FreeString(void *pContext, CX_Char *szValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	pReader->pMem->Free(pReader->pMem->pContext, szValue);

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_BinaryDataReader_FreeWString(void *pContext, CX_WChar *wszValue)
{
	CX_SB_BinaryDataReader   *pReader = (CX_SB_BinaryDataReader *)pContext;

	pReader->pMem->Free(pReader->pMem->pContext, wszValue);

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_BinaryDataReader_Create(CX_SB_DataReader *pDataReader, 
                                             CX_SB_InputStream *pInputStream, 
                                             CX_SB_Memory *pMem)
{
	CX_SB_BinaryDataReader *pReader;

	if (NULL == (pReader = (CX_SB_BinaryDataReader *)pMem->Alloc(pMem->pContext, 
	                                                           sizeof(CX_SB_BinaryDataReader))))
	{
		return CX_Status_MemAllocFailed;
	}
	pReader->cDepth                    = 0;
	pReader->pMem                     = pMem;
	pReader->pInputStream             = pInputStream;
	pReader->cbBufSize                = 0;
	pReader->cbBufOffset              = 0;
	pDataReader->pContext             = pReader;
	pDataReader->Begin                = CX_SB_BinaryDataReader_Begin;
	pDataReader->End                  = CX_SB_BinaryDataReader_End;
	pDataReader->StructBeginStruct    = &CX_SB_BinaryDataReader_StructBeginStruct;
	pDataReader->StructEndStruct      = &CX_SB_BinaryDataReader_StructEndStruct;
	pDataReader->StructBeginArray     = &CX_SB_BinaryDataReader_StructBeginArray;
	pDataReader->StructEndArray       = &CX_SB_BinaryDataReader_StructEndArray;
	pDataReader->StructReadBool       = &CX_SB_BinaryDataReader_StructReadBool;
	pDataReader->StructReadInt        = &CX_SB_BinaryDataReader_StructReadInt;
	pDataReader->StructReadReal       = &CX_SB_BinaryDataReader_StructReadReal;
	pDataReader->StructReadString     = &CX_SB_BinaryDataReader_StructReadString;
	pDataReader->StructReadWString    = &CX_SB_BinaryDataReader_StructReadWString;
	pDataReader->ArrayBeginStruct     = &CX_SB_BinaryDataReader_ArrayBeginStruct;
	pDataReader->ArrayEndStruct       = &CX_SB_BinaryDataReader_ArrayEndStruct;
	pDataReader->ArrayBeginArray      = &CX_SB_BinaryDataReader_ArrayBeginArray;
	pDataReader->ArrayEndArray        = &CX_SB_BinaryDataReader_ArrayEndArray;
	pDataReader->ArrayReadBool        = &CX_SB_BinaryDataReader_ArrayReadBool;
	pDataReader->ArrayReadInt         = &CX_SB_BinaryDataReader_ArrayReadInt;
	pDataReader->ArrayReadReal        = &CX_SB_BinaryDataReader_ArrayReadReal;
	pDataReader->ArrayReadString      = &CX_SB_BinaryDataReader_ArrayReadString;
	pDataReader->ArrayReadWString     = &CX_SB_BinaryDataReader_ArrayReadWString;
	pDataReader->FreeString           = &CX_SB_BinaryDataReader_FreeString;
	pDataReader->FreeWString          = &CX_SB_BinaryDataReader_FreeWString;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_BinaryDataReader_Destroy(CX_SB_DataReader *pDataReader)
{
	CX_SB_BinaryDataReader *pReader = (CX_SB_BinaryDataReader *)pDataReader->pContext;
	CX_SB_Memory         *pMem      = pReader->pMem;

	pMem->Free(pMem->pContext, pDataReader->pContext);
	pDataReader->pContext             = NULL;
	pDataReader->Begin                = NULL;
	pDataReader->End                  = NULL;
	pDataReader->StructBeginStruct    = NULL;
	pDataReader->StructEndStruct      = NULL;
	pDataReader->StructBeginArray     = NULL;
	pDataReader->StructEndArray       = NULL;
	pDataReader->StructReadBool       = NULL;
	pDataReader->StructReadInt        = NULL;
	pDataReader->StructReadReal       = NULL;
	pDataReader->StructReadString     = NULL;
	pDataReader->StructReadWString    = NULL;
	pDataReader->ArrayBeginStruct     = NULL;
	pDataReader->ArrayEndStruct       = NULL;
	pDataReader->ArrayBeginArray      = NULL;
	pDataReader->ArrayEndArray        = NULL;
	pDataReader->ArrayReadBool        = NULL;
	pDataReader->ArrayReadInt         = NULL;
	pDataReader->ArrayReadReal        = NULL;
	pDataReader->ArrayReadString      = NULL;
	pDataReader->ArrayReadWString     = NULL;
	pDataReader->FreeString           = NULL;
	pDataReader->FreeWString          = NULL;

	return CX_Status_OK;
}

