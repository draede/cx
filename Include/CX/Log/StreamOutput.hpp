/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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
#include "CX/Log/IOutput.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Sys/Lock.hpp"
#include "CX/Util/Timer.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Log
{

class CX_API StreamOutput : public IOutput
{
public:

	// 0 == cMSFlushDelay ? => no flush
	StreamOutput(IO::IOutputStream *pOutputStream, Size cMSFlushDelay = 3, bool bUseLock = true);

	~StreamOutput();

	virtual Status Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

private:

	Sys::Lock           m_lock;
	bool                m_bUseLock;
	Size                m_cMSFlushDelay;
	Util::Timer         m_timer;
	IO::IOutputStream   *m_pOutputStream;

};

}//namespace Log

}//namespace CX

