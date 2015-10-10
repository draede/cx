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
 
#include "CX/SimpleBuffers/BINWriter.hpp"


namespace CX
{

namespace SimpleBuffers
{

BINWriter::BINWriter()
{
}

BINWriter::~BINWriter()
{
}

Status BINWriter::Write(const void *pData, Size cbSize)
{
	Size   cbAckSize;
	Status status;

	if (!(status = m_pOutputStream->Write(pData, cbSize, &cbAckSize)))
	{
		return status;
	}
	if (cbAckSize != cbSize)
	{
		return Status_WriteFailed;
	}

	return Status();
}

Status BINWriter::WriteSize(Size cbSize, BINRW::RecordType nBaseRecordType)
{
	Status status;

	if ((Size)TYPE_UINT8_MAX >= cbSize)
	{
		UInt8 cbSizeEx = (UInt8)cbSize;

		if (!(status = Write(&nBaseRecordType, sizeof(nBaseRecordType))))
		{
			return status;
		}
		if (!(status = Write(&cbSizeEx, sizeof(cbSizeEx))))
		{
			return status;
		}
	}
	else
	if ((Size)TYPE_UINT16_MAX >= cbSize)
	{
		UInt16 cbSizeEx = (UInt16)cbSize;

		nBaseRecordType += 1;
		if (!(status = Write(&nBaseRecordType, sizeof(nBaseRecordType))))
		{
			return status;
		}
		if (!(status = Write(&cbSizeEx, sizeof(cbSizeEx))))
		{
			return status;
		}
	}
	else
	{
		UInt32 cbSizeEx = (UInt32)cbSize;

		nBaseRecordType += 2;
		if (!(status = Write(&nBaseRecordType, sizeof(nBaseRecordType))))
		{
			return status;
		}
		if (!(status = Write(&cbSizeEx, sizeof(cbSizeEx))))
		{
			return status;
		}
	}

	return Status();
}

Status BINWriter::WriteStr(const String &sString)
{
	Status status;

	if (BINRW::MAX_STRING_LEN < sString.size())
	{
		return Status_InvalidArg;
	}
	if (!(status = WriteSize(sString.size() * sizeof(Char), BINRW::RECORD_STRING_1)))
	{
		return status;
	}
	if (!(status = Write(sString.c_str(), sString.size() * sizeof(Char))))
	{
		return status;
	}

	return Status();
}

Status BINWriter::Begin(IO::IOutputStream *pOutputStream)
{
	Status status;

	while (!m_stackStates.empty())
	{
		m_stackStates.pop();
	}
	m_pOutputStream = pOutputStream;
	if (!(status = Write(&BINRW::BEGIN_MAGIC, sizeof(BINRW::BEGIN_MAGIC))))
	{
		return status;
	}
	m_stackStates.push(State_Bin);

	return Status();
}

Status BINWriter::End()
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Bin != m_stackStates.top())
	{
		return Status_InvalidCall;
	}
	m_stackStates.pop();
	if (!m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (!(status = Write(&BINRW::END_MAGIC, sizeof(BINRW::END_MAGIC))))
	{
		return status;
	}

	return Status();
}

Status BINWriter::WriteBool(Bool v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_BOOL, szName);
}

Status BINWriter::WriteInt8(Int8 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_INT8, szName);
}

Status BINWriter::WriteUInt8(UInt8 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_UINT8, szName);
}

Status BINWriter::WriteInt16(Int16 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_INT16, szName);
}

Status BINWriter::WriteUInt16(UInt16 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_UINT16, szName);
}

Status BINWriter::WriteInt32(Int32 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_INT32, szName);
}

Status BINWriter::WriteUInt32(UInt32 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_UINT32, szName);
}

Status BINWriter::WriteInt64(Int64 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_INT64, szName);
}

Status BINWriter::WriteUInt64(UInt64 v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_UINT64, szName);
}

Status BINWriter::WriteFloat(Float v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_FLOAT, szName);
}

Status BINWriter::WriteDouble(Double v, const Char *szName/* = NULL*/)
{
	return WriteScalar(v, BINRW::RECORD_DOUBLE, szName);
}

Status BINWriter::WriteString(const String &v, const Char *szName/* = NULL*/)
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (!(status = WriteStr(v)))
	{
		return status;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		if (!(status = WriteStr(szName)))
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
	}
	else
	{
		return Status_InvalidCall;
	}


	return Status();
}

Status BINWriter::WriteBLOB(const void *pData, Size cbSize, const Char *szName/* = NULL*/)
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (BINRW::MAX_BLOB_SIZE < cbSize)
	{
		return Status_InvalidArg;
	}
	if (!(status = WriteSize(cbSize, BINRW::RECORD_BLOB_1)))
	{
		return status;
	}
	if (!(status = Write(pData, cbSize)))
	{
		return status;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		if (!(status = WriteStr(szName)))
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
	}
	else
	{
		return Status_InvalidCall;
	}

	return Status();
}

Status BINWriter::BeginObject(const Char *szName/* = NULL*/)
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (!(status = Write(&BINRW::RECORD_BEGIN_OBJECT, sizeof(BINRW::RECORD_BEGIN_OBJECT))))
	{
		return status;
	}
	if (State_Bin == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
	}
	else
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		if (!(status = WriteStr(szName)))
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
	}
	else
	{
		return Status_InvalidCall;
	}
	m_stackStates.push(State_Object);

	return Status();
}

Status BINWriter::EndObject()
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object != m_stackStates.top())
	{
		return Status_InvalidCall;
	}
	if (!(status = Write(&BINRW::RECORD_END_OBJECT, sizeof(BINRW::RECORD_END_OBJECT))))
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status BINWriter::BeginArray(const Char *szName/* = NULL*/)
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (!(status = Write(&BINRW::RECORD_BEGIN_ARRAY, sizeof(BINRW::RECORD_BEGIN_ARRAY))))
	{
		return status;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		if (!(status = WriteStr(szName)))
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
	}
	else
	{
		return Status_InvalidCall;
	}
	m_stackStates.push(State_Array);

	return Status();
}

Status BINWriter::EndArray()
{
	Status status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Array != m_stackStates.top())
	{
		return Status_InvalidCall;
	}
	if (!(status = Write(&BINRW::RECORD_END_ARRAY, sizeof(BINRW::RECORD_END_ARRAY))))
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
