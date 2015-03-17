
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"


typedef void * (* CX_SB_Memory_Alloc_Func)(void *pContext, CX_Size cbSize);

typedef void * (* CX_SB_Memory_Realloc_Func)(void *pContext, void *pPtr, CX_Size cbSize);

typedef void (* CX_SB_Memory_Free_Func)(void *pContext, void *pPtr);

typedef CX_Bool (* CX_SB_Memory_IsUsageCounted_Func)(void *pContext);

typedef CX_Size (* CX_SB_Memory_GetUsage_Func)(void *pContext);

typedef CX_Size (* CX_SB_Memory_GetMaxUsage_Func)(void *pContext);


typedef struct _CX_SB_Memory
{
	void                               *pContext;
	CX_SB_Memory_Alloc_Func            Alloc;
	CX_SB_Memory_Realloc_Func          Realloc;
	CX_SB_Memory_Free_Func             Free;
	CX_SB_Memory_IsUsageCounted_Func   IsUsageCounted;
	CX_SB_Memory_GetUsage_Func         GetUsage;
	CX_SB_Memory_GetMaxUsage_Func      GetMaxUsage;
}CX_SB_Memory;


#ifdef __cplusplus
}
#endif

