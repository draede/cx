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
#include "CX/Data/NPY/Reader.hpp"
#include "CX/Data/NPY/Consts.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace Data
{

namespace NPY
{

Reader::Reader()
{
	m_header.Reset();
	m_cbAllocationGranularity = 0;
}

Reader::~Reader()
{
	Close();
}

Status Reader::Open()
{
	SYSTEM_INFO   sysinfo;
	Status        status;

	if (!(status = m_header.Read(m_mmf.GetMapData(), (Size)m_mmf.GetMapSize())))
	{
		return status;
	}
	::GetSystemInfo(&sysinfo);
	m_cbAllocationGranularity = sysinfo.dwAllocationGranularity;

	return Status();
}

Status Reader::Open(const Char *szPath)
{
	Status   status;

	Close();

	if (!(status = m_mmf.Open(szPath, 0, INITIAL_MAP_SIZE, m_mmf.Flag_AttrNormal)))
	{
		return status;
	}
	m_sPath = szPath;

	if (!(status = Open()))
	{
		Close();

		return status;
	}

	return Status();
}

Status Reader::Open(const WChar *wszPath)
{
	Status   status;

	Close();

	if (!(status = m_mmf.Open(wszPath, 0, INITIAL_MAP_SIZE, m_mmf.Flag_AttrNormal)))
	{
		return status;
	}
	m_wsPath = wszPath;

	if (!(status = Open()))
	{
		Close();

		return status;
	}

	return Status();
}

Status Reader::Close()
{
	Status   status;

	if (!(status = m_mmf.Close()))
	{
		return status;
	}
	m_sPath.clear();
	m_wsPath.clear();
	m_header.Reset();
	m_cbAllocationGranularity = 0;

	return Status();
}

const void *Reader::GetRows(Size cStartRowIndex, Size cReqRowsCount, Size *pcAckRowsCount)
{
	if (!m_mmf.IsOK())
	{
		if (NULL != pcAckRowsCount)
		{
			*pcAckRowsCount = 0;
		}

		return NULL;
	}
	if (cStartRowIndex >= m_header.cRows)
	{
		if (NULL != pcAckRowsCount)
		{
			*pcAckRowsCount = 0;
		}

		return NULL;
	}
	if (cStartRowIndex + cReqRowsCount > m_header.cRows)
	{
		cReqRowsCount = m_header.cRows - cStartRowIndex;
	}

	UInt64   cbDesiredOffset = m_header.cbTotalSize + cStartRowIndex * m_header.cbRowSize;
	UInt64   cbDesiredSize   = cReqRowsCount * m_header.cbRowSize;
	UInt64   cbActualOffset  = cbDesiredOffset;
	UInt64   cbActualSize    = cbDesiredSize;

	if (0 < (cbDesiredOffset % m_cbAllocationGranularity))
	{
		cbActualOffset = (cbDesiredOffset / m_cbAllocationGranularity) * m_cbAllocationGranularity;
		cbActualSize += cbDesiredOffset - cbActualOffset;
	}

	Status   status;

	if (!(status = m_mmf.Remap(cbActualOffset, cbActualSize)))
	{
		if (NULL != pcAckRowsCount)
		{
			*pcAckRowsCount = 0;
		}

		return NULL;
	}

	if (NULL != pcAckRowsCount)
	{
		*pcAckRowsCount = cReqRowsCount;
	}

	return (const Byte *)m_mmf.GetMapData() + cbDesiredOffset - cbActualOffset;
}

Bool Reader::IsOK() const
{
	return m_mmf.IsOK();
}

const Char *Reader::GetPathA() const
{
	if (!m_mmf.IsOK())
	{
		return "";
	}

	return m_sPath.c_str();
}

const WChar *Reader::GetPathW() const
{
	if (!m_mmf.IsOK())
	{
		return L"";
	}

	return m_wsPath.c_str();
}

Size Reader::GetRowsCount() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_header.cRows;
}

Size Reader::GetColumnsCount() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_header.cColumns;
}

Type Reader::GetType() const
{
	if (!m_mmf.IsOK())
	{
		return Type_Invalid;
	}

	return m_header.nType;
}

ByteOrder Reader::GetByteOrder() const
{
	if (!m_mmf.IsOK())
	{
		return ByteOrder_Invalid;
	}

	return m_header.nByteOrder;
}

Size Reader::GetRowSize() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_header.cbRowSize;
}

Format Reader::GetFormat() const
{
	if (!m_mmf.IsOK())
	{
		return Format_Invalid;
	}

	return m_header.nFormat;
}

Version Reader::GetVersion() const
{
	if (!m_mmf.IsOK())
	{
		return Version_Invalid;
	}

	return m_header.nVersion;
}

}//namespace NPY

}//namespace Data

}//namespace CX
