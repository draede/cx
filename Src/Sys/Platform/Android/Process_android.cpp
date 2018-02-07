/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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


#if defined(CX_OS_ANDROID)


#include "CX/Sys/Process.hpp"
#include "CX/C/stdlib.h"
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
	return (ID)getpid();
}

Status Process::GetCurrentProcessPath(String &sPath)
{
	sPath.clear();

	char    path[8192];
	ssize_t cCount;

	cCount = readlink("/proc/self/exe", path, sizeof(path) / sizeof(path[0]));

	if (0 < cCount)
	{
		sPath.assign(path, cCount);
	}

	return Status_NotSupported;
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
		const Char *pszPos = pszStart + sPath.size() - 1;

		while (pszPos >= pszStart)
		{
			if ('/' == *pszPos)
			{
				break;
			}
			pszPos--;
		}
		if (pszPos >= pszStart && '/' == *pszPos)
		{
			sDir.assign(pszStart, pszPos - pszStart);
		}
	}

	return Status();
}

}//namespace Sys

}//namespace CX


#endif

