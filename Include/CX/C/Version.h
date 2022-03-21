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


#define CX_VER_MAJOR               0
#define CX_VER_MINOR               2
#define CX_VER_BUILD               288
#define CX_VER_REVISION            0

#define CX_VER_NAME                "CX" 
#define CX_VER_COPYRIGHT           "Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com"
#define CX_VER_COMPANY             "draede [at] outlook [dot] com"         

#ifdef CX_DEBUG
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", dbg, 64b) - "                       \
		                                   "https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", dbg, 32b) - "                       \
		                                   "https://github.com/draede/cx"
	#endif
#else
	#ifdef CX_64BIT_ARCH
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", opt, 64b) - "                       \
		                                   "https://github.com/draede/cx"
	#else
		#define CX_VER_DESCRIPTION         "CX (" CX_BUILDSYS ", opt, 32b) -"                        \
		                                   " https://github.com/draede/cx"
	#endif
#endif


#define CX_VER_HLP_STRBASE(x)                 #x

#define CX_VER_HLP_STR(x)                    CX_VER_HLP_STRBASE(x)

#define CX_VER_HLP_NUMBER(a, b, c, d)        (a), (b), (c), (d)

#define CX_VER_HLP_STRING(a, b, c, d)        CX_VER_HLP_STR(a) "." CX_VER_HLP_STR(b) "."           \
                                             CX_VER_HLP_STR(c) "." CX_VER_HLP_STR(d)

#define CX_VER_HLP_SIGNATURE(a, b, c, d, n, cp, co, ds)   n " " CX_VER_HLP_STRING(a, b, c, d) ", " \
                                                          cp ", " ds

