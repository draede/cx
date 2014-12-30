
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/stdarg.h"
#else	
	#error "stdarg.h not implemented on this platform"
#endif
