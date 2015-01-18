
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Sys/Platform/Windows/RWLock.h"
#else	
	#error "RWLock.h not implemented on this platform"
#endif

