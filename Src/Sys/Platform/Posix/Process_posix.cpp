/* 
 * CX - C++ framework for general purpose development
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


#if defined(CX_OS_PROCESS)


#include "CX/Sys/Process.hpp"
#include "CX/C/stdlib.h"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"
#include <sys/types.h>
#include <unistd.h>


namespace CX
{

namespace Sys
{

Process::Process()
{
}

Process::~Process()
{
}

Process::ID Process::GetCurrentProcessID()
{
	return (Process::ID)getpid();
}

Status Process::GetCurrentProcessPath(String &sPath)
{
	Char     *szPath;
	Char     *szTmpPath;
	Size     cLen;
	Size     cRet;
	Status   status;

	szPath = NULL;
	cLen   = 128;
	for (;;)
	{
		cLen *= 2;
		if (8192 < cLen)
		{
			status = Status(Status_OperationFailed, "Path too big");

			break;
		}
		if (NULL == (szTmpPath = (Char *)Mem::Realloc(szPath, sizeof(Char) * cLen)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", sizeof(Char) * cLen);

			break;
		}
		szPath = szTmpPath;
		int nRet = readlink("/proc/self/exe", szTmpPath, cLen - 1);
		if (-1 == nRet)
		{
			status = Status(Status_OperationFailed, "readlink  failed with error code {1}", nRet);

			break;
		}
		cRet = (Size)nRet;
		if (cRet < cLen)
		{
			szTmpPath[cLen] = 0;

			break;
		}
	}
	if (status.IsOK())
	{
		sPath = szPath;
	}
	if (NULL != szPath)
	{
		Mem::Free(szPath);
	}

	return status;
}

Status Process::GetCurrentProcessPathW(WString &wsPath)
{
	String  sPath;
	Status  status;

	if ((status = GetCurrentProcessPath(sPath)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToWChar(sPath.c_str(), &wsPath, sPath.size())).IsNOK())
	{
		return status;
	}

	return Status();
}

Status Process::GetCurrentProcessDir(String &sDir)
{
	String sPath;
	Status status;

	sDir.clear();
	if ((status = GetCurrentProcessPath(sPath)).IsNOK())
	{
		return status;
	}
	if (!sPath.empty())
	{
		const Char *pszStart = sPath.c_str();
		const Char *pszPos   = pszStart + sPath.size() - 1;

		while (pszPos >= pszStart)
		{
			if ('\\' == *pszPos || '/' == *pszPos)
			{
				break;
			}
			pszPos--;
		}
		if (pszPos >= pszStart && '\\' == *pszPos)
		{
			sDir.assign(pszStart, pszPos - pszStart);
		}
	}

	return Status();
}

Status Process::GetCurrentProcessDirW(WString &wsDir)
{
	WString wsPath;
	Status status;

	wsDir.clear();
	if ((status = GetCurrentProcessPathW(wsPath)).IsNOK())
	{
		return status;
	}
	if (!wsPath.empty())
	{
		const WChar *pwszStart = wsPath.c_str();
		const WChar *pwszPos   = pwszStart + wsPath.size() - 1;

		while (pwszPos >= pwszStart)
		{
			if (L'\\' == *pwszPos || L'/' == *pwszPos)
			{
				break;
			}
			pwszPos--;
		}
		if (pwszPos >= pwszStart && (L'\\' == *pwszPos || L'/' == *pwszPos))
		{
			wsDir.assign(pwszStart, pwszPos - pwszStart);
		}
	}

	return Status();
}

}//namespace Sys

}//namespace CX


#endif

