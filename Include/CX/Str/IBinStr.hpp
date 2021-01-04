/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/Vector.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Str
{

class CX_API IBinStr
{
public:

	virtual ~IBinStr() { }

	virtual Status ToString(const void *pBinInput, Size cbBinInputSize, Char *pStrOutput, 
	                        Size cStrOutputLen) = 0;

	virtual Status ToString(const void *pBinInput, Size cbBinInputSize, String *psString) = 0;

	virtual Status FromString(const Char *pStrInput, Size cStrInputLen, void *pBinOutput, 
	                          Size cbBinOutputSize) = 0;

	virtual Size GetStrLenFromBinSize(const void *pBinInput, Size cbBinInputSize) = 0;

	virtual Size GetBinSizeFromStrLen(const Char *pStrInput, Size cStrInputLen) = 0;

	virtual Status ToStringEx(const void *pBinInput, Size cbBinInputSize, String *psStrOutput)
	{
		Char      tmp[8192];
		Char      *pTmp;
		Size      cStrOutputLen;
		Status    status;

		cStrOutputLen = GetStrLenFromBinSize(pBinInput, cbBinInputSize);
		if (cStrOutputLen < sizeof(tmp) / sizeof(tmp[0]))
		{
			pTmp = tmp;
		}
		else
		{
			if (NULL == (pTmp = (Char *)Mem::Alloc((cStrOutputLen + 1) * sizeof(Char))))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate temp buffer");
			}
		}
		if ((status = ToString(pBinInput, cbBinInputSize, pTmp, cStrOutputLen + 1)).IsNOK())
		{
			if (pTmp != tmp)
			{
				Mem::Free(pTmp);
			}

			return status;
		}
		psStrOutput->append(pTmp, cStrOutputLen);
		if (pTmp != tmp)
		{
			Mem::Free(pTmp);
		}

		return Status();
	}

};

}//namespace Str

}//namespace CX

