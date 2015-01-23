
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <stdlib.h>
#include <wchar.h>


#define cx_atoi                  atoi
#define cx_itoa                  itoa

#define cxw_atoi                 _wtoi
#define cxw_itoa                 _itow

#ifndef CX_UNICODE
#define cxt_atoi                 cx_atoi
#define cxt_itoa                 cxw_itoa
#else
#define cxt_atoi                 cxw_atoi
#define cxt_itoa                 cxw_itoa
#endif


#endif
