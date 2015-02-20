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


#include "CX/Sys/DynLib.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Sys
{

DynLib::DynLib()
{
	Status::Clear();

	m_hHandle = NULL;
}

DynLib::~DynLib()
{
	Status::Clear();

	Unload();
}

Bool DynLib::IsOK()
{
	Status::Clear();

	return (NULL != m_hHandle);
}

StatusCode DynLib::Load(const Char *szPath)
{
	Unload();
	Status::Clear();

	WString wsPath;

	if (CXNOK(Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return Status::GetCode();
	}

	if (NULL == (m_hHandle = LoadLibraryW(wsPath.c_str())))
	{
		return Status::Set(Status_FileNotFound, "Failed to load library");
	}

	return Status_OK;
}

StatusCode DynLib::Unload()
{
	Status::Clear();

	if (NULL != m_hHandle)
	{
		FreeLibrary(m_hHandle);
	}
	
	return Status_OK;
}

void *DynLib::GetFunc(const Char *szName)
{
	Status::Clear();

	if (NULL == m_hHandle)
	{
		Status::Set(Status_NotInitialized, "");

		return NULL;
	}

	return GetProcAddress(m_hHandle, szName);
}


}//namespace Sys

}//namespace CX


#endif

