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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Util/Console.hpp"


namespace CX
{

namespace Util
{

Console::Console()
{
}

Console::~Console()
{
}

Console::Data &Console::GetData()
{
	static Data data = { True, 0 };

	return data;
}

void Console::SetColors(Color fgColor, Color bgColor)
{
	SetColors((WORD)(fgColor | (bgColor << 4)));
}

void Console::SetForegroundColor(Color fgColor)
{
	SetColors((WORD)fgColor);
}

void Console::SetBackgroundColor(Color bgColor)
{
	SetColors((WORD)(bgColor << 4));
}

void Console::Reset()
{
	if (GetData().m_bFirst)
	{
		return;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetData().m_wAttr);
}

void Console::SetColors(WORD wColors)
{
	if (GetData().m_bFirst)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		GetData().m_wAttr  = csbi.wAttributes;
		GetData().m_bFirst = False;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColors);
}

}//namespace Util

}//namespace CX


#endif
