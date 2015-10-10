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
 
#include "CX/SimpleBuffers/BINReader.hpp"


namespace CX
{

namespace SimpleBuffers
{

BINReader::BINReader()
{
}

BINReader::~BINReader()
{
}

Status BINReader::Read(void *pData, Size cbSize)
{
	Size   cbAckSize;
	Status status;

	if (!(status = m_pInputStream->Read(pData, cbSize, &cbAckSize)))
	{
		return status;
	}
	if (cbAckSize != cbSize)
	{
		return Status_WriteFailed;
	}

	return Status();
}

Status BINReader::ReadSize(Size &cbSize, BINRW::RecordType nBaseRecordType)
{
	BINRW::RecordType nRecordType;
	Status            status;

	if (!(status = Read(&nRecordType, sizeof(nRecordType))))
	{
		return status;
	}
	if (nBaseRecordType == nRecordType)
	{
		UInt8 cbSizeTmp;

		if (!(status = Read(&cbSizeTmp, sizeof(cbSizeTmp))))
		{
			return status;
		}
		cbSize = cbSizeTmp;
	}
	else
	if (nBaseRecordType + 1 == nRecordType)
	{
		UInt16 cbSizeTmp;

		if (!(status = Read(&cbSizeTmp, sizeof(cbSizeTmp))))
		{
			return status;
		}
		cbSize = cbSizeTmp;
	}
	else
	if (nBaseRecordType + 2 == nRecordType)
	{
		UInt32 cbSizeTmp;

		if (!(status = Read(&cbSizeTmp, sizeof(cbSizeTmp))))
		{
			return status;
		}
		cbSize = cbSizeTmp;
	}
	else
	{
		return Status_ParseFailed;
	}

	return Status();
}

Status BINReader::ReadStr(String &sString)
{
	Size     cLen;
	Size     cbSize;
	Char     buffer[TMP_STR_BUFFER_SIZE];
	Status   status;

	if (!(status = ReadSize(cbSize, BINRW::RECORD_STRING_1)))
	{
		return status;
	}
	cLen = cbSize / sizeof(Char);
	if (BINRW::MAX_STRING_LEN < cLen)
	{
		return Status_ParseFailed;
	}
	sString.clear();
	while (0 < cLen)
	{
		Size cLenTmp;

		if (cLen > sizeof(buffer))
		{
			cLenTmp = sizeof(buffer);
		}
		else
		{
			cLenTmp = cLen;
		}
		if (!(status = Read(buffer, cLenTmp * sizeof(Char))))
		{
			return status;
		}
		sString.append(buffer, cLenTmp);
		cLen -= cLenTmp;
	}

	return Status();
}

Status BINReader::Begin(IO::IInputStream *pInputStream)
{
	BINRW::MagicType nMagic;
	Status           status;

	while (!m_stackStates.empty())
	{
		m_stackStates.pop();
	}
	m_pInputStream = pInputStream;
	m_stackStates.push(State_Bin);
	if (!(status = Read(&nMagic, sizeof(nMagic))))
	{
		return status;
	}
	if (BINRW::BEGIN_MAGIC != nMagic)
	{
		return Status_InvalidArg;
	}

	return Status();
}

Status BINReader::End()
{
	BINRW::MagicType nMagic;
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
	if (!(status = Read(&nMagic, sizeof(nMagic))))
	{
		return status;
	}
	if (BINRW::END_MAGIC != nMagic)
	{
		return Status_InvalidArg;
	}

	return Status();
}

Status BINReader::ReadBool(Bool &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_BOOL, szName);
}

Status BINReader::ReadInt8(Int8 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_INT8, szName);
}

Status BINReader::ReadUInt8(UInt8 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_UINT8, szName);
}

Status BINReader::ReadInt16(Int16 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_INT16, szName);
}

Status BINReader::ReadUInt16(UInt16 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_UINT16, szName);
}

Status BINReader::ReadInt32(Int32 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_INT32, szName);
}

Status BINReader::ReadUInt32(UInt32 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_UINT32, szName);
}

Status BINReader::ReadInt64(Int64 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_INT64, szName);
}

Status BINReader::ReadUInt64(UInt64 &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_UINT64, szName);
}

Status BINReader::ReadFloat(Float &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_FLOAT, szName);
}

Status BINReader::ReadDouble(Double &v, const Char *szName/* = NULL*/)
{
	return ReadScalar(v, BINRW::RECORD_DOUBLE, szName);
}

Status BINReader::ReadString(String &v, const Char *szName/* = NULL*/)
{
	Status   status;

	if (m_stackStates.empty())
	{
		return Status_InvalidArg;
	}
	if (State_Object != m_stackStates.top() && State_Array != m_stackStates.top())
	{
		return Status_InvalidArg;
	}
	if (!(status = ReadStr(v)))
	{
		return status;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}

		String sName;

		if (!(status = ReadStr(sName)))
		{
			return status;
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_stricmp(sName.c_str(), szName))
#pragma warning(pop)
		{
			return Status_ParseFailed;
		}
	}
	else
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
	}

	return Status();
}

Status BINReader::ReadBLOB(BLOB &v, const Char *szName/* = NULL*/)
{
	Size     cbSize;
	Status   status;

	if (m_stackStates.empty())
	{
		return Status_InvalidArg;
	}
	if (State_Object != m_stackStates.top() && State_Array != m_stackStates.top())
	{
		return Status_InvalidArg;
	}
	if (!(status = ReadSize(cbSize, BINRW::RECORD_BLOB_1)))
	{
		return status;
	}
	if (BINRW::MAX_BLOB_SIZE < cbSize)
	{
		return Status_ParseFailed;
	}
	v.resize(cbSize);
	if (!(status = Read(&v[0], cbSize)))
	{
		return status;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}

		String sName;

		if (!(status = ReadStr(sName)))
		{
			return status;
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_stricmp(sName.c_str(), szName))
#pragma warning(pop)
		{
			return Status_ParseFailed;
		}
	}
	else
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
	}

	return Status();
}

Status BINReader::BeginObject(const Char *szName/* = NULL*/)
{
	BINRW::RecordType nRecordType;
	Status            status;

	if (m_stackStates.empty())
	{
		return Status_InvalidArg;
	}
	if (State_Bin != m_stackStates.top() && State_Object != m_stackStates.top() && State_Array != m_stackStates.top())
	{
		return Status_InvalidArg;
	}
	if (!(status = Read(&nRecordType, sizeof(nRecordType))))
	{
		return status;
	}
	if (BINRW::RECORD_BEGIN_OBJECT != nRecordType)
	{
		return Status_ParseFailed;
	}
	if (State_Bin == m_stackStates.top())
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
	}
	else
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}

		String sName;

		if (!(status = ReadStr(sName)))
		{
			return status;
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_stricmp(sName.c_str(), szName))
#pragma warning(pop)
		{
			return Status_ParseFailed;
		}
	}
	else
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
	}
	m_stackStates.push(State_Object);

	return Status();
}

Status BINReader::EndObject()
{
	BINRW::RecordType nRecordType;
	Status            status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object != m_stackStates.top())
	{
		return Status_InvalidCall;
	}
	if (!(status = Read(&nRecordType, sizeof(nRecordType))))
	{
		return status;
	}
	if (BINRW::RECORD_END_OBJECT != nRecordType)
	{
		return Status_ParseFailed;
	}
	m_stackStates.pop();

	return Status();
}

Status BINReader::BeginArray(const Char *szName/* = NULL*/)
{
	BINRW::RecordType nRecordType;
	Status            status;

	if (m_stackStates.empty())
	{
		return Status_InvalidArg;
	}
	if (State_Object != m_stackStates.top() && State_Array != m_stackStates.top())
	{
		return Status_InvalidArg;
	}
	if (!(status = Read(&nRecordType, sizeof(nRecordType))))
	{
		return status;
	}
	if (BINRW::RECORD_BEGIN_OBJECT != nRecordType)
	{
		return Status_ParseFailed;
	}
	if (State_Object == m_stackStates.top())
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}

		String sName;

		if (!(status = ReadStr(sName)))
		{
			return status;
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_stricmp(sName.c_str(), szName))
#pragma warning(pop)
		{
			return Status_ParseFailed;
		}
	}
	else
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
	}
	m_stackStates.push(State_Object);

	return Status();
}

Status BINReader::EndArray()
{
	BINRW::RecordType nRecordType;
	Status            status;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Array != m_stackStates.top())
	{
		return Status_InvalidCall;
	}
	if (!(status = Read(&nRecordType, sizeof(nRecordType))))
	{
		return status;
	}
	if (BINRW::RECORD_END_OBJECT != nRecordType)
	{
		return Status_ParseFailed;
	}
	m_stackStates.pop();

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
