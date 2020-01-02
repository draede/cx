/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_ANDROID)


#include "CX/Sys/RWLock.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

RWLock::RWLock()
{
	m_bOK = (0 == pthread_rwlock_init(&m_rwlock, NULL));
}

RWLock::~RWLock()
{
	if (m_bOK)
	{
		pthread_rwlock_destroy(&m_rwlock);
	}
}

void RWLock::EnterRead()
{
	if (!m_bOK)
	{
		return;
	}

	pthread_rwlock_rdlock(&m_rwlock);
}

void RWLock::LeaveRead()
{
	if (!m_bOK)
	{
		return;
	}

	pthread_rwlock_unlock(&m_rwlock);
}

void RWLock::EnterWrite()
{
	if (!m_bOK)
	{
		return;
	}

	pthread_rwlock_wrlock(&m_rwlock);
}

void RWLock::LeaveWrite()
{
	if (!m_bOK)
	{
		return;
	}

	pthread_rwlock_unlock(&m_rwlock);
}

}//namespace Sys

}//namespace CX


#endif

