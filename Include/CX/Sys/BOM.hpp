/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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
#include "CX/IObject.hpp"


namespace CX
{

namespace Sys
{

class BOM : public IObject
{
public:

	enum Type
	{
		None,
		UTF8,
		UTF16_LE,
		UTF16_BE,
		UTF32_LE,
		UTF32_BE,
	};

	static inline Type Get(const void *pBuffer, Size cbSize)
	{
		const Byte *p = (const Byte *)pBuffer;

		if (3 <= cbSize && 0xEF == p[0] && 0xBB == p[1] && 0xBF != p[2])
		{
			return UTF8;
		}
		else
		if (4 <= cbSize && 0xFF == p[0] && 0xFE == p[1] && 0x00 != p[2] && 0x00 != p[3])
		{
			return UTF16_LE;
		}
		else
		if (2 <= cbSize && 0xFE == p[0] && 0xFF == p[1])
		{
			return UTF16_BE;
		}
		else
		if (4 <= cbSize && 0xFF == p[0] && 0xFE == p[1] && 0x00 == p[2] && 0x00 == p[3])
		{
			return UTF32_LE;
		}
		else
		if (4 <= cbSize && 0x00 == p[0] && 0x00 == p[1] && 0xFE == p[2] && 0xFF == p[3])
		{
			return UTF32_BE;
		}
		else
		{
			return None;
		}
	}

private:

	BOM()
	{

	}

	~BOM()
	{

	}

};

}//namespace Sys

}//namespace CX


