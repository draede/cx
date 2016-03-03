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

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Queue.hpp"
#include "CX/Vector.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace Sys
{

class CX_API TaskQueue : public IObject
{
public:

	class ITask
	{
	public:

		virtual ~ITask() { };

		virtual void Run() = 0;

		virtual void Release() = 0;

	};

	typedef Vector<ITask *>::Type   TasksVector;

	TaskQueue(Size cConsumers = 1);

	~TaskQueue();

	Status Push(ITask *pTask);

	Status Pop(ITask **ppTask);

	Status Pop(TasksVector *pVectorTasks, Size cCount = 0);

	Status Wait();

	Status Shutdown();

	Status Clear();

private:

	typedef CX::Queue<ITask *>::Type   TasksQueue;

#pragma warning(push)
#pragma warning(disable: 4251)
	TasksQueue       m_queueTasks;
#pragma warning(pop)
	Size             m_cConsumers;
	void             *m_pSync;

};

}//namespace Sys

}//namespace CX


#endif

