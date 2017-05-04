/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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


#include "CX/Log/ConsoleOutput.hpp"
#include "CX/C/stdio.h"
#include "CX/Status.hpp"


namespace CX
{

namespace Log
{

ConsoleOutput::ConsoleOutput()
{
}

ConsoleOutput::~ConsoleOutput()
{
}

Status ConsoleOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	CX_UNUSED(nLevel);
	CX_UNUSED(szTag);
	CX_UNUSED(cLen);

	switch (nLevel)
	{
		case Level_Error:       Console::SetColor(ERROR_FG, ERROR_BG); break;
		case Level_Warning:     Console::SetColor(WARNING_FG, WARNING_BG); break;
		case Level_Info:        Console::SetColor(INFORMATION_FG, INFORMATION_BG); break;
		case Level_Verbose:     Console::SetColor(VERBOSE_FG, VERBOSE_BG); break;
		case Level_Debug:       Console::SetColor(DEBUG_FG, DEBUG_BG); break;
	}
	cx_printf("%s", pBuffer);
	Console::Reset();

	return Status();
}

}//namespace Log

}//namespace CX


#endif
