

#pragma once


#include "CX/Platform.h"


#define CX_VER_MAJOR               0
#define CX_VER_MINOR               1
#define CX_VER_REVISION            0
#define CX_VER_BUILD               11

#define CX_VER_NAME                "CX" 
#define CX_VER_COPYRIGHT           "Copyright (C) 2014 - draede"
#define CX_VER_COMPANY             "draede"         

#ifdef CX_DEBUG
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (dbg, 64b) - https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (dbg, 32b) - https://github.com/draede/cx"
	#endif
#else
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (rel, 64b) - https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (rel, 32b) - https://github.com/draede/cx"
	#endif
#endif

