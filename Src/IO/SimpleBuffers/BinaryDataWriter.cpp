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

#include "CX/IO/SimpleBuffers/BinaryDataWriter.hpp"
#include "CX/IO/SimpleBuffers/BinaryData.hpp"
#include "CX/Limits.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

BinaryDataWriter::BinaryDataWriter(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
}

BinaryDataWriter::~BinaryDataWriter()
{
}

Status BinaryDataWriter::Write(const void *pData, Size cbSize)
{
	Size   cbAckSize;
	Status status;

	if ((status = m_pOutputStream->Write(pData, cbSize, &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (cbAckSize != cbSize)
	{
		return Status(Status_WriteFailed, "Failed to write {1} bytes ({2} bytes writter)", cbSize, cbAckSize);
	}
	m_hash.Update(pData, cbAckSize);

	return Status();
}

Status BinaryDataWriter::WriteOp(UInt8 nOp)
{
	return m_hash.Update(&nOp, sizeof(nOp));
}

Status BinaryDataWriter::Begin()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.push(StateData(State_Object));
	m_hash.Init();
	if ((status = Write(&BinaryData::MAGIC, sizeof(BinaryData::MAGIC))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_BEGIN)).IsNOK())
	{
		return status;
	}

	return Status();
}

Status BinaryDataWriter::End()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_END)).IsNOK())
	{
		return status;
	}

	UInt32 nHash;

	m_hash.Done(&nHash);

	if ((status = Write(&nHash, sizeof(nHash))).IsNOK())
	{
		return status;
	}

	m_stackStates.pop();
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}

	return Status();
}

Status BinaryDataWriter::BeginObjectObject(const Char *szName)
{
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_BEGINOBJECTOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status BinaryDataWriter::EndObjectObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_ENDOBJECTOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataWriter::BeginObjectArray(const Char *szName)
{
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_BEGINOBJECTARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status BinaryDataWriter::EndObjectArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_END, sizeof(BinaryData::ARRAY_END))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ENDOBJECTARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataWriter::WriteObjectBool(const Char *szName, Bool bValue)
{
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_OBJECTBOOL)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&bValue, sizeof(bValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteObjectInt(const Char *szName, Int64 nValue)
{
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_OBJECTINT)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&nValue, sizeof(nValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteObjectReal(const Char *szName, Double lfValue)
{
	Status status;
	
	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_OBJECTREAL)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&lfValue, sizeof(lfValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteObjectString(const Char *szName, const Char *szValue)
{
	Size   cLen;
	UInt32 cLenTmp;
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_OBJECTSTRING)).IsNOK())
	{
		return status;
	}
	cLen = cx_strlen(szValue);
	if ((Size)TYPE_UINT32_MAX < cLen)
	{
		return Status(Status_TooBig, "Value too long ({1} chars)", cLen);
	}
	cLenTmp = (UInt32)cLen;
	if ((status = Write(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	if ((status = Write(szValue, cLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteObjectWString(const Char *szName, const WChar *wszValue)
{
	Size   cLen;
	UInt32 cLenTmp;
	Status status;

	szName;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_OBJECTWSTRING)).IsNOK())
	{
		return status;
	}
	cLen = cxw_strlen(wszValue);
	if ((Size)TYPE_UINT32_MAX < cLen)
	{
		return Status(Status_TooBig, "Value too long ({1} chars)", cLen);
	}
	cLenTmp = (UInt32)cLen;
	if ((status = Write(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	if ((status = Write(wszValue, cLen * sizeof(WChar))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::BeginArrayObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_BEGINARRAYOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status BinaryDataWriter::EndArrayObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = WriteOp(BinaryData::OP_ENDARRAYOBJECT)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataWriter::BeginArrayArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_BEGINARRAYARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataWriter::EndArrayArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_END, sizeof(BinaryData::ARRAY_END))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ENDARRAYARRAY)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BinaryDataWriter::WriteArrayBool(Bool bValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ARRAYBOOL)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&bValue, sizeof(bValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteArrayInt(Int64 nValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ARRAYINT)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&nValue, sizeof(nValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteArrayReal(Double lfValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ARRAYREAL)).IsNOK())
	{
		return status;
	}
	if ((status = Write(&lfValue, sizeof(lfValue))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteArrayString(const Char *szValue)
{
	Size   cLen;
	UInt32 cLenTmp;
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ARRAYSTRING)).IsNOK())
	{
		return status;
	}
	cLen = cx_strlen(szValue);
	if ((Size)TYPE_UINT32_MAX < cLen)
	{
		return Status(Status_TooBig, "Value too long ({1} chars)", cLen);
	}
	cLenTmp = (UInt32)cLen;
	if ((status = Write(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	if ((status = Write(szValue, cLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BinaryDataWriter::WriteArrayWString(const WChar *wszValue)
{
	Size   cLen;
	UInt32 cLenTmp;
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
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
	if ((status = Write(&BinaryData::ARRAY_ITEM, sizeof(BinaryData::ARRAY_ITEM))).IsNOK())
	{
		return status;
	}
	if ((status = WriteOp(BinaryData::OP_ARRAYWSTRING)).IsNOK())
	{
		return status;
	}
	cLen = cxw_strlen(wszValue);
	if ((Size)TYPE_UINT32_MAX < cLen)
	{
		return Status(Status_TooBig, "Value too long ({1} chars)", cLen);
	}
	cLenTmp = (UInt32)cLen;
	if ((status = Write(&cLenTmp, sizeof(cLenTmp))).IsNOK())
	{
		return status;
	}
	if ((status = Write(wszValue, cLen * sizeof(WChar))).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

