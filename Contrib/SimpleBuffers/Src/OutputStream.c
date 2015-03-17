
#include "../Include/SimpleBuffers/OutputStream.h"
#include "CX/C/stdarg.h"
#include "CX/C/stdio.h"


#define CX_SB_OUTPUTSTREAM_PRINTF_MAXBUFFERLEN     8192


CX_StatusCode CX_SB_OutputStream_Printf(CX_SB_OutputStream *pOutputStream, const CX_Char *szFormat, ...)
{
	CX_Char         szBuffer[CX_SB_OUTPUTSTREAM_PRINTF_MAXBUFFERLEN];
	va_list         vaList;
	CX_Size         cbReqSize;
	int             cChars;
	CX_StatusCode   nStatus;

	va_start(vaList, szFormat);
#pragma warning(push)
#pragma warning(disable: 4996)
	cChars = cx_vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0]), szFormat, vaList);
#pragma warning(pop)
	va_end(vaList);

	if (0 > cChars || cChars >= sizeof(szBuffer) / sizeof(szBuffer[0]))
	{
		return CX_Status_OperationFailed;
	}
	if (CXNOK(nStatus = pOutputStream->Write(pOutputStream->pContext, szBuffer, 
	                                         cChars * sizeof(CX_Char), &cbReqSize)))
	{
		return nStatus;
	}
	if (cChars * sizeof(CX_Char) != cbReqSize)
	{
		return CX_Status_WriteFailed;
	}

	return CX_Status_OK;
}

