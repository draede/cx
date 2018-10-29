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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/FileOutputStream.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

FileOutputStream::FileOutputStream(const Char *szPath, bool bAppend/* = false*/)
{
	WString wsPath;

	m_hFile = NULL;
	m_sPath = szPath;
	if (Str::UTF8::ToWChar(szPath, &wsPath).IsOK())
	{
		OpenFile(wsPath.c_str(), bAppend);
	}
}

FileOutputStream::FileOutputStream(const WChar *wszPath, bool bAppend/* = false*/)
{
	m_hFile = NULL;
	Str::UTF8::FromWChar(wszPath, &m_sPath);
	OpenFile(wszPath, bAppend);
}

FileOutputStream::~FileOutputStream()
{
	if (NULL != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

Status FileOutputStream::OpenFile(const WChar *wszPath, bool bAppend)
{
	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileW(wszPath, bAppend ? FILE_APPEND_DATA : GENERIC_WRITE, 
	                                                   FILE_SHARE_READ, NULL, bAppend ? OPEN_ALWAYS : CREATE_ALWAYS, 
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileW failed with error {1}", GetLastError());
	}

	return Status();
}

Status FileOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_hFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}
	if ((Size)TYPE_UINT32_MAX < cbReqSize)
	{
		return Status_TooBig;
	}

	DWORD dwAckSize;

	if (!WriteFile(m_hFile, pBuffer, (DWORD)cbReqSize, &dwAckSize, NULL))
	{
		return Status(Status_OpenFailed, "WriteFile failed with error {1}", GetLastError());
	}
	*pcbAckSize = (Size)dwAckSize;

	return Status();
}

Status FileOutputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_hFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	LARGE_INTEGER liSize;

	if (!GetFileSizeEx(m_hFile, &liSize))
	{
		return Status(Status_OperationFailed, "GetFileSizeEx failed with error {1}", GetLastError());
	}
	*pcbSize = liSize.QuadPart;

	return Status();
}

Bool FileOutputStream::IsOK() const
{
	return (NULL != m_hFile);
}

const Char *FileOutputStream::GetPath() const
{
	if (NULL == m_hFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

Status FileOutputStream::Flush()
{
	if (NULL == m_hFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}
	FlushFileBuffers(m_hFile);

	return Status();
}

}//namespace IO

}//namespace CX


#endif

