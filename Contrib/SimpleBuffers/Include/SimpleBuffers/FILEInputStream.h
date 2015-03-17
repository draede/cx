
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/SimpleBuffers/InputStream.h"


CX_StatusCode  CX_SB_FILEInputStream_Create(CX_SB_InputStream *pInputStream, const CX_Char *szPath);

CX_StatusCode  CX_SB_FILEInputStream_CreateW(CX_SB_InputStream *pInputStream, 
                                             const CX_WChar *wszPath);

CX_StatusCode  CX_SB_FILEInputStream_Destroy(CX_SB_InputStream *pInputStream);


#ifdef __cplusplus
}
#endif

