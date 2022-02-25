/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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


#include <ctype.h>
#include <wctype.h>


//http://en.wikipedia.org/wiki/C_character_classification


#define cx_isalnum         isalnum
#define cxw_isalnum        iswalnum
#define cx_isalpha         isalpha
#define cxw_isalpha        iswalpha
#define cx_islower         islower
#define cxw_islower        iswlower
#define cx_isupper         isupper
#define cxw_isupper        iswupper
#define cx_isdigit         isdigit
#define cxw_isdigit        iswdigit
#define cx_isxdigit        isxdigit
#define cxw_isxdigit       iswxdigit
#define cx_iscntrl         iscntrl
#define cxw_iscntrl        iswcntrl
#define cx_isgraph         isgraph
#define cxw_isgraph        iswgraph
#define cx_isspace         isspace
#define cxw_isspace        iswspace
#define cx_isblank         isblank
#define cxw_isblank        iswblank
#define cx_isprint         isprint
#define cxw_isprint        iswprint
#define cx_ispunct         ispunct
#define cxw_ispunct        iswpunct
#define cx_tolower         tolower
#define cxw_tolower        towlower
#define cx_toupper         toupper
#define cxw_toupper        towupper

#ifndef CX_WCHAR_IS_DEFAULT
#define cxt_isalnum        cxw_isalnum
#define cxt_isalpha        cxw_isalpha
#define cxt_islower        cxw_islower
#define cxt_isupper        cxw_isupper
#define cxt_isdigit        cxw_isdigit
#define cxt_isxdigit       cxw_isxdigit
#define cxt_iscntrl        cxw_iscntrl
#define cxt_isgraph        cxw_isgraph
#define cxt_isspace        cxw_isspace
#define cxt_isblank        cxw_isblank
#define cxt_isprint        cxw_isprint
#define cxt_ispunct        cxw_ispunct
#define cxt_tolower        cxw_tolower
#define cxt_toupper        cxw_toupper
#else
#define cxt_isalnum        cx_isalnum
#define cxt_isalpha        cx_isalpha
#define cxt_islower        cx_islower
#define cxt_isupper        cx_isupper
#define cxt_isdigit        cx_isdigit
#define cxt_isxdigit       cx_isxdigit
#define cxt_iscntrl        cx_iscntrl
#define cxt_isgraph        cx_isgraph
#define cxt_isspace        cx_isspace
#define cxt_isblank        cx_isblank
#define cxt_isprint        cx_isprint
#define cxt_ispunct        cx_ispunct
#define cxt_tolower        cx_tolower
#define cxt_toupper        cx_toupper
#endif


#endif
