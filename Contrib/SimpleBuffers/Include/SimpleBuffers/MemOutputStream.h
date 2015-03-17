
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "OutputStream.h"
#include "Memory.h"


CX_StatusCode  CX_SB_MemOutputStream_Create(CX_SB_OutputStream *pOutputStream, CX_Size cbInitialSize, CX_Size cbChunkSize, 
                                            CX_Size cbMaxSize, CX_SB_Memory *pMem);

void *CX_SB_MemOutputStream_GetBuffer(CX_SB_OutputStream *pOutputStream);

CX_Size  CX_SB_MemOutputStream_GetUsedSize(CX_SB_OutputStream *pOutputStream);

CX_Size  CX_SB_MemOutputStream_GetTotalSize(CX_SB_OutputStream *pOutputStream);

CX_StatusCode  CX_SB_MemOutputStream_Destroy(CX_SB_OutputStream *pOutputStream, CX_Bool bDetachBuffer);


#ifdef __cplusplus
}
#endif

