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
#include "CX/DB/Flat/Writer.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace DB
{

namespace Flat
{

Writer::Writer()
{
	m_pFile            = NULL;
	m_buffer           = NULL;
	m_cbBufferSize     = 0;
	m_cbUsedBufferSize = 0;
	m_cbRecordSize     = 0;
	m_cRecords         = 0;
}

Writer::~Writer()
{
	Close();
}

Status Writer::Create(const Char *szPath, UInt32 cbRecordSize, const void *pHeader/* = NULL*/, 
                      UInt32 cbHeaderSize/* = 0*/, UInt32 cbBufferSize/* = BUFFER_SIZE*/)
{
	if (0 == cbBufferSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}
	if (0 == cbRecordSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileA(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileA('{1}') failed with error {2} at {3}:{4}", szPath, GetLastError(), 
		              __FILE__, __LINE__);
	}

	return Create(cbRecordSize, pHeader, cbHeaderSize, cbBufferSize);
}

Status Writer::Create(const WChar *wszPath, UInt32 cbRecordSize, const void *pHeader/* = NULL*/, 
                      UInt32 cbHeaderSize/* = 0*/, UInt32 cbBufferSize/* = BUFFER_SIZE*/)
{
	if (0 == cbBufferSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}
	if (0 == cbRecordSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileW(wszPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_CreateFailed, "CreateFileA('{1}') failed with error {2} at {3}:{4}", wszPath, 
		              GetLastError(), __FILE__, __LINE__);
	}

	return Create(cbRecordSize, pHeader, cbHeaderSize, cbBufferSize);
}

Status Writer::Open(const Char *szPath, UInt32 *pcbRecordSize/* = NULL*/, void **ppHeader/* = NULL*/, 
                    UInt32 *pcbHeaderSize/* = NULL*/, void **ppFooter/* = NULL*/, UInt32 *pcbFooterSize/* = NULL*/, 
                    UInt32 cbBufferSize/* = BUFFER_SIZE*/)
{
	if (0 == cbBufferSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileA(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_OpenFailed, "CreateFileA('{1}') failed with error {2} at {3}:{4}", szPath, GetLastError(), 
		              __FILE__, __LINE__);
	}

	return Open(pcbRecordSize, ppHeader, pcbHeaderSize, ppFooter, pcbFooterSize, cbBufferSize);
}

Status Writer::Open(const WChar *wszPath, UInt32 *pcbRecordSize/* = NULL*/, void **ppHeader/* = NULL*/, 
                    UInt32 *pcbHeaderSize/* = NULL*/, void **ppFooter/* = NULL*/, UInt32 *pcbFooterSize/* = NULL*/, 
                    UInt32 cbBufferSize/* = BUFFER_SIZE*/)
{
	if (0 == cbBufferSize)
	{
		return Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
	}

	Close();

	if (INVALID_HANDLE_VALUE == (m_pFile = CreateFileW(wszPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, 
	                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_pFile = NULL;

		return Status(Status_OpenFailed, "CreateFileA('{1}') failed with error {2} at {3}:{4}", wszPath, GetLastError(), 
		              __FILE__, __LINE__);
	}

	return Open(pcbRecordSize, ppHeader, pcbHeaderSize, ppFooter, pcbFooterSize, cbBufferSize);
}

Status Writer::Close(const void *pFooter/* = NULL*/, UInt32 cFooterSize/* = 0*/)
{
	return Close(True, pFooter, cFooterSize);
}

Bool Writer::IsOK() const
{
	return (NULL != m_pFile);
}

UInt32 Writer::GetRecordSize() const
{
	return m_cbRecordSize;
}

UInt32 Writer::GetRecordsCount() const
{
	return m_cRecords;
}

Status Writer::Append(const void *records, UInt32 cCount/* = 1*/)
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "Not initialized at {1}:{2}", __FILE__, __LINE__);
	}

	Status   status;

	if (!(status = Write(records, m_cbRecordSize * cCount)))
	{
		return status;
	}

	m_cRecords += cCount;

	return Status();
}

Status Writer::FreeHeader(void *pHeader)
{
	Mem::Free(pHeader);

	return Status();
}

Status Writer::FreeFooter(void *pFooter)
{
	Mem::Free(pFooter);

	return Status();
}

Status Writer::Create(UInt32 cbRecordSize, const void *pHeader, UInt32 cbHeaderSize, UInt32 cbBufferSize)
{
	Status   status;

	for (;;)
	{
		m_cbBufferSize     = cbBufferSize;
		m_cbUsedBufferSize = 0;
		m_cbRecordSize     = cbRecordSize;
		m_cRecords         = 0;
		if (NULL == (m_buffer = (Byte *)Mem::Alloc(cbBufferSize)))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate memory at {1}:{2}", __FILE__, __LINE__);
		}

		Header   header;

		header.uMagic       = Header::MAGIC;
		header.uVersion     = Header::VERSION;
		header.cbRecordSize = cbRecordSize;
		header.cbHeaderSize = cbHeaderSize;
		if (!(status = Write(&header, sizeof(header))))
		{
			break;
		}
		if (0 < cbHeaderSize)
		{
			if (!(status = Write(pHeader, cbHeaderSize)))
			{
				break;
			}
		}

		break;
	}
	if (!status)
	{
		Close();
	}

	return status;
}

Status Writer::Open(UInt32 *pcbRecordSize, void **ppHeader, UInt32 *pcbHeaderSize, void **ppFooter, 
                    UInt32 *pcbFooterSize, UInt32 cbBufferSize)
{
	Status   status;

	if (NULL != ppHeader)
	{
		*ppHeader = NULL;
	}
	if (NULL != pcbHeaderSize)
	{
		*pcbHeaderSize = 0;
	}
	if (NULL != ppFooter)
	{
		*ppFooter = NULL;
	}
	if (NULL != pcbFooterSize)
	{
		*pcbFooterSize = 0;
	}
	for (;;)
	{
		m_cbBufferSize     = cbBufferSize;
		m_cbUsedBufferSize = 0;
		m_cRecords         = 0;
		if (NULL == (m_buffer = (Byte *)Mem::Alloc(cbBufferSize)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate memory at {1}:{2}", __FILE__, __LINE__);
		}

		Header          header;
		Footer          footer;
		LARGE_INTEGER   liFileSize;
		LARGE_INTEGER   liPos;
		LARGE_INTEGER   liNewPos;

		if (!GetFileSizeEx(m_pFile, &liFileSize))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate memory at {1}:{2}", __FILE__, __LINE__);

			break;
		}
		if (sizeof(Header) + sizeof(Footer) > (UInt64)liFileSize.QuadPart)
		{
			status = Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
		}

		if (!(status = Read(&header, sizeof(Header))))
		{
			break;
		}
		if (Header::MAGIC != header.uMagic)
		{
			status = Status(Status_ParseFailed, "Invalid magic number at {1}:{2}", __FILE__, __LINE__);
		}
		if (Header::VERSION != header.uVersion)
		{
			status = Status(Status_ParseFailed, "Invalid version number at {1}:{2}", __FILE__, __LINE__);
		}
		if (NULL != pcbRecordSize)
		{
			*pcbRecordSize = header.cbRecordSize;
		}
		if (sizeof(Header) + header.cbHeaderSize + sizeof(Footer) > (UInt64)liFileSize.QuadPart)
		{
			status = Status(Status_InvalidArg, "Invalid arg at {1}:{2}", __FILE__, __LINE__);
		}

		if (NULL != pcbHeaderSize)
		{
			*pcbHeaderSize = header.cbHeaderSize;
		}
		if (NULL != ppHeader)
		{
			if (0 < header.cbHeaderSize)
			{
				if (NULL == (*ppHeader = Mem::Alloc(header.cbHeaderSize)))
				{
					status = Status(Status_MemAllocFailed, "Failed to allocate memory at {1}:{2}", __FILE__, __LINE__);

					break;
				}
				if (!(status = Read(*ppHeader, header.cbHeaderSize)))
				{
					break;
				}
			}
			else
			{
				*ppHeader = NULL;
			}
		}

		liPos.QuadPart = -((Int64)sizeof(Footer));
		if (!SetFilePointerEx(m_pFile, liPos, &liNewPos, FILE_END))
		{
			status = Status(Status_InvalidArg, "SetFilePointerEx failed with error {1} at {2}:{3}", (int)GetLastError(), 
			                __FILE__, __LINE__);

			break;
		}
		if (!(status = Read(&footer, sizeof(Footer))))
		{
			break;
		}
		if (NULL != pcbFooterSize)
		{
			*pcbFooterSize = footer.cbFooterSize;
		}
		if (NULL != ppFooter)
		{
			if (0 < footer.cbFooterSize)
			{
				if (NULL == (*ppFooter = Mem::Alloc(footer.cbFooterSize)))
				{
					status = Status(Status_MemAllocFailed, "Failed to allocate memory at {1}:{2}", __FILE__, __LINE__);

					break;
				}
				liPos.QuadPart = -((Int64)(sizeof(Footer) + footer.cbFooterSize));
				if (!SetFilePointerEx(m_pFile, liPos, &liNewPos, FILE_END))
				{
					status = Status(Status_InvalidArg, "SetFilePointerEx failed with error {1} at {2}:{3}", 
					                (int)GetLastError(), __FILE__, __LINE__);

					break;
				}
				if (!(status = Read(*ppFooter, footer.cbFooterSize)))
				{
					break;
				}
			}
			else
			{
				*ppFooter = NULL;
			}
		}
		m_cbRecordSize = header.cbRecordSize;
		m_cRecords     = footer.cRecords;
		liPos.QuadPart = -((Int64)(sizeof(Footer) + footer.cbFooterSize));
		if (!SetFilePointerEx(m_pFile, liPos, &liNewPos, FILE_END))
		{
			status = Status(Status_InvalidArg, "SetFilePointerEx failed with error {1} at {2}:{3}", 
			                (int)GetLastError(), __FILE__, __LINE__);

			break;
		}
		if (!SetEndOfFile(m_pFile))
		{
			status = Status(Status_InvalidArg, "SetEndOfFile failed with error {1} at {2}:{3}", (int)GetLastError(), 
			                __FILE__, __LINE__);

			break;
		}
		liPos.QuadPart = 0;
		if (!SetFilePointerEx(m_pFile, liPos, &liNewPos, FILE_END))
		{
			status = Status(Status_InvalidArg, "SetFilePointerEx failed with error {1} at {2}:{3}", 
			                (int)GetLastError(), __FILE__, __LINE__);

			break;
		}

		break;
	}
	if (!status)
	{
		if (NULL != ppHeader && NULL != *ppHeader)
		{
			FreeHeader(*ppHeader);
		}
		if (NULL != ppFooter && NULL != *ppFooter)
		{
			FreeFooter(*ppFooter);
		}
		Close();
	}

	return status;
}

Status Writer::Close(Bool bWriteEpilog, const void *pEpilog, UInt32 cbEpilogSize)
{
	if (NULL != m_pFile && bWriteEpilog)
	{
		Footer   footer;
		Status   status;

		if (0 < cbEpilogSize)
		{
			if (!(status = Write(pEpilog, cbEpilogSize)))
			{
				return status;
			}
		}
		footer.cRecords     = m_cRecords;
		footer.cbFooterSize = cbEpilogSize;
		if (!(status = Write(&footer, sizeof(Footer))))
		{
			return status;
		}
		if (!(status = Flush()))
		{
			return status;
		}
	}
	if (NULL != m_buffer)
	{
		Mem::Free(m_buffer);
	}
	if (NULL != m_pFile)
	{
		CloseHandle(m_pFile);
	}
	m_pFile            = NULL;
	m_buffer           = NULL;
	m_cbBufferSize     = 0;
	m_cbUsedBufferSize = 0;
	m_cbRecordSize     = 0;
	m_cRecords         = 0;

	return Status();
}

Status Writer::Write(const void *pData, UInt32 cbSize)
{
	const Byte   *pPos;
	UInt32       cbFreeSize;
	UInt32       cbTempSize;
	Status       status;

	pPos = (const Byte *)pData;
	while (0 < cbSize)
	{
		cbFreeSize = m_cbBufferSize - m_cbUsedBufferSize;
		if (0 == cbFreeSize)
		{
			if (!(status = Flush()))
			{
				return status;
			}
			cbFreeSize = m_cbBufferSize;
		}
		if (cbSize > cbFreeSize)
		{
			cbTempSize = cbFreeSize;
		}
		else
		{
			cbTempSize = cbSize;
		}
		memcpy(m_buffer + m_cbUsedBufferSize, pPos, cbTempSize);
		pPos += cbTempSize;
		cbSize -= cbTempSize;
		m_cbUsedBufferSize += cbTempSize;
	}

	return Status();
}

Status Writer::Flush()
{
	if (0 < m_cbUsedBufferSize)
	{
		DWORD    dwAckSize;

		if (!WriteFile(m_pFile, m_buffer, (DWORD)m_cbUsedBufferSize, &dwAckSize, NULL))
		{
			return Status(Status_WriteFailed, "WriteFile failed with error {1} at {2}:{3", GetLastError(), __FILE__, 
			              __LINE__);
		}
		if (m_cbUsedBufferSize != dwAckSize)
		{
			return Status(Status_WriteFailed, "WriteFile failed at {1}:{2}", __FILE__, __LINE__);
		}
		m_cbUsedBufferSize = 0;
	}

	return Status();
}

Status Writer::Read(void *pData, UInt32 cbSize)
{
	DWORD    dwAckSize;

	if (!ReadFile(m_pFile, pData, (DWORD)cbSize, &dwAckSize, NULL))
	{
		return Status(Status_ReadFailed, "ReadFile failed with error {1} at {2}:{3}", GetLastError(), __FILE__, __LINE__);
	}
	if (cbSize != dwAckSize)
	{
		return Status(Status_ReadFailed, "ReadFile failed at {1}:{2}", __FILE__, __LINE__);
	}

	return Status();
}

}//namespace Flat

}//namespace DB

}//namespace CX
