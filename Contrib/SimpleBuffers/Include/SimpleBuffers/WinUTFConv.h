
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "UTFConv.h"


CX_StatusCode CX_SB_WinUTFConv_Create(CX_SB_UTFConv *pUTFConv, CX_SB_Memory *pMem);

CX_StatusCode CX_SB_WinUTFConv_Destroy(CX_SB_UTFConv *pUTFConv);


#ifdef __cplusplus
}
#endif

