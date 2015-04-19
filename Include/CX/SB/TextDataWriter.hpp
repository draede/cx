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


#include "CX/SB/IDataWriter.hpp"
#include "CX/SB/Writers.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SB
{

class CX_API TextDataWriter : public IDataWriter
{
public:

	TextDataWriter(IO::IOutputStream *pOutputStream);

	~TextDataWriter();

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

#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
	String              m_sIndent;
#pragma warning(pop)
	Size                m_cIndent;
	IO::IOutputStream   *m_pOutputStream;

	Status WriteIndent();

	template <typename T>
	inline Status WriteValue(T p)
	{
		if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
		{
			return Status(Status_NotInitialized, "Invalid output stream");
		}
		if (m_stackStates.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Member != m_stackStates.top().nState && State_Item != m_stackStates.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");
		}

		return Print(m_pOutputStream, "{1}", p);
	}

};

}//namespace SB

}//namespace CX

