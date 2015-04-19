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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace SB
{

class CX_API IDataReader : public IObject
{
public:

	virtual ~IDataReader() { }

	template <typename T>
	inline Status ReadMember(const Char *szName, T &p)
	{
		Status status;

		if ((status = BeginMember(szName)).IsNOK())
		{
			return status;
		}
		if ((status = Read(p, this)).IsNOK())
		{
			return status;
		}
		if ((status = EndMember()).IsNOK())
		{
			return status;
		}

		return Status();
	}

	template <typename T>
	inline Status ReadItem(T &p)
	{
		Status status;

		if ((status = BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = Read(p, this)).IsNOK())
		{
			return status;
		}
		if ((status = EndItem()).IsNOK())
		{
			return status;
		}

		return Status();
	}

	virtual Status BeginMember(const Char *szName) = 0;

	virtual Status EndMember() = 0;

	virtual Status BeginItem() = 0;

	virtual Status EndItem() = 0;

	virtual Status BeginObject(Size *pcCount) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(Size *pcCount) = 0;

	virtual Status EndArray() = 0;

	virtual Status ReadBool(Bool *pbValue) = 0;

	virtual Status ReadInt8(Int8 *pnValue) = 0;

	virtual Status ReadUInt8(UInt8 *puValue) = 0;

	virtual Status ReadInt16(Int16 *pnValue) = 0;

	virtual Status ReadUInt16(UInt16 *puValue) = 0;

	virtual Status ReadInt32(Int32 *pnValue) = 0;

	virtual Status ReadUInt32(UInt32 *puValue) = 0;

	virtual Status ReadInt64(Int64 *pnValue) = 0;

	virtual Status ReadUInt64(UInt64 *puValue) = 0;

	virtual Status ReadFloat(Float *lfValue) = 0;

	virtual Status ReadDouble(Double *plfValue) = 0;

	virtual Status ReadString(String *psValue) = 0;

	virtual Status ReadWString(WString *pwsValue) = 0;

};

}//namespace SB

}//namespace CX

