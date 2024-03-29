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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Process.hpp"
#include "CX/C/stdlib.h"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"
#include "CX/Mem.hpp"


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
	return (Process::ID)GetCurrentProcessId();
}

Status Process::GetCurrentProcessPath(String &sPath)
{
	WString wsPath;
	Status  status;

	if ((status = GetCurrentProcessPathW(wsPath)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::FromWChar(wsPath.c_str(), &sPath, wsPath.size())).IsNOK())
	{
		return status;
	}

	return Status();
}

Status Process::GetCurrentProcessPathW(WString &wsPath)
{
	WChar    *wszPath;
	WChar    *wszTmpPath;
	DWORD    dwLen;
	Status   status;

	wszPath = NULL;
	dwLen   = 128;
	for (;;)
	{
		DWORD    dwRet;

		dwLen *= 2;
		if (32768 < dwLen)
		{
			status = Status(Status_OperationFailed, "Path too big");
		
			break;
		}
		if (NULL == (wszTmpPath = (WChar *)Mem::Realloc(wszPath, sizeof(WChar) * (Size)dwLen)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", sizeof(WChar) * (Size)dwLen);

			break;
		}
		wszPath = wszTmpPath;
		dwRet   = GetModuleFileNameW(NULL, wszPath, dwLen);
		if (0 == dwRet)
		{
			status = Status(Status_OperationFailed, "GetModuleFileNameW failed with error code {1}", (int)GetLastError());

			break;
		}
		if (dwRet < dwLen)
		{
			break;
		}
	}
	if (status.IsOK() && NULL != wszPath)
	{
		wsPath = wszPath;
	}
	if (NULL != wszPath)
	{
		Mem::Free(wszPath);
	}

	return status;
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
		if (pszPos >= pszStart && ('\\' == *pszPos || '/' == *pszPos))
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

