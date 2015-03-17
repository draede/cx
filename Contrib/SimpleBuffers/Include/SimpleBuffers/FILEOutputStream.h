
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "OutputStream.h"


CX_StatusCode  CX_SB_FILEOutputStream_Create(CX_SB_OutputStream *pOutputStream, const CX_Char *szPath);

CX_StatusCode  CX_SB_FILEOutputStream_CreateW(CX_SB_OutputStream *pOutputStream, 
                                              const CX_WChar *wszPath);

CX_StatusCode  CX_SB_FILEOutputStream_Destroy(CX_SB_OutputStream *pOutputStream);


#ifdef __cplusplus
}
#endif

