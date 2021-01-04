/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#if defined(CX_OS_IOS)


#include "CX/Sys/Event.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

Event::Event()
{
}

Event::~Event()
{
}

Status Event::Create(bool bManualReset/* = false*/, bool bInitialState/* = false*/)
{
	CX_UNUSED(bManualReset);
	CX_UNUSED(bInitialState);
	
	return Status_NotImplemented;
}

Status Event::Destroy()
{
	return Status_NotImplemented;
}

bool Event::IsOK()
{
	return false;
}

Status Event::Set()
{
	return Status_NotImplemented;
}

Status Event::Reset()
{
	return Status_NotImplemented;
}

Event::WaitResult Event::Wait(UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	CX_UNUSED(cTimeout);
	
	return Wait_Error;
}

Event::WaitResult Event::WaitForMultipleEvents(Event **events, Size cCount, bool bWaitAll, Size *pcIndex, 
                                               UInt32 cTimeout/* = 0xFFFFFFFF*/)
{
	CX_UNUSED(events);
	CX_UNUSED(cCount);
	CX_UNUSED(bWaitAll);
	CX_UNUSED(pcIndex);
	CX_UNUSED(cTimeout);
	
	return Wait_Error;
}

}//namespace Sys

}//namespace CX


#endif

