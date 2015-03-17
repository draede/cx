
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"


typedef CX_StatusCode (* CX_SB_DataWriter_Begin_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_End_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_StructBeginStruct_Func)(void *pContext, const CX_Char *szName);

typedef CX_StatusCode (* CX_SB_DataWriter_StructEndStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_StructBeginArray_Func)(void *pContext, const CX_Char *szName);

typedef CX_StatusCode (* CX_SB_DataWriter_StructEndArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_StructWriteBool_Func)(void *pContext, const CX_Char *szName, CX_Bool bValue);

typedef CX_StatusCode (* CX_SB_DataWriter_StructWriteInt_Func)(void *pContext, const CX_Char *szName, CX_Int64 nValue);

typedef CX_StatusCode (* CX_SB_DataWriter_StructWriteReal_Func)(void *pContext, const CX_Char *szName, CX_Double lfValue);

typedef CX_StatusCode (* CX_SB_DataWriter_StructWriteString_Func)(void *pContext, const CX_Char *szName, const CX_Char *szValue);

typedef CX_StatusCode (* CX_SB_DataWriter_StructWriteWString_Func)(void *pContext, const CX_Char *szName, 
                                                                   const CX_WChar *wszValue);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayBeginStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayEndStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayBeginArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayEndArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayWriteBool_Func)(void *pContext, CX_Bool bValue);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayWriteInt_Func)(void *pContext, CX_Int64 nValue);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayWriteReal_Func)(void *pContext, CX_Double lfValue);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayWriteString_Func)(void *pContext, const CX_Char *szValue);

typedef CX_StatusCode (* CX_SB_DataWriter_ArrayWriteWString_Func)(void *pContext, const CX_WChar *wszValue);


typedef struct _CX_SB_DataWriter
{
	void                                         *pContext;
	CX_SB_DataWriter_Begin_Func                  Begin;
	CX_SB_DataWriter_End_Func                    End;
	CX_SB_DataWriter_StructBeginStruct_Func      StructBeginStruct;
	CX_SB_DataWriter_StructEndStruct_Func        StructEndStruct;
	CX_SB_DataWriter_StructBeginArray_Func       StructBeginArray;
	CX_SB_DataWriter_StructEndArray_Func         StructEndArray;
	CX_SB_DataWriter_StructWriteBool_Func        StructWriteBool;
	CX_SB_DataWriter_StructWriteInt_Func         StructWriteInt;
	CX_SB_DataWriter_StructWriteReal_Func        StructWriteReal;
	CX_SB_DataWriter_StructWriteString_Func      StructWriteString;
	CX_SB_DataWriter_StructWriteWString_Func     StructWriteWString;
	CX_SB_DataWriter_ArrayBeginStruct_Func       ArrayBeginStruct;
	CX_SB_DataWriter_ArrayEndStruct_Func         ArrayEndStruct;
	CX_SB_DataWriter_ArrayBeginArray_Func        ArrayBeginArray;
	CX_SB_DataWriter_ArrayEndArray_Func          ArrayEndArray;
	CX_SB_DataWriter_ArrayWriteBool_Func         ArrayWriteBool;
	CX_SB_DataWriter_ArrayWriteInt_Func          ArrayWriteInt;
	CX_SB_DataWriter_ArrayWriteReal_Func         ArrayWriteReal;
	CX_SB_DataWriter_ArrayWriteString_Func       ArrayWriteString;
	CX_SB_DataWriter_ArrayWriteWString_Func      ArrayWriteWString;
}CX_SB_DataWriter;


#ifdef __cplusplus
}
#endif

