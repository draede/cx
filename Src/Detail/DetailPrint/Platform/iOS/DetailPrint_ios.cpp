/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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


#if defined(CX_OS_IOS)

#include "CX/Detail/DetailPrint/DetailPrint.hpp"
#include "../../../../../Contrib/DoubleConversion/Include/double-conversion.h"
#include "CX/IO/IOutputStream.hpp"
#include "CX/C/stdio.h"
#include "utf8.h"


namespace CX
{

namespace Detail
{

namespace DetailPrint
{

Bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision)
{
	double_conversion::DoubleToStringConverter   cvt(
	                   double_conversion::DoubleToStringConverter::NO_FLAGS, 
	                   "#", "*", 'E', 0, 0, 0, 0);
	double_conversion::StringBuilder             sb(szOutput, (int)cLen);

	if (cvt.ToFixed(lfValue, (int)cPrecision, &sb))
	{
		return true;
	}
	if (cvt.ToExponential(lfValue, (int)cPrecision, &sb))
	{
		return true;
	}

	return false;
}

CX_API Bool UTF8toWChar(const Char *szUTF8, WString *pwsWChar, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
{
	size_t   cSize;
	WChar    *pOut = NULL;
	WChar    out[8000];

	if (TYPE_SIZE_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8);
	}
	if (0 == cUTF8Len)
	{
		pwsWChar->clear();

		return true;
	}
	if (0 < (cSize = utf8_to_wchar(szUTF8, cUTF8Len, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) WChar[cSize]))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 == (cSize = utf8_to_wchar(szUTF8, cUTF8Len, pOut, cSize, 0)))
		{
			if (pOut != out)
			{
				delete [] pOut;
			}
			
			return false;
		}
		pwsWChar->assign(pOut, cSize);
		if (NULL != pOut)
		{
			delete [] pOut;
		}
	}
	else
	{
		return false;
	}

	return true;
}

CX_API Bool WChartoUTF8(const WChar *wszWChar, String *psUTF8, Size cWCharLen/* = TYPE_SIZE_MAX*/)
{
	size_t   cSize;
	Char     *pOut = NULL;
	Char     out[8000];

	if (TYPE_SIZE_MAX == cWCharLen)
	{
		cWCharLen = cxw_strlen(wszWChar);
	}
	if (0 == cWCharLen)
	{
		psUTF8->clear();

		return true;
	}
	if (0 < (cSize = wchar_to_utf8(wszWChar, cWCharLen, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) Char[cSize]))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 == (cSize = wchar_to_utf8(wszWChar, cWCharLen, pOut, cSize, 0)))
		{
			if (pOut != out)
			{
				delete [] pOut;
			}

			return false;
		}
		psUTF8->assign(pOut, cSize);
		if (pOut != out)
		{
			delete [] pOut;
		}
	}
	else
	{
		return false;
	}

	return true;
}

CX_API Bool UTF8toWCharx(const Char *szUTF8, std::wstring *pwsWChar, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
{
	size_t   cSize;
	WChar    *pOut = NULL;
	WChar    out[8000];

	if (TYPE_SIZE_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8);
	}
	if (0 == cUTF8Len)
	{
		pwsWChar->clear();

		return true;
	}
	if (0 < (cSize = utf8_to_wchar(szUTF8, cUTF8Len, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) WChar[cSize]))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 == (cSize = utf8_to_wchar(szUTF8, cUTF8Len, pOut, cSize, 0)))
		{
			if (pOut != out)
			{
				delete [] pOut;
			}
			
			return false;
		}
		pwsWChar->assign(pOut, cSize);
		if (NULL != pOut)
		{
			delete [] pOut;
		}
	}
	else
	{
		return false;
	}

	return true;
}

CX_API Bool WChartoUTF8x(const WChar *wszWChar, std::string *psUTF8, Size cWCharLen/* = TYPE_SIZE_MAX*/)
{
	size_t   cSize;
	Char     *pOut = NULL;
	Char     out[8000];

	if (TYPE_SIZE_MAX == cWCharLen)
	{
		cWCharLen = cxw_strlen(wszWChar);
	}
	if (0 == cWCharLen)
	{
		psUTF8->clear();

		return true;
	}
	if (0 < (cSize = wchar_to_utf8(wszWChar, cWCharLen, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) Char[cSize]))
			{
				return false;
			}
		}
		else
		{
			pOut = out;
		}
		if (0 == (cSize = wchar_to_utf8(wszWChar, cWCharLen, pOut, cSize, 0)))
		{
			if (pOut != out)
			{
				delete [] pOut;
			}

			return false;
		}
		psUTF8->assign(pOut, cSize);
		if (pOut != out)
		{
			delete [] pOut;
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
