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


#if defined(CX_OS_WINDOWS)


#include "CX/Str/UTF8.hpp"
#include "CX/C/string.h"
#include "CX/Status.hpp"
#include "CX/C/Platform/Windows/windows.h"


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
	int  cSize;

	if (TYPE_SIZE_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8) + 1;
	}
	if (0 == cUTF8Len)
	{
		if (NULL == wszWChar)
		{
			*pcWCharLen = 1;

			return Status();
		}
		else
		{
			if (*pcWCharLen < 1)
			{
				return Status(Status_TooSmall, "Output buffer too small");
			}
			*wszWChar = 0;

			return Status();
		}
	}
	if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, NULL, 0)))
	{
		*pcWCharLen = (Size)cSize;
		if (NULL == wszWChar)
		{
			return Status();
		}
		if (*pcWCharLen < (Size)cSize)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, wszWChar, cSize)))
		{
			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
	}
}

Status UTF8::FromWChar(const WChar *wszWChar, Size cWCharLen, Char *szUTF8, Size *pcUTF8Len)
{
	int cSize;

	if (NULL == wszWChar)
	{
		return Status(Status_InvalidArg);
	}
	if (TYPE_SIZE_MAX == cWCharLen)
	{
		cWCharLen = cxw_strlen(wszWChar) + 1;
	}
	if (0 == cWCharLen)
	{
		if (NULL == wszWChar)
		{
			*pcUTF8Len = 1;

			return Status();
		}
		else
		{
			if (*pcUTF8Len < 1)
			{
				return Status(Status_TooSmall, "Output buffer too small");
			}
			*szUTF8 = 0;

			return Status();
		}
	}
	if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, NULL, 0, NULL, NULL)))
	{
		*pcUTF8Len = (Size)cSize;
		if (NULL == szUTF8)
		{
			return Status();
		}
		if (*pcUTF8Len < (Size)cSize)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, szUTF8, cSize, NULL, NULL)))
		{
			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
	}
}

Status UTF8::ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
{
	Size     cLen;
	WChar    *pOut = NULL;
	WChar    out[4000];
	Status   status;

	if ((status = ToWChar(szUTF8, cUTF8Len, NULL, &cLen)).IsNOK())
	{
		return status;
	}
	if (cLen > sizeof(out) / sizeof(out[0]))
	{
		if (NULL == (pOut = new (std::nothrow) WChar[cLen]))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cLen * sizeof(WChar));
		}
	}
	else
	{
		pOut = out;
	}
	if ((status = ToWChar(szUTF8, cUTF8Len, pOut, &cLen)).IsNOK())
	{
		if (pOut != out)
		{
			delete[] pOut;
		}

		return status;
	}
	if (0 < cLen && 0 == pOut[cLen -1])
	{
		cLen--;
	}
	psWChar->assign(pOut, cLen);
	if (pOut != out)
	{
		delete[] pOut;
	}

	return Status();
}

Status UTF8::FromWChar(const WChar *wszWChar, String *psUTF8, Size cWCharLen/* = TYPE_SIZE_MAX*/)
{
	Size     cLen;
	Char     *pOut = NULL;
	Char     out[8000];
	Status   status;

	if ((status = FromWChar(wszWChar, cWCharLen, NULL, &cLen)).IsNOK())
	{
		return status;
	}
	if (cLen > sizeof(out) / sizeof(out[0]))
	{
		if (NULL == (pOut = new (std::nothrow) Char[cLen]))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cLen * sizeof(Char));
		}
	}
	else
	{
		pOut = out;
	}
	if ((status = FromWChar(wszWChar, cWCharLen, pOut, &cLen)).IsNOK())
	{
		if (pOut != out)
		{
			delete[] pOut;
		}

		return status;
	}
	if (0 < cLen && 0 == pOut[cLen -1])
	{
		cLen--;
	}
	psUTF8->assign(pOut, cLen);
	if (pOut != out)
	{
		delete[] pOut;
	}

	return Status();
}

}//namespace Str

}//namespace CX


#endif

