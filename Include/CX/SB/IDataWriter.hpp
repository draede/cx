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

class CX_API IDataWriter : public IObject
{
public:

	virtual ~IDataWriter() { }

	template <typename T>
	inline Status WriteMember(const Char *szName, T p)
	{
		Status status;

		if ((status = BeginMember(szName)).IsNOK())
		{
			return status;
		}
		if ((status = Write(p, this)).IsNOK())
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
	inline Status WriteItem(T p)
	{
		Status status;

		if ((status = BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = Write(p, this)).IsNOK())
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

	virtual Status BeginObject(Size cCount) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(Size cCount) = 0;

	virtual Status EndArray() = 0;

	virtual Status WriteBool(Bool bValue) = 0;

	virtual Status WriteInt8(Int8 nValue) = 0;

	virtual Status WriteUInt8(UInt8 uValue) = 0;

	virtual Status WriteInt16(Int16 nValue) = 0;

	virtual Status WriteUInt16(UInt16 uValue) = 0;

	virtual Status WriteInt32(Int32 nValue) = 0;

	virtual Status WriteUInt32(UInt32 uValue) = 0;

	virtual Status WriteInt64(Int64 nValue) = 0;

	virtual Status WriteUInt64(UInt64 uValue) = 0;

	virtual Status WriteFloat(Float fValue) = 0;

	virtual Status WriteDouble(Double lfValue) = 0;

	virtual Status WriteString(const Char *szValue) = 0;

	virtual Status WriteWString(const WChar *wszValue) = 0;

};

}//namespace SB

}//namespace CX

