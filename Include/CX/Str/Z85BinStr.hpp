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

#pragma once


#include "CX/Str/IBinStr.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Str
{

class CX_API Z85BinStr : public IBinStr
{
public:

	Z85BinStr();

	~Z85BinStr();

	virtual Status ToString(const void *pBinInput, Size cbBinInputSize, Char *pStrOutput, 
	                        Size cStrOutputLen);

	virtual Status ToString(const void *pBinInput, Size cbBinInputSize, String *psString);

	virtual Status FromString(const Char *pStrInput, Size cStrInputLen, void *pBinOutput, 
	                          Size cbBinOutputSize);

	virtual Size GetStrLenFromBinSize(const void *pBinInput, Size cbBinInputSize);

	virtual Size GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen);

};

}//namespace Str

}//namespace CX

