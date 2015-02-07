/* 
 * CX - C++ framework for general purpose developement
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


#ifdef __cplusplus
extern "C" {
#endif


#define CX_SCOPE

//don't use C++ RAII idioms in CX_SCOPE_TRY scope!
//when success/failure triggers are used destructors will not called!
//instead use CX_SCOPE_EXIT/CX_SCOPE_SUCCESS/CX_SCOPE_FAILURE as your exit points
#define CX_SCOPE_TRY                                                                               \
		CX_Scope_Result ___cx_scope_result = CX_Scope_Result_Succes;

#define CX_SCOPE_CATCH                                                                             \
		___cx_scope_label_catch:

#define CX_SCOPE_SUCCESS                                                                           \
			if (CX_Scope_Result_Succes == ___cx_scope_result)

#define CX_SCOPE_FAILURE                                                                           \
			if (CX_Scope_Result_Failure == ___cx_scope_result)

#define CX_SCOPE_EXIT

#define CX_SCOPE_TRIGGER_SUCCESS                                                                   \
			{                                                                                         \
				___cx_scope_result = CX_Scope_Result_Succes;                                           \
				goto ___cx_scope_label_catch;                                                          \
			}

#define CX_SCOPE_TRIGGER_FAILURE                                                                   \
			{                                                                                         \
				___cx_scope_result = CX_Scope_Result_Failure;                                          \
				goto ___cx_scope_label_catch;                                                          \
			}


typedef enum _CX_Scope_Result
{
	CX_Scope_Result_Failure,
	CX_Scope_Result_Succes,
}CX_Scope_Result;


#ifdef __cplusplus
}
#endif

