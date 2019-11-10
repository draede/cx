/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
	m_nFormat                 = Format_Invalid;
	m_nVersion                = Version_Invalid;
	m_cbFirstRowOffset        = 0;
	m_cRows                   = 0;
	m_cbRowSize               = 0;
	m_cbAllocationGranularity = 0;
}

Reader::~Reader()
{
	Close();
}

Status Reader::Open()
{
	SYSTEM_INFO   sysinfo;
	UInt64        cbSize = m_mmf.GetMapSize();
	Size          cbDictSize;
	Size          cbLenSize;
	Status        status;

	::GetSystemInfo(&sysinfo);
	m_cbAllocationGranularity = sysinfo.dwAllocationGranularity;

	if (sizeof(Header::MAGIC_V_1_0) > cbSize)
	{
		return Status(Status_ParseFailed, "File too small");
	}

	const Byte *header = (const Byte *)m_mmf.GetMapData();

	if (0 != memcmp(header, Header::MAGIC, sizeof(Header::MAGIC)))
	{
		return Status(Status_ParseFailed, "Invalid header");
	}

	if (0 == memcmp(header, Header::MAGIC_V_1_0, sizeof(Header::MAGIC_V_1_0)))
	{
		m_nVersion = Version_1_0;
	}
	else
	if (0 == memcmp(header, Header::MAGIC_V_2_0, sizeof(Header::MAGIC_V_2_0)))
	{
		m_nVersion = Version_2_0;
	}
	else
	if (0 == memcmp(header, Header::MAGIC_V_3_0, sizeof(Header::MAGIC_V_3_0)))
	{
		m_nVersion = Version_3_0;
	}
	else
	{
		return Status(Status_ParseFailed, "Invalid version");
	}

	if (Version_1_0 == m_nVersion)
	{
		if (sizeof(Header::MAGIC_V_1_0) + sizeof(UInt16) > cbSize)
		{
			return Status(Status_ParseFailed, "File too small");
		}
		cbLenSize = sizeof(UInt16);

		cbDictSize = (Size)*(UInt16 *)(header + sizeof(Header::MAGIC_V_1_0));
	}
	else
	{
		if (sizeof(Header::MAGIC_V_1_0) + sizeof(UInt32) > cbSize)
		{
			return Status(Status_ParseFailed, "File too small");
		}
		cbLenSize = sizeof(UInt32);

		cbDictSize = (Size)*(UInt32 *)(header + sizeof(Header::MAGIC_V_1_0));
	}

	if (sizeof(Header::MAGIC_V_1_0) + cbLenSize + cbDictSize > cbSize)
	{
		if (!(status = m_mmf.Remap(0, sizeof(Header::MAGIC_V_1_0) + cbSize + cbDictSize)))
		{
			return status;
		}
		cbSize = m_mmf.GetMapSize();
		header = (const Byte *)m_mmf.GetMapData();
	}

	m_cbFirstRowOffset = sizeof(Header::MAGIC_V_1_0) + cbLenSize + cbDictSize;

	if (!(status = ParseDictionary(header + sizeof(Header::MAGIC_V_1_0) + cbLenSize, cbDictSize)))
	{
		return status;
	}

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
	m_vectorColumns.clear();
	m_nFormat                 = Format_Invalid;
	m_nVersion                = Version_Invalid;
	m_cbFirstRowOffset        = 0;
	m_cRows                   = 0;
	m_cbRowSize               = 0;
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
	if (cStartRowIndex >= m_cRows)
	{
		if (NULL != pcAckRowsCount)
		{
			*pcAckRowsCount = 0;
		}

		return NULL;
	}
	if (cStartRowIndex + cReqRowsCount > m_cRows)
	{
		cReqRowsCount = m_cRows - cStartRowIndex;
	}

	UInt64   cbDesiredOffset = m_cbFirstRowOffset + cStartRowIndex * m_cbRowSize;
	UInt64   cbDesiredSize   = cReqRowsCount * m_cbRowSize;
	UInt64   cbActualOffset  = cbDesiredOffset;
	UInt64   cbActualSize    = cbDesiredSize;

	if (0 < (cbDesiredOffset % m_cbAllocationGranularity))
	{
		cbActualOffset = (cbDesiredOffset / m_cbAllocationGranularity) * m_cbAllocationGranularity;
		cbActualSize += cbDesiredOffset - cbActualOffset;
	}

	Status   status;

	if (!(status = m_mmf.Remap(cbActualOffset, cbActualOffset)))
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

const Size Reader::GetRowsCount() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_cRows;
}

const Size Reader::GetColumnsCount() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_vectorColumns.size();
}

const Column *Reader::GetColumns() const
{
	if (!m_mmf.IsOK())
	{
		return NULL;
	}

	return &m_vectorColumns[0];
}

Size Reader::GetRowSize() const
{
	if (!m_mmf.IsOK())
	{
		return 0;
	}

	return m_cbRowSize;
}

Format Reader::GetFormat() const
{
	if (!m_mmf.IsOK())
	{
		return Format_Invalid;
	}

	return m_nFormat;
}

Version Reader::GetVersion() const
{
	if (!m_mmf.IsOK())
	{
		return Version_Invalid;
	}

	return m_nVersion;
}

Status Reader::ParseDictionary(const void *pData, Size cbSize)
{
	SAXDictionaryObserver   observer;
	SAXDictionaryParser     parser;
	Status                  status;

	parser.AddObserver(&observer);
	if (!(status = parser.ParseBuffer(pData, cbSize)))
	{
		return status;
	}
	if (Format_Invalid == observer.m_nFormat)
	{
		return Status(Status_ParseFailed, "Invalid fortran_order");
	}
	m_nFormat = observer.m_nFormat;
	if (1 == observer.m_vectorShape.size())
	{
		m_cRows = observer.m_vectorShape[0];
		if (observer.m_vectorColumns.empty())
		{
			return Status(Status_ParseFailed, "Invalid descr");
		}
		if (Consts::MIN_COLUMNS_COUNT > observer.m_vectorColumns.size() || 
		    Consts::MAX_COLUMNS_COUNT < observer.m_vectorColumns.size())
		{
			return Status(Status_ParseFailed, "Invalid shape");
		}
		m_vectorColumns = observer.m_vectorColumns;
		m_cbRowSize     = observer.m_cbRowSize;
	}
	else
	if (2 == observer.m_vectorShape.size())
	{
		if (Type_Invalid == observer.m_all.nType)
		{
			return Status(Status_ParseFailed, "Invalid descr");
		}
		m_cRows = observer.m_vectorShape[0];
		if (Consts::MIN_COLUMNS_COUNT > observer.m_vectorShape[1] || 
		    Consts::MAX_COLUMNS_COUNT < observer.m_vectorShape[1])
		{
			return Status(Status_ParseFailed, "Invalid shape");
		}
		for (Size i = 0; i < observer.m_vectorShape[1]; i++)
		{
			m_vectorColumns.push_back(observer.m_all);
		}
		m_cbRowSize = observer.m_cbRowSize * observer.m_vectorShape[1];
	}
	else
	{
		return Status(Status_ParseFailed, "Invalid shape");
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnBeginParse()
{
	while (!m_stackNodes.empty())
	{
		m_stackNodes.pop();
	}
	m_nFormat           = Format_Invalid;
	m_cRows             = 0;
	m_cColumns          = 0;
	m_vectorColumns.clear();
	m_all.nByteOrder    = ByteOrder_Invalid;
	m_all.nType         = Type_Invalid;
	m_all.sName.clear();
	m_bShapeHasDefault  = False;
	m_vectorShape.clear();
	m_cbRowSize         = 0;

	return Status();
}

Status Reader::SAXDictionaryObserver::OnEndParse()
{
	return Status();
}

Status Reader::SAXDictionaryObserver::OnBeginObject()
{
	Node   node;

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		node.sKey = m_stackNodes.top().sKey;
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	node.nType  = Node::Type_Object;
	node.cItems = 0;
	m_stackNodes.push(node);

	return Status();
}

Status Reader::SAXDictionaryObserver::OnEndObject()
{
	if (m_stackNodes.empty() || Node::Type_Object != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid object end");
	}
	m_stackNodes.pop();

	return Status();
}

Status Reader::SAXDictionaryObserver::OnBeginArray()
{
	Node   node;

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		node.sKey = m_stackNodes.top().sKey;
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	node.nType  = Node::Type_Array;
	node.cItems = 0;
	m_stackNodes.push(node);

	return Status();
}

Status Reader::SAXDictionaryObserver::OnEndArray()
{
	if (m_stackNodes.empty() || Node::Type_Array != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid array end");
	}
	m_stackNodes.pop();

	return Status();
}

Status Reader::SAXDictionaryObserver::OnBeginTuple()
{
	Node   node;

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		node.sKey = m_stackNodes.top().sKey;
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	node.nType  = Node::Type_Tuple;
	node.cItems = 0;
	m_stackNodes.push(node);

	return Status();
}

Status Reader::SAXDictionaryObserver::OnEndTuple()
{
	if (m_stackNodes.empty() || Node::Type_Tuple != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid tuple end");
	}
	m_stackNodes.pop();

	return Status();
}

Status Reader::SAXDictionaryObserver::OnKey(const Char *pBuffer, Size cLen)
{
	Node   node;

	node.nType  = Node::Type_Named;
	node.cItems = 0;
	node.sKey.assign(pBuffer, cLen);
	m_stackNodes.push(node);

	return Status();
}

Status Reader::SAXDictionaryObserver::OnBool(Bool bBool)
{
	if (2 == m_stackNodes.size() && Node::Type_Named == m_stackNodes.top().nType && 
	    0 == cx_stricmp(m_stackNodes.top().sKey.c_str(), "fortran_order"))
	{
		if (bBool)
		{
			m_nFormat = Format_Fortran;
		}
		else
		{
			m_nFormat = Format_C;
		}
	}
	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnInt(Int64 nInt)
{
	nInt;

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnUInt(UInt64 uUInt)
{
	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		m_stackNodes.pop();
	}

	if (2 == m_stackNodes.size() && Node::Type_Tuple == m_stackNodes.top().nType && 
	    0 == cx_stricmp(m_stackNodes.top().sKey.c_str(), "shape"))
	{
		m_vectorShape.push_back(uUInt);
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnReal(Double lfReal)
{
	lfReal;

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnString(const Char *pBuffer, Size cLen)
{
	Status   status;

	if (2 == m_stackNodes.size() && Node::Type_Tuple == m_stackNodes.top().nType)
	{
		if (!(status = ParseColumnDesc(&m_all, pBuffer, cLen)))
		{
			return status;
		}
	}
	else
	if (3 == m_stackNodes.size() && Node::Type_Tuple == m_stackNodes.top().nType )
	{
		if (0 == m_stackNodes.top().cItems)
		{
			Column   column;

			column.sName.assign(pBuffer, cLen);
			m_vectorColumns.push_back(column);
		}
		else
		if (1 == m_stackNodes.top().cItems)
		{
			if (!(status = ParseColumnDesc(&m_vectorColumns[m_vectorColumns.size() - 1], pBuffer, cLen)))
			{
				return status;
			}
		}
	}

	if (!m_stackNodes.empty() && Node::Type_Named == m_stackNodes.top().nType)
	{
		m_stackNodes.pop();
	}

	if (!m_stackNodes.empty() && (Node::Type_Object == m_stackNodes.top().nType || 
	    Node::Type_Array == m_stackNodes.top().nType || Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::OnTupleDefault()
{
	if (2 == m_stackNodes.size() && Node::Type_Tuple == m_stackNodes.top().nType && 
	    0 == cx_stricmp(m_stackNodes.top().sKey.c_str(), "shape"))
	{
		m_bShapeHasDefault = True;
	}

	if (!m_stackNodes.empty() && (Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status Reader::SAXDictionaryObserver::ParseColumnDesc(Column *pColumn, const Char *pBuffer, Size cLen)
{
	if (0 == cLen)
	{
		return Status_ParseFailed;
	}
	if ('|' == *pBuffer || '<' == *pBuffer)
	{
		pColumn->nByteOrder = ByteOrder_LittleEndian;
		pBuffer++;
		cLen--;
	}
	else
	if ('>' == *pBuffer)
	{
		pColumn->nByteOrder = ByteOrder_BigEndian;
		pBuffer++;
		cLen--;
	}
	else
	{
		pColumn->nByteOrder = ByteOrder_LittleEndian;
	}

	if (2 != cLen)
	{
		return Status_ParseFailed;
	}
	if (0 == memcmp(pBuffer, "i1", 2))
	{
		m_cbRowSize += 1;
		pColumn->nType = Type_Int8;
	}
	else
	if (0 == memcmp(pBuffer, "u1", 2))
	{
		m_cbRowSize += 1;
		pColumn->nType = Type_UInt8;
	}
	else
	if (0 == memcmp(pBuffer, "i2", 2))
	{
		m_cbRowSize += 2;
		pColumn->nType = Type_Int16;
	}
	else
	if (0 == memcmp(pBuffer, "u2", 2))
	{
		m_cbRowSize += 2;
		pColumn->nType = Type_UInt16;
	}
	else
	if (0 == memcmp(pBuffer, "i4", 2))
	{
		m_cbRowSize += 4;
		pColumn->nType = Type_Int32;
	}
	else
	if (0 == memcmp(pBuffer, "u4", 2))
	{
		m_cbRowSize += 4;
		pColumn->nType = Type_UInt32;
	}
	else
	if (0 == memcmp(pBuffer, "i8", 2))
	{
		m_cbRowSize += 8;
		pColumn->nType = Type_Int64;
	}
	else
	if (0 == memcmp(pBuffer, "u8", 2))
	{
		m_cbRowSize += 8;
		pColumn->nType = Type_UInt64;
	}
	else
	if (0 == memcmp(pBuffer, "f4", 2))
	{
		m_cbRowSize += 4;
		pColumn->nType = Type_Float;
	}
	else
	if (0 == memcmp(pBuffer, "f8", 2))
	{
		m_cbRowSize += 8;
		pColumn->nType = Type_Double;
	}
	else
	{
		return Status_ParseFailed;
	}

	return Status();
}

}//namespace NPY

}//namespace Data

}//namespace CX
