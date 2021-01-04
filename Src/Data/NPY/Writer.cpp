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
#include "CX/Data/NPY/Writer.hpp"
#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

Writer::Writer()
{
	m_header.Reset();
	m_pBuffer            = NULL;
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;
}

Writer::~Writer()
{
	Close();
}

Status Writer::CheckCreateArgs(Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, Version nVersion, 
                               Size cbBufferSize)
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
	if (MIN_BUFFER_SIZE > cbBufferSize)
	{
		return Status(Status_InvalidArg, "buffer size too small");
	}
	if (MAX_BUFFER_SIZE < cbBufferSize)
	{
		return Status(Status_InvalidArg, "buffer size too big");
	}

	return Status();
}

Status Writer::Create(Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, Version nVersion, 
                      Size cbBufferSize)
{
	String   sHeader;
	DWORD    dwAckSize;
	Status   status;

	m_header.nFormat     = nFormat;
	m_header.nVersion    = nVersion;
	m_header.nType       = nType;
	m_header.nByteOrder  = nByteOrder;
	m_header.cColumns    = cColumnsCount;
	m_header.cRows       = 0;
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
	if (NULL == (m_pBuffer = (Byte *)Mem::Alloc(cbBufferSize)))
	{
		return Status_MemAllocFailed;
	}
	m_cbBufferSize = cbBufferSize;
	memset(m_pBuffer, 0, m_cbBufferSize);
	m_cbBufferUsedSize = 0;
	m_cbReceived       = 0;
	m_cbWritten        = 0;

	return Status();
}

Status Writer::Flush()
{
	DWORD    dwAckSize;
	Status   status;

	if (!WriteFile(m_pFile, m_pBuffer, (DWORD)m_cbBufferUsedSize, &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)m_cbBufferUsedSize)
	{
		return Status(Status_WriteFailed, "WriteFile failed");
	}
	m_cbWritten += m_cbBufferUsedSize;
	m_cbBufferUsedSize = 0;

	return Status();
}

Status Writer::Create(const Char *szPath, Size cColumnsCount, Type nType, 
                      ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, Format nFormat/* = Format_C*/, 
                      Version nVersion/* = Version_1_0*/, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
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

	if (!(status = Create(cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status Writer::Create(const WChar *wszPath, Size cColumnsCount, Type nType, 
                      ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, Format nFormat/* = Format_C*/, 
                      Version nVersion/* = Version_1_0*/, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
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

	if (!(status = Create(cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status Writer::Close()
{
	DWORD    dwAckSize;
	UInt64   cRows = 0;
	Status   status;

	if (NULL != m_pFile)
	{
		status = Flush();
		if (status)
		{
			if (0 != (m_cbWritten % m_header.cbRowSize))
			{
				status = Status(Status_InvalidArg, "The last row is incomplete");
			}
		}
		if (status)
		{
			cRows = m_cbWritten / m_header.cbRowSize;
			if (Consts::MAX_ROWS_COUNT < cRows)
			{
				status = Status(Status_InvalidArg, "Too many rows");
			}
		}
		if (status)
		{
			LARGE_INTEGER   liPos = { 0 };
			String          sHeader;
			Size            cInitialHeaderSize;

			if (::SetFilePointerEx(m_pFile, liPos, NULL, FILE_BEGIN))
			{
				cInitialHeaderSize = m_header.cbTotalSize;
				m_header.cRows     = (Size)cRows;
				if (!(status = m_header.Write(&sHeader, 0)))
				{
					return status;
				}
				if (sHeader.size() > cInitialHeaderSize)
				{
					return Status(Status_OperationFailed, "Final header is larger than the initial one!");
				}
				if (sHeader.size() < cInitialHeaderSize)
				{
					if (!(status = m_header.Write(&sHeader, cInitialHeaderSize - sHeader.size())))
					{
						return status;
					}
				}
				if (!::WriteFile(m_pFile, sHeader.c_str(), (DWORD)sHeader.size(), &dwAckSize, NULL))
				{
					return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
				}
				if (dwAckSize != (DWORD)sHeader.size())
				{
					return Status(Status_WriteFailed, "WriteFile failed");
				}
			}
			else
			{
				status = Status(Status_InvalidArg, "SetFilePointerEx failed with error {1}",::GetLastError());
			}
		}
	}
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
		m_pBuffer = NULL;
	}
	if (NULL != m_pFile)
	{
		::CloseHandle(m_pFile);
		m_pFile = NULL;
	}
	if (!status)
	{
		if (!m_sPath.empty())
		{
			_unlink(m_sPath.c_str());
		
		}
		else
		if (!m_wsPath.empty())
		{
			_wunlink(m_wsPath.c_str());
		}
	}
	m_sPath.clear();
	m_wsPath.clear();
	m_header.Reset();
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;

	return status;
}

Status Writer::Write(const void *pBuffer, Size cbSize)
{
	const Byte   *pPos      = (const Byte *)pBuffer;
	Size         cbLeftOver = m_cbBufferSize - m_cbBufferUsedSize;
	Size         cbChunkSize;
	Status       status;

	while (0 < cbSize)
	{
		if (cbSize > cbLeftOver)
		{
			cbChunkSize = cbLeftOver;
		}
		else
		{
			cbChunkSize = cbSize;
		}
		memcpy(m_pBuffer + m_cbBufferUsedSize, pPos, cbChunkSize);
		m_cbBufferUsedSize += cbChunkSize;
		pPos += cbChunkSize;
		cbSize -= cbChunkSize;
		m_cbReceived += cbChunkSize;
		if (m_cbBufferUsedSize == m_cbBufferSize)
		{
			if (!(status = Flush()))
			{
				return status;
			}
		}
	}

	return Status();
}

Bool Writer::IsOK() const
{
	return (NULL != m_pFile);
}

const Char *Writer::GetPathA() const
{
	if (NULL == m_pFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

const WChar *Writer::GetPathW() const
{
	if (NULL == m_pFile)
	{
		return L"";
	}

	return m_wsPath.c_str();
}

Format Writer::GetFormat() const
{
	if (NULL == m_pFile)
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Size Writer::GetColumnsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cColumns;
}

Type Writer::GetType() const
{
	if (NULL == m_pFile)
	{
		return Type_Invalid;
	}

	return m_header.nType;
}

ByteOrder Writer::GetByteOrder() const
{
	if (NULL == m_pFile)
	{
		return ByteOrder_Invalid;
	}

	return m_header.nByteOrder;
}

Size Writer::GetRowSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Version Writer::GetVersion() const
{
	if (NULL == m_pFile)
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

Size Writer::GetBufferSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbBufferSize;
}

UInt64 Writer::GetReceivedBytes() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbReceived;
}

UInt64 Writer::GetWrittenBytes() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbWritten;
}

}//namespace NPY

}//namespace Data

}//namespace CX

