
#include "../Include/SimpleBuffers/TextDataWriter.h"
#include "../Include/SimpleBuffers/SBAPI.h"


#define CX_SB_TEXTDATAWRITER_MAX_DEPTH      1024
#define CX_SB_TEXTDATAWRITER_INDENT         "   "
#define CX_SB_TEXTDATAWRITER_INDENTLEN      (sizeof(CX_SB_TEXTDATAWRITER_INDENT) / sizeof(CX_SB_TEXTDATAWRITER_INDENT[0]) - 1)


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

typedef struct _CX_SB_TextDataWriter
{
	CX_SB_OutputStream   *pOutputStream;
	CX_SB_UTFConv        *pUTFConv;
	CX_SB_Memory         *pMem;
	State                stack[CX_SB_TEXTDATAWRITER_MAX_DEPTH];
	CX_Size              cDepth;
}CX_SB_TextDataWriter;


static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteInt(void *pContext, CX_Int64 nValue);


static CX_StatusCode CX_SB_TextDataWriter_WriteAndEscapeString(CX_SB_TextDataWriter *pWriter, const CX_Char *szStr)
{
	CX_Size       cbSize;
	const CX_Char *pPos;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\"", 1, &cbSize)))
	{
		return nStatus;
	}
	if (1 != cbSize)
	{
		return CX_Status_WriteFailed;
	}

	pPos = szStr;
	while (0 != *pPos)
	{
		if ('"' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\\"", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\\' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\\\", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('/' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\/", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\b' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\b", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\f' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\f", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\n' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\n", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\r' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\r", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		if ('\t' == *pPos)
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\\t", 2, &cbSize)))
			{
				return nStatus;
			}
			if (2 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		else
		{
			if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, pPos, 1, &cbSize)))
			{
				return nStatus;
			}
			if (1 != cbSize)
			{
				return CX_Status_WriteFailed;
			}
		}
		pPos++;
	}

	if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, "\"", 1, &cbSize)))
	{
		return nStatus;
	}
	if (1 != cbSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_WriteIndent(CX_SB_TextDataWriter *pWriter)
{
	CX_Size         cIndex;
	CX_Size         cbAckSize;
	CX_StatusCode   nStatus;

	for (cIndex = 0; cIndex < pWriter->cDepth; cIndex++)
	{
		if (CXNOK(nStatus = pWriter->pOutputStream->Write(pWriter->pOutputStream->pContext, 
		                                            CX_SB_TEXTDATAWRITER_INDENT, 
		                                            CX_SB_TEXTDATAWRITER_INDENTLEN * sizeof(CX_Char), 
		                                            &cbAckSize)))
		{
			return nStatus;
		}
		if (CX_SB_TEXTDATAWRITER_INDENTLEN * sizeof(CX_Char) != cbAckSize)
		{
			return CX_Status_WriteFailed;
		}
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_Begin(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (0 != pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "{")))
	{
		return nStatus;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Struct;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_End(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 != pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n}")))
	{
		return nStatus;
	}
	pWriter->cDepth--;
	
	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructBeginStruct(void *pContext, const CX_Char *szName)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_TEXTDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\":\n", szName)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "{")))
	{
		return nStatus;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Struct;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructEndStruct(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
	{
		return nStatus;
	}
	
	pWriter->cDepth--;

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "}")))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructBeginArray(void *pContext, const CX_Char *szName)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (0 == pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_TEXTDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\":\n", szName)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "[")))
	{
		return nStatus;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Array;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;
	
	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructEndArray(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
	{
		return nStatus;
	}

	pWriter->cDepth--;
	
	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "]")))
	{
		return nStatus;
	}
	

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructWriteBool(void *pContext, const CX_Char *szName, 
                                                          CX_Bool bValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\": %s", szName, 
	                                              bValue ? "true" : "false")))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructWriteInt(void *pContext, const CX_Char *szName, 
                                                         CX_Int64 nValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\": %lld", szName,
	                                              nValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructWriteReal(void *pContext, const CX_Char *szName, 
                                                          CX_Double lfValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\": %.10lf", szName,
	                                              lfValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructWriteString(void *pContext, const CX_Char *szName, 
                                                            const CX_Char *szValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\": ", szName)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteAndEscapeString(pWriter, szValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_StructWriteWString(void *pContext, const CX_Char *szName, 
                                                             const CX_WChar *wszValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_Char                *szValue;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\"%s\": ", szName)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = pWriter->pUTFConv->UTF16toUTF8(pWriter->pUTFConv->pContext, wszValue, 
	                                                   CX_SIZE_MAX, &szValue, NULL)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteAndEscapeString(pWriter, szValue)))
	{
		CX_SB_String_Uninit(&szValue, pWriter->pMem);

		return nStatus;
	}

	CX_SB_String_Uninit(&szValue, pWriter->pMem);

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayBeginStruct(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_TEXTDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}
	
	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "{")))
	{
		return nStatus;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Struct;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayEndStruct(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Struct != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
	{
		return nStatus;
	}
	
	pWriter->cDepth--;

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "}")))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayBeginArray(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_SB_TEXTDATAWRITER_MAX_DEPTH <= pWriter->cDepth + 1)
	{
		return CX_Status_OperationFailed;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "[")))
	{
		return nStatus;
	}
	pWriter->stack[pWriter->cDepth].nType  = StateType_Array;
	pWriter->stack[pWriter->cDepth].cIndex = 0;
	pWriter->cDepth++;

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayEndArray(void *pContext)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 >= pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	if (0 == pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	pWriter->cDepth--;

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "]")))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteBool(void *pContext, CX_Bool bValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "%s",
	                                              bValue ? "true" : "false")))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteInt(void *pContext, CX_Int64 nValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "%lld", nValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteReal(void *pContext, CX_Double lfValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "%.10lf", lfValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteString(void *pContext, const CX_Char *szValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteAndEscapeString(pWriter, szValue)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_SB_TextDataWriter_ArrayWriteWString(void *pContext, 
                                                                  const CX_WChar *wszValue)
{
	CX_SB_TextDataWriter   *pWriter = (CX_SB_TextDataWriter *)pContext;
	CX_Char                *szValue;
	CX_StatusCode          nStatus;

	if (1 > pWriter->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (StateType_Array != pWriter->stack[pWriter->cDepth - 1].nType)
	{
		return CX_Status_InvalidCall;
	}

	pWriter->stack[pWriter->cDepth - 1].cIndex++;

	if (1 >= pWriter->stack[pWriter->cDepth - 1].cIndex)
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, "\n")))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_SB_OutputStream_Printf(pWriter->pOutputStream, ",\n")))
		{
			return nStatus;
		}
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteIndent(pWriter)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = pWriter->pUTFConv->UTF16toUTF8(pWriter->pUTFConv->pContext, wszValue, 
	                                                   CX_SIZE_MAX, &szValue, NULL)))
	{
		return nStatus;
	}

	if (CXNOK(nStatus = CX_SB_TextDataWriter_WriteAndEscapeString(pWriter, szValue)))
	{
		CX_SB_String_Uninit(&szValue, pWriter->pMem);

		return nStatus;
	}

	CX_SB_String_Uninit(&szValue, pWriter->pMem);

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_TextDataWriter_Create(CX_SB_DataWriter *pDataWriter, 
                                           CX_SB_OutputStream *pOutputStream, 
                                           CX_SB_UTFConv *pUTFConv, 
                                           CX_SB_Memory *pMem)
{
	CX_SB_TextDataWriter *pWriter;

	if (NULL == (pWriter = (CX_SB_TextDataWriter *)pMem->Alloc(pMem->pContext, 
	                                                           sizeof(CX_SB_TextDataWriter))))
	{
		return CX_Status_MemAllocFailed;
	}
	pWriter->cDepth                   = 0;
	pWriter->pUTFConv                 = pUTFConv;
	pWriter->pMem                     = pMem;
	pWriter->pOutputStream            = pOutputStream;
	pDataWriter->pContext             = pWriter;
	pDataWriter->Begin                = CX_SB_TextDataWriter_Begin;
	pDataWriter->End                  = CX_SB_TextDataWriter_End;
	pDataWriter->StructBeginStruct    = &CX_SB_TextDataWriter_StructBeginStruct;
	pDataWriter->StructEndStruct      = &CX_SB_TextDataWriter_StructEndStruct;
	pDataWriter->StructBeginArray     = &CX_SB_TextDataWriter_StructBeginArray;
	pDataWriter->StructEndArray       = &CX_SB_TextDataWriter_StructEndArray;
	pDataWriter->StructWriteBool      = &CX_SB_TextDataWriter_StructWriteBool;
	pDataWriter->StructWriteInt       = &CX_SB_TextDataWriter_StructWriteInt;
	pDataWriter->StructWriteReal      = &CX_SB_TextDataWriter_StructWriteReal;
	pDataWriter->StructWriteString    = &CX_SB_TextDataWriter_StructWriteString;
	pDataWriter->StructWriteWString   = &CX_SB_TextDataWriter_StructWriteWString;
	pDataWriter->ArrayBeginStruct     = &CX_SB_TextDataWriter_ArrayBeginStruct;
	pDataWriter->ArrayEndStruct       = &CX_SB_TextDataWriter_ArrayEndStruct;
	pDataWriter->ArrayBeginArray      = &CX_SB_TextDataWriter_ArrayBeginArray;
	pDataWriter->ArrayEndArray        = &CX_SB_TextDataWriter_ArrayEndArray;
	pDataWriter->ArrayWriteBool       = &CX_SB_TextDataWriter_ArrayWriteBool;
	pDataWriter->ArrayWriteInt        = &CX_SB_TextDataWriter_ArrayWriteInt;
	pDataWriter->ArrayWriteReal       = &CX_SB_TextDataWriter_ArrayWriteReal;
	pDataWriter->ArrayWriteString     = &CX_SB_TextDataWriter_ArrayWriteString;
	pDataWriter->ArrayWriteWString    = &CX_SB_TextDataWriter_ArrayWriteWString;

	return CX_Status_OK;
}

CX_StatusCode  CX_SB_TextDataWriter_Destroy(CX_SB_DataWriter *pDataWriter)
{
	CX_SB_TextDataWriter *pWriter = (CX_SB_TextDataWriter *)pDataWriter->pContext;
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

