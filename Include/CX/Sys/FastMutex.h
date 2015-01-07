
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Sys/Platform/Windows/FastMutex.h"
#else	
	#error "FastMutex.h not implemented on this platform"
#endif

