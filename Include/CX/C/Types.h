
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/Types.h"
#else	
	#error "Types.h not implemented on this platform"
#endif

