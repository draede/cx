
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "Memory.h"
#include "DataWriter.h"
#include "OutputStream.h"
#include "UTFConv.h"


CX_StatusCode CX_SB_TextDataWriter_Create(CX_SB_DataWriter *pDataWriter, 
                                                 CX_SB_OutputStream *pOutputStream, 
                                                 CX_SB_UTFConv *pUTFConv, 
                                                 CX_SB_Memory *pMem);

CX_StatusCode CX_SB_TextDataWriter_Destroy(CX_SB_DataWriter *pDataWriter);


#ifdef __cplusplus
}
#endif

