/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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


#include "CX/Platform.hpp"


#if defined(CX_OS_IOS)


#include "CX/Types.hpp"
#include "CX/Scope.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Sys
{

class CX_API RWLock
{
public:

	RWLock();

	virtual ~RWLock();

	void EnterRead();

	void LeaveRead();

	void EnterWrite();

	void LeaveWrite();

private:

	bool               m_bRWLockIsOK;

};

typedef Scope<RWLock, &RWLock::EnterRead, &RWLock::LeaveRead>     RLocker;
typedef Scope<RWLock, &RWLock::EnterWrite, &RWLock::LeaveWrite>   WLocker;

}//namespace Sys

}//namespace CX


#endif

