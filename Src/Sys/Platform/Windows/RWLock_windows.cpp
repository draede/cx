/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/RWLock.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

RWLock::RWLock()
	: m_cWritersWaiting(0), m_cReadersWaiting(0), m_cActiveWriterReaders(0)
{
	InitializeCriticalSection(&m_cs);

	if (NULL == (m_hReadyToRead = CreateEvent(NULL,TRUE, FALSE, NULL)))
	{
		return;
	}

	if (NULL == (m_hReadyToWrite = CreateSemaphore(NULL, 0, 1, NULL)))
	{
		CloseHandle(m_hReadyToRead);

		return;
	}
}

RWLock::~RWLock()
{
	if (NULL != m_hReadyToWrite)
	{
		CloseHandle(m_hReadyToWrite);
	}

	if (NULL != m_hReadyToRead)
	{
		CloseHandle(m_hReadyToRead);
	}

	DeleteCriticalSection(&m_cs);
}

void RWLock::EnterRead()
{
	Bool fNotifyReaders = False;

	EnterCriticalSection(&m_cs);

	// Block readers from acquiring the lock if 
	// there are any writers waiting or if a writer
	// has already acquired the lock.
	if ((m_cWritersWaiting > 0) || (HIWORD(m_cActiveWriterReaders) > 0))
	{
		++m_cReadersWaiting;

		_ASSERTE(m_cReadersWaiting > 0);

		for (;;)
		{
			ResetEvent(m_hReadyToRead);

			LeaveCriticalSection(&m_cs);

			WaitForSingleObject(m_hReadyToRead, INFINITE);

			EnterCriticalSection(&m_cs);

			// The reader is only allowed to read if there aren't
			// any writers waiting and if a writer doesn't own the
			// lock.
			if ((m_cWritersWaiting == 0) && (HIWORD(m_cActiveWriterReaders) == 0))
			{
				break;
			}
		}

		// Reader is done waiting.
		--m_cReadersWaiting;

		_ASSERTE(m_cReadersWaiting >= 0);

		// Reader can read.
		++m_cActiveWriterReaders;
	}
	else
	{
		// Reader can read.
		if ((++m_cActiveWriterReaders == 1) && (m_cReadersWaiting != 0))
		{
			// Set flag to notify other waiting readers
			// outside of the critical section
			// so that they don't when the threads
			// are dispatched by the scheduler they
			// don't immediately block on the critical
			// section that this thread is holding.
			fNotifyReaders = True;
		}
	}

	_ASSERTE(HIWORD(m_cActiveWriterReaders) == 0);

	LeaveCriticalSection(&m_cs);

	if (fNotifyReaders)
	{
		SetEvent(m_hReadyToRead);
	}
}

void RWLock::LeaveRead()
{
	EnterCriticalSection(&m_cs);

	// Assert that the lock isn't held by a writer.
	_ASSERTE(HIWORD(m_cActiveWriterReaders) == 0);

	// Assert that the lock is held by readers.
	_ASSERTE(LOWORD(m_cActiveWriterReaders > 0));

	// Decrement the number of active readers.
	if (--m_cActiveWriterReaders == 0)
	{
		ResetEvent(m_hReadyToRead);
	}

	// if writers are waiting and this is the last reader
	// hand owneership over to a writer.
	if ((m_cWritersWaiting != 0) && (m_cActiveWriterReaders == 0))
	{
		// Decrement the number of waiting writers
		--m_cWritersWaiting;

		// Pass ownership to a writer thread.
		m_cActiveWriterReaders = MAKELONG(0, 1);
		ReleaseSemaphore(m_hReadyToWrite, 1, NULL);
	}

	LeaveCriticalSection(&m_cs);
}

void RWLock::EnterWrite()
{
	EnterCriticalSection(&m_cs);

	// Are there active readers?
	if (m_cActiveWriterReaders != 0)
	{
		++m_cWritersWaiting;

		_ASSERTE(m_cWritersWaiting > 0);

		LeaveCriticalSection(&m_cs);

		WaitForSingleObject(m_hReadyToWrite, INFINITE);

		// Upon wakeup theirs no need for the writer
		// to acquire the critical section.  It 
		// already has been transfered ownership of the
		// lock by the signaler.
	}
	else
	{
		_ASSERTE(m_cActiveWriterReaders == 0);

		// Set that the writer owns the lock.
		m_cActiveWriterReaders = MAKELONG(0, 1);;

		LeaveCriticalSection(&m_cs);
	}
}

void RWLock::LeaveWrite()
{
	Bool fNotifyWriter = False;
	Bool fNotifyReaders = False;

	EnterCriticalSection(&m_cs);

	// Assert that the lock is owned by a writer.
	_ASSERTE(HIWORD(m_cActiveWriterReaders) == 1);

	// Assert that the lock isn't owned by one or more readers
	_ASSERTE(LOWORD(m_cActiveWriterReaders) == 0);

	if (m_cWritersWaiting != 0)
	{
		// Writers waiting, decrement the number of
		// waiting writers and release the semaphore
		// which means ownership is passed to the thread
		// that has been released.
		--m_cWritersWaiting;
		fNotifyWriter = True;
	}
	else
	{
		// There aren't any writers waiting
		// Release the exclusive hold on the lock.
		m_cActiveWriterReaders = 0;

		// if readers are waiting set the flag
		// that will cause the readers to be notified
		// once the critical section is released.  This
		// is done so that an awakened reader won't immediately
		// block on the critical section which is still being
		// held by this thread.
		if (m_cReadersWaiting != 0)
		{
			fNotifyReaders = True;
		}
	}

	LeaveCriticalSection(&m_cs);

	if (fNotifyWriter)
	{
		ReleaseSemaphore(m_hReadyToWrite, 1, NULL);
	}
	else if (fNotifyReaders)
	{
		SetEvent(m_hReadyToRead);
	}
}

}//namespace Sys

}//namespace CX


#endif

