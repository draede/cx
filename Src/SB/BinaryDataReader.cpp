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

#include "CX/SB/BinaryDataReader.hpp"


namespace CX
{

namespace SB
{

BinaryDataReader::BinaryDataReader(IO::IInputStream *pInputStream)
{
	m_pInputStream = pInputStream;
}

BinaryDataReader::~BinaryDataReader()
{
}

void BinaryDataReader::ReadState(Byte nState)
{
	m_chksum.Update(&nState, sizeof(nState));
}

Status BinaryDataReader::BeginMember(const Char *szName)
{
	Status status;

	szName;
	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_BEGINMEMBER);

	return Status();
}

Status BinaryDataReader::EndMember()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_ENDMEMBER);

	return Status();
}

Status BinaryDataReader::BeginItem()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_BEGINITEM);

	return Status();
}

Status BinaryDataReader::EndItem()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_ENDITEM);

	return Status();
}

Status BinaryDataReader::BeginObject(Size *pcCount)
{
	BinaryData::MagicType nMagic;
	UInt32                cCountTmp;
	Status                status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
	}
	if (m_stackStates.empty())
	{
		if ((status = Read(&nMagic)).IsNOK())
		{
			return status;
		}
		if (BinaryData::MAGIC != nMagic)
		{
			return Status(Status_TooBig, "Invalid magic number", BinaryData::MAX_OBJECT_ITEMS);
		}
	}
	ReadState(BinaryData::STATE_BEGINOBJECT);
	if ((status = Read(&cCountTmp)).IsNOK())
	{
		return status;
	}
	if (BinaryData::MAX_OBJECT_ITEMS < cCountTmp)
	{
		return Status(Status_TooBig, "Too many object members (max is {1})", BinaryData::MAX_OBJECT_ITEMS);
	}
	*pcCount = (Size)cCountTmp;
	m_stackStates.push(StateData(State_Object, *pcCount));

	return Status();
}

Status BinaryDataReader::EndObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_ENDOBJECT);
	if (m_stackStates.empty())
	{
		BinaryData::CheckSumType data1;
		BinaryData::CheckSumType data2;

		m_chksum.Done(&data1);
		if ((status = Read(&data2)).IsNOK())
		{
			return status;
		}
		if (data1 != data2)
		{
			return Status(Status_ParseFailed, "Checksums mismatched");
		}
	}

	return Status();
}

Status BinaryDataReader::BeginArray(Size *pcCount)
{
	BinaryData::MagicType nMagic;
	UInt32                cCountTmp;
	Status                status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
	}
	if (m_stackStates.empty())
	{
		if ((status = Read(&nMagic)).IsNOK())
		{
			return status;
		}
		if (BinaryData::MAGIC != nMagic)
		{
			return Status(Status_TooBig, "Invalid magic number", BinaryData::MAX_OBJECT_ITEMS);
		}
	}
	ReadState(BinaryData::STATE_BEGINARRAY);
	if ((status = Read(&cCountTmp)).IsNOK())
	{
		return status;
	}
	if (BinaryData::MAX_ARRAY_ITEMS < cCountTmp)
	{
		return Status(Status_TooBig, "Too many array items (max is {1})", BinaryData::MAX_ARRAY_ITEMS);
	}
	*pcCount = (Size)cCountTmp;
	m_stackStates.push(StateData(State_Array, *pcCount));

	return Status();
}

Status BinaryDataReader::EndArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid input stream");
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
	ReadState(BinaryData::STATE_ENDARRAY);
	if (m_stackStates.empty())
	{
		BinaryData::CheckSumType data1;
		BinaryData::CheckSumType data2;

		m_chksum.Done(&data1);
		if ((status = Read(&data2)).IsNOK())
		{
			return status;
		}
		if (data1 != data2)
		{
			return Status(Status_ParseFailed, "Checksums mismatched");
		}
	}

	return Status();
}

Status BinaryDataReader::ReadBool(Bool *pbValue)
{
	ReadState(BinaryData::STATE_BOOL);

	return Read(pbValue);
}

Status BinaryDataReader::ReadInt8(Int8 *pnValue)
{
	ReadState(BinaryData::STATE_INT8);

	return Read(pnValue);
}

Status BinaryDataReader::ReadUInt8(UInt8 *puValue)
{
	ReadState(BinaryData::STATE_UINT8);

	return Read(puValue);
}

Status BinaryDataReader::ReadInt16(Int16 *pnValue)
{
	ReadState(BinaryData::STATE_INT16);

	return Read(pnValue);
}

Status BinaryDataReader::ReadUInt16(UInt16 *puValue)
{
	ReadState(BinaryData::STATE_UINT16);

	return Read(puValue);
}

Status BinaryDataReader::ReadInt32(Int32 *pnValue)
{
	ReadState(BinaryData::STATE_INT32);

	return Read(pnValue);
}

Status BinaryDataReader::ReadUInt32(UInt32 *puValue)
{
	ReadState(BinaryData::STATE_UINT32);

	return Read(puValue);
}

Status BinaryDataReader::ReadInt64(Int64 *pnValue)
{
	ReadState(BinaryData::STATE_INT64);

	return Read(pnValue);
}

Status BinaryDataReader::ReadUInt64(UInt64 *puValue)
{
	ReadState(BinaryData::STATE_UINT32);

	return Read(puValue);
}

Status BinaryDataReader::ReadFloat(Float *pfValue)
{
	ReadState(BinaryData::STATE_FLOAT);

	return Read(pfValue);
}

Status BinaryDataReader::ReadDouble(Double *plfValue)
{
	ReadState(BinaryData::STATE_DOUBLE);

	return Read(plfValue);
}

Status BinaryDataReader::ReadString(String *psValue)
{
	ReadState(BinaryData::STATE_STRING);

	return Read(psValue);
}

Status BinaryDataReader::ReadWString(WString *pwsValue)
{
	ReadState(BinaryData::STATE_WSTRING);

	return Read(pwsValue);
}

}//namespace SB

}//namespace CX

