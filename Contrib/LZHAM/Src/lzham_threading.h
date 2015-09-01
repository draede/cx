// File: lzham_threading.h
// See Copyright Notice and license at the end of include/lzham.h

#if 0
#if LZHAM_USE_WIN32_API
   #include "lzham_win32_threading.h"
#elif LZHAM_USE_PTHREADS_API
   #include "lzham_pthreads_threading.h"
#else
   #include "lzham_null_threading.h"
#endif

#endif

#include "lzham_null_threading.h"

