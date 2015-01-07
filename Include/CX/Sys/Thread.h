
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Sys/Platform/Windows/Thread.h"
#else	
	#error "Thread.h not implemented on this platform"
#endif

