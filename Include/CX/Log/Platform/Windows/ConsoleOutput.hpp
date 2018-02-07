/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/Log/IOutput.hpp"
#include "CX/Util/Console.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Log
{

class CX_API ConsoleOutput : public IOutput
{
public:

	static const Console::Color ERROR_FG       = Console::BRIGHT_RED;
	static const Console::Color ERROR_BG       = Console::BLACK;
	static const Console::Color WARNING_FG     = Console::BRIGHT_YELLOW;
	static const Console::Color WARNING_BG     = Console::BLACK;
	static const Console::Color INFORMATION_FG = Console::WHITE;
	static const Console::Color INFORMATION_BG = Console::BLACK;
	static const Console::Color VERBOSE_FG     = Console::CYAN;
	static const Console::Color VERBOSE_BG     = Console::BLACK;
	static const Console::Color DEBUG_FG       = Console::GRAY;
	static const Console::Color DEBUG_BG       = Console::BLACK;
	
	ConsoleOutput();

	~ConsoleOutput();

	virtual Status Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

private:

};

}//namespace Log

}//namespace CX


#endif
