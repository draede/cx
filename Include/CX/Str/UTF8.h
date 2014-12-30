
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
	#include "CX/Str/Platform/Windows/UTF8.h"
#else	
	#error "UTF8.h not implemented on this platform"
#endif

