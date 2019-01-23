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


#include "CX/IO/MemoryMappedFile.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace IO
{

MemoryMappedFile::MemoryMappedFile()
{
	m_cbFileSize   = 0;
	m_pMapData     = NULL;
	m_cbMapOffset  = 0;
	m_cbMapSize    = 0;
	m_pFile        = NULL;
	m_pFileMap     = NULL;
}

MemoryMappedFile::MemoryMappedFile(const Char *szPath, UInt64 cbMapOffset/* = 0*/, UInt64 cbMapSize/* = 0*/, 
                                   UInt32 nFlags/* = Flag_AttrNormal*/)
{
	m_cbFileSize   = 0;
	m_pMapData     = NULL;
	m_cbMapOffset  = 0;
	m_cbMapSize    = 0;
	m_pFile        = NULL;
	m_pFileMap     = NULL;
	Open(szPath, cbMapOffset, cbMapSize, nFlags);
}

MemoryMappedFile::MemoryMappedFile(const WChar *wszPath, UInt64 cbMapOffset/* = 0*/, UInt64 cbMapSize/* = 0*/, 
                                   UInt32 nFlags/* = Flag_AttrNormal*/)
{
	m_cbFileSize   = 0;
	m_pMapData     = NULL;
	m_cbMapOffset  = 0;
	m_cbMapSize    = 0;
	m_pFile        = NULL;
	m_pFileMap     = NULL;
	Open(wszPath, cbMapOffset, cbMapSize, nFlags);
}

MemoryMappedFile::~MemoryMappedFile()
{
	Close();
}

Status MemoryMappedFile::Open(const Char *szPath, UInt64 cbMapOffset/* = 0*/, UInt64 cbMapSize/* = 0*/,
                              UInt32 nFlags/* = Flag_AttrNormal*/)
{
	WString   wsPath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return status;
	}
	if (!(status = OpenInternal(wsPath.c_str(), cbMapOffset, cbMapSize, nFlags)))
	{
		return status;
	}
	m_sPath = szPath;
	
	return Status();
}

Status MemoryMappedFile::Open(const WChar *wszPath, UInt64 cbMapOffset/* = 0*/, UInt64 cbMapSize/* = 0*/, 
                              UInt32 nFlags/* = Flag_AttrNormal*/)
{
	Status    status;

	if (!(status = OpenInternal(wszPath, cbMapOffset, cbMapSize, nFlags)))
	{
		return status;
	}
	Str::UTF8::FromWChar(wszPath, &m_sPath);

	return Status();
}

Status MemoryMappedFile::OpenInternal(const WChar *wszPath, UInt64 cbMapOffset/* = 0*/, UInt64 cbMapSize/* = 0*/,
                                      UInt32 nFlags/* = Flag_AttrNormal*/)
{
	DWORD           dwFlags;
	LARGE_INTEGER   liSize;
	Status          status;

	dwFlags = 0;
	if (Flag_AttrNormal == (Flag_AttrNormal & nFlags))
	{
		dwFlags |= FILE_ATTRIBUTE_NORMAL;
	}
	if (Flag_AttrSequentialScan == (Flag_AttrSequentialScan & nFlags))
	{
		dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;
	}
	if (Flag_AttrRandomAccess == (Flag_AttrRandomAccess & nFlags))
	{
		dwFlags |= FILE_FLAG_RANDOM_ACCESS;
	}

	for (;;)
	{
		if (INVALID_HANDLE_VALUE == (m_pFile = ::CreateFileW(wszPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		                                                     dwFlags, NULL)))
		{
			m_pFile = NULL;
			status  = Status(Status_OpenFailed, "CreateFileW failed with error {1}", GetLastError());

			break;
		}
		if (!GetFileSizeEx(m_pFile, &liSize))
		{
			status = Status(Status_OpenFailed, "GetFileSizeEx failed with error {1}", GetLastError());

			break;
		}
		m_cbFileSize = (UInt64)liSize.QuadPart;
		if (cbMapOffset > m_cbFileSize)
		{
			status = Status(Status_OpenFailed, "Invalid offset");

			break;
		}
		if (NULL == (m_pFileMap = ::CreateFileMapping(m_pFile, NULL, PAGE_READONLY, 0, 0, NULL)))
		{
			status = Status(Status_OpenFailed, "CreateFileMapping failed with error {1}", GetLastError());

			break;
		}
		if (0 == cbMapSize)
		{
			cbMapSize = m_cbFileSize;
		}
		if (cbMapSize > m_cbFileSize - cbMapOffset)
		{
			cbMapSize = m_cbFileSize - cbMapOffset;
		}
		if (!(status = Remap(cbMapOffset, cbMapSize)))
		{
			break;
		}
		m_cbMapOffset = cbMapOffset;
		m_cbMapSize   = cbMapSize;

		break;
	}
	if (!status)
	{
		Close();
	}

	return status;
}

Status MemoryMappedFile::Close()
{
	if (NULL != m_pMapData)
	{
		UnmapViewOfFile(m_pMapData);
	}
	if (NULL != m_pFileMap)
	{
		CloseHandle(m_pFileMap);
	}
	if (NULL != m_pFile)
	{
		CloseHandle(m_pFile);
	}
	m_cbFileSize   = 0;
	m_pMapData     = NULL;
	m_cbMapOffset  = 0;
	m_cbMapSize    = 0;
	m_pFile        = NULL;
	m_pFileMap     = NULL;

	return Status();
}

Bool MemoryMappedFile::IsOK() const
{
	return (NULL != m_pMapData);
}

UInt64 MemoryMappedFile::GetFileSize() const
{
	return m_cbFileSize;
}

const void *MemoryMappedFile::GetMapData() const
{
	return m_pMapData;
}

UInt64 MemoryMappedFile::GetMapOffset() const
{
	return m_cbMapOffset;
}

UInt64 MemoryMappedFile::GetMapSize() const
{
	return m_cbMapSize;
}

const Char *MemoryMappedFile::GetPath() const
{
	return m_sPath.c_str();
}

Status MemoryMappedFile::Remap(UInt64 cbNewMapOffset, UInt64 cbNewMapSize/* = 0*/)
{
	DWORD   dwOffsetHigh;
	DWORD   dwOffsetLow;

	if (NULL == m_pFileMap)
	{
		return Status_InvalidCall;
	}
	if (cbNewMapOffset >= m_cbFileSize)
	{
		return Status_InvalidArg;
	}
	if (0 == cbNewMapSize)
	{
		cbNewMapSize = m_cbFileSize - cbNewMapOffset;
	}
	else
	{
		if (cbNewMapOffset + cbNewMapSize > m_cbFileSize)
		{
			cbNewMapSize = m_cbFileSize - cbNewMapOffset;
			if (0 == cbNewMapSize)
			{
				return Status_InvalidArg;
			}
		}
	}
	if (NULL != m_pMapData)
	{
		UnmapViewOfFile(m_pMapData);
	}
	m_pMapData     = NULL;
	m_cbMapOffset  = 0;
	m_cbMapSize    = 0;

	dwOffsetHigh = (DWORD)(cbNewMapOffset >> 32);
	dwOffsetLow  = (DWORD)(cbNewMapOffset & 0xFFFFFFFF);
	if (NULL == (m_pMapData = ::MapViewOfFile(m_pFileMap, FILE_MAP_READ, dwOffsetHigh, dwOffsetLow, 
	                                          (SIZE_T)cbNewMapSize)))
	{
		return Status(Status_OperationFailed, "MapViewOfFile failed with error {1}", GetLastError());
	}

	return Status();
}

}//namespace IO

}//namespace CX


#endif

