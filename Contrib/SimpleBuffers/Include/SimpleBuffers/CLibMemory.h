
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "Memory.h"


CX_StatusCode CX_SB_CLibMemory_Create(CX_SB_Memory *pMemory);

CX_StatusCode CX_SB_CLibMemory_Destroy(CX_SB_Memory *pMemory);


#ifdef __cplusplus
}
#endif

