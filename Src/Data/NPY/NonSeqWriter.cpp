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
#include "CX/Data/NPY/NonSeqWriter.hpp"
#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

NonSeqWriter::NonSeqWriter()
{
	m_pFile = NULL;
	m_header.Reset();
}

NonSeqWriter::~NonSeqWriter()
{
	Close();
}

Status NonSeqWriter::CheckCreateArgs(Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, 
                                     Version nVersion)
{
	if (Consts::MIN_COLUMNS_COUNT > cColumnsCount)
	{
		return Status(Status_InvalidArg, "columns count too small");
	}
	if (Consts::MAX_COLUMNS_COUNT < cColumnsCount)
	{
		return Status(Status_InvalidArg, "columns count too big");
	}
	if (Type_Invalid >= nType || Type_Double < nType)
	{
		return Status(Status_InvalidArg, "type is invalid");
	}
	if (ByteOrder_Invalid >= nByteOrder || ByteOrder_BigEndian < nByteOrder)
	{
		return Status(Status_InvalidArg, "byte order is invalid");
	}
	if (Format_Invalid >= nFormat || Format_Fortran < nFormat)
	{
		return Status(Status_InvalidArg, "format is invalid");
	}
	if (Version_Invalid >= nVersion || Version_3_0 < nVersion)
	{
		return Status(Status_InvalidArg, "version is invalid");
	}

	return Status();
}

Status NonSeqWriter::Create(Size cRowsCount, Size cColumnsCount, Type nType, ByteOrder nByteOrder, 
                            Format nFormat, Version nVersion)
{
	String   sHeader;
	void     *pBuffer;
	Size     cbBufferSize;
	UInt64   cbDataSize;
	DWORD    dwReqSize;
	DWORD    dwAckSize;
	Status   status;

	m_header.nFormat     = nFormat;
	m_header.nVersion    = nVersion;
	m_header.nType       = nType;
	m_header.nByteOrder  = nByteOrder;
	m_header.cColumns    = cColumnsCount;
	m_header.cRows       = cRowsCount;
	m_header.cbRowSize   = 0;
	m_header.cbUsedSize  = 0;
	m_header.cbTotalSize = 0;
	m_header.ComputeRowSize();
	if (!(status = m_header.Write(&sHeader, MIN_EXTRA_PADDING)))
	{
		return status;
	}
	if (!::WriteFile(m_pFile, sHeader.c_str(), (DWORD)sHeader.size(), &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)sHeader.size())
	{
		return Status(Status_WriteFailed, "WriteFile failed");
	}

	cbDataSize   = m_header.cbRowSize * cRowsCount;
	cbBufferSize = 1024 * 1024 * 8;
	if (NULL == (pBuffer = Mem::Alloc(cbBufferSize)))
	{
		return Status(Status_MemAllocFailed, "Failed to allocate temp buffer ({1} bytes)", cbBufferSize);
	}
	memset(pBuffer, 0 , cbBufferSize);

	status.Clear();
	while (0 < cbDataSize)
	{
		if (cbDataSize > (UInt64)cbBufferSize)
		{
			dwReqSize = (DWORD)cbBufferSize;
		}
		else
		{
			dwReqSize = (DWORD)cbDataSize;
		}
		if (!::WriteFile(m_pFile, pBuffer, dwReqSize, &dwAckSize, NULL))
		{
			return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
		}
		cbDataSize -= dwAckSize;
	}

	Mem::Free(pBuffer);

	return status;
}

Status NonSeqWriter::Create(const Char *szPath, Size cRowsCount, Size cColumnsCount, Type nType, 
                            ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, Format nFormat/* = Format_C*/, 
                            Version nVersion/* = Version_1_0*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cColumnsCount, nType, nByteOrder, nFormat, nVersion)))
	{
		return status;
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileA(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
	                                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileA failed with error {1}", ::GetLastError());
	}
	m_sPath = szPath;

	if (!(status = Create(cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion)))
	{
		Close();

		return status;
	}

	return Status();
}

Status NonSeqWriter::Create(const WChar *wszPath, Size cRowsCount, Size cColumnsCount, Type nType, 
                            ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, Format nFormat/* = Format_C*/, 
                            Version nVersion/* = Version_1_0*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cColumnsCount, nType, nByteOrder, nFormat, nVersion)))
	{
		return status;
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileW(wszPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
	                                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileW failed with error {1}", ::GetLastError());

	}
	m_wsPath = wszPath;

	if (!(status = Create(cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion)))
	{
		Close();

		return status;
	}

	return Status();
}

Status NonSeqWriter::Close()
{
	Status   status;

	if (NULL != m_pFile)
	{
		::CloseHandle(m_pFile);
		m_pFile = NULL;
	}
	m_sPath.clear();
	m_wsPath.clear();
	m_header.Reset();

	return status;
}

Status NonSeqWriter::Write(const void *pRows, Size cRowsCount, Size cRowsIndex)
{
	DWORD   dwAckSize;

	if (NULL == m_pFile)
	{
		return Status_NotInitialized;
	}
	if (cRowsIndex >= m_header.cRows)
	{
		return Status_InvalidArg;
	}
	if (cRowsIndex + cRowsCount > m_header.cRows)
	{
		return Status_InvalidArg;
	}

	LARGE_INTEGER   liPos;

	liPos.QuadPart = m_header.cbTotalSize + cRowsIndex * m_header.cbRowSize;

	if (!::SetFilePointerEx(m_pFile, liPos, NULL, FILE_BEGIN))
	{
		return Status(Status_InvalidArg, "SetFilePointerEx failed with error {1}",::GetLastError());
	}

	if (!::WriteFile(m_pFile, pRows, (DWORD)(m_header.cbRowSize * cRowsCount), &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)(m_header.cbRowSize * cRowsCount))
	{
		return Status(Status_WriteFailed, "WriteFile failed");
	}

	return Status();
}

Bool NonSeqWriter::IsOK() const
{
	return (NULL != m_pFile);
}

const Char *NonSeqWriter::GetPathA() const
{
	if (NULL == m_pFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

const WChar *NonSeqWriter::GetPathW() const
{
	if (NULL == m_pFile)
	{
		return L"";
	}

	return m_wsPath.c_str();
}

Format NonSeqWriter::GetFormat() const
{
	if (NULL == m_pFile)
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Size NonSeqWriter::GetRowsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cRows;
}

Size NonSeqWriter::GetColumnsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cColumns;
}

Type NonSeqWriter::GetType() const
{
	if (NULL == m_pFile)
	{
		return Type_Invalid;
	}

	return m_header.nType;
}

ByteOrder NonSeqWriter::GetByteOrder() const
{
	if (NULL == m_pFile)
	{
		return ByteOrder_Invalid;
	}

	return m_header.nByteOrder;
}

Size NonSeqWriter::GetRowSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Version NonSeqWriter::GetVersion() const
{
	if (NULL == m_pFile)
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

}//namespace NPY

}//namespace Data

}//namespace CX

