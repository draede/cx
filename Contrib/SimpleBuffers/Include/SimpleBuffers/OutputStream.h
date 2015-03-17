
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"


typedef CX_StatusCode (* CX_SB_OutputStream_Write_Func)(void *pContext, const void *pBuffer, 
                                                        CX_Size cbReqSize, CX_Size *pcbAckSize);


typedef struct _CX_SB_OutputStream
{
	void                            *pContext;
	CX_SB_OutputStream_Write_Func   Write;
}CX_SB_OutputStream;


CX_StatusCode CX_SB_OutputStream_Printf(CX_SB_OutputStream *pOutputStream, const CX_Char *szFormat, ...);


#ifdef __cplusplus
}
#endif

