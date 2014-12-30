
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/stdlib.h"
#else	
	#error "stdlib.h not implemented on this platform"
#endif
