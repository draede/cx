/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_ANDROID)


#include <string.h>
#include <wchar.h>


//http://en.wikipedia.org/wiki/C_string_handling


//String manipulation
#define cx_strcpy          strcpy
#define cxw_strcpy         wcscpy
#define cx_strncpy         strncpy
#define cxw_strncpy        wcsncpy
#define cx_strcat          strcat
#define cxw_strcat         wcscat
#define cx_strncat         strncat
#define cxw_strncat        wcsncat
#define cx_strxfrm         strxfrm
#define cxw_strxfrm        wcsxfrm

#ifndef CX_WCHAR_IS_DEFAULT
#define cxt_strcpy         cxw_strcpy
#define cxt_strncpy        cxw_strncpy
#define cxt_strcat         cxw_strcat
#define cxt_strncat        cxw_strncat
#define cxt_strxfrm        cxw_strxfrm
#else
#define cxt_strcpy         cx_strcpy
#define cxt_strncpy        cx_strncpy
#define cxt_strcat         cx_strcat
#define cxt_strncat        cx_strncat
#define cxt_strxfrm        cx_strxfrm
#endif


//String examination
#define cx_strlen          strlen
#define cxw_strlen         wcslen
#define cx_strcmp          strcmp
#define cxw_strcmp         wcscmp
#define cx_strncmp         strncmp
#define cxw_strncmp        wcsncmp
#define cx_stricmp         strcasecmp
#define cxw_stricmp        wcscasecmp
#define cx_strnicmp        strncasecmp
#define cxw_strnicmp       wcsncasecmp
#define cx_strcoll         strcoll
#define cxw_strcoll        wcscoll
#define cx_strchr          strchr
#define cxw_strchr         wcschr
#define cx_strrchr         strrchr
#define cxw_strrchr        wcsrchr
#define cx_strspn          strspn
#define cxw_strspn         wcsspn
#define cx_strcspn         strcspn
#define cxw_strcspn        wcscspn
#define cx_strpbrk         strpbrk
#define cxw_wcspbrk        wcspbrk
#define cxw_strpbrk        wcspbrk
#define cx_strstr          strstr
#define cxw_wcsstr         wcsstr
#define cxw_strstr         wcsstr
#define cx_strtok          strtok
#define cxw_wcstok         wcstok
#define cxw_strtok         wcstok

#ifndef CX_WCHAR_IS_DEFAULT
#define cxt_strlen         cxw_strlen
#define cxt_strcmp         cxw_strcmp
#define cxt_strncmp        cxw_strncmp
#define cxt_stricmp        cxw_stricmp
#define cxt_strnicmp       cxw_strnicmp
#define cxt_strcoll        cxw_strcoll
#define cxt_strchr         cxw_strchr
#define cxt_strrchr        cxw_strrchr
#define cxt_strspn         cxw_strspn
#define cxt_strcspn        cxw_strcspn
#define cxt_strpbrk        cxw_strpbrk
#define cxt_strstr         cxw_strstr
#define cxt_strtok         cxw_strtok
#else
#define cxt_strlen         cx_strlen
#define cxt_strcmp         cx_strcmp
#define cxt_strncmp        cx_strncmp
#define cxt_stricmp        cx_stricmp
#define cxt_strnicmp       cx_strnicmp
#define cxt_strcoll        cx_strcoll
#define cxt_strchr         cx_strchr
#define cxt_strrchr        cx_strrchr
#define cxt_strspn         cx_strspn
#define cxt_strcspn        cx_strcspn
#define cxt_strpbrk        cx_strpbrk
#define cxt_strstr         cx_strstr
#define cxt_strtok         cx_strtok
#endif


//Miscellaneous
#define cx_strerror        strerror


//Memory manipulation
#define cx_memset          memset
#define cxw_memset         wmemset
#define cx_memcpy          memcpy
#define cxw_memcpy         wmemcpy
#define cx_memmove         memmove
#define cxw_memmove        wmemmove
#define cx_memcmp          memcmp
#define cxw_memcmp         wmemcmp
#define cx_memchr          memchr
#define cxw_memchr         wmemchr

#ifndef CX_WCHAR_IS_DEFAULT
#define cxt_memset         cxw_memset
#define cxt_memcpy         cxw_memcpy
#define cxt_memmove        cxw_memmove
#define cxt_memcmp         cxw_memcmp
#define cxt_memchr         cxw_memchr
#else
#define cxt_memset         cx_memset
#define cxt_memcpy         cx_memcpy
#define cxt_memmove        cx_memmove
#define cxt_memcmp         cx_memcmp
#define cxt_memchr         cx_memchr
#endif


#endif

