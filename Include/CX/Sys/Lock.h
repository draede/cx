
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Sys/Platform/Windows/Lock.h"
#else	
	#error "Lock.h not implemented on this platform"
#endif

