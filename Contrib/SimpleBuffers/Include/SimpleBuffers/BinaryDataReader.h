
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "Memory.h"
#include "DataReader.h"
#include "InputStream.h"


CX_StatusCode  CX_SB_BinaryDataReader_Create(CX_SB_DataReader *pDataReader, CX_SB_InputStream *pInputStream, CX_SB_Memory *pMem);

CX_StatusCode  CX_SB_BinaryDataReader_Destroy(CX_SB_DataReader *pDataReader);


#ifdef __cplusplus
}
#endif

