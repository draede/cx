/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Str/Z85BinStr.hpp"
#include "CX/Status.hpp"
#include "../../Contrib/Z85/Include/z85.h"


namespace CX
{

namespace Str
{

Z85BinStr::Z85BinStr()
{
}

Z85BinStr::~Z85BinStr()
{
}

Status Z85BinStr::ToString(const void *pBinInput, Size cbBinInputSize, Char *pStrOutput, 
                           Size cStrOutputLen)
{
	if (GetStrLenFromBinSize(pBinInput, cbBinInputSize) > cStrOutputLen)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetStrLenFromBinSize");
	}

	Z85_encode_with_padding((const char *)pBinInput, pStrOutput, cbBinInputSize);

	return Status();
}

Status Z85BinStr::ToString(const void *pBinInput, Size cbBinInputSize, String *psString)
{
	Size cLen = GetStrLenFromBinSize(pBinInput, cbBinInputSize);

	psString->resize(cLen);

	return ToString(pBinInput, cbBinInputSize, &(*psString)[0], cLen);
}

Status Z85BinStr::FromString(const Char *pStrInput, Size cStrInputLen, void *pBinOutput, 
                             Size cbBinOutputSize)
{
	if (GetBinSizeFromStrLen(pStrInput, cStrInputLen) > cbBinOutputSize)
	{
		return Status(Status_TooSmall, "Output buffer to small. See GetBinSizeFromStrLen");
	}

	Z85_decode_with_padding(pStrInput, (char *)pBinOutput, cStrInputLen);

	return Status();
}

Size Z85BinStr::GetStrLenFromBinSize(const void *pBinInput, Size cbBinInputSize)
{
	CX_UNUSED(pBinInput);

	return Z85_encode_with_padding_bound(cbBinInputSize);
}

Size Z85BinStr::GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen)
{
	return Z85_decode_with_padding_bound(pStrInput, cStrInputLen);
}

}//namespace Str

}//namespace CX

