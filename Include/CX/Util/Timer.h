
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Util/Platform/Windows/Timer.h"
#else	
	#error "Timer.h not implemented on this platform"
#endif

