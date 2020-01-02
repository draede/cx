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


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"
#include "bson.h"


namespace CX
{

namespace SimpleBuffers
{

class CX_API BSONReader : public IReader
{
public:

	struct CustomData
	{
		void *pData;
		Size cbSize;
	};

	BSONReader();

	~BSONReader();

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

	virtual Status ReadCustom(ICustom::Type nType, void *pData, ICustom::Allocator *pAllocator, 
	                          const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	static const Size TMP_STR_BUFFER_SIZE  = 4096;

	enum State
	{
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
		}

		State       nState;
		bson_iter_t iter;
		bson_iter_t iterStart;
	};

	typedef Stack<StateData>::Type StatesStack;

	bson_t             *m_pBSON;
	uint8_t            *m_pData;
	IO::IInputStream   *m_pInputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(pop)

	Status PrepareRead(const Char *szName, bson_type_t *pnType);

};

}//namespace SimpleBuffers

}//namespace CX

