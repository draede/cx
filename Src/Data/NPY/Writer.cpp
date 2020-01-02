/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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
	m_cbHeaderSize      = 0;
	m_cbHeaderDictSize  = 0;
	m_pFile             = NULL;
	m_nFormat           = Format_Invalid;
	m_nVersion          = Version_Invalid;
	m_cbRowSize         = 0;
	m_pBuffer           = NULL;
	m_cbBufferSize      = 0;
	m_cbBufferUsedSize  = 0;
	m_cbReceived        = 0;
	m_cbWritten         = 0;
}

Writer::~Writer()
{
	Close();
}

Status Writer::CheckCreateArgs(const Column *columns, Size cColumnsCount, Format nFormat, Version nVersion, 
                               Size cbBufferSize)
{
	columns;

	if (Consts::MIN_COLUMNS_COUNT > cColumnsCount)
	{
		return Status(Status_InvalidArg, "columns count too small");
	}
	if (Consts::MAX_COLUMNS_COUNT < cColumnsCount)
	{
		return Status(Status_InvalidArg, "columns count too big");
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

Status Writer::CreateHeader(const Column *columns, Size cColumnsCount, Format nFormat, Version nVersion)
{
	m_sHeader.clear();
	switch (nVersion)
	{
		case Version_1_0: m_sHeader.append((const Char *)Header::MAGIC_V_1_0, sizeof(Header::MAGIC_V_1_0)); break;
		case Version_2_0: m_sHeader.append((const Char *)Header::MAGIC_V_2_0, sizeof(Header::MAGIC_V_2_0)); break;
		case Version_3_0: m_sHeader.append((const Char *)Header::MAGIC_V_3_0, sizeof(Header::MAGIC_V_3_0)); break;
		default:          return Status_InvalidArg;
	}
	if (Version_1_0 == nVersion)
	{
		UInt16   cbLen = 0;

		m_sHeader.append((const Char *)&cbLen, sizeof(cbLen));
	}
	else
	{
		UInt32   cbLen = 0;

		m_sHeader.append((const Char *)&cbLen, sizeof(cbLen));
	}

	m_sHeader += "{'descr': [";
	for (Size i = 0; i < cColumnsCount; i++)
	{
		Print(&m_sHeader, "('{1}', '", columns[i].sName.c_str());
		if (Type_Int8 == columns[i].nType || Type_UInt8 == columns[i].nType)
		{
			m_sHeader += '|';
		}
		else
		{
			switch (columns[i].nByteOrder)
			{
				case ByteOrder_LittleEndian: m_sHeader += "<"; break;
				case ByteOrder_BigEndian:    m_sHeader += ">"; break;
				default:                     return Status_InvalidArg;
			}
		}
		switch (columns[i].nType)
		{
			case Type_Int8:   m_sHeader += "i1"; break;
			case Type_UInt8:  m_sHeader += "u1"; break;
			case Type_Int16:  m_sHeader += "i2"; break;
			case Type_UInt16: m_sHeader += "u2"; break;
			case Type_Int32:  m_sHeader += "i4"; break;
			case Type_UInt32: m_sHeader += "u4"; break;
			case Type_Int64:  m_sHeader += "i8"; break;
			case Type_UInt64: m_sHeader += "u8"; break;
			case Type_Float:  m_sHeader += "f4"; break;
			case Type_Double: m_sHeader += "f8"; break;
			default:                     return Status_InvalidArg;
		}
		m_sHeader += "')";
		if (i + 1 < cColumnsCount)
		{
			m_sHeader += ", ";
		}
	}
	m_sHeader += "], 'fortran_order': ";
	switch (nFormat)
	{
		case Format_C:       m_sHeader += "False"; break;
		case Format_Fortran: m_sHeader += "True"; break;
		default:             return Status_InvalidArg;
	}
	m_sHeader += ", 'shape': (";

	String   sEnding;

	Print(&sEnding, ",), }\n");

	m_cbHeaderSize     = sizeof(Header::MAGIC) + sizeof(UInt16);
	m_cbHeaderDictSize = m_sHeader.size() - sizeof(Header::MAGIC) - sizeof(UInt16);
	if (Version_1_0 == nVersion)
	{
		m_cbHeaderSize += sizeof(UInt16);
		m_cbHeaderDictSize -= sizeof(UInt16);
	}
	else
	{
		m_cbHeaderSize += sizeof(UInt32);
		m_cbHeaderDictSize -= sizeof(UInt32);
	}
	m_cbHeaderDictSize += MAX_ROWS_DIGITS; // # of rows and eventual padding
	m_cbHeaderDictSize += sEnding.size();
	m_cbHeaderSize += m_cbHeaderDictSize;

	Size cbLeftOver = m_cbHeaderSize % 64;

	if (0 < cbLeftOver)
	{
		m_cbHeaderSize += 64 - cbLeftOver;
		m_cbHeaderDictSize += 64 - cbLeftOver;
	}

	if (Version_1_0 == nVersion)
	{
		UInt16   *pcLen = (UInt16 *)(((Byte *)&m_sHeader[0]) + sizeof(Header::MAGIC) + sizeof(UInt16));

		*pcLen = (UInt16)m_cbHeaderDictSize;
	}
	else
	{
		UInt32   *pcLen = (UInt32 *)(((Byte *)&m_sHeader[0]) + sizeof(Header::MAGIC) + sizeof(UInt16));

		*pcLen = (UInt32)m_cbHeaderDictSize;
	}



	return Status();
}

Status Writer::Create(const Column *columns, Size cColumnsCount, Format nFormat, Version nVersion, 
                      Size cbBufferSize)
{
	Size     cbRowSize = 0;
	Size     cbHeaderSize;
	Size     cbChunkSize;
	DWORD    dwAckSize;
	Status   status;

	if (!(status = CreateHeader(columns, cColumnsCount, nFormat, nVersion)))
	{
		return status;
	}
	if (NULL == (m_pBuffer = (Byte *)Mem::Alloc(cbBufferSize)))
	{
		return Status_MemAllocFailed;
	}
	m_cbBufferSize = cbBufferSize;
	memset(m_pBuffer, 0, m_cbBufferSize);

	cbHeaderSize = m_cbHeaderSize;
	while (0 < cbHeaderSize)
	{
		if (cbHeaderSize > m_cbBufferSize)
		{
			cbChunkSize = m_cbBufferSize;
		}
		else
		{
			cbChunkSize = cbHeaderSize;
		}
		if (!::WriteFile(m_pFile, m_pBuffer, (DWORD)cbChunkSize, &dwAckSize, NULL))
		{
			return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
		}
		if (dwAckSize != (DWORD)cbChunkSize)
		{
			return Status(Status_WriteFailed, "WriteFile failed");
		}
		cbHeaderSize -= cbChunkSize;
	}

	m_cbRowSize = 0;
	for (Size i = 0; i < cColumnsCount; i++)
	{
		if (0 == (cbRowSize = TypeSize::Get(columns[i].nType)))
		{
			return Status(Status_InvalidArg, "Invalid column type for column index {1}", i);
		}
		if (ByteOrder_Invalid >= columns[i].nByteOrder || ByteOrder_BigEndian < columns[i].nByteOrder)
		{
			return Status(Status_InvalidArg, "Invalid column byte order for column index {1}", i);
		}
		m_vectorColumns.push_back(columns[i]);
		m_cbRowSize += cbRowSize;
	}
	m_nFormat  = nFormat;
	m_nVersion = nVersion;

	return 0;
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

Status Writer::Create(const Char *szPath, const Column *columns, Size cColumnsCount, Format nFormat/* = Format_C*/, 
                      Version nVersion/* = Version_1_0*/, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(columns, cColumnsCount, nFormat, nVersion, cbBufferSize)))
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

	if (!(status = Create(columns, cColumnsCount, nFormat, nVersion, cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status Writer::Create(const WChar *wszPath, const Column *columns, Size cColumnsCount, Format nFormat/* = Format_C*/, 
                      Version nVersion/* = Version_1_0*/, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(columns, cColumnsCount, nFormat, nVersion, cbBufferSize)))
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

	if (!(status = Create(columns, cColumnsCount, nFormat, nVersion, cbBufferSize)))
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
			if (0 != (m_cbWritten % m_cbRowSize))
			{
				status = Status(Status_InvalidArg, "The last row is incomplete");
			}
		}
		if (status)
		{
			cRows = m_cbWritten / m_cbRowSize;
			if (Consts::MAX_ROWS_COUNT < cRows)
			{
				status = Status(Status_InvalidArg, "Too many rows");
			}
		}
		if (status)
		{
			LARGE_INTEGER   liPos = { 0 };

			if (::SetFilePointerEx(m_pFile, liPos, NULL, FILE_BEGIN))
			{
				String   sRows;

				Print(&sRows, "{1}", cRows);

				Print(&m_sHeader, "{1},), ", sRows);
				m_sHeader += "}";
				while (m_sHeader.size() + 1 < m_cbHeaderSize)
				{
					m_sHeader += ' ';
				}
				m_sHeader += "\n";

				if (::WriteFile(m_pFile, m_sHeader.c_str(), (DWORD)m_sHeader.size(), &dwAckSize, NULL))
				{
					if (dwAckSize != (DWORD)m_sHeader.size())
					{
						status = Status(Status_WriteFailed, "WriteFile failed");
					}
				}
				else
				{
					status = Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
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
	m_sHeader.clear();
	m_sPath.clear();
	m_wsPath.clear();
	m_vectorColumns.clear();
	m_cbHeaderSize      = 0;
	m_cbHeaderDictSize  = 0;
	m_nFormat           = Format_Invalid;
	m_nVersion          = Version_Invalid;
	m_cbRowSize         = 0;
	m_cbBufferSize      = 0;
	m_cbBufferUsedSize  = 0;
	m_cbReceived        = 0;
	m_cbWritten         = 0;

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

	return m_nFormat;
}

const Size Writer::GetColumnsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_vectorColumns.size();
}

const Column *Writer::GetColumns() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return &m_vectorColumns[0];
}

Size Writer::GetRowSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbRowSize;
}

Version Writer::GetVersion() const
{
	if (NULL == m_pFile)
	{
		return Version_Invalid;
	}

	return m_nVersion;
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

