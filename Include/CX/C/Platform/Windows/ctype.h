
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <ctype.h>


#define cx_isalnum        isalnum 
#define cx_isalpha        isalpha
#define cx_iscntrl        iscntrl
#define cx_isdigit        isdigit
#define cx_isgraph        isgraph
#define cx_islower        islower
#define cx_isupper        isupper
#define cx_isprint        isprint
#define cx_ispunct        ispunct
#define cx_isspace        isspace
#define cx_isxdigit       isxdigit
#define cx_tolower        tolower
#define cx_toupper        toupper


#endif
