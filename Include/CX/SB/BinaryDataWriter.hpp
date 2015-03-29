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


#include "CX/SB/IDataWriter.hpp"
#include "CX/SB/Writers.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Stack.hpp"
#include "CX/SB/BinaryData.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SB
{

class CX_API BinaryDataWriter : public IDataWriter
{
public:

	BinaryDataWriter(IO::IOutputStream *pOutputStream);

	~BinaryDataWriter();

	virtual Status BeginMember(const Char *szName);

	virtual Status EndMember();

	virtual Status BeginItem();

	virtual Status EndItem();

	virtual Status BeginObject(Size cCount);

	virtual Status EndObject();

	virtual Status BeginArray(Size cCount);

	virtual Status EndArray();

	virtual Status WriteBool(Bool bValue);

	virtual Status WriteInt8(Int8 nValue);

	virtual Status WriteUInt8(UInt8 uValue);

	virtual Status WriteInt16(Int16 nValue);

	virtual Status WriteUInt16(UInt16 uValue);

	virtual Status WriteInt32(Int32 nValue);

	virtual Status WriteUInt32(UInt32 uValue);

	virtual Status WriteInt64(Int64 nValue);

	virtual Status WriteUInt64(UInt64 uValue);

	virtual Status WriteFloat(Float fValue);

	virtual Status WriteDouble(Double lfValue);

	virtual Status WriteString(const Char *szValue);

	virtual Status WriteWString(const WChar *wszValue);

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
#pragma warning(pop)
	IO::IOutputStream      *m_pOutputStream;

	void WriteState(Byte nState);

	template <typename T>
	Status Write(T p)
	{
		Size   cbAckSize;
		Status status;

		if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid output stream");
		}
		if ((status = m_pOutputStream->Write(&p, sizeof(p), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(p) != cbAckSize)
		{
			return Status(Status_WriteFailed, "Failed to write {1} bytes", sizeof(p));
		}
		m_chksum.Update(&p, sizeof(p));

		return Status();
	}

	template <>
	Status Write<const Char *>(const Char *p)
	{
		Size   cLen;
		UInt32 cLenTmp;
		Size   cbAckSize;
		Status status;

		if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid output stream");
		}
		cLen = cx_strlen(p);
		if (BinaryData::MAX_STRING_LEN < cLen)
		{
			return Status(Status_TooBig, "String too big (max len is {1})", BinaryData::MAX_STRING_LEN);
		}
		cLenTmp = (UInt32)cLen;
		if ((status = m_pOutputStream->Write(&cLenTmp, sizeof(cLenTmp), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(cLenTmp) != cbAckSize)
		{
			return Status(Status_WriteFailed, "Failed to write {1} bytes", sizeof(cLenTmp));
		}
		m_chksum.Update(&cLenTmp, sizeof(cLenTmp));
		if ((status = m_pOutputStream->Write(p, sizeof(Char) * cLen, &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(Char) * cLen != cbAckSize)
		{
			return Status(Status_WriteFailed, "Failed to write {1} bytes", sizeof(Char) * cLen);
		}
		m_chksum.Update(p, sizeof(Char) * cLen);

		return Status();
	}

	template <>
	Status Write<const WChar *>(const WChar *p)
	{
		Size   cLen;
		UInt32 cLenTmp;
		Size   cbAckSize;
		Status status;

		if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid output stream");
		}
		cLen = cxw_strlen(p);
		if (BinaryData::MAX_STRING_LEN < cLen)
		{
			return Status(Status_TooBig, "String too big (max len is {1})", BinaryData::MAX_STRING_LEN);
		}
		cLenTmp = (UInt32)cLen;
		if ((status = m_pOutputStream->Write(&cLenTmp, sizeof(cLenTmp), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(cLenTmp) != cbAckSize)
		{
			return Status(Status_WriteFailed, "Failed to write {1} bytes", sizeof(cLenTmp));
		}
		m_chksum.Update(&cLenTmp, sizeof(cLenTmp));
		if ((status = m_pOutputStream->Write(p, sizeof(WChar) * cLen, &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (sizeof(WChar) * cLen != cbAckSize)
		{
			return Status(Status_WriteFailed, "Failed to write {1} bytes", sizeof(WChar) * cLen);
		}
		m_chksum.Update(p, sizeof(WChar) * cLen);

		return Status();
	}

};

}//namespace SB

}//namespace CX

