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

//http://msdn.microsoft.com/en-us/magazine/cc163405.aspx
//http://www.codeproject.com/Articles/32685/Testing-reader-writer-locks
//RWLockFavorWriters

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Scope.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Platform/Windows/windows.h"


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

	LONG               m_cWritersWaiting;
	LONG               m_cReadersWaiting;

	DWORD              m_cActiveWriterReaders;

	HANDLE             m_hReadyToRead;
	HANDLE             m_hReadyToWrite;

	CRITICAL_SECTION   m_cs;

};

typedef Scope<RWLock, &RWLock::EnterRead, &RWLock::LeaveRead>     RLocker;
typedef Scope<RWLock, &RWLock::EnterWrite, &RWLock::LeaveWrite>   WLocker;

}//namespace Sys

}//namespace CX


#endif

