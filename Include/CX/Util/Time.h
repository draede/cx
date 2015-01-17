
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Util/Platform/Windows/Time.h"
#else	
	#error "Time.h not implemented on this platform"
#endif

