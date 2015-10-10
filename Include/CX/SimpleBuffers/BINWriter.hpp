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
 
#pragma once


#include "CX/SimpleBuffers/IWriter.hpp"
#include "CX/SimpleBuffers/BINRW.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API BINWriter : public IWriter, public IObject
{
public:

	BINWriter();

	~BINWriter();

	virtual Status Begin(IO::IOutputStream *pOutputStream);

	virtual Status End();

	virtual Status WriteBool(Bool v, const Char *szName = NULL);

	virtual Status WriteInt8(Int8 v, const Char *szName = NULL);

	virtual Status WriteUInt8(UInt8 v, const Char *szName = NULL);

	virtual Status WriteInt16(Int16 v, const Char *szName = NULL);

	virtual Status WriteUInt16(UInt16 v, const Char *szName = NULL);

	virtual Status WriteInt32(Int32 v, const Char *szName = NULL);

	virtual Status WriteUInt32(UInt32 v, const Char *szName = NULL);

	virtual Status WriteInt64(Int64 v, const Char *szName = NULL);

	virtual Status WriteUInt64(UInt64 v, const Char *szName = NULL);

	virtual Status WriteFloat(Float v, const Char *szName = NULL);

	virtual Status WriteDouble(Double v, const Char *szName = NULL);

	virtual Status WriteString(const String &v, const Char *szName = NULL);

	virtual Status WriteBLOB(const void *pData, Size cbSize, const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	enum State
	{
		State_Bin, 
		State_Object,
		State_Array,
	};

	typedef Stack<State>::Type StatesStack;

	IO::IOutputStream   *m_pOutputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(pop)

	Status Write(const void *pData, Size cbSize);

	Status WriteSize(Size cbSize, BINRW::RecordType nBaseRecordType);

	Status WriteStr(const String &sString);

	template <typename T>
	Status WriteScalar(T v, BINRW::RecordType nRecordType, const Char *szName = NULL)
	{
		Status status;

		if (m_stackStates.empty())
		{
			return Status_InvalidCall;
		}
		if (!(status = Write(&nRecordType, sizeof(nRecordType))))
		{
			return status;
		}
		if (!(status = Write(&v, sizeof(v))))
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

};

}//namespace SimpleBuffers

}//namespace CX
