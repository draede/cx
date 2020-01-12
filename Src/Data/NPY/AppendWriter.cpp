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
#include "CX/Data/NPY/AppendWriter.hpp"
#include "CX/Data/NPY/Reader.hpp"
#include "CX/Mem.hpp"
#include "CX/Util/MemPool.hpp"
#include "CX/Print.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

AppendWriter::AppendWriter()
{
	m_header.nFormat     = Format_Invalid;
	m_header.nVersion    = Version_Invalid;
	m_header.cRows       = 0;
	m_header.cbRowSize   = 0;
	m_header.cbUsedSize  = 0;
	m_header.cbTotalSize = 0;
	m_pBuffer            = NULL;
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;
}

AppendWriter::~AppendWriter()
{
	Close();
}

Status AppendWriter::Open(Size cbBufferSize)
{
	Util::DynMemPool   mem;
	DWORD              dwAckSize;
	LARGE_INTEGER      liSize;
	Status             status;

	if (!(status = mem.SetSize(MAX_HEADER_SIZE)))
	{
		return status;
	}
	if (!(ReadFile(m_pFile, mem.GetMem(), (DWORD)mem.GetSize(), &dwAckSize, NULL)))
	{
		return Status_ReadFailed;
	}
	if (!(status = m_header.Read(mem.GetMem(), (Size)dwAckSize)))
	{
		return status;
	}
	if (!GetFileSizeEx(m_pFile, &liSize))
	{
		return Status_GetSize;
	}
	if ((Int64)(m_header.cbTotalSize + m_header.cRows * m_header.cbRowSize) != liSize.QuadPart)
	{
		return Status(Status_InvalidArg, "Invalid file size");
	}
	
	LARGE_INTEGER   liPos = { 0 };

	if (!::SetFilePointerEx(m_pFile, liPos, NULL, FILE_END))
	{
		return Status_SetSize;
	}
	if (NULL == (m_pBuffer = (Byte *)Mem::Alloc(cbBufferSize)))
	{
		return Status_MemAllocFailed;
	}
	m_cbBufferSize = cbBufferSize;
	memset(m_pBuffer, 0, m_cbBufferSize);
	m_cbBufferUsedSize = 0;

	return Status();
}

Status AppendWriter::Flush()
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

Status AppendWriter::Open(const Char *szPath, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileA(szPath, GENERIC_WRITE | FILE_SHARE_READ, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileA failed with error {1}", ::GetLastError());
	}
	m_sPath = szPath;

	if (!(status = Open(cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status AppendWriter::Open(const WChar *wszPath, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileW(wszPath, GENERIC_WRITE | FILE_SHARE_READ, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileW failed with error {1}", ::GetLastError());

	}
	m_wsPath = wszPath;

	if (!(status = Open(cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status AppendWriter::Close()
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
				m_header.cRows += (Size)cRows;
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
	m_header.vectorColumns.clear();
	m_header.nFormat     = Format_Invalid;
	m_header.nVersion    = Version_Invalid;
	m_header.cRows       = 0;
	m_header.cbRowSize   = 0;
	m_header.cbUsedSize  = 0;
	m_header.cbTotalSize = 0;
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;

	return status;
}

Status AppendWriter::Write(const void *pBuffer, Size cbSize)
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

Bool AppendWriter::IsOK() const
{
	return (NULL != m_pFile);
}

const Char *AppendWriter::GetPathA() const
{
	if (NULL == m_pFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

const WChar *AppendWriter::GetPathW() const
{
	if (NULL == m_pFile)
	{
		return L"";
	}

	return m_wsPath.c_str();
}

Format AppendWriter::GetFormat() const
{
	if (NULL == m_pFile)
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Size AppendWriter::GetColumnsCount() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.vectorColumns.size();
}

const Column *AppendWriter::GetColumns() const
{
	if (NULL == m_pFile)
	{
		return NULL;
	}

	return &m_header.vectorColumns[0];
}

Size AppendWriter::GetRowSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Version AppendWriter::GetVersion() const
{
	if (NULL == m_pFile)
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

Size AppendWriter::GetBufferSize() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbBufferSize;
}

UInt64 AppendWriter::GetReceivedBytes() const
{
	if (NULL == m_pFile)
	{
		return 0;
	}

	return m_cbReceived;
}

UInt64 AppendWriter::GetWrittenBytes() const
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

