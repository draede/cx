
#pragma once


#include "CX/C/Platform.h"
#include "CX/C/Types.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/string.h"
#else	
	#error "string.h not implemented on this platform"
#endif

