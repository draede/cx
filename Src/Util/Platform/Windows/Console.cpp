/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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

Bool             Console::m_bHasDefaultColors = False;
Console::Color   Console::m_fgDefaultColor    = Console::WHITE;
Console::Color   Console::m_bgDefaultColor    = Console::BLACK;

Console::Console()
{
}

Console::~Console()
{
}

Status Console::SetColors(Color fgColor, Color bgColor)
{
	if (!m_bHasDefaultColors)
	{
		GetColors(&m_fgDefaultColor, &m_bgDefaultColor);
		m_bHasDefaultColors = True;
	}
	if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((fgColor & 0xF) | ((bgColor & 0xF) << 4))))
	{
		return Status_OperationFailed;
	}

	return Status();
}

Status Console::SetForegroundColor(Color fgColor)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;

	if (!m_bHasDefaultColors)
	{
		GetColors(&m_fgDefaultColor, &m_bgDefaultColor);
		m_bHasDefaultColors = True;
	}
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}

	return SetColors(fgColor, (Color)((csbinfo.wAttributes & 0xFF) >> 4));
}

Status Console::SetBackgroundColor(Color bgColor)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;

	if (!m_bHasDefaultColors)
	{
		GetColors(&m_fgDefaultColor, &m_bgDefaultColor);
		m_bHasDefaultColors = True;
	}
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}

	return SetColors((Color)(csbinfo.wAttributes & 0xF), bgColor);
}

Status Console::GetColors(Color *pfgColor, Color *pbgColor)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}
	*pfgColor = (Color)(csbinfo.wAttributes & 0xF);
	*pbgColor = (Color)((csbinfo.wAttributes & 0xFF) >> 4);

	return Status();
}

Status Console::SetCursorPos(UInt16 cX, UInt16 cY)
{
	COORD   coord;

	coord.X = cX;
	coord.Y = cY;
	if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
	{
		return Status_OperationFailed;
	}

	return Status();
}

Status Console::GetCursorPos(UInt16 *pcX, UInt16 *pcY)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}
	*pcX = csbinfo.dwCursorPosition.X;
	*pcY = csbinfo.dwCursorPosition.Y;

	return Status();
}

Status Console::GetWindowSize(UInt16 *pcWidth, UInt16 *pcHeight)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}
	*pcWidth  = csbinfo.srWindow.Right + 1;
	*pcHeight = csbinfo.srWindow.Bottom + 1;

	return Status();
}

Status Console::Clear(Char chChar/* = ' '*/, UInt16 cLeft/* = 0*/, UInt16 cTop/* = 0*/, 
                      UInt16 cRight/* = 65535*/, UInt16 cBottom/* = 65535*/)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbinfo;
	COORD                        pos;
	DWORD                        dwLength;
	DWORD                        dwNumberOfCharsWritten;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbinfo))
	{
		return Status_OperationFailed;
	}
	if (cRight > csbinfo.srWindow.Right)
	{
		cRight = csbinfo.srWindow.Right;
	}
	if (cBottom > csbinfo.srWindow.Bottom)
	{
		cBottom = csbinfo.srWindow.Bottom;
	}
	if (cRight < cLeft)
	{
		return Status_InvalidArg;
	}
	if (cBottom < cTop)
	{
		return Status_InvalidArg;
	}

	pos.X    = cLeft;
	dwLength = cRight - cLeft + 1;
	for (pos.Y = cTop; pos.Y <= cBottom; pos.Y++)
	{
		if (!FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbinfo.wAttributes, dwLength, pos, 
		                                &dwNumberOfCharsWritten))
		{
			return Status_OperationFailed;
		}
		if (!FillConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), chChar, dwLength, pos, &dwNumberOfCharsWritten))
		{
			return Status_OperationFailed;
		}
	}

	return Status();
}

Status Console::Reset()
{
	if (!m_bHasDefaultColors)
	{
		return Status();
	}

	return SetColors(m_fgDefaultColor, m_bgDefaultColor);
}

}//namespace Util

}//namespace CX


#endif
