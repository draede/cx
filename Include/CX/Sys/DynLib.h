
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/Sys/Platform/Windows/DynLib.h"
#else	
	#error "DynLib.h not implemented on this platform"
#endif

