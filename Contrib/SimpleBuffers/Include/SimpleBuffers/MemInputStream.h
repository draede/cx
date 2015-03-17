
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "InputStream.h"
#include "Memory.h"


CX_StatusCode  CX_SB_MemInputStream_Create(CX_SB_InputStream *pInputStream, const void *pBuffer, CX_Size cbSize, CX_SB_Memory *pMemory);

CX_StatusCode  CX_SB_MemInputStream_Destroy(CX_SB_InputStream *pInputStream);


#ifdef __cplusplus
}
#endif

