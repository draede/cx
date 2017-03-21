/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/BINRW.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API BINReader : public IReader
{
public:

	BINReader();

	~BINReader();

	virtual Status Begin(IO::IInputStream *pInputStream);

	virtual Status End();

	virtual Status ReadBool(Bool &v, const Char *szName = NULL);

	virtual Status ReadInt8(Int8 &v, const Char *szName = NULL);

	virtual Status ReadUInt8(UInt8 &v, const Char *szName = NULL);

	virtual Status ReadInt16(Int16 &v, const Char *szName = NULL);

	virtual Status ReadUInt16(UInt16 &v, const Char *szName = NULL);

	virtual Status ReadInt32(Int32 &v, const Char *szName = NULL);

	virtual Status ReadUInt32(UInt32 &v, const Char *szName = NULL);

	virtual Status ReadInt64(Int64 &v, const Char *szName = NULL);

	virtual Status ReadUInt64(UInt64 &v, const Char *szName = NULL);

	virtual Status ReadFloat(Float &v, const Char *szName = NULL);

	virtual Status ReadDouble(Double &v, const Char *szName = NULL);

	virtual Status ReadString(String &v, const Char *szName = NULL);

	virtual Status ReadBLOB(BLOB &v, const Char *szName = NULL);

	virtual Status ReadCustom(ICustom *pCustom, const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	static const Size TMP_STR_BUFFER_SIZE  = 4096;

	enum State
	{
		State_Bin,
		State_Object,
		State_Array,
	};

	typedef Stack<State>::Type StatesStack;

	IO::IInputStream   *m_pInputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(pop)

	Status Read(void *pData, Size cbSize);

	Status ReadSize(Size &cbSize, BINRW::RecordType nBaseRecordType);

	Status ReadStr(String &sString);

	template <typename T>
	Status ReadScalar(T &v, BINRW::RecordType nReqRecordType, const Char *szName/* = NULL*/)
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
		if (nReqRecordType != nRecordType)
		{
			return Status_ParseFailed;
		}
		if (!(status = Read(&v, sizeof(v))))
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

};

}//namespace SimpleBuffers

}//namespace CX

