/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/IO/SimpleBuffers/BinaryDataReader.hpp"
#include "CX/IO/SimpleBuffers/BinaryData.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

BinaryDataReader::BinaryDataReader(IO::IInputStream *pInputStream)
{
	m_pInputStream    = pInputStream;
	m_pBuffer         = NULL;
	m_cbBufSize       = 0;
	m_cbBufUsedSize   = 0;
	m_cbBufOffset     = 0;
	m_bIsEOF          = false;
}

BinaryDataReader::~BinaryDataReader()
{
	if (NULL != m_pBuffer)
	{
		Free(m_pBuffer);
	}
}

Status BinaryDataReader::Read(void *pData, Size cbSize)
{
	Size   cbReqSize;
	Size   cbAckSize;
	Status status;

	if (m_bIsEOF)
	{
		return Status(Status_EOF, "End of stream reached");
	}
	if (NULL == m_pBuffer)
	{
		if (NULL == (m_pBuffer = (Byte *)Alloc(READ_BUFFER_SIZE)))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate internal buffer of {1} bytes", READ_BUFFER_SIZE);
		}
		m_cbBufSize     = READ_BUFFER_SIZE;
		m_cbBufOffset   = 0;
		m_cbBufUsedSize = 0;
	}

	Byte *pPos    = (Byte *)pData;
	Size cbOffset = 0;

	while (cbOffset < cbSize)
	{
		if (m_cbBufOffset >= m_cbBufUsedSize)
		{
			if ((status = m_pInputStream->Read(m_pBuffer, m_cbBufSize, &cbAckSize)).IsNOK())
			{
				if (status.GetCode() == Status_EOF)
				{
					m_bIsEOF = true;
				}
				m_hash.Update(pData, cbOffset);

				return status;
			}
			m_cbBufUsedSize = cbAckSize;
			m_cbBufOffset   = 0;
		}
		cbReqSize = m_cbBufUsedSize - m_cbBufOffset;
		if (cbReqSize >= cbSize - cbOffset)
		{
			cbReqSize = cbSize - cbOffset;
		}
		memcpy(pPos, m_pBuffer + m_cbBufOffset, cbReqSize);
		m_cbBufOffset += cbReqSize;
		pPos += cbReqSize;
		cbOffset += cbReqSize;
	}
	m_hash.Update(pData, cbSize);

	return Status();
}

Status BinaryDataReader::ReadOp(UInt8 nOp)
{
	return m_hash.Update(&nOp, sizeof(nOp));
}

Status BinaryDataReader::Begin()
{
	UInt32 nMagic;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_hash.Init();
	if ((status = Read(&nMagic, sizeof(nMagic))).IsNOK())
	{
		return status;
	}
	if (BinaryData::MAGIC != nMagic)
	{
		return Status(Status_ParseFailed, "Invalid magic number");
	}
	if ((status = ReadOp(BinaryData::OP_BEGIN)).IsNOK())
	{
		return status;
	}
	m_nCrArrayItem = BinaryData::ARRAY_ITEM;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status BinaryDataReader::End()
{
	UInt32 nHash;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_END)).IsNOK())
	{
		return status;
	}

	UInt32 nComputedHash;

	m_hash.Done(&nComputedHash);

	if ((status = Read(&nHash, sizeof(nHash))).IsNOK())
	{
		return status;
	}
	if (nComputedHash != nHash)
	{
		return Status(Status_ParseFailed, "Checksum mismatched");
	}

	m_stackStates.pop();

	return Status();
}

Status BinaryDataReader::BeginObjectObject(const Char *szName)
{
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_BEGINOBJECTOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status BinaryDataReader::EndObjectObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_ENDOBJECTOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataReader::BeginObjectArray(const Char *szName)
{
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_BEGINOBJECTARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status BinaryDataReader::EndObjectArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (BinaryData::ARRAY_END != m_nCrArrayItem)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_ENDOBJECTARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataReader::ReadObjectBool(const Char *szName, Bool *pbValue)
{
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_OBJECTBOOL)).IsNOK())
	{
		return status;
	}
	if ((status = Read(pbValue, sizeof(*pbValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataReader::ReadObjectInt(const Char *szName, Int64 *pnValue)
{
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_OBJECTINT)).IsNOK())
	{
		return status;
	}
	if ((status = Read(pnValue, sizeof(*pnValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataReader::ReadObjectReal(const Char *szName, Double *plfValue)
{
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_OBJECTREAL)).IsNOK())
	{
		return status;
	}
	if ((status = Read(plfValue, sizeof(*plfValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataReader::ReadObjectString(const Char *szName, String *psValue)
{
	Char   buffer[4096];
	Size   cLenRead;
	UInt32 cLenTmp;
	Size   cLen;
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	psValue->clear();
	if ((status = ReadOp(BinaryData::OP_OBJECTSTRING)).IsNOK())
	{
		return status;
	}
	if ((status = Read(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	cLen = (Size)cLenTmp;
	while (0 < cLen)
	{
		if (sizeof(buffer) / sizeof(buffer[0]) >= cLen)
		{
			cLenRead = cLen;
		}
		else
		{
			cLenRead = sizeof(buffer) / sizeof(buffer[0]);
		}
		if ((status = Read(buffer, cLenRead * sizeof(Char))).IsNOK())
		{
			return status;
		}
		psValue->append(buffer, cLenRead);
		cLen -= cLenRead;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataReader::ReadObjectWString(const Char *szName, WString *pwsValue)
{
	WChar  buffer[2048];
	Size   cLenRead;
	UInt32 cLenTmp;
	Size   cLen;
	Status status;

	szName;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	pwsValue->clear();
	if ((status = ReadOp(BinaryData::OP_OBJECTWSTRING)).IsNOK())
	{
		return status;
	}
	if ((status = Read(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	cLen = (Size)cLenTmp;
	while (0 < cLen)
	{
		if (sizeof(buffer) / sizeof(buffer[0]) >= cLen)
		{
			cLenRead = cLen;
		}
		else
		{
			cLenRead = sizeof(buffer) / sizeof(buffer[0]);
		}
		if ((status = Read(buffer, cLenRead * sizeof(WChar))).IsNOK())
		{
			return status;
		}
		pwsValue->append(buffer, cLenRead);
		cLen -= cLenRead;
	}	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataReader::BeginArrayObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_BEGINARRAYOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status BinaryDataReader::EndArrayObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_ENDARRAYOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataReader::BeginArrayArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}

	if ((status = ReadOp(BinaryData::OP_BEGINARRAYARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status BinaryDataReader::EndArrayArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (BinaryData::ARRAY_END != m_nCrArrayItem)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = ReadOp(BinaryData::OP_ENDARRAYARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status BinaryDataReader::ReadArrayBool(Bool *pbValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_ARRAYBOOL)).IsNOK())
	{
		return status;
	}
	if ((status = Read(pbValue, sizeof(*pbValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status BinaryDataReader::ReadArrayInt(Int64 *pnValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_ARRAYINT)).IsNOK())
	{
		return status;
	}
	if ((status = Read(pnValue, sizeof(*pnValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status BinaryDataReader::ReadArrayReal(Double *plfValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_ARRAYREAL)).IsNOK())
	{
		return status;
	}
	if ((status = Read(plfValue, sizeof(*plfValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status BinaryDataReader::ReadArrayString(String *psValue)
{
	Char   buffer[4096];
	Size   cLenRead;
	UInt32 cLenTmp;
	Size   cLen;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_ARRAYSTRING)).IsNOK())
	{
		return status;
	}
	psValue->clear();
	if ((status = Read(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	cLen = (Size)cLenTmp;
	while (0 < cLen)
	{
		if (sizeof(buffer) / sizeof(buffer[0]) >= cLen)
		{
			cLenRead = cLen;
		}
		else
		{
			cLenRead = sizeof(buffer) / sizeof(buffer[0]);
		}
		if ((status = Read(buffer, cLenRead * sizeof(Char))).IsNOK())
		{
			return status;
		}
		psValue->append(buffer, cLenRead);
		cLen -= cLenRead;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status BinaryDataReader::ReadArrayWString(WString *pwsValue)
{
	WChar  buffer[2048];
	Size   cLenRead;
	UInt32 cLenTmp;
	Size   cLen;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Read(&m_nCrArrayItem, sizeof(m_nCrArrayItem))).IsNOK())
	{
		return status;
	}
	if (BinaryData::ARRAY_END == m_nCrArrayItem)
	{
		return Status(Status_NoMoreItems, "End of array reached");
	}
	if ((status = ReadOp(BinaryData::OP_ARRAYWSTRING)).IsNOK())
	{
		return status;
	}
	pwsValue->clear();
	if ((status = Read(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	cLen = (Size)cLenTmp;
	while (0 < cLen)
	{
		if (sizeof(buffer) / sizeof(buffer[0]) >= cLen)
		{
			cLenRead = cLen;
		}
		else
		{
			cLenRead = sizeof(buffer) / sizeof(buffer[0]);
		}
		if ((status = Read(buffer, cLenRead * sizeof(WChar))).IsNOK())
		{
			return status;
		}
		pwsValue->append(buffer, cLenRead);
		cLen -= cLenRead;
	}
	m_stackStates.top().cCount++;

	return Status();
}

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

