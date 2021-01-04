/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/Stack.hpp"
#include "CX/Print.hpp"
#include "CX/Str/Z85BinStr.hpp"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API JSONWriter : public IWriter
{
public:

	JSONWriter();

	~JSONWriter();

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

	virtual Status WriteCustom(ICustom::Type nType, const void *pData, const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	static const Size INDENT_TAB_SIZE = 3;

	enum State
	{
		State_Root,
		State_Object,
		State_Array,
	};

	struct StateData
	{
		StateData()
		{
		}

		StateData(State nState)
		{
			this->nState = nState;
			this->cElems = 0;
		}

		State nState;
		Size  cElems;

	};

	typedef Stack<StateData>::Type  StatesStack;

#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
	String              m_sIndent;
#pragma warning(pop)
	IO::IOutputStream   *m_pOutputStream;
	Size                m_cIndent;

	template <typename T>
	static Status WriteFunc(IO::IOutputStream *pOutputStream, const T &v, const Char *szName)
	{
		if (NULL != szName)
		{
			return Print(pOutputStream, "\"{1}\": {2}", szName, v);
		}
		else
		{
			return Print(pOutputStream, "{1}", v);
		}
	}

	template <typename T>
	Status Write(Status (* WriteFuncProc)(IO::IOutputStream *, const T &, const Char *szName), const T &v, 
	             const Char *szName = NULL)
	{
		if (m_stackStates.empty())
		{
			return Status(Status_InvalidCall, "Begin was not called");
		}

		Status status;

		if (State_Object == m_stackStates.top().nState)
		{
			if (NULL == szName)
			{
				return Status(Status_InvalidCall, "Member must have a name");
			}
		}
		else
		if (State_Array == m_stackStates.top().nState)
		{
			if (NULL != szName)
			{
				return Status(Status_InvalidCall, "Item must not have a name");
			}
		}
		else
		{
			return Status(Status_InvalidCall, "Data must be placed inside objects or arrays");
		}
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = WriteFuncProc(m_pOutputStream, v, szName)).IsNOK())
		{
			return status;
		}
		m_stackStates.top().cElems++;

		return Status();
	}

	Status PrintFieldsSeparator(bool bEnd);

	Status PrintIndent();

};

template <>
inline Status JSONWriter::WriteFunc<Bool>(IO::IOutputStream *pOutputStream, const Bool &v, const Char *szName)
{
	if (NULL != szName)
	{
		return Print(pOutputStream, "\"{1}\": {2}", szName, v ? "true" : "false");
	}
	else
	{
		return Print(pOutputStream, "{1}", v ? "true" : "false");
	}
}

template <>
inline Status JSONWriter::WriteFunc<String>(IO::IOutputStream *pOutputStream, const String &v, const Char *szName)
{
	String sStr;

	Data::JSON::SAXParser::EscapeString(v.c_str(), &sStr);
	if (NULL != szName)
	{
		return Print(pOutputStream, "\"{1}\": \"{2}\"", szName, sStr);
	}
	else
	{
		return Print(pOutputStream, "\"{1}\"", sStr);
	}
}

}//namespace SimpleBuffers

}//namespace CX
