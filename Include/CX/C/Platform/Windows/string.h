/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#if defined(CX_OS_WINDOWS)


#include <string.h>
#include <wchar.h>


#define cx_strcat                    strcat
#define cx_strchr                    strchr
#define cx_strcmp                    strcmp
#define cx_strcoll                   strcoll
#define cx_strcpy                    strcpy
#define cx_strcspn                   strcspn
#define cx_strdup                    strdup
#define cx_strerror                  strerror
#define cx_stricmp                   _stricmp
#define cx_stricoll                  _stricoll
#define cx_strlen                    strlen
#define cx_strlwr                    _strlwr
#define cx_strncat                   strncat
#define cx_strncmp                   strncmp
#define cx_strncoll                  _strncoll
#define cx_strncpy                   strncpy
#define cx_strnicmp                  _strnicmp
#define cx_strnicoll                 _strnicoll
#define cx_strnset                   _strnset
#define cx_strpbrk                   strpbrk
#define cx_strrev                    _strrev
#define cx_strrchr                   strrchr
#define cx_strset                    _strset
#define cx_strspn                    strspn
#define cx_strstr                    strstr
#define cx_strtok                    strtok
#define cx_strupr                    _strupr
#define cx_strxfrm                   strxfrm

#define cxw_strcat                   wcscat
#define cxw_strchr                   wcschr
#define cxw_strcmp                   wcscmp
#define cxw_strcoll                  wcscoll
#define cxw_strcpy                   wcscpy
#define cxw_strcspn                  wcscspn
#define cxw_strdup                   _wcsdup
#define cxw_strerror                 wcserror
#define cxw_stricmp                  _wcsicmp
#define cxw_stricoll                 _wcsicoll
#define cxw_strlen                   wcslen
#define cxw_strlwr                   _wcslwr
#define cxw_strncat                  wcsncat
#define cxw_strncmp                  wcsncmp
#define cxw_strncoll                 _wcsncoll
#define cxw_strncpy                  wcsncpy
#define cxw_strnicmp                 _wcsnicmp
#define cxw_strnicoll                _wcsnicoll
#define cxw_strnset                  _wcsnset
#define cxw_strpbrk                  wcspbrk
#define cxw_strrchr                  wcsrchr
#define cxw_strrev                   _wcsrev
#define cxw_strspn                   wcsspn
#define cxw_strset                   _wcsset
#define cxw_strstr                   wcsstr
#define cxw_strtok                   wcstok
#define cxw_strupr                   _wcsupr
#define cxw_strxfrm                  wcsxfrm

#ifndef CX_UNICODE
#define cxt_strcat                   cx_strcat
#define cxt_strchr                   cx_strchr
#define cxt_strcmp                   cx_strcmp
#define cxt_strcoll                  cx_strcoll
#define cxt_strcpy                   cx_strcpy
#define cxt_strcspn                  cx_strcspn
#define cxt_strdup                   cx_strdup
#define cxt_strerror                 cx_strerror
#define cxt_stricmp                  cx_stricmp
#define cxt_stricoll                 cx_stricoll
#define cxt_strlen                   cx_strlen
#define cxt_strlwr                   cx_strlwr
#define cxt_strncat                  cx_wcsncat
#define cxt_strncmp                  cx_strncmp
#define cxt_strncoll                 cx_strncoll
#define cxt_strncpy                  cx_strncpy
#define cxt_strnicmp                 cx_strnicmp
#define cxt_strnicoll                cx_strnicoll
#define cxt_strnset                  cx_strnset
#define cxt_strpbrk                  cx_strpbrk
#define cxt_strrchr                  cx_strrchr
#define cxt_strrev                   cx_strrev
#define cxt_strspn                   cx_strspn
#define cxt_strset                   cx_strset
#define cxt_strstr                   cx_strstr
#define cxt_strtok                   cx_strtok
#define cxt_strupr                   cx_strupr
#define cxt_strxfrm                  cx_strxfrm
#else
#define cxt_strcat                   cxw_strcat
#define cxt_strchr                   cxw_strchr
#define cxt_strcmp                   cxw_strcmp
#define cxt_strcoll                  cxw_strcoll
#define cxt_strcpy                   cxw_strcpy
#define cxt_strcspn                  cxw_strcspn
#define cxt_strdup                   cxw_strdup
#define cxt_strerror                 cxw_strerror
#define cxt_stricmp                  cxw_stricmp
#define cxt_stricoll                 cxw_stricoll
#define cxt_strlen                   cxw_strlen
#define cxt_strlwr                   cxw_strlwr
#define cxt_strncat                  cxw_wcsncat
#define cxt_strncmp                  cxw_strncmp
#define cxt_strncoll                 cxw_strncoll
#define cxt_strncpy                  cxw_strncpy
#define cxt_strnicmp                 cxw_strnicmp
#define cxt_strnicoll                cxw_strnicoll
#define cxt_strnset                  cxw_strnset
#define cxt_strpbrk                  cxw_strpbrk
#define cxt_strrchr                  cxw_strrchr
#define cxt_strrev                   cxw_strrev
#define cxt_strspn                   cxw_strspn
#define cxt_strset                   cxw_strset
#define cxt_strstr                   cxw_strstr
#define cxt_strtok                   cxw_strtok
#define cxt_strupr                   cxw_strupr
#define cxt_strxfrm                  cxw_strxfrm
#endif


#endif

