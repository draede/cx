
#include "../Include/SimpleBuffers/TextDataReader.h"
#include "../Include/SimpleBuffers/SBAPI.h"
#include "CX/C/ctype.h"
#include "CX/C/string.h"


#define CX_SB_TEXTDATAREADER_MAX_DEPTH        1024
#define CX_SB_TEXTDATAREADER_BUFFER_SIZE      16384
#define CX_SB_TEXTDATAREADER_MAX_STR_LEN      1024


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

typedef struct _CX_SB_TextDataReader
{
	CX_SB_InputStream    *pInputStream;
	CX_SB_Memory         *pMem;
	CX_SB_UTFConv        *pUTFConv;
	State                stack[CX_SB_TEXTDATAREADER_MAX_DEPTH];
	CX_Size              cDepth;
	CX_Byte              buffer[CX_SB_TEXTDATAREADER_MAX_DEPTH];
	CX_Size              cbBufSize;
	CX_Size              cbBufOffset;
}CX_SB_TextDataReader;


static CX_Bool CXSBTDREOF(CX_SB_TextDataReader *pReader)
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

static int CXSBTDRGET(CX_SB_TextDataReader *pReader)
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

static void CXSBTDRNEXT(CX_SB_TextDataReader *pReader)
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

static CX_StatusCode CXSBTDRSKSP(CX_SB_TextDataReader *pReader)
{
	while (!CXSBTDREOF(pReader) && cx_isspace(CXSBTDRGET(pReader)))
	{
		CXSBTDRNEXT(pReader);
	}

	return CX_Status_OK;
}

static CX_StatusCode CXSBTDRSKCOMMA(CX_SB_TextDataReader *pReader)
{
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (',' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ReadName(CX_SB_TextDataReader *pReader, const CX_Char *szName)
{
	const CX_Char   *pPos;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('"' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pPos = szName;
	while (!CXSBTDREOF(pReader) && *pPos == CXSBTDRGET(pReader))
	{
		pPos++;
		CXSBTDRNEXT(pReader);
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('"' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (':' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ReadBool(CX_SB_TextDataReader *pReader, CX_Bool *pbValue)
{
	CX_Char         szStr[16];
	CX_Size         cLen;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	cLen        = 0;
	szStr[cLen] = 0;
	while (!CXSBTDREOF(pReader) && cx_isalpha(CXSBTDRGET(pReader)))
	{
		if (cLen + 1 >= sizeof(szStr) / sizeof(szStr[0]))
		{
			return CX_Status_OutOfBounds;
		}
		szStr[cLen] = (CX_Char)CXSBTDRGET(pReader);
		cLen++;
		szStr[cLen] = 0;
		CXSBTDRNEXT(pReader);
	}
	if (0 == cx_stricmp(szStr, "false"))
	{
		*pbValue = CX_False;

		return CX_Status_OK;
	}
	else
	if (0 == cx_stricmp(szStr, "true"))
	{
		*pbValue = CX_True;

		return CX_Status_OK;
	}
	else
	{
		return CX_Status_ParseFailed;
	}
}

static CX_StatusCode CX_SB_TextDataReader_ReadInt(CX_SB_TextDataReader *pReader, CX_Int64 *pnValue)
{
	CX_Size         cDigits;
	CX_Bool         bNeg;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	cDigits     = 0;
	bNeg        = CX_False;
	*pnValue    = 0;
	if ('+' == CXSBTDRGET(pReader))
	{
		CXSBTDRNEXT(pReader);
	}
	else
	if ('-' == CXSBTDRGET(pReader))
	{
		bNeg = CX_True;
		CXSBTDRNEXT(pReader);
	}
	while (!CXSBTDREOF(pReader) && cx_isdigit(CXSBTDRGET(pReader)))
	{
		(*pnValue) *= 10;
		(*pnValue) += CXSBTDRGET(pReader) - '0';
		cDigits++;
		CXSBTDRNEXT(pReader);
	}
	if (0 == cDigits)
	{
		return CX_Status_ParseFailed;
	}
	if (bNeg)
	{
		(*pnValue) = -(*pnValue);
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ReadReal(CX_SB_TextDataReader *pReader, CX_Double *plfValue)
{
	CX_Bool         bHasPoint;
	CX_Double       lfFactor;
	CX_Size         cDigits;
	CX_Bool         bNeg;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	cDigits      = 0;
	bNeg         = CX_False;
	bHasPoint    = CX_False;
	*plfValue    = 0.0;
	lfFactor     = 1.0;
	while (!CXSBTDREOF(pReader))
	{
		if ('.' == CXSBTDRGET(pReader))
		{
			if (bHasPoint)
			{
				return CX_Status_ParseFailed;
			}
			bHasPoint = CX_True;
			CXSBTDRNEXT(pReader);
		}
		else
		if (cx_isdigit(CXSBTDRGET(pReader)))
		{
			cDigits++;
			if (bHasPoint)
			{
				lfFactor *= 10;
			}
			(*plfValue) *= 10;
			(*plfValue) += CXSBTDRGET(pReader) - '0';
			CXSBTDRNEXT(pReader);
		}
		else
		{
			break;
		}
	}
	(*plfValue) /= lfFactor;
	if (0 == cDigits)
	{
		return CX_Status_ParseFailed;
	}
	if (bNeg)
	{
		(*plfValue) = -(*plfValue);
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ReadString(CX_SB_TextDataReader *pReader, CX_Char **pszStr)
{
	CX_Size         cLen;
	CX_Size         cTotalLen;
	CX_Char         chLast;
	CX_Char         ch;
	CX_Char         *pTmp;
	CX_Bool         bSkip;
	CX_Bool         bEscaped;
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('"' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);

	cTotalLen = 1024;
	if (NULL == (*pszStr = (CX_Char *)pReader->pMem->Alloc(pReader->pMem->pContext, cTotalLen * sizeof(CX_Char))))
	{
		return CX_Status_MemAllocFailed;
	}
	cLen            = 0;
	(*pszStr)[cLen] = 0;
	chLast          = ' ';
	for (;;)
	{
		if (CXSBTDREOF(pReader))
		{
			pReader->pMem->Free(pReader->pMem->pContext, *pszStr);

			return CX_Status_ParseFailed;
		}
		ch       = (CX_Char)CXSBTDRGET(pReader);
		bSkip    = CX_False;
		bEscaped = CX_False;
		if ('\\' == chLast)
		{
			bEscaped = CX_True;
			if ('"' == ch)
			{
				//mkey
			}
			else
			if ('\\' == ch)
			{
				//mkey
			}
			else
			if ('/' == ch)
			{
				//mkey
			}
			else
			if ('b' == ch)
			{
				ch = '\b';
			}
			else
			if ('f' == ch)
			{
				ch = '\f';
			}
			else
			if ('n' == ch)
			{
				ch = '\n';
			}
			else
			if ('r' == ch)
			{
				ch = '\r';
			}
			else
			if ('t' == ch)
			{
				ch = '\t';
			}
			else
			{
				pReader->pMem->Free(pReader->pMem->pContext, *pszStr);

				return CX_Status_ParseFailed;
			}
		}
		else
		{
			if ('\\' == ch)
			{
				chLast = '\\';
				CXSBTDRNEXT(pReader);
				bSkip = CX_True;
			}
			else
			{
				CXSBTDRNEXT(pReader);
			}
		}

		if (!bSkip && !bEscaped && '"' == ch)
		{
			break;
		}

		if (!bSkip)
		{
			if (cLen + 1 >= cTotalLen)
			{
				cTotalLen += 1024;
				if (NULL == (pTmp = (CX_Char *)pReader->pMem->Realloc(pReader->pMem->pContext, *pszStr, cTotalLen * sizeof(CX_Char))))
				{
					pReader->pMem->Free(pReader->pMem->pContext, *pszStr);

					return CX_Status_MemAllocFailed;
				}
				*pszStr = pTmp;
			}
			(*pszStr)[cLen] = ch;
			cLen++;
			(*pszStr)[cLen] = 0;
		}
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_Begin(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('{' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_End(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('}' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructBeginStruct(void *pContext, const CX_Char *szName)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_TEXTDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('{' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructEndStruct(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('}' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructBeginArray(void *pContext, const CX_Char *szName)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_TEXTDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('[' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Array;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructEndArray(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructReadBool(void *pContext, const CX_Char *szName, CX_Bool *pbValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadBool(pReader, pbValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructReadInt(void *pContext, const CX_Char *szName, CX_Int64 *pnValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadInt(pReader, pnValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_StructReadReal(void *pContext, const CX_Char *szName, CX_Double *plfValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadReal(pReader, plfValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_String_Uninit
static CX_StatusCode CX_SB_TextDataReader_StructReadString(void *pContext, const CX_Char *szName, CX_Char **pszValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadString(pReader, pszValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_UTFConv.FreeUTF16Mem
static CX_StatusCode CX_SB_TextDataReader_StructReadWString(void *pContext, const CX_Char *szName, CX_WChar **pwszValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_Char                *szValue;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadName(pReader, szName)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadString(pReader, &szValue)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = pReader->pUTFConv->UTF8toUTF16(pReader->pUTFConv->pContext, szValue, CX_SIZE_MAX, pwszValue, NULL)))
	{
		CX_SB_String_Uninit(&szValue, pReader->pMem);

		return nStatus;
	}
	CX_SB_String_Uninit(&szValue, pReader->pMem);
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayBeginStruct(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_TEXTDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('{' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Struct;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayEndStruct(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Struct != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('}' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayBeginArray(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (pReader->cDepth >= CX_SB_TEXTDATAREADER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if ('[' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->stack[pReader->cDepth - 1].cIndex++;
	pReader->stack[pReader->cDepth].nType  = StateType_Array;
	pReader->stack[pReader->cDepth].cIndex = 0;
	pReader->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayEndArray(void *pContext)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' != CXSBTDRGET(pReader))
	{
		return CX_Status_ParseFailed;
	}
	CXSBTDRNEXT(pReader);
	pReader->cDepth--;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayReadBool(void *pContext, CX_Bool *pbValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' == CXSBTDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadBool(pReader, pbValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayReadInt(void *pContext, CX_Int64 *pnValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' == CXSBTDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadInt(pReader, pnValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataReader_ArrayReadReal(void *pContext, CX_Double *plfValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' == CXSBTDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadReal(pReader, plfValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_String_Uninit
static CX_StatusCode CX_SB_TextDataReader_ArrayReadString(void *pContext, CX_Char **pszValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' == CXSBTDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadString(pReader, pszValue)))
	{
		return nStatus;
	}
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

//free with CX_SB_UTFConv.FreeUTF16Mem
static CX_StatusCode CX_SB_TextDataReader_ArrayReadWString(void *pContext, CX_WChar **pwszValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;
	CX_Char                *szValue;
	CX_StatusCode          nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidArg;
	}
	if (StateType_Array != pReader->stack[pReader->cDepth - 1].nType)
	{
		return CX_Status_InvalidArg;
	}
	if (CXNOK(nStatus = CXSBTDRSKSP(pReader)))
	{
		return nStatus;
	}
	if (CXSBTDREOF(pReader))
	{
		return CX_Status_ParseFailed;
	}
	if (']' == CXSBTDRGET(pReader))
	{
		return CX_Status_OutOfBounds;
	}
	if (0 < pReader->stack[pReader->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CXSBTDRSKCOMMA(pReader)))
		{
			return nStatus;
		}
	}
	if (CXNOK(nStatus = CX_SB_TextDataReader_ReadString(pReader, &szValue)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = pReader->pUTFConv->UTF8toUTF16(pReader->pUTFConv->pContext, szValue, CX_SIZE_MAX, pwszValue, NULL)))
	{
		CX_SB_String_Uninit(&szValue, pReader->pMem);

		return nStatus;
	}
	CX_SB_String_Uninit(&szValue, pReader->pMem);
	pReader->stack[pReader->cDepth - 1].cIndex++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_DataReader_FreeString(void *pContext, CX_Char *szValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;

	pReader->pMem->Free(pReader->pMem->pContext, szValue);

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_DataReader_FreeWString(void *pContext, CX_WChar *wszValue)
{
	CX_SB_TextDataReader   *pReader = (CX_SB_TextDataReader *)pContext;

	pReader->pMem->Free(pReader->pMem->pContext, wszValue);

	return CX_Status_OK;
}


CX_StatusCode  CX_SB_TextDataReader_Create(CX_SB_DataReader *pDataReader, 
                                           CX_SB_InputStream *pInputStream, 
                                           CX_SB_UTFConv *pUTFConv,
                                           CX_SB_Memory *pMem)
{
	CX_SB_TextDataReader *pReader;

	if (NULL == (pReader = (CX_SB_TextDataReader *)pMem->Alloc(pMem->pContext, 
	                                                           sizeof(CX_SB_TextDataReader))))
	{
		return CX_Status_MemAllocFailed;
	}
	pReader->cDepth                    = 0;
	pReader->pMem                     = pMem;
	pReader->pUTFConv                 = pUTFConv;
	pReader->pInputStream             = pInputStream;
	pReader->cbBufSize                = 0;
	pReader->cbBufOffset              = 0;
	pDataReader->pContext             = pReader;
	pDataReader->Begin                = CX_SB_TextDataReader_Begin;
	pDataReader->End                  = CX_SB_TextDataReader_End;
	pDataReader->StructBeginStruct    = &CX_SB_TextDataReader_StructBeginStruct;
	pDataReader->StructEndStruct      = &CX_SB_TextDataReader_StructEndStruct;
	pDataReader->StructBeginArray     = &CX_SB_TextDataReader_StructBeginArray;
	pDataReader->StructEndArray       = &CX_SB_TextDataReader_StructEndArray;
	pDataReader->StructReadBool       = &CX_SB_TextDataReader_StructReadBool;
	pDataReader->StructReadInt        = &CX_SB_TextDataReader_StructReadInt;
	pDataReader->StructReadReal       = &CX_SB_TextDataReader_StructReadReal;
	pDataReader->StructReadString     = &CX_SB_TextDataReader_StructReadString;
	pDataReader->StructReadWString    = &CX_SB_TextDataReader_StructReadWString;
	pDataReader->ArrayBeginStruct     = &CX_SB_TextDataReader_ArrayBeginStruct;
	pDataReader->ArrayEndStruct       = &CX_SB_TextDataReader_ArrayEndStruct;
	pDataReader->ArrayBeginArray      = &CX_SB_TextDataReader_ArrayBeginArray;
	pDataReader->ArrayEndArray        = &CX_SB_TextDataReader_ArrayEndArray;
	pDataReader->ArrayReadBool        = &CX_SB_TextDataReader_ArrayReadBool;
	pDataReader->ArrayReadInt         = &CX_SB_TextDataReader_ArrayReadInt;
	pDataReader->ArrayReadReal        = &CX_SB_TextDataReader_ArrayReadReal;
	pDataReader->ArrayReadString      = &CX_SB_TextDataReader_ArrayReadString;
	pDataReader->ArrayReadWString     = &CX_SB_TextDataReader_ArrayReadWString;
	pDataReader->FreeString           = &CX_SB_DataReader_FreeString;
	pDataReader->FreeWString          = &CX_SB_DataReader_FreeWString;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_TextDataReader_Destroy(CX_SB_DataReader *pDataReader)
{
	CX_SB_TextDataReader *pReader = (CX_SB_TextDataReader *)pDataReader->pContext;
	CX_SB_Memory         *pMem    = pReader->pMem;

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

