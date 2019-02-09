/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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


#include "CX/Types.hpp"
#include "CX/Status.hpp"


#define CX_PROF_CONCAT(x, y)         x##y
#define CX_PROF_MACRO_CONCAT(x, y)   CX_PROF_CONCAT(x, y)

#ifdef CX_PROFILER
#define CX_SCOPEPROF(name)                                                                                             \
CX::Debug::ScopeProfiler   CX_PROF_MACRO_CONCAT(__cx_scope_prof__, __COUNTER)(__FILE__, name, __LINE__);
#define CX_ASCOPEPROF                CX_SCOPEPROF(# __LINE__)
#define CX_FUNCPROF                  CX_SCOPEPROF(__FUNCTION__);
#else
#define CX_SCOPEPROF(name)
#define CX_ASCOPEPROF
#define CX_FUNCPROF
#endif


namespace CX
{

namespace Debug
{

class ScopeProfiler
{
public:

	ScopeProfiler(const Char *szFileName, const Char *szScopeName, int cLineNo);

	~ScopeProfiler();

};

}//namespace Debug

}//namespace CX

