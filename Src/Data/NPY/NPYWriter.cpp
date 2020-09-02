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
#include "CX/Data/NPY/NPZWriter.hpp"
#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "zip.h"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

NPZWriter::NPZWriter()
{
	m_header.Reset();
	m_pZIP             = NULL;
	m_pBuffer          = NULL;
	m_cbBufferSize     = 0;
	m_cbBufferUsedSize = 0;
	m_cbReceived       = 0;
	m_cbWritten        = 0;
}

NPZWriter::~NPZWriter()
{
	Close();
}

Status NPZWriter::CheckCreateArgs(Size cRowsCount, Size cColumnsCount, Type nType, ByteOrder nByteOrder, 
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

Status NPZWriter::Create(const Char *szName, int nCompression, Size cRowsCount, Size cColumnsCount, Type nType, 
                         ByteOrder nByteOrder, Format nFormat, Version nVersion, Size cbBufferSize)
{
	String   sHeader;
	int      nRet;
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

	if (ZIP_OK != (nRet = zipOpenNewFileInZip64(m_pZIP, szName, NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, nCompression, 
	                                    ((UInt64)m_header.cbRowSize * (UInt64)cRowsCount > (UInt64)0xFFFFFFFF) ? 1 : 0)))
	{
		return Status(Status_OperationFailed, "Failed to open new zip in file : {1}", nRet);
	}
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

Status NPZWriter::WriteInternal(const void *pBuffer, Size cbSize)
{
	int      nRet;
	Status   status;

	if (ZIP_OK != (nRet = zipWriteInFileInZip(m_pZIP, pBuffer, cbSize)))
	{
		return Status(Status_WriteFailed, "Write failed with error {1}", nRet);
	}

	return Status();
}

Status NPZWriter::Flush()
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


Status NPZWriter::Create(const Char *szPath, const Char *szName, int nCompression, Size cRowsCount, Size cColumnsCount, 
                         Type nType, ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, Format nFormat/* = Format_C*/, 
                         Version nVersion/* = Version_1_0*/, Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	Status   status;

	if (!(status = CheckCreateArgs(cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion, cbBufferSize)))
	{
		return status;
	}

	Close();

	_unlink(szPath);
	if (NULL == (m_pZIP = zipOpen64(szPath, 0)))
	{
		return Status(Status_CreateFailed, "Failed to create zip '{1}'", szPath);
	}
	if (!(status = Create(szName, nCompression, cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, nVersion, 
	                      cbBufferSize)))
	{
		Close();

		return status;
	}
	m_sPath = szPath;

	return Status();
}

Status NPZWriter::Create(const WChar *wszPath, const Char *szName, int nCompression, Size cRowsCount, 
                         Size cColumnsCount, Type nType, ByteOrder nByteOrder/* = ByteOrder_LittleEndian*/, 
                         Format nFormat/* = Format_C*/, Version nVersion/* = Version_1_0*/, 
                         Size cbBufferSize/* = DEFAULT_BUFFER_SIZE*/)
{
	String   sPath;
	Status   status;

	_wunlink(wszPath);
	if (!(status = Str::UTF8::FromWChar(wszPath, &sPath)))
	{
		return status;
	}
	if (!(status = Create(sPath.c_str(), szName, nCompression, cRowsCount, cColumnsCount, nType, nByteOrder, nFormat, 
	                      nVersion, cbBufferSize)))
	{
		return status;
	}
	m_sPath.clear();
	m_wsPath = wszPath;

	return Status();
}

Status NPZWriter::Close()
{


	DWORD    dwAckSize;
	UInt64   cRows = 0;
	Status   status;

	if (NULL != m_pZIP)
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
	if (NULL != m_pZIP)
	{
		zipCloseFileInZip(m_pZIP);
		zipClose(m_pZIP, NULL);
		m_pZIP = NULL;
	}
	m_header.Reset();
	m_cbBufferSize       = 0;
	m_cbBufferUsedSize   = 0;
	m_cbReceived         = 0;
	m_cbWritten          = 0;

	return status;
}

Status NPZWriter::Write(const void *pBuffer, Size cbSize)
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

Bool NPZWriter::IsOK() const
{
	return (NULL != m_pZIP);
}

const Char *NPZWriter::GetPathA() const
{
	return m_sPath.c_str();
}

const WChar *NPZWriter::GetPathW() const
{
	return m_wsPath.c_str();
}

Format NPZWriter::GetFormat() const
{
	if (NULL == m_pZIP)
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Size NPZWriter::GetRowsCount() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_header.cRows;
}

Size NPZWriter::GetColumnsCount() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_header.cColumns;
}

Type NPZWriter::GetType() const
{
	if (NULL == m_pZIP)
	{
		return Type_Invalid;
	}

	return m_header.nType;
}

ByteOrder NPZWriter::GetByteOrder() const
{
	if (NULL == m_pZIP)
	{
		return ByteOrder_Invalid;
	}

	return m_header.nByteOrder;
}

Size NPZWriter::GetRowSize() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Version NPZWriter::GetVersion() const
{
	if (NULL == m_pZIP)
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

Size NPZWriter::GetBufferSize() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_cbBufferSize;
}

UInt64 NPZWriter::GetReceivedBytes() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_cbReceived;
}

UInt64 NPZWriter::GetWrittenBytes() const
{
	if (NULL == m_pZIP)
	{
		return 0;
	}

	return m_cbWritten;
}

}//namespace NPY

}//namespace Data

}//namespace CX

