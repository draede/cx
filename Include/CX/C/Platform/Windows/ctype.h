
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <ctype.h>
#include <wchar.h>


#define cx_isalnum                 isalnum 
#define cx_isalpha                 isalpha
#define cx_iscntrl                 iscntrl
#define cx_isdigit                 isdigit
#define cx_isgraph                 isgraph
#define cx_islower                 islower
#define cx_isupper                 isupper
#define cx_isprint                 isprint
#define cx_ispunct                 ispunct
#define cx_isspace                 isspace
#define cx_isxdigit                isxdigit
#define cx_tolower                 tolower
#define cx_toupper                 toupper

#define cxw_isalnum                iswalnum
#define cxw_isalpha                iswalpha
#define cxw_iscntrl                iswcntrl
#define cxw_isdigit                iswdigit
#define cxw_isgraph                iswgraph
#define cxw_islower                iswlower
#define cxw_isupper                iswupper
#define cxw_isprint                iswprint
#define cxw_ispunct                iswpunct
#define cxw_isspace                iswspace
#define cxw_isxdigit               iswxdigit
#define cxw_tolower                towlower
#define cxw_toupper                towupper

#ifndef CX_UNICODE
#define cxt_isalnum                cx_isalnum 
#define cxt_isalpha                cx_isalpha
#define cxt_iscntrl                cx_iscntrl
#define cxt_isdigit                cx_isdigit
#define cxt_isgraph                cx_isgraph
#define cxt_islower                cx_islower
#define cxt_isupper                cx_isupper
#define cxt_isprint                cx_isprint
#define cxt_ispunct                cx_ispunct
#define cxt_isspace                cx_isspace
#define cxt_isxdigit               cx_isxdigit
#define cxt_tolower                cx_tolower
#define cxt_toupper                cx_toupper
#else
#define cxt_isalnum                cxw_isalnum 
#define cxt_isalpha                cxw_isalpha
#define cxt_iscntrl                cxw_iscntrl
#define cxt_isdigit                cxw_isdigit
#define cxt_isgraph                cxw_isgraph
#define cxt_islower                cxw_islower
#define cxt_isupper                cxw_isupper
#define cxt_isprint                cxw_isprint
#define cxt_ispunct                cxw_ispunct
#define cxt_isspace                cxw_isspace
#define cxt_isxdigit               cxw_isxdigit
#define cxt_tolower                cxw_tolower
#define cxt_toupper                cxw_toupper
#endif


#endif
