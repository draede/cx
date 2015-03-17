
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"


typedef CX_StatusCode (* CX_SB_InputStream_Read_Func)(void *pContext, void *pBuffer, CX_Size cbReqSize, CX_Size *pcbAckSize);


typedef struct _CX_SB_InputStream
{
	void                         *pContext;
	CX_SB_InputStream_Read_Func   Read;
}CX_SB_InputStream;


#ifdef __cplusplus
}
#endif

