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

#pragma once


#include "CX/Log/Logger.hpp"
#include "CX/Util/Timer.hpp"


#define CX_LOG_CONCAT_IMPL(a, b)      a ## b
#define CX_LOG_CONCAT(a, b)           CX_LOG_CONCAT_IMPL(a, b)


#if CX_LOG_LEVEL >= CX_LOG_DEBUG
	#define CXLOGSCOPE(tag, name)      CX::Log::LogScope   CX_LOG_CONCAT(__logscope__, __COUNTER__)(tag, name);
	#define CXLOGFUNC(tag)             CX::Log::LogScope   CX_LOG_CONCAT(__logscope__, __COUNTER__)(tag, __FUNCTION__);
#else
	#define CXLOGSCOPE(tag, name);
	#define CXLOGFUNC(tag);
#endif


namespace CX
{

namespace Log
{

class LogScope
{
public:

	LogScope(const Char *szTag, const Char *szName);

	~LogScope();

private:

	String      m_sTag;
	String      m_sName;
	Util::Timer m_timer;

};

}//namespace Log

}//namespace CX

