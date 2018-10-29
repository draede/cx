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


#include "CX/DB/Flat/Reader.hpp"
#include "CX/C/stdio.h"


namespace CX
{

namespace DB
{

namespace Flat
{

Reader::Reader()
{
	m_nMagic       = 0;
	m_cbHeaderSize = 0;
	m_cbRecordSize = 0;
	m_cRecordCount = 0;
}

Reader::~Reader()
{
	Close();
}

Status Reader::OpenInternal()
{
	Status   status;

	if (HEADER_SIZE > m_mf.GetMapSize())
	{
		return Status_ParseFailed;
	}

	const UInt32 *pMem = (const UInt32 *)m_mf.GetMapData();

	m_nMagic       = *pMem; pMem++;
	m_cbHeaderSize = *pMem; pMem++;
	m_cbRecordSize = *pMem; pMem++;
	m_cRecordCount = *pMem; pMem++;
	if (HEADER_SIZE + m_cbHeaderSize > m_mf.GetMapSize())
	{
		return Status_ParseFailed;
	}

	return Status();
}

Status Reader::Open(const Char *szPath)
{
	if (m_mf.IsOK())
	{
		return Status_Busy;
	}

	Status   status;

	if (!(status = m_mf.Open(szPath, 0, 0, IO::MemoryMappedFile::Flag_AttrRandomAccess)))
	{
		return status;
	}

	if (!(status = OpenInternal()))
	{
		m_mf.Close();
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
	}

	return Status();
}

Status Reader::Open(const WChar *wszPath)
{
	if (m_mf.IsOK())
	{
		return Status_Busy;
	}

	Status   status;

	if (!(status = m_mf.Open(wszPath, 0, 0, IO::MemoryMappedFile::Flag_AttrRandomAccess)))
	{
		return status;
	}

	if (!(status = OpenInternal()))
	{
		m_mf.Close();
		m_nMagic       = 0;
		m_cbHeaderSize = 0;
		m_cbRecordSize = 0;
		m_cRecordCount = 0;
	}

	return Status();
}

Status Reader::Close()
{
	Status   status;

	if (!m_mf.IsOK())
	{
		return Status_InvalidCall;
	}

	m_mf.Close();
	m_nMagic       = 0;
	m_cbHeaderSize = 0;
	m_cbRecordSize = 0;
	m_cRecordCount = 0;

	return Status();
}

Bool Reader::IsOK() const
{
	return m_mf.IsOK();
}

UInt32 Reader::GetMagic() const
{
	return m_nMagic;
}

UInt32 Reader::GetHeaderSize() const
{
	return m_cbHeaderSize;
}

UInt32 Reader::GetRecordSize() const
{
	return m_cbRecordSize;
}

UInt32 Reader::GetRecordCount() const
{
	return m_cRecordCount;
}

const void *Reader::GetHeader() const
{
	if (!m_mf.IsOK())
	{
		return NULL;
	}
	if (0 == m_cbHeaderSize)
	{
		return "";
	}

	return (const Byte *)m_mf.GetMapData() + HEADER_SIZE;
}
	
const void *Reader::GetRecord(UInt32 cIndex, UInt32 *pcbSize) const
{
	if (!m_mf.IsOK())
	{
		*pcbSize = 0;

		return NULL;
	}
	if (cIndex >= m_cRecordCount)
	{
		*pcbSize = 0;

		return NULL;
	}

	const Byte   *pPos      = (const Byte *)m_mf.GetMapData() + HEADER_SIZE + m_cbHeaderSize;
	UInt64       cbFileSize = m_mf.GetMapSize() - HEADER_SIZE - m_cbHeaderSize;
	UInt32       cbSize;

	for (;;)
	{
		if (0 == m_cbRecordSize)
		{
			if (sizeof(UInt32) > cbFileSize)
			{
				*pcbSize = 0;

				return NULL;
			}
			cbSize = *(UInt32 *)pPos;
			pPos += sizeof(UInt32);
		}
		else
		{
			cbSize = m_cbRecordSize;
		}
		if (cbSize > cbFileSize)
		{
			*pcbSize = 0;

			return NULL;
		}
		if (0 == cIndex)
		{
			*pcbSize = cbSize;

			return pPos;
		}
		else
		{
			cIndex--;
			pPos += cbSize;
			cbFileSize -= cbSize;
		}
	}
}

Status Reader::EnumerateRecords(EnumRecordsHandler pfnHandler, void *pUsrCtx)
{
	if (!m_mf.IsOK())
	{
		return Status_InvalidCall;
	}

	const Byte   *pPos      = (const Byte *)m_mf.GetMapData() + HEADER_SIZE + m_cbHeaderSize;
	UInt64       cbFileSize = m_mf.GetMapSize() - HEADER_SIZE - m_cbHeaderSize;
	UInt32       cbSize;
	UInt32       cIndex     = 0;
	Status       status;

	while (0 < cbFileSize)
	{
		if (cIndex > m_cRecordCount)
		{
			return Status_ReadFailed;
		}

		if (0 == m_cbRecordSize)
		{
			if (sizeof(UInt32) > cbFileSize)
			{
				return Status_ReadFailed;
			}
			cbSize = *(UInt32 *)pPos;
			pPos += sizeof(UInt32);
		}
		else
		{
			cbSize = m_cbRecordSize;
		}
		if (cbSize > cbFileSize)
		{
			return Status_ReadFailed;
		}
		if (!(status = pfnHandler(pUsrCtx, cIndex, pPos, cbSize)))
		{
			return status;
		}
		cIndex++;
		pPos += cbSize;
		cbFileSize -= cbSize;
	}

	return Status();
}

}//namespace Flat

}//namespace DB

}//namespace CX


#endif
