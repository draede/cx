
#pragma once


#include "CX/Platform.h"


#ifdef CX_API_EXPORTS
	#define CX_API         CX_DLL_EXPORT
#else
	#define CX_API         CX_DLL_IMPORT
#endif


