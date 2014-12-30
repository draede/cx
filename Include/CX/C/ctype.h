
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/ctype.h"
#else	
	#error "ctype.h not implemented on this platform"
#endif

