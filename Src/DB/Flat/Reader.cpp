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
#include "CX/DB/Flat/Reader.hpp"


namespace CX
{

namespace DB
{

namespace Flat
{

Reader::Reader()
{
	m_cRecords     = 0;
	m_cbRecordSize = 0;
	m_pRecords     = NULL;
	m_cbHeaderSize = 0;
	m_pHeader      = NULL;
	m_cbFooterSize = 0;
	m_pFooter      = NULL;
}

Reader::~Reader()
{
	Close();
}

Status Reader::Open(const Char *szPath)
{
	Close();

	Status   status;

	if (!(status = m_mmf.Open(szPath)))
	{
		return status;
	}

	return Open();
}

Status Reader::Open(const WChar *wszPath)
{
	Close();

	Status   status;

	if (!(status = m_mmf.Open(wszPath)))
	{
		return status;
	}

	return Open();
}

Status Reader::Close()
{
	m_mmf.Close();
	m_cRecords     = 0;
	m_cbRecordSize = 0;
	m_pRecords     = NULL;
	m_cbHeaderSize = 0;
	m_pHeader      = NULL;
	m_cbFooterSize = 0;
	m_pFooter      = NULL;

	return NULL;
}

Bool Reader::IsOK() const
{
	return m_mmf.IsOK();
}

UInt32 Reader::GetRecordSize() const
{
	return m_cbRecordSize;
}

UInt32 Reader::GetRecordsCount() const
{
	return m_cRecords;
}

UInt32 Reader::GetHeaderSize() const
{
	return m_cbHeaderSize;
}

const void *Reader::GetHeader() const
{
	return m_pHeader;
}

UInt32 Reader::GetFooterSize() const
{
	return m_cbFooterSize;
}

const void *Reader::GetFooter() const
{
	return m_pFooter;
}

const void *Reader::GetRecords() const
{
	return m_pRecords;
}

const void *Reader::GetRecord(UInt32 cIndex) const
{
	if (m_cRecords <= cIndex)
	{
		return NULL;
	}

	return (const Byte *)m_pRecords + cIndex * m_cbRecordSize;
}

Status Reader::Open()
{
	const Byte     *pMem;
	UInt64         cbSize;
	const Header   *pHeader;
	const Footer   *pFooter;
	Status         status;

	for (;;)
	{
		pMem   = (const Byte *)m_mmf.GetMapData();
		cbSize = m_mmf.GetMapSize();
		if (sizeof(Header) + sizeof(Footer) > cbSize)
		{
			status = Status(Status_OutOfBounds, "File too small at {1}:{2}", __FILE__, __LINE__);
		}
		pHeader = (const Header *)pMem;
		if (Header::MAGIC != pHeader->uMagic)
		{
			status = Status(Status_ParseFailed, "Invalid magic number at {1}:{2}", __FILE__, __LINE__);
		}
		if (Header::VERSION != pHeader->uVersion)
		{
			status = Status(Status_ParseFailed, "Invalid version number at {1}:{2}", __FILE__, __LINE__);
		}
		if (sizeof(Header) + pHeader->cbHeaderSize + sizeof(Footer) > cbSize)
		{
			status = Status(Status_OutOfBounds, "File too small at {1}:{2}", __FILE__, __LINE__);
		}
		if (0 < pHeader->cbHeaderSize)
		{
			m_pHeader = pMem + sizeof(Header);
		}
		else
		{
			m_pHeader = NULL;
		}
		m_cbHeaderSize = pHeader->cbHeaderSize;
		pFooter = (const Footer *)(pMem + cbSize - sizeof(Footer));
		if (sizeof(Header) + pHeader->cbHeaderSize + sizeof(Footer) + pFooter->cbFooterSize > cbSize)
		{
			status = Status(Status_OutOfBounds, "File too small at {1}:{2}", __FILE__, __LINE__);
		}
		if (sizeof(Header) + pHeader->cbHeaderSize + pHeader->cbRecordSize *pFooter->cRecords + 
		    sizeof(Footer) + pFooter->cbFooterSize > cbSize)
		{
			status = Status(Status_OutOfBounds, "File too small at {1}:{2}", __FILE__, __LINE__);
		}
		if (0 < pFooter->cbFooterSize)
		{
			m_pFooter = pMem + cbSize - sizeof(Footer) - pFooter->cbFooterSize;
		}
		else
		{
			m_pFooter = NULL;
		}
		m_cbFooterSize = pFooter->cbFooterSize;
		m_cbRecordSize = pHeader->cbRecordSize;
		m_cRecords     = pFooter->cRecords;
		m_pRecords     = pMem + sizeof(Header) + pHeader->cbHeaderSize;

		break;
	}
	if (!status)
	{
		Close();
	}

	return status;
}

}//namespace Flat

}//namespace DB

}//namespace CX
