/* 
 * CX - C++ framework for general purpose development
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

#include "CX/SB/BinaryDataWriter.hpp"


namespace CX
{

namespace SB
{

BinaryDataWriter::BinaryDataWriter(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
}

BinaryDataWriter::~BinaryDataWriter()
{
}

void BinaryDataWriter::WriteState(Byte nState)
{
	m_chksum.Update(&nState, sizeof(nState));
}

Status BinaryDataWriter::BeginMember(const Char *szName)
{
	Status status;

	szName;
	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.top().cIndex++;
	if (m_stackStates.top().cIndex > m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "Too many members");
	}
	m_stackStates.push(StateData(State_Member));
	WriteState(BinaryData::STATE_BEGINMEMBER);

	return Status();
}

Status BinaryDataWriter::EndMember()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Member != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	WriteState(BinaryData::STATE_ENDMEMBER);

	return Status();
}

Status BinaryDataWriter::BeginItem()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.top().cIndex++;
	if (m_stackStates.top().cIndex > m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "Too many items");
	}
	m_stackStates.push(StateData(State_Item));
	WriteState(BinaryData::STATE_BEGINITEM);

	return Status();
}

Status BinaryDataWriter::EndItem()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Item != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array!= m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	WriteState(BinaryData::STATE_ENDITEM);

	return Status();
}

Status BinaryDataWriter::BeginObject(Size cCount)
{
	UInt32 cCountTmp;
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (BinaryData::MAX_OBJECT_ITEMS < cCount)
	{
		return Status(Status_TooBig, "Too many object members (max is {1})", BinaryData::MAX_OBJECT_ITEMS);
	}
	if (m_stackStates.empty())
	{
		if ((status = Write(BinaryData::MAGIC)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(StateData(State_Object, cCount));
	cCountTmp = (UInt32)cCount;
	WriteState(BinaryData::STATE_BEGINOBJECT);
	if ((status = Write(cCountTmp)).IsNOK())
	{
		return status;
	}

	return Status();
}

Status BinaryDataWriter::EndObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "More members are expected");
	}
	m_stackStates.pop();
	WriteState(BinaryData::STATE_ENDOBJECT);
	if (m_stackStates.empty())
	{
		BinaryData::CheckSumType data;

		m_chksum.Done(&data);
		if ((status = Write(data)).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status BinaryDataWriter::BeginArray(Size cCount)
{
	UInt32 cCountTmp;
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (BinaryData::MAX_ARRAY_ITEMS < cCount)
	{
		return Status(Status_TooBig, "Too many array items (max is {1})", BinaryData::MAX_ARRAY_ITEMS);
	}
	if (m_stackStates.empty())
	{
		if ((status = Write(BinaryData::MAGIC)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(StateData(State_Array, cCount));
	cCountTmp = (UInt32)cCount;
	WriteState(BinaryData::STATE_BEGINARRAY);
	if ((status = Write(cCountTmp)).IsNOK())
	{
		return status;
	}

	return Status();
}

Status BinaryDataWriter::EndArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "More items are expected");
	}
	m_stackStates.pop();
	WriteState(BinaryData::STATE_ENDARRAY);
	if (m_stackStates.empty())
	{
		BinaryData::CheckSumType data;

		m_chksum.Done(&data);
		if ((status = Write(data)).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status BinaryDataWriter::WriteBool(Bool bValue)
{
	WriteState(BinaryData::STATE_BOOL);

	return Write(bValue);
}

Status BinaryDataWriter::WriteInt8(Int8 nValue)
{
	WriteState(BinaryData::STATE_INT8);

	return Write(nValue);
}

Status BinaryDataWriter::WriteUInt8(UInt8 uValue)
{
	WriteState(BinaryData::STATE_UINT8);

	return Write(uValue);
}

Status BinaryDataWriter::WriteInt16(Int16 nValue)
{
	WriteState(BinaryData::STATE_INT16);

	return Write(nValue);
}

Status BinaryDataWriter::WriteUInt16(UInt16 uValue)
{
	WriteState(BinaryData::STATE_UINT16);

	return Write(uValue);
}

Status BinaryDataWriter::WriteInt32(Int32 nValue)
{
	WriteState(BinaryData::STATE_INT32);

	return Write(nValue);
}

Status BinaryDataWriter::WriteUInt32(UInt32 uValue)
{
	WriteState(BinaryData::STATE_UINT32);

	return Write(uValue);
}

Status BinaryDataWriter::WriteInt64(Int64 nValue)
{
	WriteState(BinaryData::STATE_INT64);

	return Write(nValue);
}

Status BinaryDataWriter::WriteUInt64(UInt64 uValue)
{
	WriteState(BinaryData::STATE_UINT32);

	return Write(uValue);
}

Status BinaryDataWriter::WriteFloat(Float fValue)
{
	WriteState(BinaryData::STATE_FLOAT);

	return Write(fValue);
}

Status BinaryDataWriter::WriteDouble(Double lfValue)
{
	WriteState(BinaryData::STATE_DOUBLE);

	return Write(lfValue);
}

Status BinaryDataWriter::WriteString(const Char *szValue)
{
	WriteState(BinaryData::STATE_STRING);

	return Write(szValue);
}

Status BinaryDataWriter::WriteWString(const WChar *wszValue)
{
	WriteState(BinaryData::STATE_WSTRING);

	return Write(wszValue);
}

}//namespace SB

}//namespace CX

