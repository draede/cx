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
#include "CX/Str/WildMat.hpp"
#include "CX/Platform.hpp"
#include "CX/C/ctype.h"
#ifdef CX_OS_WINDOWS
#include <windows.h>
#else
#include <wchar.h>
#endif


namespace CX
{

namespace Str
{

WildMat::WildMat()
{
}

WildMat::~WildMat()
{
}

WildMat &WildMat::Get()
{
	static WildMat wm;

	return wm;
}

bool WildMat::MatchA(const Char *szString, const Char *szPattern, bool bIgnoreCase)
{
	return (0 == StrMatchA(szString, szPattern, bIgnoreCase));
}

bool WildMat::MatchW(const WChar *wszString, const CX::WChar *wszPattern, bool bIgnoreCase)
{
	return (0 == StrMatchW(wszString, wszPattern, bIgnoreCase));
}

//from apache
/** StrMatchA ANSI string pattern matching
 * Match = 0, NoMatch = 1, Abort = -1
 * Based loosely on sections of wildmat.c by Rich Salz
 */
int WildMat::StrMatchA(const Char *szString, const Char *szPattern, bool bIgnoreCase)
{
	int x, y;

	for (x = 0, y = 0; szPattern[y]; ++y, ++x)
	{
		if (!szString[x] && (szPattern[y] != '*' && szPattern[y] != '?'))
		{
			return -1;
		}
		if (szPattern[y] == '*')
		{
			while (szPattern[++y] == '*');
			if (!szPattern[y])
			{
				return 0;
			}
			while (szString[x])
			{
				int rc;

				if ((rc = StrMatchA(&szString[x++], &szPattern[y], bIgnoreCase)) != 1)
				{
					return rc;
				}
			}

			return -1;
		}
		else if (szPattern[y] != '?')
		{
			if (bIgnoreCase)
			{
#ifdef CX_OS_WINDOWS
				if (CharLowerA((LPSTR)((SIZE_T)szString[x])) != CharLower((LPSTR)((SIZE_T)szPattern[y])))
#else
				if (cx_tolower(szString[x]) != cx_tolower(szPattern[y]))
#endif
				{
					return 1;
				}
			}
			else
			{
				if (szString[x] != szPattern[y])
				{
					return 1;
				}
			}
		}
	}

	return (szString[x] != '\0');
}

//from apache
/** StrMatchW WIDE string pattern matching
 * Match = 0, NoMatch = 1, Abort = -1
 * Based loosely on sections of wildmat.c by Rich Salz
 */
int WildMat::StrMatchW(const WChar *wszString, const CX::WChar *wszPattern, bool bIgnoreCase)
{
	int x, y;

	for (x = 0, y = 0; wszPattern[y]; ++y, ++x)
	{
		if (!wszString[x] && (wszPattern[y] != L'*' && wszPattern[y] != L'?'))
		{
			return -1;
		}
		if (wszPattern[y] == L'*')
		{
			while (wszPattern[++y] == L'*');
			if (!wszPattern[y])
			{
				return 0;
			}
			while (wszString[x])
			{
				int rc;

				if ((rc = StrMatchW(&wszString[x++], &wszPattern[y], bIgnoreCase)) != 1)
				{
					return rc;
				}
			}

			return -1;
		}
		else if (wszPattern[y] != L'?')
		{
			if (bIgnoreCase)
			{
#ifdef CX_OS_WINDOWS
				if (CharLowerW((LPWSTR)((SIZE_T)wszString[x])) != CharLowerW((LPWSTR)((SIZE_T)wszPattern[y])))
#else
				if (cxw_tolower(wszString[x]) != cxw_tolower(wszPattern[y]))
#endif
				{
					return 1;
				}
			}
			else
			{
				if (wszString[x] != wszPattern[y])
				{
					return 1;
				}
			}
		}
	}

	return (wszString[x] != L'\0');
}

}//namespace Str

}//namespace CX
