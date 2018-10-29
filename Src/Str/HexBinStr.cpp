/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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
#include "CX/Str/HexBinStr.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Str
{

HexBinStr::HexBinStr(unsigned int nFlags/* = Uppercase*/)
{
	SetFlags(nFlags);
}

HexBinStr::~HexBinStr()
{
}

void HexBinStr::SetFlags(unsigned int nFlags/* = Uppercase*/)
{
	m_nFlags = nFlags;
}

unsigned int HexBinStr::GetFlags()
{
	return m_nFlags;
}

Status HexBinStr::ToString(const void *pBinInput, Size cbBinInputSize, Char *pStrOutput,
                           Size cStrOutputLen)
{
	static Char hdu[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	static Char hdl[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	Char  *hd;

	if (GetStrLenFromBinSize(pBinInput, cbBinInputSize) > cStrOutputLen)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetStrLenFromBinSize");
	}
	if ((Uppercase == m_nFlags))
	{
		hd = hdu;
	}
	else
	{
		hd = hdl;
	}
	for (Size i = 0; i < cbBinInputSize; i++)
	{
		pStrOutput[i * 2]     = hd[((const Byte *)pBinInput)[i] / 16];
		pStrOutput[i * 2 + 1] = hd[((const Byte *)pBinInput)[i] % 16];
	}

	return Status();
}

Status HexBinStr::ToString(const void *pBinInput, Size cbBinInputSize, String *psString)
{
	Size cLen = GetStrLenFromBinSize(pBinInput, cbBinInputSize);

	psString->resize(cLen);

	return ToString(pBinInput, cbBinInputSize, &(*psString)[0], cLen);
}

Status HexBinStr::FromString(const Char *pStrInput, Size cStrInputLen, void *pBinOutput, 
                             Size cbBinOutputSize)
{
	Char ch;

	if (0 != (cStrInputLen % 2))
	{
		return Status(Status_InvalidArg, "cStrInputLen must be a multiple of two");
	}
	if (GetBinSizeFromStrLen(pStrInput, cStrInputLen) > cbBinOutputSize)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetBinSizeFromStrLen");
	}
	for (Size i = 0; i < cStrInputLen; i += 2)
	{
		ch = pStrInput[i];
		if ('A' <= ch && 'F' >= ch)
		{
			((Byte *)pBinOutput)[i / 2] = ch - 'A' + 10;
		}
		else
		if ('a' <= ch && 'f' >= ch)
		{
			((Byte *)pBinOutput)[i / 2] = ch - 'a' + 10;
		}
		else
		{
			((Byte *)pBinOutput)[i / 2] = ch - '0';
		}
		((Byte *)pBinOutput)[i / 2] *= 16;
		ch = pStrInput[i + 1];
		if ('A' <= ch && 'F' >= ch)
		{
			((Byte *)pBinOutput)[i / 2] += ch - 'A' + 10;
		}
		else
		if ('a' <= ch && 'f' >= ch)
		{
			((Byte *)pBinOutput)[i / 2] += ch - 'a' + 10;
		}
		else
		{
			((Byte *)pBinOutput)[i / 2] += ch - '0';
		}
	}

	return Status();
}

Size HexBinStr::GetStrLenFromBinSize(const void *pBinInput, Size cbBinInputSize)
{
	CX_UNUSED(pBinInput);

	return cbBinInputSize * 2;
}

Size HexBinStr::GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen)
{
	CX_UNUSED(pStrInput);

	return cStrInputLen / 2;
}

}//namespace Str

}//namespace CX

