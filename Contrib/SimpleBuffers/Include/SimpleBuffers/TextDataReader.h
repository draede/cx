
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "Memory.h"
#include "DataReader.h"
#include "InputStream.h"
#include "UTFConv.h"
#include "CX/C/APIDefs.h"


CX_StatusCode  CX_SB_TextDataReader_Create(CX_SB_DataReader *pDataReader, 
                                           CX_SB_InputStream *pInputStream, 
                                           CX_SB_UTFConv *pUTFConv,
                                           CX_SB_Memory *pMem);

CX_StatusCode  CX_SB_TextDataReader_Destroy(CX_SB_DataReader *pDataReader);


#ifdef __cplusplus
}
#endif

