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
#include "CX/Str/Base64BinStr.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Str
{

const Char Base64BinStr::ENCODING_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const Byte Base64BinStr::DECODING_TABLE[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0, 0, 0,63,52,53,54,55,56,57,58,59,60,61, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, 0, 0, 0, 0, 0,
	0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

Base64BinStr::Base64BinStr()
{
}

Base64BinStr::~Base64BinStr()
{
}

Status Base64BinStr::ToString(const void *pBinInput, Size cbBinInputSize, Char *pStrOutput, 
                           Size cStrOutputLen)
{
	if (GetStrLenFromBinSize(pBinInput, cbBinInputSize) > cStrOutputLen)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetStrLenFromBinSize");
	}

	const Byte *pIn  = (const Byte *)pBinInput;
	Char       *pOut = pStrOutput;

	while (0 < cbBinInputSize)
	{
		ToBase64(ENCODING_TABLE, pIn[0], 1 < cbBinInputSize ? pIn[1] : 0, 2 < cbBinInputSize ? pIn[2] : 0, pOut);
		if (1 == cbBinInputSize)
		{
			pOut[2] = '=';
			pOut[3] = '=';
		}
		else
		if (2 == cbBinInputSize)
		{
			pOut[3] = '=';
		}
		if (3 > cbBinInputSize)
		{
			break;
		}
		pOut += 4;
		pIn += 3;
		cbBinInputSize -= 3;
	}

	return Status();
}

Status Base64BinStr::ToString(const void *pBinInput, Size cbBinInputSize, String *psString)
{
	Size cLen = GetStrLenFromBinSize(pBinInput, cbBinInputSize);

	psString->resize(cLen);

	return ToString(pBinInput, cbBinInputSize, &(*psString)[0], cLen);
}

Status Base64BinStr::FromString(const Char *pStrInput, Size cStrInputLen, void *pBinOutput, 
                             Size cbBinOutputSize)
{
	if (GetBinSizeFromStrLen(pStrInput, cStrInputLen) > cbBinOutputSize)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetBinSizeFromStrLen");
	}

	const Char *pIn  = pStrInput;
	Byte       *pOut = (Byte *)pBinOutput;

	while(0 < cStrInputLen)
	{
		FromBase64(DECODING_TABLE, pIn[0], pIn[1], pIn[2], pIn[3], pOut);
		if (4 >= cStrInputLen)
		{
			if ('=' == pIn[2])
			{
				pOut += 1;
			}
			else
			if ('=' == pIn[3])
			{
				pOut += 2;
			}
			else
			{
				pOut += 3;
			}
		}
		else
		{
			pOut += 3;
		}
		if (4 > cStrInputLen)
		{
			return Status(Status_InvalidArg);
		}
		pIn += 4;
		cStrInputLen -= 4;
	}

	return Status();
}

Size Base64BinStr::GetStrLenFromBinSize(const void *pBinInput, Size cbBinInputSize)
{
	CX_UNUSED(pBinInput);

	Size cLen;

	cLen = 4 * (cbBinInputSize / 3);
	if (0 < (cbBinInputSize % 3))
	{
		cLen += 4;
	}

	return cLen;
}

Size Base64BinStr::GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen)
{
	CX_UNUSED(pStrInput);

	Size cLen;

	if (0 != (cStrInputLen % 4) || 0 == cStrInputLen)
	{
		return 0;
	}
	cLen = (cStrInputLen / 4) * 3;
	if ('=' == ((Byte *)pStrInput)[cStrInputLen - 1] && '=' == ((Byte *)pStrInput)[cStrInputLen - 2])
	{
		cLen -= 2;
	}
	else
	if ('=' == ((Byte *)pStrInput)[cStrInputLen - 1])
	{
		cLen -= 1;
	}

	return cLen;
}

}//namespace Str

}//namespace CX

