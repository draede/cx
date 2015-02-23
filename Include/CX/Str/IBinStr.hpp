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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Str
{

class CX_API IBinStr
{
public:

	virtual ~IBinStr() { }

	virtual Status ToString(const Byte *pBinInput, Size cbBinInputSize, Char *pStrOutput, 
	                        Size cStrOutputLen) = 0;

	virtual Status FromString(const Char *pStrInput, Size cStrInputLen, Byte *pBinOutput, 
	                          Size cbBinOutputSize) = 0;

	virtual Size GetStrLenFromBinSize(const Byte *pBinInput, Size cbBinInputSize) = 0;

	virtual Size GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen) = 0;

	virtual Status ToStringEx(const Byte *pBinInput, Size cbBinInputSize, String *psStrOutput)
	{
		Size   cStrOutputLen;

		cStrOutputLen = GetStrLenFromBinSize(pBinInput, cbBinInputSize);
		psStrOutput->reserve(cStrOutputLen + 1);
		(&(*psStrOutput)[0])[cStrOutputLen] = 0;

		return ToString(pBinInput, cbBinInputSize, &(*psStrOutput)[0], cStrOutputLen);
	}

};

}//namespace Str

}//namespace CX

