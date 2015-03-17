
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "Memory.h"
#include "DataWriter.h"
#include "OutputStream.h"


CX_StatusCode  CX_SB_BinaryDataWriter_Create(CX_SB_DataWriter *pDataWriter, 
                                             CX_SB_OutputStream *pOutputStream, 
                                             CX_SB_Memory *pMem);

CX_StatusCode  CX_SB_BinaryDataWriter_Destroy(CX_SB_DataWriter *pDataWriter);


#ifdef __cplusplus
}
#endif

