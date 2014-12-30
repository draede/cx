
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <string.h>


#define cx_strcat      strcat
#define cx_strncat     strncat
#define cx_strchr      strchr
#define cx_strrchr     strrchr
#define cx_strcmp      strcmp
#define cx_strncmp     strncmp
#define cx_strcoll     strcoll
#define cx_strcpy      strcpy
#define cx_strncpy     strncpy
#define cx_strerror    strerror
#define cx_strlen      strlen
#define cx_strspn      strspn
#define cx_strcspn     strcspn
#define cx_strpbrk     strpbrk
#define cx_strstr      strstr
#define cx_strtok      strtok
#define cx_strxfrm     strxfrm
#define cx_strset      _strset
#define cx_strnset     _strnset
#define cx_strlwr      _strlwr
#define cx_strupr      _strupr
#define cx_stricmp     _stricmp
#define cx_strnicmp    _strnicmp
#define cx_stricoll    _stricoll
#define cx_strncoll    _strncoll
#define cx_strnicoll   _strnicoll
#define cx_strrev      _strrev
#define cx_strdup      strdup


#endif
