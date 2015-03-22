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

Status UTF8::ToUTF16(const Char *szUTF8, Size cUTF8Len, WChar *wszUTF16, Size *pcUTF16Len)
{
	int cSize;

	if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8,
	                                        TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, NULL, 0)))
	{
		if (NULL == wszUTF16)
		{
			*pcUTF16Len = (Size)(cSize + 1);

			return Status();
		}
		if (*pcUTF16Len < (Size)cSize + 1)
		{
			return Status(Status_TooSmall, "Output buffer too small");
		}
		if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8,
		                                       TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, wszUTF16, 
		                                       cSize + 1)))
		{
			wszUTF16[cSize] = 0;

			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}",
			              GetLastError());
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}",
		              GetLastError());
	}
}

Status UTF8::FromUTF16(const WChar *wszUTF16, Size cUTF16Len, Char *szUTF8, Size *pcUTF8Len)
{
	int cSize;

	if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
		                                    TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, NULL, 0, 
		                                    NULL, NULL)))
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
		if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
		                                       TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, szUTF8, 
			                                    cSize, NULL, NULL)))
		{
			szUTF8[cSize] = 0;

			return Status();
		}
		else
		{
			return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}",
			              GetLastError());
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}",
		              GetLastError());
	}
}

Status UTF8::ToUTF16(const Char *szUTF8, WString *psUTF16, Size cUTF8Len/* = TYPE_SIZE_MAX*/)
{
	int   cSize;
	WChar *pOut = NULL;
	WChar out[8000];

	if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, 
	                                       TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, NULL, 0)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = NewArr<WChar>(cSize)))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", 
				              cSize * sizeof(WChar));
			}
		}
		else
		{
			pOut = out;
		}
		if (0 >= (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, 
		                                    TYPE_SIZE_MAX == cUTF8Len ? -1 : (int)cUTF8Len, pOut, cSize)))
		{
			if (pOut != out)
			{
				DeleteArr(pOut);
			}
			
			return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", 
			              GetLastError());
		}
		psUTF16->assign(pOut, cSize);
		if (pOut != pOut)
		{
			DeleteArr(pOut);
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", 
		              GetLastError());
	}

	return Status();
}

Status UTF8::FromUTF16(const WChar *wszUTF16, String *psUTF8, Size cUTF16Len/* = TYPE_SIZE_MAX*/)
{
	int cSize;

	Char *pOut = NULL;
	Char out[8000];

	if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
		                                    TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, NULL, 0, 
		                                    NULL, NULL)))
	{
		if (cSize > 8000)
		{
			if (NULL == (pOut = NewArr<Char>(cSize)))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes",
					            cSize * sizeof(Char));
			}
		}
		else
		{
			pOut = out;
		}
		if (0 >= (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszUTF16, 
		                                        TYPE_SIZE_MAX == cUTF16Len ? -1 : (int)cUTF16Len, pOut, 
			                                     cSize, NULL, NULL)))
		{
			if (pOut != out)
			{
				DeleteArr(pOut);
			}

			return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", 
			              GetLastError());
		}
		psUTF8->assign(pOut, cSize);
		if (pOut != out)
		{
			DeleteArr(pOut);
		}
	}
	else
	{
		return Status(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", 
		              GetLastError());
	}

	return Status();
}

}//namespace Str

}//namespace CX


#endif

