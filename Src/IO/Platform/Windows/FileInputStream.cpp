/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#include "CX/IO/FileInputStream.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

FileInputStream::FileInputStream(const Char *szPath)
{
	WString wsPath;

	m_hFile = NULL;
	m_sPath = szPath;
	if (Str::UTF8::ToWChar(szPath, &wsPath).IsOK())
	{
		OpenFile(wsPath.c_str());
	}
}

FileInputStream::FileInputStream(const WChar *wszPath)
{
	m_hFile = NULL;
	Str::UTF8::FromWChar(wszPath, &m_sPath);
	OpenFile(wszPath);
}

FileInputStream::~FileInputStream()
{
	if (NULL != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

Status FileInputStream::OpenFile(const WChar *wszPath)
{
	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileW(wszPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileW failed with error {1}", GetLastError());
	}

	return Status();
}

Status FileInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
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

	if (!ReadFile(m_hFile, pBuffer, (DWORD)cbReqSize, &dwAckSize, NULL))
	{
		return Status(Status_OpenFailed, "ReadFile failed with error {1}", GetLastError());
	}
	*pcbAckSize = (Size)dwAckSize;
	if (0 == dwAckSize)
	{
		return Status_EOF;
	}

	return Status();
}

Status FileInputStream::SetPos(UInt64 cbPos)
{
	if (NULL == m_hFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	LARGE_INTEGER liPos;

	liPos.QuadPart = cbPos;
	if (!SetFilePointerEx(m_hFile, liPos, NULL, FILE_BEGIN))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	return Status();
}

Status FileInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_hFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	LARGE_INTEGER liPos;
	LARGE_INTEGER liAckPos;

	liPos.QuadPart = 0;
	if (!SetFilePointerEx(m_hFile, liPos, &liAckPos, FILE_CURRENT))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}
	*pcbPos = (UInt64)liAckPos.QuadPart;

	return Status();
}

Status FileInputStream::GetSize(UInt64 *pcbSize) const
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

Bool FileInputStream::IsEOF() const
{
	if (NULL == m_hFile)
	{
		return True;
	}

	UInt64   cbSize;
	UInt64   cbPos;
	Status   status;

	if (!(status = GetPos(&cbPos)))
	{
		return True;
	}
	if (!(status = GetSize(&cbSize)))
	{
		return True;
	}

	return (cbPos >= cbSize);
}

Bool FileInputStream::IsOK() const
{
	return (NULL != m_hFile);
}

const Char *FileInputStream::GetPath() const
{
	if (NULL == m_hFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

}//namespace IO

}//namespace CX


#endif

