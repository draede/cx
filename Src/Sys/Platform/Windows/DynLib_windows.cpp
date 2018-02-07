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


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/DynLib.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

DynLib::DynLib()
{
	m_hHandle = NULL;
}

DynLib::~DynLib()
{
	Unload();
}

Bool DynLib::IsOK()
{
	return (NULL != m_hHandle);
}

Status DynLib::Load(const Char *szPath)
{
	Unload();

	if (NULL == (m_hHandle = LoadLibraryA(szPath)))
	{
		return Status(Status_FileNotFound, "Failed to load library");
	}

	return Status();
}

Status DynLib::Load(const WChar *wszPath)
{
	Unload();

	if (NULL == (m_hHandle = LoadLibraryW(wszPath)))
	{
		return Status(Status_FileNotFound, "Failed to load library");
	}

	return Status();
}


Status DynLib::Unload()
{
	if (NULL != m_hHandle)
	{
		FreeLibrary(m_hHandle);
		m_hHandle = NULL;
	}
	
	return Status();
}

void *DynLib::GetFunc(const Char *szName)
{
	if (NULL == m_hHandle)
	{
		return NULL;
	}

	return GetProcAddress(m_hHandle, szName);
}


}//namespace Sys

}//namespace CX


#endif

