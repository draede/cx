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


#include "CX/IO/FileOutputStream.hpp"
#include "CX/Log/IOutput.hpp"
#include "CX/Sys/Lock.hpp"
#include "CX/Sys/Thread.hpp"
#include "CX/Sys/Event.hpp"
#include "CX/Vector.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Log
{

class CX_API OptimizedStreamOutput : public IOutput
{
public:

	OptimizedStreamOutput(const Char *szPath, bool bAppend = false, UInt32 cFlushDelay = 3000, Size cbMaxMem = 1048576);

	OptimizedStreamOutput(const WChar *wszPath, bool bAppend = false, UInt32 cFlushDelay = 3000, 
	                      Size cbMaxMem = 1048576);

	~OptimizedStreamOutput();

	virtual Status Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

	virtual bool NeedsReopenWithNewPath(String *psPath)
	{
		CX_UNUSED(psPath);

		return false;
	}

	virtual bool NeedsReopenWithNewPath(WString *pwsPath)
	{
		CX_UNUSED(pwsPath);

		return false;
	}

private:

	typedef Vector<String>::Type StringsVector;

	IO::FileOutputStream   *m_pFOS;
	Sys::Thread            m_threadWrite;
	Sys::Event             m_eventStop;
	StringsVector          *m_pVectorStrings;
	Sys::Lock              m_lockStrings;
	UInt32                 m_cFlushDelay;
	Size                   m_cbMaxMem;
	Size                   m_cbCrMem;
	bool                   m_bWide;

	void WriteThread();

	Status Init(UInt32 cFlushDelay, Size cMaxMem);

	Status Uninit();

	Status Reopen(const Char *szPath);

	Status Reopen(const WChar *wszPath);

};

}//namespace Log

}//namespace CX
