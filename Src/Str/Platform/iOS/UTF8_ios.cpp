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
#include "CX/Platform.hpp"


#if defined(CX_OS_IOS)


#include "CX/Str/UTF8.hpp"
#include "CX/C/string.h"
#include "CX/Status.hpp"
#include "utf8.h"

//NOT TESTED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace CX
{

namespace Str
{

UTF8::UTF8()
{
}

UTF8::~UTF8()
{
}

Status UTF8::ToWChar(const Char *szUTF8, Size cUTF8Len, WChar *wszWChar, Size *pcWCharLen)
{
	size_t cSize;

	if (TYPE_SIZE_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8);
	}
	if (0 == cUTF8Len)
	{
		if (1 > *pcWCharLen)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		*pcWCharLen = 0;
		wszWChar[0] = 0;

		return Status();
	}
	if (0 < (cSize = utf8_to_wchar(szUTF8, cUTF8Len, NULL, 0, 0)))
	{
		if (NULL == wszWChar)
		{
			*pcWCharLen = (Size)(cSize + 1);

			return Status();
		}
		if (*pcWCharLen < (Size)cSize + 1)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		if (0 < (cSize = utf8_to_wchar(szUTF8, cUTF8Len, wszWChar, *pcWCharLen, 0)))
		{
			wszWChar[cSize] = 0;
			*pcWCharLen = cSize;

			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "utf8_to_wchar failed");
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "utf8_to_wchar failed");
	}
}

Status UTF8::FromWChar(const WChar *wszWChar, Size cWCharLen, Char *szUTF8, Size *pcUTF8Len)
{
	size_t cSize;

	if (TYPE_SIZE_MAX == cWCharLen)
	{
		cWCharLen = cxw_strlen(wszWChar);
	}
	if (0 == cWCharLen)
	{
		if (1 > *pcUTF8Len)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		*pcUTF8Len = 0;
		szUTF8[0]  = 0;

		return Status();
	}
	if (0 < (cSize = wchar_to_utf8(wszWChar, cWCharLen, NULL, 0, 0)))
	{
		if (NULL == szUTF8)
		{
			*pcUTF8Len = (Size)(cSize + 1);

			return Status();
		}
		if (*pcUTF8Len < (Size)cSize + 1)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		if (0 < (cSize = wchar_to_utf8(wszWChar, cWCharLen, szUTF8, *pcUTF8Len, 0)))
		{
			szUTF8[cSize] = 0;
			*pcUTF8Len    = cSize;

			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "wchar_to_utf8 failed");
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "wchar_to_utf8 failed");
	}
}

Status UTF8::ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
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
		psWChar->clear();

		return Status();
	}
	if (0 < (cSize = utf8_to_wchar(szUTF8, cUTF8Len, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) WChar[cSize]))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cSize * sizeof(WChar));
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
			
			return Status(Status_ConversionFailed, "utf8_to_wchar failed");
		}
		psWChar->assign(pOut, cSize);
		if (NULL != pOut)
		{
			delete [] pOut;
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "utf8_to_wchar failed");
	}

	return Status();
}

Status UTF8::FromWChar(const WChar *wszWChar, String *psUTF8, Size cWCharLen/* = TYPE_SIZE_MAX*/)
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

		return Status();
	}
	if (0 < (cSize = wchar_to_utf8(wszWChar, cWCharLen, NULL, 0, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = new (std::nothrow) Char[cSize]))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cSize * sizeof(Char));
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

			return Status(Status_ConversionFailed, "wchar_to_utf8 failed");
		}
		psUTF8->assign(pOut, cSize);
		if (pOut != out)
		{
			delete [] pOut;
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "wchar_to_utf8 failed");
	}

	return Status();
}

}//namespace Str

}//namespace CX


#endif

