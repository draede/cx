/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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

namespace Util
{

class Console
{
public:

	enum Color
	{
		BLACK          = 0x00,
		BLUE           = 0x01,
		GREEN          = 0x02,
		CYAN           = 0x03,
		RED            = 0x04,
		MAGENTA        = 0x05,
		YELLOW         = 0x06,
		WHITE          = 0x07,
		GRAY           = 0x08,
		BRIGHT_BLUE    = 0x09,
		BRIGHT_GREEN   = 0x0A,
		BRIGHT_CYAN    = 0x0B,
		BRIGHT_RED     = 0x0C,
		BRIGHT_MAGENTA = 0x0D,
		BRIGHT_YELLOW  = 0x0E,
		BRIGHT_WHITE   = 0x0F,
	};

	static Status SetColors(Color fgColor, Color bgColor);

	static Status SetForegroundColor(Color fgColor);

	static Status SetBackgroundColor(Color bgColor);

	static Status GetColors(Color *pfgColor, Color *pbgColor);

	static Status SetCursorPos(UInt16 cX, UInt16 cY);

	static Status GetCursorPos(UInt16 *pcX, UInt16 *pcY);

	static Status GetWindowSize(UInt16 *pcWidth, UInt16 *pcHeight);

	static Status Clear(Char chChar = ' ', UInt16 cLeft = 0, UInt16 cTop = 0, 
	                    UInt16 cRight = 65535, UInt16 cBottom = 65535);

	static Status Reset();

private:

	Console();

	~Console();

};

}//namespace Util

}//namespace CX

#endif
