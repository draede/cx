
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/stdio.h"
#else	
	#error "stdio.h not implemented on this platform"
#endif
