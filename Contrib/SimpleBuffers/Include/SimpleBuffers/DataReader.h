
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"


typedef CX_StatusCode (* CX_SB_DataReader_Begin_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_End_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_StructBeginStruct_Func)(void *pContext, const CX_Char *szName);

typedef CX_StatusCode (* CX_SB_DataReader_StructEndStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_StructBeginArray_Func)(void *pContext, const CX_Char *szName);

typedef CX_StatusCode (* CX_SB_DataReader_StructEndArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_StructReadBool_Func)(void *pContext, const CX_Char *szName, CX_Bool *pbValue);

typedef CX_StatusCode (* CX_SB_DataReader_StructReadInt_Func)(void *pContext, const CX_Char *szName, CX_Int64 *pnValue);

typedef CX_StatusCode (* CX_SB_DataReader_StructReadReal_Func)(void *pContext, const CX_Char *szName, CX_Double *plfValue);

//free with CX_SB_DataReader_FreeString_Func
typedef CX_StatusCode (* CX_SB_DataReader_StructReadString_Func)(void *pContext, const CX_Char *szName, CX_Char **pszValue);

//free with CX_SB_DataReader_FreeWString_Func
typedef CX_StatusCode (* CX_SB_DataReader_StructReadWString_Func)(void *pContext, const CX_Char *szName, CX_WChar **pwszValue);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayBeginStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayEndStruct_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayBeginArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayEndArray_Func)(void *pContext);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayReadBool_Func)(void *pContext, CX_Bool *pbValue);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayReadInt_Func)(void *pContext, CX_Int64 *pnValue);

typedef CX_StatusCode (* CX_SB_DataReader_ArrayReadReal_Func)(void *pContext, CX_Double *plfValue);

//free with CX_SB_DataReader_FreeString_Func
typedef CX_StatusCode (* CX_SB_DataReader_ArrayReadString_Func)(void *pContext, CX_Char **pszValue);

//free with CX_SB_DataReader_FreeWString_Func
typedef CX_StatusCode (* CX_SB_DataReader_ArrayReadWString_Func)(void *pContext, CX_WChar **pwszValue);

typedef CX_StatusCode (* CX_SB_DataReader_FreeString_Func)(void *pContext, CX_Char *szValue);

typedef CX_StatusCode (* CX_SB_DataReader_FreeWString_Func)(void *pContext, CX_WChar *wszValue);


typedef struct _CX_SB_DataReader
{
	void                                         *pContext;
	CX_SB_DataReader_Begin_Func                  Begin;
	CX_SB_DataReader_End_Func                    End;
	CX_SB_DataReader_StructBeginStruct_Func      StructBeginStruct;
	CX_SB_DataReader_StructEndStruct_Func        StructEndStruct;
	CX_SB_DataReader_StructBeginArray_Func       StructBeginArray;
	CX_SB_DataReader_StructEndArray_Func         StructEndArray;
	CX_SB_DataReader_StructReadBool_Func         StructReadBool;
	CX_SB_DataReader_StructReadInt_Func          StructReadInt;
	CX_SB_DataReader_StructReadReal_Func         StructReadReal;
	CX_SB_DataReader_StructReadString_Func       StructReadString;
	CX_SB_DataReader_StructReadWString_Func      StructReadWString;
	CX_SB_DataReader_ArrayBeginStruct_Func       ArrayBeginStruct;
	CX_SB_DataReader_ArrayEndStruct_Func         ArrayEndStruct;
	CX_SB_DataReader_ArrayBeginArray_Func        ArrayBeginArray;
	CX_SB_DataReader_ArrayEndArray_Func          ArrayEndArray;
	CX_SB_DataReader_ArrayReadBool_Func          ArrayReadBool;
	CX_SB_DataReader_ArrayReadInt_Func           ArrayReadInt;
	CX_SB_DataReader_ArrayReadReal_Func          ArrayReadReal;
	CX_SB_DataReader_ArrayReadString_Func        ArrayReadString;
	CX_SB_DataReader_ArrayReadWString_Func       ArrayReadWString;
	CX_SB_DataReader_FreeString_Func             FreeString;
	CX_SB_DataReader_FreeWString_Func            FreeWString;
}CX_SB_DataReader;


#ifdef __cplusplus
}
#endif

