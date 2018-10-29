/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/DB/Flat/Writer.hpp"
#include "CX/C/stdio.h"


namespace CX
{

namespace DB
{

namespace Flat
{

Writer::Writer()
{
	m_hFile        = NULL;
	m_nMagic       = 0;
	m_cbHeaderSize = 0;
	m_cbRecordSize = 0;
	m_cRecordCount = 0;
	m_cbBufferSize = 0;
}

Writer::~Writer()
{
	Close();
}

Status Writer::Write(const void *pData, Size cbSize)
{
	DWORD   dwSize;

	if (!WriteFile(m_hFile, pData, (DWORD)cbSize, &dwSize, NULL))
	{
		return Status(Status_WriteFailed, "Failed to write file");
	}
	if ((Size)dwSize != cbSize)
	{
		return Status(Status_WriteFailed, "Failed to write {1} bytes", cbSize);
	}

	return Status();
}

Status Writer::Read(void *pData, Size cbSize)
{
	DWORD   dwSize;

	if (!ReadFile(m_hFile, pData, (DWORD)cbSize, &dwSize, NULL))
	{
		return Status(Status_ReadFailed, "Failed to read file");
	}
	if ((Size)dwSize != cbSize)
	{
		return Status(Status_ReadFailed, "Failed to read {1} bytes", cbSize);
	}

	return Status();
}

Status Writer::CreateInternal(UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
                              Size cbBufferSize/* = BUFFER_SIZE*/)
{
	Status   status;

	m_nMagic       = nMagic;
	m_cbHeaderSize = cbHeaderSize;
	m_cbRecordSize = cbRecordSize;
	m_cRecordCount = 0;
	m_cbBufferSize = 0;

	if (MIN_BUFFER_SIZE > cbBufferSize)
	{
		cbBufferSize = MIN_BUFFER_SIZE;
	}
	if (MAX_BUFFER_SIZE < cbBufferSize)
	{
		cbBufferSize = MAX_BUFFER_SIZE;
	}
	if (!(status = m_buffer.SetSize(cbBufferSize)))
	{
		return Status(Status_OpenFailed, "Failed to allocate {1} bytes", cbBufferSize);
	}
	memset(m_buffer.GetMem(), 0, m_buffer.GetSize());
	if (!(status = Write(m_nMagic)))
	{
		return status;
	}
	if (!(status = Write(m_cbHeaderSize)))
	{
		return status;
	}
	if (!(status = Write(m_cbRecordSize)))
	{
		return status;
	}
	if (!(status = Write(m_cRecordCount)))
	{
		return status;
	}
	while (0 < cbHeaderSize)
	{
		if (cbHeaderSize > m_buffer.GetSize())
		{
			if (!(status = Write(m_buffer.GetMem(), m_buffer.GetSize())))
			{
				return status;
			}
			cbHeaderSize -= (UInt32)m_buffer.GetSize();
		}
		else
		{
			if (!(status = Write(m_buffer.GetMem(), cbHeaderSize)))
			{
				return status;
			}
			cbHeaderSize = 0;
		}
	}

	return Status();
}

Status Writer::OpenInternal(Size cbBufferSize/* = BUFFER_SIZE*/)
{
	Status   status;

	if (MIN_BUFFER_SIZE > cbBufferSize)
	{
		cbBufferSize = MIN_BUFFER_SIZE;
	}
	if (MAX_BUFFER_SIZE < cbBufferSize)
	{
		cbBufferSize = MAX_BUFFER_SIZE;
	}
	if (!(status = m_buffer.SetSize(cbBufferSize)))
	{
		return Status(Status_OpenFailed, "Failed to allocate {1} bytes", cbBufferSize);
	}
	if (!(status = Read(m_nMagic)))
	{
		return status;
	}
	if (!(status = Read(m_cbHeaderSize)))
	{
		return status;
	}
	if (!(status = Read(m_cbRecordSize)))
	{
		return status;
	}
	if (!(status = Read(m_cRecordCount)))
	{
		return status;
	}

	LARGE_INTEGER liPos;

	liPos.QuadPart = 0;
	if (!SetFilePointerEx(m_hFile, liPos, NULL, FILE_END))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	return Status();
}

//0 == cbRecordSize => variable size for records
Status Writer::Create(const Char *szPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
                      Size cbBufferSize/* = BUFFER_SIZE*/)
{
	if (NULL != m_hFile)
	{
		return Status_Busy;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileA(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileA('{1}') failed with error {2}", szPath, GetLastError());
	}

	Status   status;

	if (!(status = CreateInternal(nMagic, cbHeaderSize, cbRecordSize, cbBufferSize)))
	{
		CloseHandle(m_hFile);
		m_hFile        = NULL;
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
		m_cbBufferSize = 0;
		unlink(szPath);
	}

	return Status();
}

//0 == cbRecordSize => variable size for records
Status Writer::Create(const WChar *wszPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
                      Size cbBufferSize/* = BUFFER_SIZE*/)
{
	if (NULL != m_hFile)
	{
		return Status_Busy;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileW(wszPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileW('{1}') failed with error {2}", wszPath, GetLastError());
	}

	Status   status;

	if (!(status = CreateInternal(nMagic, cbHeaderSize, cbRecordSize, cbBufferSize)))
	{
		CloseHandle(m_hFile);
		m_hFile        = NULL;
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
		m_cbBufferSize = 0;
		_wunlink(wszPath);
	}

	return Status();
}

Status Writer::Open(const Char *szPath, Size cbBufferSize/* = BUFFER_SIZE*/)
{
	if (NULL != m_hFile)
	{
		return Status_Busy;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileA(szPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileA('{1}') failed with error {2}", szPath, GetLastError());
	}

	Status   status;

	if (!(status = OpenInternal(cbBufferSize)))
	{
		CloseHandle(m_hFile);
		m_hFile        = NULL;
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
		m_cbBufferSize = 0;
	}

	return Status();
}

Status Writer::Open(const WChar *wszPath, Size cbBufferSize/* = BUFFER_SIZE*/)
{
	if (NULL != m_hFile)
	{
		return Status_Busy;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileW(wszPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status(Status_OpenFailed, "CreateFileW('{1}') failed with error {2}", wszPath, GetLastError());
	}

	Status   status;

	if (!(status = OpenInternal(cbBufferSize)))
	{
		CloseHandle(m_hFile);
		m_hFile        = NULL;
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
		m_cbBufferSize = 0;
	}

	return Status();
}

//0 == cbRecordSize => variable size for records
Status Writer::CreateOrOpen(const Char *szPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
                            Size cbBufferSize/* = BUFFER_SIZE*/)
{
	Status   status;

	if ((status = Open(szPath, cbBufferSize)))
	{
		if (GetMagic() != nMagic)
		{
			Close();

			return Status_InvalidArg;
		}
		if (GetHeaderSize() != cbHeaderSize)
		{
			Close();

			return Status_InvalidArg;
		}
		if (GetRecordSize() != cbRecordSize)
		{
			Close();

			return Status_InvalidArg;
		}
	}
	else
	{
		if ((status = Create(szPath, nMagic, cbHeaderSize, cbRecordSize, cbBufferSize)))
		{
			return status;
		}
	}

	return Status();
}

//0 == cbRecordSize => variable size for records
Status Writer::CreateOrOpen(const WChar *wszPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
                            Size cbBufferSize/* = BUFFER_SIZE*/)
{
	Status   status;

	if ((status = Open(wszPath, cbBufferSize)))
	{
		if (GetMagic() != nMagic)
		{
			Close();

			return Status_InvalidArg;
		}
		if (GetHeaderSize() != cbHeaderSize)
		{
			Close();

			return Status_InvalidArg;
		}
		if (GetRecordSize() != cbRecordSize)
		{
			Close();

			return Status_InvalidArg;
		}
	}
	else
	{
		if ((status = Create(wszPath, nMagic, cbHeaderSize, cbRecordSize, cbBufferSize)))
		{
			return status;
		}
	}

	return Status();
}

Status Writer::Close(const void *pCustomHeader/* = NULL*/, UInt32 cbCustomHeaderSize/* = 0*/)
{
	Status   status;

	if (NULL == m_hFile)
	{
		return Status_InvalidCall;
	}
	if (m_cbHeaderSize != cbCustomHeaderSize && 0 != cbCustomHeaderSize)
	{
		return Status_InvalidArg;
	}

	if (!(status = Flush()))
	{
		return Status();
	}

	LARGE_INTEGER liPos;

	liPos.QuadPart = sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32);
	if (!SetFilePointerEx(m_hFile, liPos, NULL, FILE_BEGIN))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	if (!(status = Write(m_cRecordCount)))
	{
		return status;
	}
	if (0 < cbCustomHeaderSize)
	{
		if (!(status = Write(pCustomHeader, cbCustomHeaderSize)))
		{
			return status;
		}
	}
	CloseHandle(m_hFile);
	m_hFile        = NULL;
	m_nMagic       = 0;
	m_cbHeaderSize = 0;
	m_cbRecordSize = 0;
	m_cRecordCount = 0;
	m_cbBufferSize = 0;

	return Status();
}

Status Writer::Flush()
{
	Status   status;

	if (NULL == m_hFile)
	{
		return Status_InvalidCall;
	}
	if (0 < m_cbBufferSize)
	{
		if (!(status = Write(m_buffer.GetMem(), m_cbBufferSize)))
		{
			return status;
		}
		m_cbBufferSize = 0;
	}

	return Status();
}

Bool Writer::IsOK() const
{
	return (NULL != m_hFile);
}

UInt32 Writer::GetMagic() const
{
	return m_nMagic;
}

UInt32 Writer::GetHeaderSize() const
{
	return m_cbHeaderSize;
}

UInt32 Writer::GetRecordSize() const
{
	return m_cbRecordSize;
}

UInt32 Writer::GetRecordCount() const
{
	return m_cRecordCount;
}

Size Writer::GetBufferSize() const
{
	return m_buffer.GetSize();
}

Status Writer::GetHeader(void *pHeader, UInt32 cbSize)
{
	if (NULL == m_hFile)
	{
		return Status_InvalidCall;
	}
	if (cbSize != m_cbHeaderSize)
	{
		return Status_InvalidArg;
	}
	if (0 == m_cbHeaderSize)
	{
		return Status_InvalidCall;
	}

	LARGE_INTEGER liPos;
	LARGE_INTEGER liCrPos;

	liPos.QuadPart = 0;
	if (!SetFilePointerEx(m_hFile, liPos, &liCrPos, FILE_CURRENT))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	liPos.QuadPart = sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32);
	if (!SetFilePointerEx(m_hFile, liPos, NULL, FILE_BEGIN))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	Status   status;

	if (!(status = Read(pHeader, cbSize)))
	{
		SetFilePointerEx(m_hFile, liCrPos, NULL, FILE_BEGIN);

		return status;
	}

	if (!SetFilePointerEx(m_hFile, liCrPos, NULL, FILE_BEGIN))
	{
		return Status(Status_OperationFailed, "SetFilePointerEx failed with error {1}", GetLastError());
	}

	return Status();
}

Status Writer::AddRecord(const void *pData, UInt32 cbSize)
{
	if (NULL == m_hFile)
	{
		return Status_InvalidCall;
	}
	if (0 == cbSize)
	{
		return Status_InvalidArg;
	}
	if (0 != m_cbRecordSize && m_cbRecordSize != cbSize)
	{
		return Status_InvalidArg;
	}

	if (0 == m_cbRecordSize)
	{
		Status   status;
		
		if (!(status = BufferedWrite(&cbSize, sizeof(UInt32))))
		{
			return status;
		}
	}
	m_cRecordCount++;

	return BufferedWrite(pData, (Size)cbSize);
}

Status Writer::BufferedWrite(const void *pData, Size cbSize)
{
	const Byte   *pPos = (const Byte *)pData;
	Size         cbTmpSize;
	Status       status;

	while (0 < cbSize)
	{
		if (cbSize > m_buffer.GetSize() || m_cbBufferSize > m_buffer.GetSize() - cbSize)
		{
			cbTmpSize = m_buffer.GetSize() - m_cbBufferSize;
		}
		else
		{
			cbTmpSize = cbSize;
		}
		memcpy((Byte *)m_buffer.GetMem() + m_cbBufferSize, pPos, cbTmpSize);
		if (m_buffer.GetSize() == m_cbBufferSize)
		{
			if (!(status = Write(m_buffer.GetMem(), m_buffer.GetSize())))
			{
				return status;
			}
			m_cbBufferSize = 0;
		}
		else
		{
			m_cbBufferSize += cbTmpSize;
		}
		pPos += cbTmpSize;
		cbSize -= cbTmpSize;
	}

	return Status();
}

}//namespace Flat

}//namespace DB

}//namespace CX


#endif
