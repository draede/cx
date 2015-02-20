/* 
 * CX - C++ framework for general purpose developement
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

#include "CX/Str/HexString.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Str
{

HexString::HexString()
{
}

HexString::~HexString()
{
}

Status HexString::ToString(const Byte *pBinary, Size cbBinarySize, String *psString, 
                               int nFlags/* = Flag::Lowercase*/)
{
	static Char hexdigits_u[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	static Char hexdigits_l[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	Byte hi;
	Byte lo;

	for (Size i = 0; i < cbBinarySize; i++)
	{
		hi = pBinary[i] / 16;
		lo = pBinary[i] % 16;
		if (10 <= hi)
		{
			if (Flag::Uppercase == nFlags)
			{
				*psString += hexdigits_u[hi];
			}
			else
			{
				*psString += hexdigits_l[hi];
			}
		}
		else
		{
			*psString += '0' + hi;
		}
		if (10 <= lo)
		{
			if (Flag::Uppercase == nFlags)
			{
				*psString += hexdigits_u[lo];
			}
			else
			{
				*psString += hexdigits_l[lo];
			}
		}
		else
		{
			*psString += '0' + lo;
		}
	}

	return Status();
}

Status HexString::FromString(const Char *szString, Byte *pBinary, Size cbBinarySize)
{
	const Char   *pszPos;
	Size         cIndex;

	cIndex = 0;
	pszPos = szString;
	while('\0' != *pszPos && cIndex < cbBinarySize)
	{
		pBinary[cIndex] = 0;
		if ('A' <= *pszPos && 'F' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - 'A' + 10) * 16;
		}
		else
		if ('0' <= *pszPos && '9' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - '0') * 16;
		}
		else
		{
			return Status_ParseFailed;
		}
		pszPos++;
		if ('A' <= *pszPos && 'F' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - 'A' + 10);
		}
		else
		if ('0' <= *pszPos && '9' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - '0');
		}
		else
		{
			return Status_ParseFailed;
		}
		pszPos++;
		cIndex++;
	}
	if (cIndex != cbBinarySize || '\0' != *pszPos)
	{
		return Status_ParseFailed;
	}

	return Status();
}

Size HexString::GetStringLen(Size cbBinarySize)
{
	return cbBinarySize * 2;
}

Size HexString::GetBinarySize(Size cStringLen)
{
	return cStringLen / 2;
}

}//namespace Str

}//namespace CX

