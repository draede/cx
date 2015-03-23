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

#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)

#include "CX/Detail/DetailPrint/DetailPrint.hpp"
#include "../../../../../Contrib/DoubleConversion/Include/double-conversion.h"
#include "CX/IO/IOutputStream.hpp"
#include "CX/C/stdio.h"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Detail
{

namespace DetailPrint
{

Bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision)
{
	double_conversion::DoubleToStringConverter   cvt(
	                   double_conversion::DoubleToStringConverter::Flags::NO_FLAGS, 
	                   "#", "*", 'E', 0, 0, 0, 0);
	double_conversion::StringBuilder             sb(szOutput, (int)cLen);

	return cvt.ToFixed(lfValue, (int)cPrecision, &sb);
}

CX_API Bool UTF8toUTF16(const Char *szUTF8, WString *pwsUTF16, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
{
	int   cSize;
	WChar *pOut = NULL;
	WChar out[8000];

	if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, NULL, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = NewArr<WChar>(cSize)))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 >= (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, pOut, cSize)))
		{
			if (pOut != out)
			{
				DeleteArr(pOut);
			}
			
			return false;
		}
		pwsUTF16->append(pOut, cSize);
		if (pOut != pOut)
		{
			DeleteArr(pOut);
		}
	}
	else
	{
		return false;
	}

	return true;
}

CX_API Bool UTF16toUTF8(const WChar *wszUTF16, String *psUTF8, Size cUTF16Len/* = TYPE_SIZE_MAX*/)
{
	int cSize;

	Char *pOut = NULL;
	Char out[8000];

	if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, NULL, 0, 
		                                    NULL, NULL)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = NewArr<Char>(cSize)))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 >= (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, pOut, 
			                                     cSize, NULL, NULL)))
		{
			if (pOut != out)
			{
				DeleteArr(pOut);
			}

			return false;
		}
		psUTF8->append(pOut, cSize);
		if (pOut != out)
		{
			DeleteArr(pOut);
		}
	}
	else
	{
		return false;
	}

	return true;
}

StatusCode WriteStream(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen)
{
	Size cbAckSize;

	return pOutputStream->Write(pBuffer, cLen * sizeof(Char), &cbAckSize).GetCode();
}

}//namespace DetailPrint

}//namespace Detail

}//namespace CX


#endif