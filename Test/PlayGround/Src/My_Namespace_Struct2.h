
#pragma once


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/Environment.h"
#include "CX/CBuffers/Types.h"
#include "My_Namespace_Struct1.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _My_Namespace_Struct2
{
	CX_StatusCode (* SetMyBool)(struct _My_Namespace_Struct2 *pThis, CX_Bool value);
	CX_Bool (* GetMyBool)(struct _My_Namespace_Struct2 *pThis);
	//You must call Destroy on ppStruct when done
	CX_StatusCode (* GetMyStruct1)(struct _My_Namespace_Struct2 *pThis, My_Namespace_Struct1 **ppStruct);
	CX_CB_Size (* GetMyStruct1StaticArrayCount)(struct _My_Namespace_Struct2 *pThis);
	//You must call Destroy on ppStruct when done
	CX_StatusCode (* GetMyStruct1StaticArrayItem)(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cIndex, My_Namespace_Struct1 **ppStruct);
	CX_CB_Size (* GetMyStruct1ArrayCount)(struct _My_Namespace_Struct2 *pThis);
	CX_StatusCode (* SetMyStruct1ArrayCount)(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cCount);
	//You must call Destroy on ppStruct when done
	CX_StatusCode (* GetMyStruct1ArrayItem)(struct _My_Namespace_Struct2 *pThis, CX_CB_Size cIndex, My_Namespace_Struct1 **ppStruct);
	CX_StatusCode (* Destroy)(struct _My_Namespace_Struct2 *pThis);
}My_Namespace_Struct2;

CX_StatusCode My_Namespace_Struct2_Create(My_Namespace_Struct2 **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset);

#ifdef __cplusplus
}
#endif

