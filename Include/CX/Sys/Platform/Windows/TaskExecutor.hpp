/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/Sys/TaskQueue.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Sys
{

class CX_API TaskExecutor
{
public:

	static const Size MIN_THREADS = 1;
	static const Size MAX_THREADS = 60;

	enum Priority
	{
		Priority_Background,
		Priority_Normal,
		Priority_Idle,
		Priority_TimeCritical,
		Priority_AboveNormal,
		Priority_BellowNormal,
		Priority_Highest,
		Priority_Lowest,
	};

	class ICustomExecutor
	{
	public:

		virtual ~ICustomExecutor() = 0;

		virtual Size GetTasksCount() = 0;

		virtual void ExecuteTasks(TaskQueue::TasksVector &vectorTasks) = 0;

	};

	TaskExecutor();

	~TaskExecutor();

	Status Start(Size cThreads, Priority nPriority, Size cFirstRunDelayInMS = 0, Size cRunDelayInMS = 0, 
	             ICustomExecutor *pCustomExecutor = NULL);

	Status Add(TaskQueue::ITask *pTask);

	Status Stop(bool bWaitForTasks);

	bool IsRunning();

	Size GetThreadCount() const;

	Size GetPriority() const;

	Size GetFirstRunDelay() const;

	Size GetRunDelay() const;

private:

	TaskQueue        *m_pQueue;
	Size             m_cThreads;
	Priority         m_nPriority;
	void             **m_threads;
	Size             m_cRunDelayInMS;
	Size             m_cFirstRunDelayInMS;
	void             *m_pStopSemaphore;
	ICustomExecutor  *m_pCustomExecutor;

	static unsigned long __stdcall ThreadProc(void *pArg);

};

}//namespace Sys

}//namespace CX


#endif

