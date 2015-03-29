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

#pragma once


#include "CX/SB/IDataReader.hpp"
#include "CX/SB/Writers.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/Stack.hpp"
#include "CX/SB/BinaryData.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SB
{

class CX_API BinaryDataReader : public IDataReader
{
public:

	BinaryDataReader(IO::IInputStream *pInputStream);

	~BinaryDataReader();

	virtual Status BeginMember(const Char *szName);

	virtual Status EndMember();

	virtual Status BeginItem();

	virtual Status EndItem();

	virtual Status BeginObject(Size *pcCount);

	virtual Status EndObject();

	virtual Status BeginArray(Size *pcCount);

	virtual Status EndArray();

	virtual Status ReadBool(Bool *pbValue);

	virtual Status ReadInt8(Int8 *pnValue);

	virtual Status ReadUInt8(UInt8 *puValue);

	virtual Status ReadInt16(Int16 *pnValue);

	virtual Status ReadUInt16(UInt16 *puValue);

	virtual Status ReadInt32(Int32 *pnValue);

	virtual Status ReadUInt32(UInt32 *puValue);

	virtual Status ReadInt64(Int64 *pnValue);

	virtual Status ReadUInt64(UInt64 *puValue);

	virtual Status ReadFloat(Float *pfValue);

	virtual Status ReadDouble(Double *plfValue);

	virtual Status ReadString(String *psValue);

	virtual Status ReadWString(WString *pwsValue);

private:

	enum State
	{
		State_Object,
		State_Array,
		State_Member,
		State_Item,
	};

	typedef struct _StateData
	{
		_StateData()
		{
		}

		_StateData(State nState)
		{
			this->nState = nState;
			this->cCount = 0;
			this->cIndex = 0;
		}

		_StateData(State nState, Size cCount)
		{
			this->nState = nState;
			this->cCount = cCount;
			this->cIndex = 0;
		}

		State nState;
		Size  cCount;
		Size  cIndex;
	}StateData;

	typedef Stack<StateData>::Type   StatesStack;

	BinaryData::CheckSum   m_chksum;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack            m_stackStates;
#pragma warning(push)
	IO::IInputStream       *m_pInputStream;

	void ReadState(Byte nState);

	template <typename T>
	Status Read(T *p)
	{
		Size   cbAckSize;
		Status status;

		if (NULL == m_pInputStream || !m_pInputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid input stream");
		}
		if ((status = m_pInputStream->Read(p, sizeof(*p), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(*p) != cbAckSize)
		{
			return Status(Status_ReadFailed, "Failed to read {1} bytes", sizeof(*p));
		}
		m_chksum.Update(p, sizeof(*p));

		return Status();
	}

	template <>
	Status Read<String>(String *p)
	{
		Char   buffer[8192];
		Size   cLen;
		UInt32 cLenTmp;
		Size   cReqLen;
		Size   cbAckSize;
		Status status;

		if (NULL == m_pInputStream || !m_pInputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid input stream");
		}
		if ((status = m_pInputStream->Read(&cLenTmp, sizeof(cLenTmp), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(cLenTmp) != cbAckSize)
		{
			return Status(Status_ReadFailed, "Failed to read {1} bytes", sizeof(cLenTmp));
		}
		m_chksum.Update(&cLenTmp, sizeof(cLenTmp));
		cLen = (Size)cLenTmp;
		if (BinaryData::MAX_STRING_LEN < cLen)
		{
			return Status(Status_TooBig, "String too big (max len is {1})", BinaryData::MAX_STRING_LEN);
		}
		p->clear();
		while (0 < cLen)
		{
			cReqLen = sizeof(buffer) / sizeof(buffer[0]);
			if (cReqLen > cLen)
			{
				cReqLen = cLen;
			}
			if ((status = m_pInputStream->Read(buffer, cReqLen * sizeof(Char), &cbAckSize)).IsNOK())
			{
				return status;
			}
			if (cReqLen * sizeof(Char) != cbAckSize)
			{
				return Status(Status_ReadFailed, "Failed to read {1} bytes", cReqLen * sizeof(Char));
			}
			m_chksum.Update(buffer, cReqLen * sizeof(Char));
			p->append(buffer, cReqLen);
			cLen -= cReqLen;
		}

		return Status();
	}

	template <>
	Status Read<WString>(WString *p)
	{
		WChar  buffer[4096];
		Size   cLen;
		UInt32 cLenTmp;
		Size   cReqLen;
		Size   cbAckSize;
		Status status;

		if (NULL == m_pInputStream || !m_pInputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid input stream");
		}
		if ((status = m_pInputStream->Read(&cLenTmp, sizeof(cLenTmp), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(cLenTmp) != cbAckSize)
		{
			return Status(Status_ReadFailed, "Failed to read {1} bytes", sizeof(cLenTmp));
		}
		m_chksum.Update(&cLenTmp, sizeof(cLenTmp));
		cLen = (Size)cLenTmp;
		if (BinaryData::MAX_STRING_LEN < cLen)
		{
			return Status(Status_TooBig, "String too big (max len is {1})", BinaryData::MAX_STRING_LEN);
		}
		p->clear();
		while (0 < cLen)
		{
			cReqLen = sizeof(buffer) / sizeof(buffer[0]);
			if (cReqLen > cLen)
			{
				cReqLen = cLen;
			}
			if ((status = m_pInputStream->Read(buffer, cReqLen * sizeof(WChar), &cbAckSize)).IsNOK())
			{
				return status;
			}
			if (cReqLen * sizeof(WChar) != cbAckSize)
			{
				return Status(Status_ReadFailed, "Failed to read {1} bytes", cReqLen * sizeof(WChar));
			}
			m_chksum.Update(buffer, cReqLen * sizeof(WChar));
			p->append(buffer, cReqLen);
			cLen -= cReqLen;
		}

		return Status();
	}

};

}//namespace SB

}//namespace CX

