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
#include "CX/Data/NPY/StreamWriter.hpp"
#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

StreamWriter::StreamWriter()
{
	m_header.Reset();
	m_pOutputStream      = NULL;
	m_bTakeOwnership     = False;
	m_pBuffer            = NULL;
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;
}

StreamWriter::~StreamWriter()
{
	Close();
}

Status StreamWriter::CheckCreateArgs(Size cRowsCount, Size cColumnsCount, Type nType, ByteOrder nByteOrder, 
                                     Format nFormat, Version nVersion, Size cbBufferSize)
{
	if (0 == cRowsCount)
	{
		return Status(Status_InvalidArg, "rows count too small");
	}
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

Status StreamWriter::Create(Size cRowsCount, Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, 
                            Version nVersion, Size cbBufferSize)
{
	String   sHeader;
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
	if (!(status = m_header.Write(&sHeader, 0)))
	{
		return status;
	}
	if (!(status = WriteInternal(sHeader.c_str(), sHeader.size())))
	{
		return status;
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

Status StreamWriter::WriteInternal(const void *pBuffer, Size cbSize)
{
	Size     cbAckSize;
	Status   status;

	if (!(status = m_pOutputStream->Write(pBuffer, cbSize, &cbAckSize)))
	{
		return Status(Status_WriteFailed, "Write failed with error {1} ({2})", status.GetCode(), status.GetMsg());
	}
	if (cbSize != cbAckSize)
	{
		return Status(Status_WriteFailed, "Write failed");
	}

	return Status();
}

Status StreamWriter::Flush()
{
	Status   status;

	if (!(status = WriteInternal(m_pBuffer, m_cbBufferUsedSize)))
	{
		return status;
	}
	m_cbWritten += m_cbBufferUsedSize;
	m_cbBufferUsedSize = 0;

	return Status();
}

Status StreamWriter::Create(IO::IOutputStream *pOutputStream, Bool bTakeOwnership, Size cRowsCount, Size cColumnsCount, 
                            Type nType, ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, 
                            Format nFormat/* = Format_C*/, Version nVersion/* = Version_1_0*/, 
                            Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
	{
		return status;
	}

	Close();

	m_pOutputStream  = pOutputStream;
	m_bTakeOwnership = bTakeOwnership;
	if (!(status = Create(cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
	{
		Close();

		return status;
	}

	return Status();
}

Status StreamWriter::Close()
{
	DWORD    dwAckSize;
	UInt64   cRows = 0;
	Status   status;

	if (NULL != m_pOutputStream)
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
			if (m_header.cRows != cRows)
			{
				status = Status(Status_InvalidArg, "Invalid number of rows written ({1}, expected {2})", cRows, 
				                m_header.cRows);
			}
			if (Consts::MAX_ROWS_COUNT < cRows)
			{
				status = Status(Status_InvalidArg, "Too many rows");
			}
		}
	}
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
		m_pBuffer = NULL;
	}
	if (NULL != m_pOutputStream)
	{
		if (m_bTakeOwnership)
		{
			delete m_pOutputStream;
		}
		m_pOutputStream = NULL;
	}
	m_bTakeOwnership = False;
	m_header.Reset();
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;

	return status;
}

Status StreamWriter::Write(const void *pBuffer, Size cbSize)
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

Bool StreamWriter::IsOK() const
{
	return (NULL != m_pOutputStream);
}

Format StreamWriter::GetFormat() const
{
	if (NULL == m_pOutputStream)
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Size StreamWriter::GetRowsCount() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_header.cRows;
}

Size StreamWriter::GetColumnsCount() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_header.cColumns;
}

Type StreamWriter::GetType() const
{
	if (NULL == m_pOutputStream)
	{
		return Type_Invalid;
	}

	return m_header.nType;
}

ByteOrder StreamWriter::GetByteOrder() const
{
	if (NULL == m_pOutputStream)
	{
		return ByteOrder_Invalid;
	}

	return m_header.nByteOrder;
}

Size StreamWriter::GetRowSize() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Version StreamWriter::GetVersion() const
{
	if (NULL == m_pOutputStream)
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

Size StreamWriter::GetBufferSize() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_cbBufferSize;
}

UInt64 StreamWriter::GetReceivedBytes() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_cbReceived;
}

UInt64 StreamWriter::GetWrittenBytes() const
{
	if (NULL == m_pOutputStream)
	{
		return 0;
	}

	return m_cbWritten;
}

}//namespace NPY

}//namespace Data

}//namespace CX

