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

#include "CX/Platform.hpp"


#if defined(CX_OS_IOS)


#include "CX/Util/StackTrace.hpp"
#include "CX/Status.hpp"
#include "CX/C/Alloc.h"
#include "CX/C/stdlib.h"


namespace CX
{

namespace Util
{

StackTrace::StackTrace()
{
}

StackTrace::~StackTrace()
{
}

Status StackTrace::GetStackTrace(CallsVector &vectorCalls, Size cMaxEntries/* = MAX_STACK_ENTRIES*/, 
                                 unsigned int nFlags/* = Flag_IgnoreRuntimeInitCalls*/)
{
	vectorCalls;
	cMaxEntries;
	nFlags;

	return Status(Status_NotSupported);
}

}//namespace Util

}//namespace CX


#endif
