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


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Vector.hpp"
#include "CX/APIDefs.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Sys
{

class CX_API SyncedWorkerThreads
{
public:

	static const Size MIN_THREADS         = 1;
	static const Size MAX_THREADS         = 2048;

	typedef Bool (* JobFunction)(void *pJob, Size cbSize);

	SyncedWorkerThreads();

	~SyncedWorkerThreads();

	//call this from a single thread
	Status Start(Size cThreads);

	//call this from a single thread
	//the jobs will be dispatched to the threads and when all the threads finish this function will return
	Status RunJobs(void *jobs, Size cJobs, Size cbJobSize, JobFunction pfnJobFunction);

	//call this from a single thread
	Status Stop();

private:

	static const CX::Size   MAX_THREADS_PER_GROUP = 64;

	typedef CX::Vector<HANDLE>::Type       HandlesVector;

	struct ThreadArg
	{
		SyncedWorkerThreads   *pThis;
		HANDLE                hWaitEvent;
		HANDLE                hReadyEvent;
		void                  *jobs;
		Size                  cJobs;
		Size                  cbJobSize;
		JobFunction           pfnJob;
	};

	typedef CX::Vector<ThreadArg *>::Type   ThreadArgsVector;

	struct Group
	{
		ThreadArgsVector   vectorThreadArgs;
		HandlesVector      vectorThreads;
		HandlesVector      vectorWaitEvents;
		HandlesVector      vectorReadyEvents;
	};

	typedef CX::Vector<Group>::Type        GroupsVector;

	Size           m_cThreads;
	Bool           m_bRunning;
	GroupsVector   m_vectorGroups;

	static DWORD WINAPI Worker(void *pThreadArgs);

};

}//namespace Sys

}//namespace CX


#endif
