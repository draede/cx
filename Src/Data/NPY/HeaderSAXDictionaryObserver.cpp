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
#include "CX/Data/NPY/HeaderSAXDictionaryObserver.hpp"
#include "CX/Data/NPY/Consts.hpp"
#include "CX/Data/NPY/SAXDictionaryParser.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

Status HeaderSAXDictionaryObserver::Parse(const void *pBuffer, Size cbSize, Header *pHeader)
{
	HeaderSAXDictionaryObserver   observer;
	SAXDictionaryParser           parser;
	Status                        status;

	observer.m_pHeader = pHeader;
	parser.AddObserver(&observer);
	if (!(status = parser.ParseBuffer(pBuffer, cbSize, &pHeader->cbUsedSize)))
	{
		return status;
	}
	if (Format_Invalid == pHeader->nFormat)
	{
		return Status(Status_ParseFailed, "Invalid fortran_order");
	}

	return Status();
}

Status HeaderSAXDictionaryObserver::OnBeginParse()
{
	while (!m_stackNodes.empty())
	{
		m_stackNodes.pop();
	}
	m_pHeader->Reset();
	m_vectorShape.clear();

	return Status();
}

Status HeaderSAXDictionaryObserver::OnEndParse()
{
	if (1 == m_vectorShape.size())
	{
		m_pHeader->cColumns = 1;
		m_pHeader->cRows    = (Size)m_vectorShape[0];
	}
	else
	if (2 == m_vectorShape.size())
	{
		if (Consts::MIN_COLUMNS_COUNT > m_vectorShape[1] || 
			   Consts::MAX_COLUMNS_COUNT < m_vectorShape[1])
		{
			return Status(Status_ParseFailed, "Invalid shape (columns)");
		}
		m_pHeader->cColumns = (Size)m_vectorShape[1];
		m_pHeader->cRows    = (Size)m_vectorShape[0];
	}
	else
	{
		return Status(Status_ParseFailed, "Invalid shape");
	}
	m_pHeader->ComputeRowSize();

	return Status();
}

Status HeaderSAXDictionaryObserver::OnBeginObject()
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

Status HeaderSAXDictionaryObserver::OnEndObject()
{
	if (m_stackNodes.empty() || Node::Type_Object != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid object end");
	}
	m_stackNodes.pop();

	return Status();
}

Status HeaderSAXDictionaryObserver::OnBeginArray()
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

Status HeaderSAXDictionaryObserver::OnEndArray()
{
	if (m_stackNodes.empty() || Node::Type_Array != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid array end");
	}
	m_stackNodes.pop();

	return Status();
}

Status HeaderSAXDictionaryObserver::OnBeginTuple()
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

Status HeaderSAXDictionaryObserver::OnEndTuple()
{
	if (m_stackNodes.empty() || Node::Type_Tuple != m_stackNodes.top().nType)
	{
		return Status(Status_ParseFailed, "Invalid tuple end");
	}
	m_stackNodes.pop();

	return Status();
}

Status HeaderSAXDictionaryObserver::OnKey(const Char *pBuffer, Size cLen)
{
	Node   node;

	node.nType  = Node::Type_Named;
	node.cItems = 0;
	node.sKey.assign(pBuffer, cLen);
	m_stackNodes.push(node);

	return Status();
}

Status HeaderSAXDictionaryObserver::OnBool(Bool bBool)
{
	if (2 == m_stackNodes.size() && Node::Type_Named == m_stackNodes.top().nType && 
	    0 == cx_stricmp(m_stackNodes.top().sKey.c_str(), "fortran_order"))
	{
		if (bBool)
		{
			m_pHeader->nFormat = Format_Fortran;
		}
		else
		{
			m_pHeader->nFormat = Format_C;
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

Status HeaderSAXDictionaryObserver::OnInt(Int64 nInt)
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

Status HeaderSAXDictionaryObserver::OnUInt(UInt64 uUInt)
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

Status HeaderSAXDictionaryObserver::OnReal(Double lfReal)
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

Status HeaderSAXDictionaryObserver::OnString(const Char *pBuffer, Size cLen)
{
	Status   status;

	if (2 == m_stackNodes.size() && Node::Type_Named == m_stackNodes.top().nType)
	{
		if (!(status = ParseColumnDesc(pBuffer, cLen)))
		{
			return status;
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

Status HeaderSAXDictionaryObserver::OnTupleDefault()
{
	if (!m_stackNodes.empty() && (Node::Type_Tuple == m_stackNodes.top().nType))
	{
		m_stackNodes.top().cItems++;
	}

	return Status();
}

Status HeaderSAXDictionaryObserver::ParseColumnDesc(const Char *pBuffer, Size cLen)
{
	if (0 == cLen)
	{
		return Status_ParseFailed;
	}
	if ('|' == *pBuffer || '<' == *pBuffer)
	{
		m_pHeader->nByteOrder = ByteOrder_LittleEndian;
		pBuffer++;
		cLen--;
	}
	else
	if ('>' == *pBuffer)
	{
		m_pHeader->nByteOrder = ByteOrder_BigEndian;
		pBuffer++;
		cLen--;
	}
	else
	{
		m_pHeader->nByteOrder = ByteOrder_LittleEndian;
	}

	if (2 != cLen)
	{
		return Status_ParseFailed;
	}
	if (0 == memcmp(pBuffer, "i1", 2))
	{
		m_pHeader->nType = Type_Int8;
	}
	else
	if (0 == memcmp(pBuffer, "u1", 2))
	{
		m_pHeader->nType = Type_UInt8;
	}
	else
	if (0 == memcmp(pBuffer, "i2", 2))
	{
		m_pHeader->nType = Type_Int16;
	}
	else
	if (0 == memcmp(pBuffer, "u2", 2))
	{
		m_pHeader->nType = Type_UInt16;
	}
	else
	if (0 == memcmp(pBuffer, "i4", 2))
	{
		m_pHeader->nType = Type_Int32;
	}
	else
	if (0 == memcmp(pBuffer, "u4", 2))
	{
		m_pHeader->nType = Type_UInt32;
	}
	else
	if (0 == memcmp(pBuffer, "i8", 2))
	{
		m_pHeader->nType = Type_Int64;
	}
	else
	if (0 == memcmp(pBuffer, "u8", 2))
	{
		m_pHeader->nType = Type_UInt64;
	}
	else
	if (0 == memcmp(pBuffer, "f4", 2))
	{
		m_pHeader->nType = Type_Float;
	}
	else
	if (0 == memcmp(pBuffer, "f8", 2))
	{
		m_pHeader->nType = Type_Double;
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
