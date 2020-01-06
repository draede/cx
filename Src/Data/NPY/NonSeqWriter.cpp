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
	m_pFile             = NULL;
	m_nFormat           = Format_Invalid;
	m_nVersion          = Version_Invalid;
	m_cRowsCount        = 0;
	m_cbRowSize         = 0;
}

NonSeqWriter::~NonSeqWriter()
{
	Close();
}

Status NonSeqWriter::CheckCreateArgs(const Column *columns, Size cColumnsCount, Format nFormat, Version nVersion, 
                                     Bool *pbSame)
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

	*pbSame = True;
	for (Size i = 1; i < cColumnsCount; i++)
	{
		if (!columns[i].sName.empty())
		{
			*pbSame = False;

			break;
		}
		if (columns[i].nByteOrder != columns[0].nByteOrder)
		{
			*pbSame = False;

			break;
		}
		if (columns[i].nType != columns[0].nType)
		{
			*pbSame = False;

			break;
		}
	}

	return Status();
}

Status NonSeqWriter::CreateHeader(Size cRowsCount, const Column *columns, Size cColumnsCount, Bool bSameColumn, 
                                  Format nFormat, Version nVersion)
{
	Size   cbPrefixSize;
	Size   cbLenIndex;

	m_sHeader.clear();
	switch (nVersion)
	{
		case Version_1_0: 
		{
			m_sHeader.append((const Char *)Header::MAGIC_V_1_0, sizeof(Header::MAGIC_V_1_0)); 
		}
		break;
		case Version_2_0: 
		{
			m_sHeader.append((const Char *)Header::MAGIC_V_2_0, sizeof(Header::MAGIC_V_2_0)); 
		}
		break;
		case Version_3_0: 
		{
			m_sHeader.append((const Char *)Header::MAGIC_V_3_0, sizeof(Header::MAGIC_V_3_0)); 
		}
		break;
		default: return Status_InvalidArg;
	}
	cbLenIndex = m_sHeader.size();
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

	cbPrefixSize = m_sHeader.size();

	if (bSameColumn)
	{
		m_sHeader += "{'descr': '";
		if (Type_Int8 == columns[0].nType || Type_UInt8 == columns[0].nType)
		{
			m_sHeader += '|';
		}
		else
		{
			switch (columns[0].nByteOrder)
			{
				case ByteOrder_LittleEndian: m_sHeader += "<"; break;
				case ByteOrder_BigEndian:    m_sHeader += ">"; break;
				default:                     return Status_InvalidArg;
			}
		}
		switch (columns[0].nType)
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
		m_sHeader += "'";
	}
	else
	{
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
		m_sHeader += "]";
	}
	m_sHeader += ", 'fortran_order': ";
	switch (nFormat)
	{
		case Format_C:       m_sHeader += "False"; break;
		case Format_Fortran: m_sHeader += "True"; break;
		default:             return Status_InvalidArg;
	}
	Print(&m_sHeader, ", 'shape': ({1}, {2}, ), }", cRowsCount, cColumnsCount);

	while (63!= (m_sHeader.size() % 64))
	{
		m_sHeader += ' ';
	}
	m_sHeader += '\n';

	if (Version_1_0 == nVersion)
	{
		UInt16   *pcbLen = (UInt16 *)(m_sHeader.c_str() + cbLenIndex);

		*pcbLen = (UInt16)(m_sHeader.size() - cbPrefixSize);
	}
	else
	{
		UInt32   *pcbLen = (UInt32 *)(m_sHeader.c_str() + cbLenIndex);

		*pcbLen = (UInt32)(m_sHeader.size() - cbPrefixSize);
	}

	return Status();
}

Status NonSeqWriter::Create(Size cRowsCount, const Column *columns, Size cColumnsCount, Bool bSameColumn, 
                            Format nFormat, Version nVersion)
{
	void     *pBuffer;
	Size     cbBufferSize;
	Size     cbRowSize = 0;
	UInt64   cbDataSize;
	DWORD    dwReqSize;
	DWORD    dwAckSize;
	Status   status;

	if (!(status = CreateHeader(cRowsCount, columns, cColumnsCount, bSameColumn, nFormat, nVersion)))
	{
		return status;
	}

	if (!::WriteFile(m_pFile, m_sHeader.c_str(), (DWORD)m_sHeader.size(), &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)m_sHeader.size())
	{
		return Status(Status_WriteFailed, "WriteFile failed");
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
	m_nFormat    = nFormat;
	m_nVersion   = nVersion;
	m_cRowsCount = cRowsCount;

	cbDataSize   = m_cbRowSize * cRowsCount;
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

Status NonSeqWriter::Create(const Char *szPath, Size cRowsCount, const Column *columns, Size cColumnsCount, 
                            Format nFormat/* = Format_C*/, Version nVersion/* = Version_1_0*/)
{
	Bool     bSame;
	Status   status;

	if (!(status = CheckCreateArgs(columns, cColumnsCount, nFormat, nVersion, &bSame)))
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

	if (!(status = Create(cRowsCount, columns, cColumnsCount, bSame, nFormat, nVersion)))
	{
		Close();

		return status;
	}

	return Status();
}

Status NonSeqWriter::Create(const WChar *wszPath, Size cRowsCount, const Column *columns, Size cColumnsCount, 
                            Format nFormat/* = Format_C*/, Version nVersion/* = Version_1_0*/)
{
	Bool     bSame;
	Status   status;

	if (!(status = CheckCreateArgs(columns, cColumnsCount, nFormat, nVersion, &bSame)))
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

	if (!(status = Create(cRowsCount, columns, cColumnsCount, bSame, nFormat, nVersion)))
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
	m_sHeader.clear();
	m_sPath.clear();
	m_wsPath.clear();
	m_vectorColumns.clear();
	m_nFormat           = Format_Invalid;
	m_nVersion          = Version_Invalid;
	m_cbRowSize         = 0;
	m_cRowsCount        = 0;

	return status;
}

Status NonSeqWriter::Write(const void *pRows, Size cRowsCount, Size cRowsIndex)
{
	DWORD   dwAckSize;

	if (NULL == m_pFile)
	{
		return Status_NotInitialized;
	}
	if (cRowsIndex >= m_cRowsCount)
	{
		return Status_InvalidArg;
	}
	if (cRowsIndex + cRowsCount > m_cRowsCount)
	{
		return Status_InvalidArg;
	}

	LARGE_INTEGER   liPos;

	liPos.QuadPart = m_sHeader.size() + cRowsIndex * m_cbRowSize;

	if (!::SetFilePointerEx(m_pFile, liPos, NULL, FILE_BEGIN))
	{
		return Status(Status_InvalidArg, "SetFilePointerEx failed with error {1}",::GetLastError());
	}

	if (!::WriteFile(m_pFile, pRows, (DWORD)(m_cbRowSize * cRowsCount), &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)(m_cbRowSize * cRowsCount))
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

	return m_nFormat;
}

Size NonSeqWriter::GetRowsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cRowsCount;
}

Size NonSeqWriter::GetColumnsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_vectorColumns.size();
}

const Column *NonSeqWriter::GetColumns() const
{
	if (NULL == m_pFile)
	{
		return NULL;
	}

	return &m_vectorColumns[0];
}

Size NonSeqWriter::GetRowSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbRowSize;
}

Version NonSeqWriter::GetVersion() const
{
	if (NULL == m_pFile)
	{
		return Version_Invalid;
	}

	return m_nVersion;
}

}//namespace NPY

}//namespace Data

}//namespace CX

