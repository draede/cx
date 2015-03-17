
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "CX/C/Limits.h"
#include "Memory.h"


//if cUTF8Len == CX_SIZE_MAX => use null terminator
//pcUTF16Len can be NULL
//free with CX_SB_WString_Uninit
typedef CX_StatusCode (* CX_SB_UTFConv_UTF8toUTF16_Func)(void *pContext, 
                                                         const CX_Char *szUTF8, CX_Size cUTF8Len,
                                                         CX_WChar **pwszUTF16, 
                                                         CX_Size *pcUTF16Len);

//if cUTF16Len == CX_SIZE_MAX => use null terminator
//pcUTF8Len can be NULL
//free with CX_SB_String_Uninit
typedef CX_StatusCode (* CX_SB_UTFConv_UTF16toUTF8_Func)(void *pContext, 
                                                         const CX_WChar *wszUTF16, 
                                                         CX_Size cUTF16Len,
                                                         CX_Char **pszUTF8, 
                                                         CX_Size *pcUTF8Len);


typedef struct _CX_SB_UTFConv
{
	void                              *pContext;
	CX_SB_UTFConv_UTF8toUTF16_Func    UTF8toUTF16;
	CX_SB_UTFConv_UTF16toUTF8_Func    UTF16toUTF8;
}CX_SB_UTFConv;


#ifdef __cplusplus
}
#endif

