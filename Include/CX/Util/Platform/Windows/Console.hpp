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

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

class Console
{
public:

	enum Color
	{
		BLACK          = 0x00,
		BLUE           = 0x01,
		GREN           = 0x02,
		CYAN           = 0x03,
		RED            = 0x04,
		MAGENTA        = 0x05,
		YELLOW         = 0x06,
		WHITE          = 0x07,
		GRAY           = 0x08,
		BRIGHT_BLUE    = 0x09,
		BRIGHT_GREN    = 0x0A,
		BRIGHT_CYAN    = 0x0B,
		BRIGHT_RED     = 0x0C,
		BRIGHT_MAGENTA = 0x0D,
		BRIGHT_YELLOW  = 0x0E,
		BRIGHT_WHITE   = 0x0F,
	};

	static void SetColor(Color fgColor, Color bgColor);

	static void Reset();

private:

	struct Data
	{
		Bool  m_bFirst;
		WORD  m_wAttr;
	};

	Console();

	~Console();

	static Data &GetData();

};

}//namespace CX

#endif
