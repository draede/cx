/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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
#include "CX/APIDefs.hpp"
#include "bson.h"


namespace CX
{

namespace SimpleBuffers
{

class CX_API BSONWriter : public IWriter
{
public:

	BSONWriter();

	~BSONWriter();

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

	enum State
	{
		State_Object,
		State_Array,
	};

	struct StateData
	{
		StateData()
		{
			this->cCount = 0;
		}

		StateData(State nState)
		{
			this->nState = nState;
			this->cCount = 0;
		}

		State  nState;
		Size   cCount;
		bson_t *pBSON;

	};

	typedef Stack<StateData>::Type StatesStack;

	bson_t              *m_pBSON;
	IO::IOutputStream   *m_pOutputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(pop)

	Status PrepareWrite(const Char *szName, const Char **pszName);

};

}//namespace SimpleBuffers

}//namespace CX
