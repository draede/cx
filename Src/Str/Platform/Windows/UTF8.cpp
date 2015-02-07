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


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Str/UTF8.h"
#include "CX/C/string.h"
#include "CX/Status.h"
#include <windows.h>


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

Status UTF8::ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len/* = SIZET_MAX*/)
{
	int cSize;

	if (SIZET_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8);
	}
	if (0 == cUTF8Len)
	{
		psWChar->clear();
	}
	else
	{
		if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, NULL, 0)))
		{
			psWChar->resize(cSize);
			if (0 >= (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, &(*psWChar)[0], cSize)))
			{
				return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
			}
		}
		else
		{
			return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
		}
	}

	return Status();
}

Status UTF8::FromWChar(const WChar *wszWChar, String *psUTF8, Size cWCharLen/* = SIZET_MAX*/)
{
	int cSize;

	if (SIZET_MAX == cWCharLen)
	{
		cWCharLen = wcslen(wszWChar);
	}
	if (0 == cWCharLen)
	{
		psUTF8->clear();
	}
	else
	{
		if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, NULL, 0, NULL, NULL)))
		{
			psUTF8->resize(cSize);
			if (0 >= (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, &(*psUTF8)[0], cSize, NULL, NULL)))
			{
				return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
			}
		}
		else
		{
			return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
		}
	}

	return Status();
}

}//namespace Str

}//namespace CX


#endif

